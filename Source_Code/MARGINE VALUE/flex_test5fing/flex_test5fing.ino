const int THUMB_PIN = 32;
//THUMB-25
// Variables to store our smoothed data
int baselineValue = 0;

// THE TRIGGER THRESHOLD
// If the sensor changes by this many points from the baseline, it triggers!
// You may need to change this to 20, 30, or 60 depending on your exact sensor.
const int TRIGGER_MARGIN = 8; 

// Function to take 50 readings and average them to kill the noise
int getSmoothedReading() {
  long total = 0;
  for (int i = 0; i < 50; i++) {
    total += analogRead(THUMB_PIN);
    delay(1); // 1 millisecond delay between reads
  }
  return total / 50;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("--- STARTING AUTO-CALIBRATION ---");
  Serial.println("KEEP SENSOR COMPLETELY FLAT!");
  delay(2000);
  
  // Calculate the resting state of your specific sensor
  baselineValue = getSmoothedReading();
  
  Serial.print("Baseline Locked at: ");
  Serial.println(baselineValue);
  Serial.print("Trigger set to fire at: ");
  Serial.println(baselineValue + TRIGGER_MARGIN); // Or minus, depending on wiring
  Serial.println("---------------------------------");
  delay(2000);
}

void loop() {
  int currentSmoothedValue = getSmoothedReading();
  
  // Print for debugging
  Serial.print("Current: ");
  Serial.print(currentSmoothedValue);
  
  // Check if the difference between current and baseline is bigger than our margin
  // abs() makes sure it works whether the number goes up OR down
  if (abs(currentSmoothedValue - baselineValue) > TRIGGER_MARGIN) {
    Serial.println("  >>> SWITCH TRIGGERED! (SENDING SIGNAL) <<<");
    
    // In your final code, this is where you would do:
    // myData.command = 'c';
    // esp_now_send(...);
  } else {
    Serial.println("  ... switch off");
  }
  
  delay(100); 
}