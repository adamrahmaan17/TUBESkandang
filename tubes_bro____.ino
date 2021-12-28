   //membuka library servo 
#include "DHT.h"
#define DHTTYPE DHT11
#include "RTC_DS3231_RP.h" 
#include <Wire.h>
#include <Servo.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 4); // Alamat I2C, jumlah kolom, jumlah baris

#define MakanPertama                DateTime(0, 1, 1,  7, 0, 0, 0)//Disetting jam 7 pagi >> waktu makan pertama
#define MakanKedua                  DateTime(0, 1, 1,  19, 0, 0, 0)//Disetting jam 7 malam >> waktu makan kedua
#define servo1 32
#define servo2 33
#define ledmerah 23
#define DHTPIN 13
#define relay_on 1
#define relay_off 0
#define relay_lamp 25
#define relay_fan1 26
#define relay_fan2 27
#define trig 12
#define echo 14
#define trig1 4
#define echo2 5
#define waktuservo_ON               2000//milidetik
#define servo_ON                    20//derajat
#define servo_OFF                   60//derajat


Servo iniservo1;
Servo iniservo2;
char weekDay[][7] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu" };   
byte detikSebelumnya;
char buf[17];
 


#define TAMPIL_SERIAL true

DHT dht(DHTPIN, DHTTYPE);
const byte KARAKTER_DERAJAT = 0;
byte derajat[] = {
  B00111,
  B00101,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
int hum = 0, temp = 0;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
     iniservo1.attach(servo1);
    iniservo2.attach (servo2);
  iniservo1.write(servo_OFF);
  iniservo2.write(servo_OFF);
  rtc.begin();

  Serial.println("JAM PAKAN DAN MINUM ");
  sprintf(buf, "Set waktu 1 = %02d:%02d (%lu)", MakanPertama.hour(), MakanPertama.minute(), MakanPertama.get());
  Serial.println(buf);
  sprintf(buf, "Set waktu 2 = %02d:%02d (%lu)", MakanKedua.hour(), MakanKedua.minute(), MakanKedua.get());
  Serial.println(buf);
 



  dht.begin();

  //wifi

  #if TAMPIL_SERIAL
  Serial.println(F("DHT 11 !"));
#endif


  pinMode(relay_lamp,OUTPUT);
    digitalWrite (relay_lamp,relay_off);
  pinMode(relay_fan1,OUTPUT);
  digitalWrite (relay_fan1,relay_off);
  pinMode (relay_fan2,OUTPUT);
  digitalWrite (relay_fan2,relay_off);
  pinMode (trig,OUTPUT);
  pinMode (echo,INPUT);
  pinMode (trig1,OUTPUT);
  pinMode (echo2,INPUT);
  pinMode (ledmerah,OUTPUT);

  



}

