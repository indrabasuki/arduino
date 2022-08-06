#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h> 
#include <SPI.h>
#include <RFID.h>
#include <Wire.h>


#define SS_PIN D4
#define RST_PIN D3

LiquidCrystal_I2C lcd(0x27, 20, 4);
RFID rfid(SS_PIN, RST_PIN); 

// Setup variables:
    int serNum0;
    int serNum1;
    int serNum2;
    int serNum3;
    int serNum4;


const char* ssid     = "CASPER";
const char* password = "Admin123";
const char* host = "192.168.182.83";

String hostMode = "http://192.168.182.83/sistem_kehadiran/public/rfid/mode";
String ModeAlat="";

WiFiClient client;
const int httpPort = 80;
String url;

unsigned long timeout;

const int id_device = 22;
  
void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("SISTEM KEHADIRAN");
  lcd.setCursor(3,1);      
  lcd.print("BERBASIS RFID");
  lcd.setCursor(7,2);      
  lcd.print("SMPIT");  
  lcd.setCursor(1,3);      
  lcd.print("RAUDHATUL MUTTAQIN");  
  delay (1000);
  Serial.println();

  SPI.begin(); 
  rfid.init();
  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sedang Menghubungkan");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

   ModeDevice();
}


void ModeDevice(){
  HTTPClient http;

  Serial.print("Request Link:");
  Serial.println(hostMode);  
  http.begin(hostMode);
  
  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload from server

  Serial.print("Response Code:"); //200 is OK
  Serial.println(httpCode);       //Print HTTP return code
  Serial.print("Returned data from Server:");
  Serial.println(payload);    //Print request response payload
  
  if(httpCode == 200)
  {
    DynamicJsonDocument doc(1024);
  
   // Parse JSON object
    auto error = deserializeJson(doc, payload);
    if (error) {
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(error.c_str());
      return;
    }
  
    // Decode JSON/Extract values
    String responMode = doc["mode"].as<String>();
 
    Serial.println();    
    Serial.print("mode : ");
    Serial.println(responMode);
  
    Serial.println("-------------------");
    Serial.println();

    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("SISTEM KEHADIRAN");
    if (responMode == "1"){
      ModeAlat = "SCAN";
      Serial.println("SCAN RFID");
      lcd.setCursor(3,1);
      lcd.print("TAB KARTU ANDA");
    }else{
      ModeAlat = "ADD";
      Serial.println("ADD RFID");
      lcd.setCursor(1,1);
      lcd.print("MODE =TAMBAH KARTU");
      lcd.setCursor(0,2);
      lcd.print("");
      lcd.setCursor(2,3);
      lcd.print("TAB KARTU ANDA");

    }
  }
  else
  {
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Error in response");
    Serial.println("Error in response");
  }
  http.end();
  delay(100);
}


