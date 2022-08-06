#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h> 

const char* ssid     = "CASPER";
const char* password = "Admin123";
const char* host = "192.168.59.83";
String hostRellay1 = "http://192.168.59.83/nodemcu_rellay/control/read1";
String hostRellay2 = "http://192.168.59.83/nodemcu_rellay/control/read2";
String hostRellay3 = "http://192.168.59.83/nodemcu_rellay/control/read3";
String hostRellay4 = "http://192.168.59.83/nodemcu_rellay/control/read4";

const int relay1 = 5;
const int relay2 = 4;
const int relay3 = 0;
const int relay4 = 2;

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);

}

void loop() {

  WiFiClient client;
  const int port=80;

  if(!client.connect(host, port))
  {
     Serial.println("Gagal koneksi ke server");
     return;
  }
  HTTPClient http;
  Serial.print("Request Link:");
  Serial.println(hostRellay1);  
  http.begin(hostRellay1);
  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload from server
  Serial.print("Res:");
  Serial.print(payload);
  http.end();
  digitalWrite(relay1,payload.toInt());

  HTTPClient http2;
  Serial.print("Request Link:");
  Serial.println(hostRellay2);  
  http2.begin(hostRellay2);
  int httpCode2 = http2.GET();            //Send the request
  String payload2 = http2.getString();    //Get the response payload from server
  Serial.print("Res:");
  Serial.print(payload2);
  http2.end();
  digitalWrite(relay2,payload2.toInt());

  HTTPClient http3;
  Serial.print("Request Link:");
  Serial.println(hostRellay3);  
  http3.begin(hostRellay3);
  int httpCode3 = http3.GET();            //Send the request
  String payload3 = http3.getString();    //Get the response payload from server
  Serial.print("Res:");
  Serial.print(payload3);
  http3.end();
  digitalWrite(relay3,payload3.toInt());

  HTTPClient http4;
  Serial.print("Request Link:");
  Serial.println(hostRellay4);  
  http4.begin(hostRellay4);
  int httpCode4 = http4.GET();            //Send the request
  String payload4 = http4.getString();    //Get the response payload from server
  Serial.print("Res:");
  Serial.print(payload4);
  http4.end();
  digitalWrite(relay4,payload4.toInt());

}
