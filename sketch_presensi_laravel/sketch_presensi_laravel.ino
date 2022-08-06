#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiClient.h>

WiFiClient wifiClient;


//GANTI PAKE PUNYA MASING-MASING
const char* ssid = "CASPER";
const char* password = "Admin123";

//IP LAPTOP
const char* host = "192.168.182.234";

#define LED_PIN 15 //D8
#define BTN_PIN 16  //D1

//sediakan variabel untuk RFID
#define SDA_PIN 2  //D4
#define RST_PIN 0  //D3
LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(SDA_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);

  //Initial LCD
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();

  //setting koneksi wifi
  WiFi.hostname("NodeMCU");
  WiFi.begin(ssid, password);

  //cek koneksi wifi
  while(WiFi.status() != WL_CONNECTED)
  {
    //progress sedang mencari WiFi
    delay(500);
    Serial.print(".");
    lcd.setCursor(0, 0);
    lcd.print("Silahkan Tunggu");


  }

  Serial.println("Wifi Connected");
  Serial.println("IP Address : ");
  Serial.println(WiFi.localIP());
  lcd.setCursor(0, 0);
  lcd.print(" WIFI TERHUBUNG ");
  delay(2000);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, OUTPUT);

  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Dekatkan Kartu RFID Anda ke Reader");
  Serial.println();
  lcd.setCursor(0, 0);
  lcd.print(" TEMPELKAN KARTU ");
  lcd.setCursor(0, 1);
  lcd.print("    KE READER  ");
}

void loop() {
   if(digitalRead(BTN_PIN)==1)    
    {
     Serial.println("OK");
     digitalWrite(LED_PIN, HIGH);
     while(digitalRead(BTN_PIN)==1) ;   
     String getData, url ;
     HTTPClient http ;
     url = "http://192.168.182.234/presensi/public/rfid/mode";
     http.begin(wifiClient,url);
     int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
     http.end();
    }
  digitalWrite(LED_PIN, LOW);


  delay(2000);
}
