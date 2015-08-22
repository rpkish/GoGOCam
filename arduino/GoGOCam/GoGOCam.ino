#include <Bridge.h>
#include <YunClient.h>
#include <Process.h>

 
YunClient client;
IPAddress server(127, 0, 0, 1);
int port = 10000;
int X = 0;
int S = 0;
String cmd;
int red = 6;   //red pin
int green = 5; //green pin
int blue = 3;  //blue pin
int ledlock = 0;
int stateButton;
int resetTimer;
int num;
int RecStat = 0;
int blinky = 0;
String Status;

void setup()
{
  delay(2500);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(3, OUTPUT);
  Bridge.begin();
  Serial.begin(115200);
  analogWrite(red, 255);
  analogWrite(green, 255);
}
 
void loop()
{
  Serial.println("connecting...");
  if (client.connect(server, port)) {
    Serial.println("connected");
  } else {
    Serial.println("connection failed");
    SolidLED(0,0,255);
    delay (2500);
  }
 
  while (client.connected()) {
    num++;    
    String Return;
    cmd = "led.Status";
    client.write((uint8_t*)&cmd[0], cmd.length());  
    while (client.available()) {
        char c = client.read();
        Return += String(c);
        }
      Return = Return.substring(0,2);
      if (Return == "") {
      }
      else {
        Status = Return;
      }
      int ledStatus = Status.toInt();
      int ledColor = (ledStatus / 10);
      int reBoot = (ledColor * 10 - ledStatus);
      Serial.println(reBoot);
      if (ledColor == 1 ) {
        BlinkLED(90,0,0,0,0,90,1);
      }
      if (ledColor == 2 ) {
        SolidLED(0,255,0);
      }
      if (ledColor == 3 ) {
        BlinkLED(255,0,0,0,0,0,25);
      }
      if (ledColor == 4 ) {
        BlinkLED(255,0,40,0,0,0,25);
      }
      if (ledColor == 5 ) {
        SolidLED(0,255,35);
      }
      if (reBoot == 0 ) {
        resetTimer = 0;
      }
      if (reBoot == -1 ) {
        Serial.println ("Wan is down");
        resetTimer ++;
        delay (250);
        if (resetTimer > 50 ) {
          Serial.println ("Resetting System");
          Process p;
          p.runShellCommand("/usr/bin/reboot");
        }
      client.flush();
      }

      stateButton = digitalRead(12);
      if ( stateButton == 0) {
        if ( X == 0) {
          cmd = "record.Start";
          Serial.println("sending record command");
          X = 1;
          client.write((uint8_t*)&cmd[0], cmd.length());
          client.flush();
        }
      }
      if ( stateButton == 1) {
        if ( X != 0) {
          Serial.println("Sending Stop Record Command");
          cmd = "record.Stop";
          X = 0;
          client.write((uint8_t*)&cmd[0], cmd.length());
          client.flush();
        }
      }
    }
  }

void SolidLED(byte redByte, byte greenByte, byte blueByte){
  analogWrite(blue, blueByte);
  analogWrite(green,greenByte);
  analogWrite(red, redByte);
  RecStat = 0;
}

void BlinkLED(byte redByte1,byte greenByte1,byte blueByte1,byte redByte2,byte greenByte2,byte blueByte2,int rate){
  if (RecStat == 1) {
    if (blinky < rate) {
      analogWrite(blue,blueByte1);
      analogWrite(green,greenByte1);
      analogWrite(red,redByte1);
    }
    if (blinky > rate) {
      analogWrite(blue,blueByte2);
      analogWrite(green,greenByte2);
      analogWrite(red,redByte2);
    }
    if (blinky > (rate *2)) {
      blinky = -1;
    }
  }
  blinky++;
  RecStat = 1;
}