void loop() {
   if (ModeAlat == "SCAN"){
      if (rfid.isCard()) {
        if (rfid.readCardSerial()) {
            if (rfid.serNum[0] != serNum0
                && rfid.serNum[1] != serNum1
                && rfid.serNum[2] != serNum2
                && rfid.serNum[3] != serNum3
                && rfid.serNum[4] != serNum4
            ) {
                /* With a new cardnumber, show it. */
                Serial.println(" ");
                Serial.println("Card found");
                serNum0 = rfid.serNum[0];
                serNum1 = rfid.serNum[1];
                serNum2 = rfid.serNum[2];
                serNum3 = rfid.serNum[3];
                serNum4 = rfid.serNum[4];
               
                //Serial.println(" ");
                Serial.println("Cardnumber:");
                Serial.print("Dec: ");
                Serial.print(rfid.serNum[0],DEC);
                Serial.print(", ");
                Serial.print(rfid.serNum[1],DEC);
                Serial.print(", ");
                Serial.print(rfid.serNum[2],DEC);
                Serial.print(", ");
                Serial.print(rfid.serNum[3],DEC);
                Serial.print(", ");
                Serial.print(rfid.serNum[4],DEC);
                Serial.println(" ");
                        
                Serial.print("Hex: ");
                Serial.print(rfid.serNum[0],HEX);
                Serial.print(", ");
                Serial.print(rfid.serNum[1],HEX);
                Serial.print(", ");
                Serial.print(rfid.serNum[2],HEX);
                Serial.print(", ");
                Serial.print(rfid.serNum[3],HEX);
                Serial.print(", ");
                Serial.print(rfid.serNum[4],HEX);
                Serial.println(" ");

                String UIDcard = String(rfid.serNum[0],HEX) +"-"+ String(rfid.serNum[1],HEX) +"-"+ String(rfid.serNum[2],HEX) +"-"+ String(rfid.serNum[3],HEX) +"-"+ String(rfid.serNum[4],HEX);
                
                Serial.print("connecting to ");
                Serial.println(host);
              
                if (!client.connect(host, httpPort)) {
                  Serial.println("connection failed");
                  //return;
                }
              
              // We now create a URI for the request
                url = "/sistem_kehadiran/public/rfid/";
                url += id_device;
                url += "/";
                url += UIDcard;
                
                Serial.print("Requesting URL: ");
                Serial.println(url);
              
              // This will send the request to the server
                client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                             "Host: " + host + "\r\n" + 
                             "Connection: close\r\n\r\n");
                timeout = millis();
                while (client.available() == 0) {
                  if (millis() - timeout > 5000) {
                    Serial.println(">>> Client Timeout !");
                    client.stop();
                    return;
                  }
                }
              
              // Read all the lines of the reply from server and print them to Serial
                while(client.available()){
                  String line = client.readStringUntil('\r');
                  Serial.print(line);
                }
              
                Serial.println();
                Serial.println("closing connection");
                Serial.println();

             } else {
               /* If we have the same ID, just write a dot. */
               Serial.print(".");
             }
          }
    }
    
    rfid.halt();
   }else{
    if (rfid.isCard()) {
        if (rfid.readCardSerial()) {
            if (rfid.serNum[0] != serNum0
                && rfid.serNum[1] != serNum1
                && rfid.serNum[2] != serNum2
                && rfid.serNum[3] != serNum3
                && rfid.serNum[4] != serNum4
            ) {
                /* With a new cardnumber, show it. */
                Serial.println(" ");
                Serial.println("Card found");
                serNum0 = rfid.serNum[0];
                serNum1 = rfid.serNum[1];
                serNum2 = rfid.serNum[2];
                serNum3 = rfid.serNum[3];
                serNum4 = rfid.serNum[4];
               
                //Serial.println(" ");
                Serial.println("Cardnumber:");
                Serial.print("Dec: ");
                Serial.print(rfid.serNum[0],DEC);
                Serial.print(", ");
                Serial.print(rfid.serNum[1],DEC);
                Serial.print(", ");
                Serial.print(rfid.serNum[2],DEC);
                Serial.print(", ");
                Serial.print(rfid.serNum[3],DEC);
                Serial.print(", ");
                Serial.print(rfid.serNum[4],DEC);
                Serial.println(" ");
                        
                Serial.print("Hex: ");
                Serial.print(rfid.serNum[0],HEX);
                Serial.print(", ");
                Serial.print(rfid.serNum[1],HEX);
                Serial.print(", ");
                Serial.print(rfid.serNum[2],HEX);
                Serial.print(", ");
                Serial.print(rfid.serNum[3],HEX);
                Serial.print(", ");
                Serial.print(rfid.serNum[4],HEX);
                Serial.println(" ");

                String UIDcard = String(rfid.serNum[0],HEX) +"-"+ String(rfid.serNum[1],HEX) +"-"+ String(rfid.serNum[2],HEX) +"-"+ String(rfid.serNum[3],HEX) +"-"+ String(rfid.serNum[4],HEX);
                
                Serial.print("connecting to ");
                Serial.println(host);
              
                if (!client.connect(host, httpPort)) {
                  Serial.println("connection failed");
                  //return;
                }
              
              // We now create a URI for the request
                url = "/sistem_kehadiran/public/rfid/add/";
                url += id_device;
                url += "/";
                url += UIDcard;
                
                Serial.print("Requesting URL: ");
                Serial.println(url);
              
              // This will send the request to the server
                client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                             "Host: " + host + "\r\n" + 
                             "Connection: close\r\n\r\n");
                timeout = millis();
                while (client.available() == 0) {
                  if (millis() - timeout > 5000) {
                    Serial.println(">>> Client Timeout !");
                    client.stop();
                    return;
                  }
                }
              
              // Read all the lines of the reply from server and print them to Serial
                while(client.available()){
                  String line = client.readStringUntil('\r');
                  Serial.print(line);
                }
              
                Serial.println();
                Serial.println("closing connection");
                Serial.println();

             } else {
               /* If we have the same ID, just write a dot. */
               Serial.print(".");
             }
          }
    }
    
    rfid.halt();  
    
    
   }

}
