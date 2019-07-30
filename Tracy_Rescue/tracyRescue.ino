#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <WiFiClient.h>
#include <WiFiAP.h>

#include <HardwareSerial.h>
HardwareSerial RfSerial(1);

const char *ssid = "TRACY_RESCUE";
const char *password = "tracy1234";


AsyncWebServer server(80);
String receiveMsg = "Received:\n";
boolean duplex = false;
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


void setup() {
  Serial.begin(115200);
  RfSerial.begin(2400, SERIAL_8N1, 16, 17);

  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    const char* PARAM_KEY = "phone";
    int paramsNr = request->params();
    if (request->hasParam("phone")) {
      phone = request->getParam(0)->value();
    }
    if (request->hasParam("message")) {
      Message = request->getParam(5)->value();
    }

    if (request->hasParam("name")) {
      fname = request->getParam(1)->value();
    }

    if (request->hasParam("apartment")) {
      apartment = request->getParam(3)->value();
    }

    if (request->hasParam("street")) {
      street = request->getParam(2)->value();
    }
    if (request->hasParam("disablity")){
        disablity = request->getParam(4)->value();
      }
    
    transmit("<PHONE>" + phone + "</PHONE><NAME>" + fname + "</NAME><STREET>"+ street + "</STREET><APARTMENT>" + apartment + "</APARTMENT><MESSAGE>" +Message + "</MESSAGE><DISABLITY>" + disablity +"</DISABLITY>");
    request->send(200, "text/html",part1+part2+part3+part4+part5);
  });


  server.on("/log", HTTP_GET, [](AsyncWebServerRequest * request) {
    int paramsNr = request->params();
    AsyncWebParameter* p = request->getParam(0);
    request->send(200, "text/html", "<html><head><meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no' /></head><body><div style='width:100%;height:100%;font-size:20px;'><textarea style='width:100%;height:70%;margin:5px;font-size:20px;background-color:grey;font-color:white;'>" + receiveMsg + "</textarea></div></body></html>");
  });

  server.begin();
}

void loop() {
  String received = "";
  if (!duplex && RfSerial.available()) {
    received = char(RfSerial.read());
    receiveMsg += received;
  }

  /* Read from Serial to transmit */
  if (Serial.available() > 0) {
    String incomingBytes;
    incomingBytes = Serial.readString();
    transmit(incomingBytes);
    Serial.flush();
  }
  delay(500);
}
