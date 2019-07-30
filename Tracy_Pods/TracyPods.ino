#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <WiFiClient.h>
#include <WiFiAP.h>


#include <HardwareSerial.h>
HardwareSerial RfSerial(1);


const char *ssid = "TRACY_PODS";
const char *password = "tracy1234";

String receiveMsg="Received:\n";
String phone;
String Message;
bool duplex = false;

AsyncWebServer server(80);

void transmit(String data){
  String data2 = data;
  if(RfSerial.available()){
  duplex = true;
  RfSerial.flush();
  RfSerial.println("dfadsfadfadfadfadfdad<PADDING>" + data2 + "</PADDING>");
  duplex = false;
  }
}


void setup(){
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
    request->send(200, "text/html", "<html><head><meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no' /></head><body scroll='no' style='background-color: black;'><div style='width:100%;height:100%;font-size:20px;'><form><input style='width:100%;height:10%;margin:5px;font-size:20px;background-color:grey;font-color:white;' type='tel' name='phone' placeholder='Enter Registed Mobile No.'><br /><textarea placeholder='Enter your message here' style='width:100%;height:70%;margin:5px;font-size:20px;background-color:grey;font-color:white;' name='message'></textarea><br /><input style='margin:5px; width:100%;' type='submit' /></form></div></body></html>");
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
  delay(50);
}
