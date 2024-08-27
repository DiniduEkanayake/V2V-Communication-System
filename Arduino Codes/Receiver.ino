#include <SPI.h>                // Include SPI library for communication with other devices
#include <nRF24L01.h>           // Include nRF24L01 library for wireless communication
#include <RF24.h>               // Include RF24 library for interfacing with NRF24L01 transceiver
#include <Wire.h>               // Include Wire library for I2C communication
#include <LiquidCrystal_I2C.h>  // Include LiquidCrystal_I2C library for controlling LCD display

LiquidCrystal_I2C lcd(0x27, 16, 2);    // Initialize LCD object with I2C address 0x27, 16 columns, and 2 rows
RF24 radio(7, 8);                       // Initialize NRF24L01 radio object with CE and CSN pins

const byte address[6] = "00001";        // Define address for communication with NRF24L01 module

void setup() {
  Serial.begin(9600);                   // Initialize serial communication at 9600 baud rate
  Wire.begin();                          // Initialize I2C bus
  lcd.init();                            // Initialize the LCD display
  lcd.backlight();                       // Turn on the backlight of the LCD
  radio.begin();                         // Initialize radio communication
  radio.openReadingPipe(0, address);     // Open a reading pipe with the defined address
  radio.setPALevel(RF24_PA_MIN);         // Set the power amplifier level to minimum
  radio.startListening();                // Start listening for incoming data
}

void loop() {
  if (radio.available()) {               // Check if there is data available for reading
    float dis[2];                        // Create an array to store distance and speed
    radio.read(&dis, sizeof(dis));       // Read data from the radio module into the 'dis' array
    Serial.println(round(dis[1]));       // Print the rounded value of distance to the serial monitor
    lcd.clear();                         // Clear the LCD screen
    lcd.setCursor(0, 0);                 // Set the cursor to the beginning of the first line
    lcd.print("D:");                     // Print label for distance
    lcd.print(round(dis[1]));            // Print the rounded value of distance
    lcd.print("cm");                     // Print unit for distance
    lcd.print(" ");                      // Print a space
    lcd.print("S:");                     // Print label for speed
    lcd.print(round(dis[0]));            // Print the rounded value of speed
    lcd.print("cm/s");                   // Print unit for speed
    lcd.setCursor(3, 1);                 // Set the cursor to the fourth column of the second line
    if (round(dis[1]) > 100) {           // Check if the distance is greater than 100cm
      lcd.print("   SAFE");              // If so, print "SAFE" on the LCD
    } else {
      lcd.print("NOT SAFE!");            // If not, print "NOT SAFE!" on the LCD
    }
    delay(250);                          // Delay for stability
  }
}
