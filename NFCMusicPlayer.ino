// Arduino Nano + RC522 + DFPlayer

#include <SPI.h>
#include <MFRC522.h>


#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>


#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

// Create MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);
//To store TAD UID
unsigned long uid = 0;
bool firstrun = true;


SoftwareSerial mySoftwareSerial(6, 7); // RX, TX
// instance to manage MP3 player
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup() {

  mySoftwareSerial.begin(9600);

  Serial.begin(115200);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  Serial.println();
  Serial.println();
  Serial.println("~~*~~*~~*~~*~~*~~*~~*~~*~~*~~*~~*~~*");
  Serial.println("Initialize RFID");
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  Serial.println();
  Serial.println("Initialize MP3");
  
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(1);  //Set volume value. From 0 to 30
  //myDFPlayer.play(1);  //Play the first mp3
  Serial.println("End of setup");

}

void loop() {
  if (firstrun)
  {
    Serial.println("Begin of loop");
    firstrun = false;
  }


  uid = 0;

  if (mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("INTO C");
    uid = getID();
    if (uid != -1) {
      Serial.print("Card detected, UID: "); Serial.println(uid);
    }

    switch (uid) {
      case 4294941018 :
        Serial.println("MUSIC 01");
        myDFPlayer.play(1);  //Play the first mp3
        break;
      case 4294960474 :
        Serial.println("MUSIC 02");
        break;
      case 4294963034 :
        Serial.println("MUSIC 03");
        break;
      default :
        Serial.println("UNKNOWN TAG");
        break;
    }
  }
}


/**
  mfrc522.PICC_IsNewCardPresent() should be checked before
  @return the card UID
*/
unsigned long getID() {
  if (! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
    return -1;
  }
  unsigned long hex_num;
  hex_num = mfrc522.uid.uidByte[0] << 24;
  hex_num += mfrc522.uid.uidByte[1] << 16;
  hex_num += mfrc522.uid.uidByte[2] << 8;
  hex_num += mfrc522.uid.uidByte[3];
  mfrc522.PICC_HaltA(); // Stop reading
  return hex_num;
}


void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
