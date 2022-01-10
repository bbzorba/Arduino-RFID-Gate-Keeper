#include <MFRC522.h>
#include <SPI.h>
#include <Servo.h>

#define SAD 10
#define RST 5

Servo myservo;
MFRC522 nfc(SAD, RST);

void setup() {
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  
  SPI.begin();
  Serial.begin(9600);
  Serial.println("Looking for MFRC522.");
  nfc.begin();
  myservo.attach(3);

  byte version = nfc.getFirmwareVersion();

  Serial.print("Found chip MFRC522 ");
  Serial.print("Firmware ver. 0x");
  Serial.print(version, HEX);
  Serial.println(".");
}

byte keyA[6] = {0x53, 0x17, 0xB4, 0x79 };
byte keyB[6] = {0x75, 0x69, 0x50, 0xD3 };

void loop() {
  
  byte status;
  byte data[MAX_LEN];
  byte serial[5];
  int i, j, pos;

  // Send a general request out into the aether. If there is a tag in
  // the area it will respond and the status will be MI_OK.
  status = nfc.requestTag(MF1_REQIDL, data);

  if (status == MI_OK) {
    Serial.println("Tag detected.");
    Serial.print("Type: ");
    Serial.print(data[0], HEX);
    Serial.print(", ");
    Serial.println(data[1], HEX);

    // calculate the anti-collision value for the currently detected
    // tag and write the serial into the data array.
    status = nfc.antiCollision(data);
    memcpy(serial, data, 5);

    Serial.println("The serial nb of the tag is:");
    for (i = 0; i < 3; i++) {
      Serial.print(serial[i], HEX);
      Serial.print(", ");
    }
    Serial.println(serial[3], HEX);

    if ((serial[0] == keyA[0]) && (serial[1] == keyA[1]) && (serial[2] == keyA[2]) && (serial[3] == keyA[3]))
    {
      digitalWrite(5,HIGH);
      Serial.println("WELCOME MR. ZORBA");
      int pos = 90;
      
      for (pos = 90; pos > 0; pos -= 1) // goes from 0 degrees to 180 degrees
      {
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }

      delay(2000);

      for (pos = 0; pos < 90; pos += 1) // goes from 180 degrees to 0 degrees
      {
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
      digitalWrite(5,LOW);
    }
    
    else if ((serial[0] == keyB[0]) && (serial[1] == keyB[1]) && (serial[2] == keyB[2]) && (serial[3] == keyB[3]))
    {
      digitalWrite(6,HIGH);
      Serial.println("WELCOME MR. GURHAN");
      int pos = 90;
      
      for (pos = 90; pos > 0; pos -= 1) // goes from 0 degrees to 180 degrees
      {
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }

      delay(2000);

      for (pos = 0; pos < 90; pos += 1) // goes from 180 degrees to 0 degrees
      {
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
      digitalWrite(6,LOW);
    }
    
    else
    {
      Serial.println("ACCESS DENIED");
    }

    nfc.haltTag();
  }
  delay(1000);
}