void loop() {
  DateTime now = rtc.now();
   if (detikSebelumnya != now.second())
  {
    sprintf(buf, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());

    Serial.print(buf);
    detikSebelumnya = now.second();
    uint32_t epoch = now.get() % 86400;//hanya jam menit detik
   
    if ((epoch == MakanPertama.get()) ||
        (epoch == MakanKedua.get()))
    {
      char buf[17];
      sprintf(buf, "SEKARANG: %02d:%02d", now.hour(), now.minute());//untuk tampilan saat waktu makan
      Serial.println(buf);
      
      //tambah atau kurangi program ini untuk setting berapa kali servo berjalan
      //di setting 2X
      
      iniservo1.write(servo_ON);
      iniservo2.write (servo_ON);
      
      delay(waktuservo_ON);
      
      iniservo1.write(servo_OFF);
      iniservo2.write(servo_OFF);
      delay(1000); 
      
      iniservo1.write(servo_ON);
      iniservo2.write (servo_ON);
      
      delay(waktuservo_ON);
      
      iniservo1.write(servo_OFF);
      iniservo2.write (servo_OFF);
      delay(1000); 
    }
  }

int duration, distance,durasi , jarak ;
digitalWrite (trig, HIGH);
digitalWrite (trig1,HIGH);
duration = pulseIn (echo,HIGH);
durasi  =  pulseIn (echo2,HIGH);


distance = ( duration / 2 ) /29.1;
jarak   = (durasi/2) /29.1; 
  
  delay(2000);  // Delay untuk 
  
  hum = dht.readHumidity();
  temp = dht.readTemperature(); // Baca temperatur dalam celcius (default)

  if (isnan(hum) || isnan(temp)) {
#if TAMPIL_SERIAL
  Serial.println(F("Failed to read from DHT sensor!"));
#endif
    return;
  }

  //TAMPIL DI LCD 
   lcd.clear();
  lcd.init();
  lcd.createChar(KARAKTER_DERAJAT, derajat);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("KELEMBAPAN : ");
  lcd.setCursor(0, 1);
  lcd.print("SUHU :");
  lcd.setCursor(13, 1);
  lcd.print(temp);
  lcd.setCursor(13, 0);
  lcd.print(hum);
  
  if(temp < 10){
    lcd.setCursor(14, 1);
  }else{
    lcd.setCursor(15, 1);
  }
  lcd.write(KARAKTER_DERAJAT);

  if(temp < 10){
    lcd.setCursor(15, 1);
  }else{
    lcd.setCursor(16, 1);
  }
  lcd.print("C");

  if(hum < 10){
    lcd.setCursor(14, 0);
  }else{
    lcd.setCursor(15, 0);
  }
  lcd.print("%");
  delay (10000);

//==================================================================
// penggerak lampu dan fan DHT 11
if (temp >=30 && temp <=32){
delay(20000);
digitalWrite (relay_lamp,relay_on);
  digitalWrite (relay_fan1,relay_off);
digitalWrite (relay_fan2,relay_on);
Serial.println(" SUHU HANGAT( LAMPU PENGHANGAT MENYALA DAN FAN MENYALA) ");
Serial.println ("\n");
}
else if (temp >=33){
delay(20000);
digitalWrite (relay_lamp,relay_off);  
digitalWrite (relay_fan1,relay_on);
digitalWrite (relay_fan2,relay_on);
Serial.println ("SUHU PANAS (LAMPU PENGHANGAT MATI DAN FAN MENYALA");

}


else if (temp <=29){
delay(20000);
digitalWrite(relay_lamp,relay_on);
  digitalWrite (relay_fan1,relay_off);
digitalWrite (relay_fan2,relay_off);
Serial.println(" SUHU DINGIN( LAMPU PENGHANGAT MENYALA DAN FAN MATI) ");
}
  
// ========================================================================
lcd.clear ();
lcd.setCursor(0,0);
lcd.print ("AIR :");
lcd.setCursor (0,1);
lcd.print ("PAKAN :");

// ultrasonic sebagai minum ayam
if(  (distance >=0) && (distance <= 5)   ) 
{
Serial.println("AIR MASIH PENUH");
lcd.setCursor (6,0);
lcd.print ("PENUH");
} 
else if(  (distance > 6) && (distance <= 10)  ) 
{
Serial.println (" air cukup penuh ");
 lcd.setCursor (6,0);
lcd.print ("SEDANG");

}
else if(  (distance > 10) && (distance <= 13)  ) 
{
 
Serial.println ("air sudah hampir habis "):
lcd.setCursor (6,0);
lcd.print ("CUKUP");
} 
else if (distance>14){

//parameter led menyala  
  digitalWrite(ledmerah,HIGH);
  delay(1000);
digitalWrite (ledmerah,LOW);
delay(1000);


  Serial.printl ("air sudah habis "):
  lcd.setCursor (6,0);
lcd.print ("HABIS");
}

 //pakann ayam 

  if ((jarak >=0) && (jarak <=3)){
   Serial.println ("PENUH ");
   lcd.setCursor (6,1);
   lcd.print ("PENUH");
}

if ((jarak>=4 ) && (jarak >=8)){
 lcd.setCursor (6,1);
   Serial.println ("CUKUP");
   lcd.setCursor (6,1);
   lcd.print ("PENUH");
}

else if (jarak >8){
 digitalWrite(ledmerah,HIGH);
  delay(100);
digitalWrite (ledmerah,LOW);
delay(100);

   Serial.println ("HABIS");
   lcd.setCursor (6,1);
   lcd.print ("HABIS");

  if (distance>14){

    digitalWrite(ledmerah,HIGH);
    
  }
}
delay(10000);
lcd.clear();


#if TAMPIL_SERIAL
  Serial.println("\n================");
  Serial.print(F("Humidity: ")); Serial.print(hum);
  Serial.println("%");
  Serial.print(F("Temperature: ")); Serial.print(temp);
  Serial.println(F("°C "));
#endif

}
