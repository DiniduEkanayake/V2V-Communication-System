#include <SPI.h>                // Include SPI library for communication with other devices
#include <nRF24L01.h>           // Include nRF24L01 library for wireless communication
#include <RF24.h>               // Include RF24 library for interfacing with NRF24L01 transceiver
#include <HCSR04.h>             // Include HCSR04 library for interfacing with ultrasonic sensor

#define GPS Serial              // Define GPS as Serial for communication
String speedo = "kmh";          // String to store speed unit
const unsigned char UBX_HEADER[] = {0xB5, 0x62};   // Define UBX header for GPS communication

struct NAV_VELNED {             // Define a structure to store GPS velocity information
  unsigned char cls;            // Class identifier
  unsigned char id;             // ID identifier
  unsigned short len;           // Length of payload
  unsigned long iTOW;          // GPS time of week
  long velN;                    // North velocity
  long velE;                    // East velocity
  long velD;                    // Down velocity
  unsigned long speed;          // Ground speed
  unsigned long gSpeed;         // Ground speed in cm/s
  long heading;                 // Heading
  unsigned long sAcc;           // Speed accuracy
  unsigned long cAcc;           // Course/heading accuracy
};

NAV_VELNED velned;              // Instantiate NAV_VELNED structure

void calcChecksum(unsigned char* CK) {  // Function to calculate the checksum for GPS data
  memset(CK, 0, 2);             // Clear the checksum
  for (int i = 0; i < (int)sizeof(NAV_VELNED); i++) {  // Iterate through the structure
    CK[0] += ((unsigned char*)(&velned))[i];  // Add each byte to the first checksum byte
    CK[1] += CK[0];             // Add the result to the second checksum byte
  }
}

bool processGPS() {             // Function to process GPS data
  static int fpos = 0;          // Static variable to track position in GPS data
  static unsigned char checksum[2];  // Array to store checksum
  const int payloadSize = sizeof(NAV_VELNED); // Size of the GPS payload

  while (GPS.available()) {     // Check if GPS data is available
    byte c = GPS.read();        // Read a byte from GPS
    if (fpos < 2) {             // Check if within UBX_HEADER
      if (c == UBX_HEADER[fpos])  // Check if byte matches UBX_HEADER
        fpos++;                 // Increment position
      else
        fpos = 0;               // Reset position if not matching
    } else {
      if ((fpos - 2) < payloadSize)  // Check if within payload size
        ((unsigned char*)(&velned))[fpos - 2] = c;  // Store byte in NAV_VELNED structure

      fpos++;                   // Increment position

      if (fpos == (payloadSize + 2)) {  // Check if reached end of payload
        calcChecksum(checksum);  // Calculate checksum
      } else if (fpos == (payloadSize + 3)) {  // Check if reading first checksum byte
        if (c != checksum[0])   // Compare calculated checksum with received checksum
          fpos = 0;             // Reset position if not matching
      } else if (fpos == (payloadSize + 4)) {  // Check if reading second checksum byte
        fpos = 0;               // Reset position
        if (c == checksum[1]) { // Check if second checksum byte matches
          return true;          // Return true if checksum is valid
        }
      } else if (fpos > (payloadSize + 4)) {  // Check if past end of packet
        fpos = 0;               // Reset position
      }
    }
  }
  return false;                 // Return false if no valid packet found
}

int i = 0;                      // Variable for loop iteration
RF24 radio(7, 8);               // Define NRF24L01 radio object with CE and CSN pins
#define trigPin 4               // Define ultrasonic sensor trigger pin
#define echoPin 3               // Define ultrasonic sensor echo pin
const byte address[6] = "00001"; // Define address for communication with NRF24L01 module
unsigned long gSpeed = 0;       // Variable to store ground speed
unsigned long lastSerialUpdate = 0;  // Variable to store last serial update time

void setup() {
  Serial.begin(9600);           // Initialize serial communication at 9600 baud rate
  GPS.begin(9600);              // Initialize GPS serial communication at 9600 baud rate
  pinMode(trigPin, OUTPUT);     // Set trigPin as output
  pinMode(echoPin, INPUT);      // Set echoPin as input
  radio.begin();                // Initialize radio communication
  radio.openWritingPipe(address);  // Open writing pipe with defined address
  radio.setPALevel(RF24_PA_MIN);   // Set power amplifier level to minimum
  radio.stopListening();        // Stop radio from listening
}

void loop() {
  cal_speed();                  // Measure speed using ultrasonic sensor
  if (processGPS()) {           // Check if GPS data is available and valid
    gSpeed = velned.gSpeed;     // Update ground speed
    printSpeed();               // Print speed to radio
  }
}

float duration, distance;       // Variables to store duration and distance measured by ultrasonic sensor

void cal_speed(){
  digitalWrite(trigPin, LOW);  // Set trigPin low
  delayMicroseconds(2);         // Wait for stabilization
  digitalWrite(trigPin, HIGH); // Set trigPin high
  delayMicroseconds(10);        // Wait
  digitalWrite(trigPin, LOW);  // Set trigPin low again
  duration = pulseIn(echoPin, HIGH);  // Measure duration of pulse
  distance = (duration/2) / 29.1;     // Calculate distance based on duration
  Serial.println(distance);     // Print distance to serial monitor
}

void printSpeed() {
  String speedoText;            // String to store speed unit text
  int speedCalc;                // Variable to store calculated speed

  if (speedo == "kmh") {        // Check if speed unit is km/h
    speedoText = "Km/h";        // Set speed unit text
    speedCalc = gSpeed * 0.036; // Calculate speed in km/h
  } else if (speedo == "mph") { // Check if speed unit is mph
    speedoText = "mph";         // Set speed unit text
    speedCalc = gSpeed * 0.0223694; // Calculate speed in mph
  }
  float dis[2];                 // Array to store speed and distance
  dis[0] = speedCalc * (100000/3600);  // Convert speed to cm/s and store
  dis[1] = distance;            // Store distance
  radio.write(&dis, sizeof(dis)); // Send speed and distance data via radio
  delay(250);                   // Delay for stability
}
