#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <WiFiClient.h>
#include <WiFiAP.h>


#include <HardwareSerial.h>
HardwareSerial RfSerial(1);


const char *ssid = "TRACY";
const char *password = "tracy1234";

int checkPower = 15;
int checkSensor = 18;
int buzzer = 25;
int batteryOn=19;
bool duplex = false;
String receiveMsg= "";
String address="This is the address where Tracy Protect is installed."; // !important

String phone;
String fname;
String street;
String Message;
String apartment;
String disablity;

String part1 = "<html><head><meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no' /><style>input{ width:100%;height:5%;margin:5px;font-size:20px;background-color:white;}</style></head><body scroll='no' style='background-color: black;'>";
String part2 = "<div style='width:100%;height:100%;font-size:20px;'><h5 style='color:red;'>Tracy 1.0</h5>";
String part3 = "<form><input  type='tel' name='phone' placeholder='Enter Mobile No.'></br /><input type='text' placeholder='Full Name' name='name'><br /><input type='text' placeholder='Street' name='street'><br /><input type='text' name='apartment' placeholder='Apartment No.'><br />";
String part4 = "<textarea placeholder='If you have any disablity, please mention about it in detail over here.(Make sure this field is blank if you dont have any disablity)' style='width:100%;height:30%;margin:5px;font-size:20px;background-white:grey;' name='disablity'></textarea><br />";
String part5 = "<textarea placeholder='Enter your message here. Specify the type of help needed clearly.' style='width:100%;height:70%;margin:5px;font-size:20px;background-color:white;' name='message'></textarea><br /><input style='margin:5px; width:100%;' type='submit' /></form></div></body></html>";

void transmit(String data) {
  String data2 = data;
  if (RfSerial.available()) {
    duplex = true;
    RfSerial.flush();
    RfSerial.print("gfdfsgdfgdfsgfsgdfsgf <PADDING>" + data2 + "</PADDING> hghdfghfghfgdhghdg");
    duplex = false;
  }
}


AsyncWebServer server(80);
void setup(){
  pinMode(checkPower,INPUT);
  pinMode(checkSensor,INPUT);
  pinMode(batteryOn,OUTPUT);
  pinMode(buzzer,OUTPUT);
  digitalWrite(batteryOn,LOW);
  digitalWrite(checkPower,LOW);
  digitalWrite(buzzer,LOW);
  Serial.begin(115200);
  Serial.println();
  
/* Define pins for radio transmit and receive */
  RfSerial.begin(2400,SERIAL_8N1,16,17);

/* WiFi Hotspot logic */
  Serial.println("Configuring access point...");
  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.println("Server started");
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    const char* PARAM_KEY = "phone";
    int paramsNr = request->params();
    if(request->hasParam("phone")){
      phone = request->getParam(0)->value();
   }
   if(request->hasParam("message")){
      Message = request->getParam(1)->value();
   }
    transmit(phone+"\n"+Message+"\n");
    request->send(200, "text/html", part1+part2+part3+part4+part5);
  });

  server.on("/log", HTTP_GET, [](AsyncWebServerRequest *request){
    int paramsNr = request->params();
    AsyncWebParameter* p = request->getParam(0);
    request->send(200, "text/html", "<html><head><meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no' /></head><body><div style='width:100%;height:100%;font-size:20px;'><textarea style='width:100%;height:70%;margin:5px;font-size:20px;background-color:grey;font-color:white;'>" + receiveMsg + "</textarea></div></body></html>");
  });
  
  server.begin();
}

void loop(){
  String received="";
  if(!duplex && RfSerial.available()){
      received =char(RfSerial.read());
      receiveMsg += received;
  }  
  Serial.print(receiveMsg);
/* Switch to battery if POWER CUT */
  if(digitalRead(checkPower) == LOW){
      Serial.write("low");
      if(digitalRead(batteryOn) == LOW){
          digitalWrite(batteryOn,HIGH);
        }
  }

  if(digitalRead(checkPower) == HIGH){
      if(digitalRead(batteryOn) == HIGH){
          digitalWrite(batteryOn,LOW);
      }    
   }
/* Battery Logic till here */


/* Check Fire Sensor and give buzzer */
if(digitalRead(checkSensor) == LOW){
  if(digitalRead(buzzer) == LOW){
      digitalWrite(buzzer,HIGH);    
      transmit("FIRE WARNING:" + address);
      Serial.println(address);
  }
}

if(digitalRead(checkSensor) == HIGH){
  if(digitalRead(buzzer) == HIGH){
      digitalWrite(buzzer,LOW);
  }
}
/*  Fire sensor Logic till here  */  
}
