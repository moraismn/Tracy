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

String address = "Call for code challenge is on fire! Let the battle begin! ";
String receiveMsg="Received:\n";
String phone;
String Message;
bool duplex = false;

AsyncWebServer server(80);

void transmit(String data){
  if(RfSerial.available()){
  duplex = true;
  RfSerial.flush();
  RfSerial.println("PADDING" + data + "\n\n");
  duplex = false;
  }
}


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
  server.begin();
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
    request->send(200, "text/html", "<html><body><form><input type='text' name='phone'><textarea name='message'></textarea><br /><input type='submit' /></form></body></html>");
  });

  server.on("/log", HTTP_GET, [](AsyncWebServerRequest *request){
    int paramsNr = request->params();
    AsyncWebParameter* p = request->getParam(0);
    request->send(200, "text/html", "<html><body><a href='#'><button>Refresh</button></a><br /><textarea>"+receiveMsg+"</textarea></body></html>");
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
      transmit("PADDING FIRE WARNING:" + address + "\n\n");
  }
}

if(digitalRead(checkSensor) == HIGH){
  if(digitalRead(buzzer) == HIGH){
      digitalWrite(buzzer,LOW);
  }
}
/*  Fire sensor Logic till here  */  

}
