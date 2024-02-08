#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
LiquidCrystal_I2C lcd(0x27, 16, 2);
int buzzer = 5;
int r_led = 6;
int g_led = 7;
int b_led = 8;
byte mycard[] = {0x0E, 0xE9, 0x8A, 0xCB}; //Old Card: 0E E9 8A CB
byte card1[] = {0x05, 0x11, 0x0F, 0x03}; // WCard : 05 11 0F 03
byte ncard[] = {0xF3, 0xF4, 0xCE, 0xA7}; // F3 F4 CE A7

Servo myservo;


// ##################################################################
// Function to compare two arrays (UIDs)
bool compareUID(byte *uid1, byte *uid2, byte size) {
  for (byte i = 0; i < size; i++) {
    if (uid1[i] != uid2[i]) {
      return false;
    }
  }
  return true;
}
// ##################################################################
void setup() {
  pinMode(buzzer,OUTPUT);
  myservo.attach(4);
  myservo.write(0);
  pinMode(r_led, OUTPUT);
  pinMode(g_led, OUTPUT);
  pinMode(b_led, OUTPUT);
  lcd.begin();
  lcd.backlight();
  lcd.print("Scan your card:");
	Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
}

void loop() {
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}
	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}
	// Dump debug info about the card; PICC_HaltA() is automatically called
  // ############################################################
  // Look for new cards
    Serial.println("Card detected!");
// Compare the UID with the authorized card
    if (compareUID(mfrc522.uid.uidByte, mycard, mfrc522.uid.size)) {
      // Serial.println("Authorized card !");
      lcd.clear();
      lcd.print("   Welcome !");h
      tone(buzzer,2000,200);
      analogWrite(r_led,0);
      analogWrite(g_led,255);
      analogWrite(b_led,0);
      delay(1000);
      noTone(buzzer);
      myservo.write(90);
      delay(2000);
      myservo.write(0);

    } 
    else if (compareUID(mfrc522.uid.uidByte, card1, mfrc522.uid.size))
    {
      // Serial.println("Unauthorized card !");
      lcd.clear();
      lcd.print("    Expired !"); 
      analogWrite(r_led,0);
      analogWrite(g_led,0);
      analogWrite(b_led,255);
      delay(2000);
    }

    else{
      lcd.clear();
      lcd.print("    Denied !"); 
      analogWrite(r_led,255);
      analogWrite(g_led,0);
      analogWrite(b_led,0);
      delay(2000);
    }

     // Delay to avoid multiple readings
      analogWrite(r_led,0);
      analogWrite(g_led,0);
      analogWrite(b_led,0);      
      lcd.clear();
      lcd.print("Scan your card:");
}
