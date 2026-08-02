/*
 * ESP32 Individual Flex Sensor Diagnostic
 * Use the Serial Monitor to select and read one sensor at a time.
 */

// Define the ADC1 pins based on the connection diagram [cite: 73]
const int thumbPin  = 36; // ADC1_0
const int indexPin  = 39; // ADC1_3
const int middlePin = 34; // ADC1_6
const int ringPin   = 35; // ADC1_7
const int littlePin = 32; // ADC1_4

// Variable to track which sensor we are currently testing
int currentSensor = 1; // Default to Thumb

void setup() {
  // Initialize Serial Monitor at 115200 baud rate for ESP32 [cite: 44, 97]
  Serial.begin(115200);
  
  // Set resolution to 12-bit (0-4095), which is default for ESP32 [cite: 13]
  analogReadResolution(12);
  
  Serial.println("\n--- Individual Flex Sensor Diagnostic Started ---");
  Serial.println("Type a number (1-5) in the input box above and press Enter:");
  Serial.println("1: Thumb | 2: Index | 3: Middle | 4: Ring | 5: Little");
  delay(2000);
}

void loop() {
  // Check if you typed a new number in the Serial Monitor
  if (Serial.available() > 0) {
    char input = Serial.read();
    
    // Update the selected sensor if input is between 1 and 5
    if (input >= '1' && input <= '5') {
      currentSensor = input - '0'; // Convert char to integer
      Serial.print("\n>>> Switched to Sensor ");
      Serial.println(currentSensor);
      delay(1000); // Brief pause to read the switch confirmation
    }
  }

  // Read and print the value of the currently selected sensor
  int bendValue = 0;
  String fingerName = "";

  switch (currentSensor) {
    case 1: 
      bendValue = analogRead(thumbPin); 
      fingerName = "Thumb"; 
      break;
    case 2: 
      bendValue = analogRead(indexPin); 
      fingerName = "Index"; 
      break;
    case 3: 
      bendValue = analogRead(middlePin); 
      fingerName = "Middle"; 
      break;
    case 4: 
      bendValue = analogRead(ringPin); 
      fingerName = "Ring"; 
      break;
    case 5: 
      bendValue = analogRead(littlePin); 
      fingerName = "Little"; 
      break;
  }

  // Output the bending value to the Serial Monitor
  Serial.print(fingerName);
  Serial.print(" Bend Value: ");
  Serial.println(bendValue);

  // Delay for readability so the numbers don't scroll too fast
  delay(250); 
}