#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
String readString;
const char* ssid = "HITAM_cloud";
const char* password = "Hitam4_2023";
const char* host = "script.google.com";
const int httpsPort = 443;
WiFiClientSecure client;
const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";
String GAS_ID = "AKfycby3gP6WJ7QQRRCJ0meCScHXxDsRoQqzAq0W237Hs79xN2Y4bpUlCtp5V4wER9dLuB-JrQ";  // Replace by your GAS service id
#include <Servo.h>

int servocount = 0;               // count = 0
char input[13];              // character array of size 12
boolean flag = 0;            // flag =0
const int relay = D4;
char id1[13] = "280018D3AC4F";
char id2[13] = "28001773BFF3";
char id3[13] = "550039617D70";

//char id[13];
//const char *inputArray[]={"280018D3AC4F","28001773BFF3","550039617D70"};
int a = 0;
int pos;

int count=1;
//char card_no[13]="280018D3AC4F";
char card_no[13];
int cards;


Servo myservo;

void setup()
{
  myservo.attach(D8);
  myservo.write(0);
  pinMode(relay, OUTPUT);
  Serial.begin(9600);       // begin serial port with baud rate 9600bps
  digitalWrite(2, HIGH); //to set the state of a pin

WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }  
  
}

void loop()
{
  if (Serial.available())
  {
    servocount = 0;
    while (Serial.available() && servocount < 12)         // Read 12 characters and store them in input array
    {
      input[servocount] = Serial.read();
      servocount++;
      delay(5);
    }
    Serial.print(input);                             // Print RFID tag number
    
//for(int i=0;i<3;i++){
//  if(input==inputArray[i]){
//      strcpy(id,input);
//      Serial.print("ids");
//      Serial.println(id);  
//    }
//}

    if ((input[0] ^ input[2] ^ input[4] ^ input[6] ^ input[8] == input[10]) &&
        (input[1] ^ input[3] ^ input[5] ^ input[7] ^ input[9] == input[11])) {
      if (compare1() || compare2()||compare3()) {
        Serial.println("Authentication Successful");
        digitalWrite(relay, !digitalRead(relay));
        if (a == 0) {
          for (pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees
          { // in steps of 1 degree
            myservo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(10);                       // waits 15ms for the servo to reach the position
          }
          a = 1;
        }
        else {
          for (pos = 180; pos >= 0; pos -= 1) // goes from 180 degrees to 0 degrees
          {
            myservo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(10);                       // waits 15ms for the servo to reach the position
          }
          a = 0;
        }
        sendData(input);
        delay(3000);
        serialflush();
      }
      else {
        sendData(input);
        Serial.print("Authentication Failed");
        delay(3000);
        serialflush();
      }
    }
    else {
      Serial.println("Error");
    }
  }
  }


void serialflush() {
  while (Serial.available() > 0) {
    char t = Serial.read();
  }
}

bool compare1() {
  
  for (int i = 0; i < 12; i++) {
    
    if (input[i] != id1[i]) {
      return false;
    }
  }
  return true;
    }

bool compare2(){
   for (int i = 0; i < 12; i++) {
    
    if (input[i] != id2[i]) {
      return false;
    }
  }
  return true;
  
  
  
  }
  bool compare3(){
   for (int i = 0; i < 12; i++) {
    
    if (input[i] != id3[i]) {
      return false;
    }
  }
  return true;
  
  
  
  }


void sendData(char x[13])//char x[12]
{
 client.setInsecure();
 Serial.print("connecting to ");
 Serial.println(host);
 if (!client.connect(host, httpsPort)) {
   Serial.println("connection failed");
   return;
 }
// if (client.verify(fingerprint, host)) {
// Serial.println("certificate matches");
// } else {
// Serial.println("certificate doesn't match");
// }

//String string_x=  String(strtol(x,NULL,16));
 //String string_x=  String(x,DEC);
 String string_x=x;
 Serial.println(string_x);
 
 String url = "/macros/s/" + GAS_ID + "/exec?value1=" + string_x ;
 Serial.print("requesting URL: ");
 Serial.println(url);
 client.print(String("GET ") + url + " HTTP/1.1\r\n" +
        "Host: " + host + "\r\n" +
        "User-Agent: BuildFailureDetectorESP8266\r\n" +
        "Connection: close\r\n\r\n");
 Serial.println("request sent");
 while (client.connected()) {
 String line = client.readStringUntil('\n');
 if (line == "\r") {
   Serial.println("headers received");
   break;
 }
 }
 String line = client.readStringUntil('\n');
 if (line.startsWith("{\"state\":\"success\"")) {
 Serial.println("esp8266/Arduino CI successfull!");
 } else {
 Serial.println("esp8266/Arduino CI has failed");
 }
 Serial.println("reply was:");
 Serial.println("==========");
 Serial.println(line);
 Serial.println("==========");
 Serial.println("closing connection");
 
}




void infos()
{
   if(Serial.available())
   {
      count = 0;
      while(Serial.available() && count < 12)
      {
         card_no[count] = Serial.read();
         count++;
         delay(5);
      }
      Serial.print(card_no);}
}
