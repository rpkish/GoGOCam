#include <Bridge.h>
#include <YunClient.h>
#include <Process.h>
#include <toneAC.h>
 
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
int SyncSound;
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
    //digitalWrite(green, LOW);
    analogWrite(green, 0);
    digitalWrite(blue, HIGH);
    analogWrite(red, 0);
    delay (2500);
  }
 
  while (client.connected()) {
    num++;
    SyncSound = digitalRead(13);
    if (SyncSound == 0)
      {
      for (unsigned long freq = 2550; freq <= 2900; freq += 1){  
        toneAC(freq); 
        delay(1);   
      }
      toneAC(0); 
    }

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
    Serial.println(Status);
    int ledStatus = Status.toInt();
    int ledColor = (ledStatus / 10);
    int reBoot = (ledColor * 10 - ledStatus);
    if (ledColor == 1 ) {
      //make it blue 
      digitalWrite(blue, HIGH);
      digitalWrite(green, LOW);
      analogWrite(red, 0);
      RecStat = 0;
    }
    if (ledColor == 2 ) {
      //make it green
      digitalWrite(blue, LOW);
      digitalWrite(green, HIGH);
      analogWrite(red, 0);
      RecStat = 0;
    }
    if (ledColor == 3 ) {
      //make it red
      if (RecStat == 1) {
        if (blinky < 5) {
          analogWrite(blue, 0);
          digitalWrite(green, LOW);
          analogWrite(red, 255);
        }
        if (blinky > 5) {
          analogWrite(blue, 0);
          digitalWrite(green, LOW);
          analogWrite(red, 0);
        }
        if (blinky > 10) {
          blinky = -1;
        }
      }
      blinky++;
      RecStat = 1;
    }
    if (ledColor == 4 ) {
      //make it red + blue
      //digitalWrite(blue, HIGH);
      if (RecStat == 1) {
        if (blinky < 25) {
          analogWrite(blue, 35);
          digitalWrite(green, LOW);
          analogWrite(red, 255);
        }
        if (blinky > 25) {
          analogWrite(blue, 0);
          digitalWrite(green, LOW);
          analogWrite(red, 0);
        }
        if (blinky > 50) {
          blinky = -1;
        }
      }
      blinky++;
      RecStat = 1;
    }
    if (ledColor == 5 ) {
      //make it green + blue
      //digitalWrite(blue, HIGH);
      analogWrite(blue, 35);
      digitalWrite(green, HIGH);
      analogWrite(red, 0);
      RecStat = 0;
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
 
        p.runShellCommand("/root/reboot");
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
