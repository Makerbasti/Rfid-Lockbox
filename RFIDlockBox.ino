#include <MFRC522.h>
#include <Servo.h>
#include <SPI.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define SS_PIN  D8 // RFID-RC522 SDA
#define RST_PIN D3 // RFID-RC522 RST

const byte servoPin  = D3;  // digital Pin - Servo



Servo myServo;
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Hier die UUID fuer erlaubte Karten eingeben und in "erlaubteKartenCount"
// Anzahl der Eintraege angeben
long erlaubteKarten[] = { 2138930/*, 1139890*/ };
byte erlaubteKartenCount = 3;




void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("Start");


  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("MiFare INIT");

  //pinMode(buzzerPin, OUTPUT);
  pinMode(servoPin, OUTPUT);


  myServo.attach(servoPin);
  myServo.write(65);
 
}



void loop() {
  if (mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("\nCARD");
    if (mfrc522.PICC_ReadCardSerial()) {
      Serial.println("READ");

      long code = 0;

      for (byte i = 0; i < mfrc522.uid.size; i++) {
        code = ((code + mfrc522.uid.uidByte[i]) * 10);
      }

      Serial.print(F("Erkannte Kartennummer:"));
      Serial.print(code);

      if (PruefeKartenberechtigung(code) == true) {
        Serial.println(F(" [BERECHTIGT]"));
   

        myServo.write(0);
        delay(1500);
        myServo.write(65);
        delay(100);
      } else {
        Serial.println(F(" [NICHT BERECHTIGT]"));

   
      }
      
    delay(2000);
    }
    delay(500);

  }
}



boolean PruefeKartenberechtigung(long KartenUUID) {
  boolean erlaubt = false;

  for (int i = 0; i < erlaubteKartenCount; i++) {
    if (erlaubteKarten[i] == KartenUUID) {
      erlaubt = true;
    }
  }
  return erlaubt;
}
