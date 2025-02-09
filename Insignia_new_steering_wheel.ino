#include <SoftwareSerial.h>
#include <SPI.h>
#include <mcp_can.h>

unsigned long currentMillis;

//cruise control#####################################################
//relais cruis control
int cc_stop = 3;
int cc_set = 5;
int cc_res = 7;
int cc_aan_uit = 9;
bool cc_actief = false;
// read seeting wheel buttons
int steering_wheel = A5;
int read = 0;
int read_old = 0;
int read_old_min;
int read_old_plus;
bool press = false;
bool cc_aan = false;
unsigned long previousMillis_CC = 0;
const long interval_CC = 350;
//####################################################################

//radio knoppen#######################################################
//linbus aansluiting
SoftwareSerial mySerial(A1, A0);  // RX, TX
int inByte = 0;
static int count = 0;
bool Press = false;
unsigned long previousMillis_radio = 0;
const long interval_radio = 250;
//canbus
int zend_knop = 0;
byte data1[1] = { 0x01 };
byte data2[1] = { 0x02 };
byte data3[1] = { 0x03 };
byte data4[1] = { 0x04 };
byte data5[1] = { 0x06 };
byte data6[1] = { 0x07 };
byte data7[1] = { 0x00 };
MCP_CAN CAN0(10);  // Set CS to pin 10
//######################################################################

void setup() {
  //algemeen
  Serial.begin(115200);
  Serial.println("nano gestart");
  //linbus
  mySerial.begin(10400);
  //cruiscontrol
  pinMode(cc_stop, OUTPUT);
  pinMode(cc_set, OUTPUT);
  pinMode(cc_res, OUTPUT);
  pinMode(cc_aan_uit, OUTPUT);

  pinMode(steering_wheel, INPUT);

  digitalWrite(cc_stop, LOW);
  digitalWrite(cc_set, LOW);
  digitalWrite(cc_res, LOW);
  digitalWrite(cc_aan_uit, LOW);
  //canbus
  if (CAN0.begin(MCP_ANY, CAN_33K3BPS, MCP_8MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);  // Change to normal mode to allow messages to be transmitted
}
void loop() {
  currentMillis = millis();
  //radio knoppen#########################################################################################################
  static int count = 0;

  if (mySerial.available()) {
    inByte = mySerial.read();
    //Serial.print(inByte);
    if (inByte == 0 & count >= 8) {
      count = 0;
    }

    if (Press == true) {
      inByte = 0;
      count = 0;
      Press = false;
    }

    if (currentMillis - previousMillis_radio >= interval_radio) {
      if (inByte == 4 && count == 7) {  //Vol plus
        //Serial.println("volume hoog");
        byte sndStat = CAN0.sendMsgBuf(0x10438040, 1, 1, data1);
        delay(5);
        if (sndStat == CAN_OK) {
          byte sndStat = CAN0.sendMsgBuf(0x10438040, 1, 1, data7);
        }
        Press = true;
        previousMillis_radio = currentMillis;
      }
      if (inByte == 8 && count == 7) {  //Vol min
        //Serial.println("volume laag");
        byte sndStat = CAN0.sendMsgBuf(0x10438040, 1, 1, data2);
        delay(5);
        if (sndStat == CAN_OK) {
          byte sndStat = CAN0.sendMsgBuf(0x10438040, 1, 1, data7);
        }
        Press = true;
        previousMillis_radio = currentMillis;
      }
      if (inByte == 1 && count == 7) {  //omhoog
        byte sndStat = CAN0.sendMsgBuf(0x10438040, 1, 1, data3);
        delay(5);
        if (sndStat == CAN_OK) {
          byte sndStat = CAN0.sendMsgBuf(0x10438040, 1, 1, data7);
        }
        Press = true;
        previousMillis_radio = currentMillis;
      }
      if (inByte == 2 && count == 7) {  //omlaag
        byte sndStat = CAN0.sendMsgBuf(0x10438040, 1, 1, data4);
        delay(5);
        if (sndStat == CAN_OK) {
          byte sndStat = CAN0.sendMsgBuf(0x10438040, 1, 1, data7);
        }
        Press = true;
        previousMillis_radio = currentMillis;
      }
      if (inByte == 2 && count == 5) {  //opnemen
        byte sndStat = CAN0.sendMsgBuf(0x10438040, 1, 1, data5);
        delay(5);
        if (sndStat == CAN_OK) {
          byte sndStat = CAN0.sendMsgBuf(0x10438040, 1, 1, data7);
        }
        Press = true;
        previousMillis_radio = currentMillis;
      }

      if (inByte == 4 && count == 5) {  //ophangen
        byte sndStat = CAN0.sendMsgBuf(0x10438040, 1, 1, data6);
        delay(0);
        if (sndStat == CAN_OK) {
          byte sndStat = CAN0.sendMsgBuf(0x10438040, 1, 1, data7);
        }
        Press = true;
        previousMillis_radio = currentMillis;
      }
    }
    ++count;
  }

  if (Press == true) {
    byte sndStat = CAN0.sendMsgBuf(0x10438040, 1, 1, data7);
    if (sndStat != CAN_OK) {
      byte sndStat = CAN0.sendMsgBuf(0x10438040, 1, 1, data7);
      if (sndStat != CAN_OK) {
        byte sndStat = CAN0.sendMsgBuf(0x10438040, 1, 1, data7);
        if (sndStat != CAN_OK) {
          byte sndStat = CAN0.sendMsgBuf(0x10438040, 1, 1, data7);
          if (sndStat != CAN_OK) {
            byte sndStat = CAN0.sendMsgBuf(0x10438040, 1, 1, data7);
            if (sndStat != CAN_OK) {
              byte sndStat = CAN0.sendMsgBuf(0x10438040, 1, 1, data7);
              if (sndStat != CAN_OK) {
                byte sndStat = CAN0.sendMsgBuf(0x10438040, 1, 1, data7);
              }
            }
          }
        }
      }
    }
  }
  //cruiscontrol################################################################################################################
  if (currentMillis - previousMillis_CC >= interval_CC) {
    previousMillis_CC = currentMillis;

    read = analogRead(steering_wheel);
    //Serial.println(read);

    if (read > 670 && read < 700 && cc_actief == false) {
      //cc_actief = true;
      digitalWrite(cc_aan_uit, HIGH);
      //Serial.println("CC aan");
      press = true;
      cc_aan = true;
    }
    if (read > 400 && read < 450 && cc_aan == true) {
      digitalWrite(cc_res, HIGH);
      //Serial.println("res aan");
      cc_actief = true;
      press = true;
      delay(50);
      digitalWrite(cc_res, LOW);
    }
    if (read > 505 && read < 540 && cc_aan == true) {
      digitalWrite(cc_set, HIGH);
      cc_actief = true;
      //Serial.println("set aan");
      press = true;
      delay(50);
      digitalWrite(cc_set, LOW);
    }
    if (read > 700) {
      if (cc_actief == true) {
        digitalWrite(cc_stop, HIGH);
        cc_actief = false;
        press = true;
        // Serial.println("stop aan");
        delay(50);
        digitalWrite(cc_stop, LOW);
      } else {
        cc_actief = false;
        digitalWrite(cc_aan_uit, LOW);
        cc_aan = false;
        //Serial.println("CC uit");
      }
    }
  }
  if (press == true) {
    digitalWrite(cc_res, LOW);
    digitalWrite(cc_set, LOW);
    digitalWrite(cc_stop, LOW);
    press = false;
  }
  read_old = read;
}
