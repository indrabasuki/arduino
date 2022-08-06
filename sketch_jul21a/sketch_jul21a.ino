#include <LiquidCrystal_I2C.h>
 
// Konstruk object LCD dengan alamat I2C
// Ganti 0x3F sesuai dengan alamat I2C modul kalian
// Jika tidak tahu dapat menggunakan LCD I2C Scanner
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
void setup() {

 Serial.begin(9600);
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
 

  lcd.setCursor(0, 0);
  lcd.print("== Sistem Presensi ==");
  lcd.setCursor(0, 1);      
  lcd.print("INSAN MULIA INFORMATIKA");

  Serial.setTimeout(10000);

  lcd.setCursor(0, 0);
  lcd.print("Pilih Menu ==>>");
  lcd.setCursor(0, 1);      
  lcd.print("1.Mode Absen   2.Daftar Pengguna");
}
 
void loop() {
  //Kode loop silahkan disesuaikan
  //...
 
}
