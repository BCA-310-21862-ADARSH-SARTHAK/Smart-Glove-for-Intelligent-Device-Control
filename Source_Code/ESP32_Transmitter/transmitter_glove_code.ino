//glove transmitter program.
#include <esp_now.h>
#include <WiFi.h>

// --- Pin Definitions ---
const int THUMB_PIN  = 32;
const int INDEX_PIN  = 33;
const int MIDDLE_PIN = 34;
const int RING_PIN   = 35;
const int LITTLE_PIN = 39;

// --- Trigger Margins ---
const int MARGIN_T = 8;
const int MARGIN_I = 12;
const int MARGIN_M = 22;
const int MARGIN_R = 22;
const int MARGIN_L = 22;

// --- Baseline Variables ---
int baseT = 0, baseI = 0, baseM = 0, baseR = 0, baseL = 0;

// --- EMA Smoothing Variables (The "Shock Absorber") ---
float emaT = 0, emaI = 0, emaM = 0, emaR = 0, emaL = 0;
float smoothing = 0.1; // 10% New Reading, 90% Old Reading

// --- Lockout Timer Variables (The "Security Guard") ---
char active_command = 'x';  // The currently locked-in command
char pending_command = 'x'; // The command we are evaluating
unsigned long cmd_start_time = 0;
const unsigned long HOLD_TIME = 500; // 500ms to lock a command

// RECEIVER'S MAC ADDRESS
uint8_t broadcastAddress[] = {0xC8, 0x2E, 0x18, 0x25, 0xB1, 0xC8};

typedef struct struct_message {
  char command;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

// --- The EMA Math Filter ---
void getSmoothedReadings(int &t, int &i, int &m, int &r, int &l) {
  // Blend the new raw reading with the heavy old reading
  emaT = (emaT * (1.0 - smoothing)) + (analogRead(THUMB_PIN) * smoothing);
  emaI = (emaI * (1.0 - smoothing)) + (analogRead(INDEX_PIN) * smoothing);
  emaM = (emaM * (1.0 - smoothing)) + (analogRead(MIDDLE_PIN) * smoothing);
  emaR = (emaR * (1.0 - smoothing)) + (analogRead(RING_PIN) * smoothing);
  emaL = (emaL * (1.0 - smoothing)) + (analogRead(LITTLE_PIN) * smoothing);

  // Convert the smoothed decimal math back into whole numbers
  t = (int)emaT;
  i = (int)emaI;
  m = (int)emaM;
  r = (int)emaR;
  l = (int)emaL;
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  // "Prime" the math filter so it doesn't start at zero
  emaT = analogRead(THUMB_PIN);
  emaI = analogRead(INDEX_PIN);
  emaM = analogRead(MIDDLE_PIN);
  emaR = analogRead(RING_PIN);
  emaL = analogRead(LITTLE_PIN);

  // --- AUTO CALIBRATION PHASE ---
  Serial.println("CALIBRATING: KEEP HAND COMPLETELY FLAT!");
  delay(3000); // Give user 3 seconds to flatten hand
  
  // Lock in the resting baselines
  getSmoothedReadings(baseT, baseI, baseM, baseR, baseL);
  
  Serial.println("Baselines Locked! Ready to control cobot.");
}

void loop() {
  int currT, currI, currM, currR, currL;
  
  // Get the heavily smoothed, noise-free numbers
  getSmoothedReadings(currT, currI, currM, currR, currL);

  // Determine if the finger has deliberately moved past the margin
  bool t = abs(currT - baseT) > MARGIN_T;
  bool i = abs(currI - baseI) > MARGIN_I;
  bool m = abs(currM - baseM) > MARGIN_M;
  bool r = abs(currR - baseR) > MARGIN_R;
  bool l = abs(currL - baseL) > MARGIN_L;

  char raw_cmd = 'x'; // Default to STOP

  // --- Gesture Logic Mapping ---
  if (!t && !i && !m && !r && !l) {
    raw_cmd = 'x'; // Open palm -> Stop
  } 
  else if (t && i && m && r && l) {
    raw_cmd = 'c'; // Full fist -> Close gripper
  }
  else if (!t && i && m && r && l) {
    raw_cmd = 'o'; // Four fingers bent, thumb straight -> Open gripper
  }
  else if (t && i && !m && !r && !l) {
    raw_cmd = 'w'; // Thumb + Index bent -> +X
  }
  else if (t && !i && m && !r && !l) {
    raw_cmd = 's'; // Thumb + Middle bent -> -X
  }
  else if (t && !i && !m && r && !l) {
    raw_cmd = 'a'; // Thumb + Ring bent -> +Y
  }
  else if (t && !i && !m && !r && l) {
    raw_cmd = 'd'; // Thumb + Little bent -> -Y
  }
  else if (t && i && m && !r && !l) {
    raw_cmd = 'u'; // Thumb + Index + Middle bent -> +Z
  }
  else if (t && !i && !m && r && l) {
    raw_cmd = 'n'; // Thumb + Ring + Little bent -> -Z
  }

  // --- STRICT COMMAND LOCKOUT LOGIC ---
  
  if (active_command != 'x') {
    // 1. WE ARE CURRENTLY MOVING (LOCKED STATE)
    // We completely ignore all twitches and cross-talk. We ONLY look for an open hand ('x') to stop.
    if (raw_cmd == 'x') {
      // Hand opened! Break the lock and stop instantly.
      active_command = 'x';
      pending_command = 'x';
      myData.command = 'x';
      esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
      Serial.println(">>> STOPPING & UNLOCKING <<<");
    } else {
      // User is still holding gesture (or twitching). Keep sending the locked command.
      myData.command = active_command;
      esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    }
  } 
  else {
    // 2. WE ARE IDLE (WAITING FOR A NEW GESTURE)
    if (raw_cmd == 'x') {
      // Hand is flat. Stay idle.
      pending_command = 'x';
    } 
    else {
      // Attempting a new gesture!
      if (raw_cmd != pending_command) {
        // Just started the gesture. Start the stopwatch.
        pending_command = raw_cmd;
        cmd_start_time = millis();
      } 
      else {
        // Holding the gesture. Has half a second passed?
        if (millis() - cmd_start_time >= HOLD_TIME) {
          // TIME REACHED! LOCK IT IN!
          active_command = raw_cmd;
          myData.command = active_command;
          esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
          
          Serial.print(">>> LOCKED IN COMMAND: ");
          Serial.println(active_command);
        }
      }
    }
  }

  // A tiny 10ms delay to prevent flooding the receiver
  delay(10); 
}