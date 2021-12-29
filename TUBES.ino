  //membuka library servo 
#include <ESP32Servo.h>
#include <DHT.h>
#define DHTTYPE DHT11
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 4); // Alamat I2C, jumlah kolom, jumlah baris
#define DHTPIN 13
#define relay_on 1
#define relay_off 0
#define relay_lamp 25
#define relay_fan1 26
#define relay_fan2 27
#define trig 12
#define echo 14
#define trig1 5
#define echo2 4
#define servopakan 32 
#define servominum 33
#define buzzer 23

#define TAMPIL_SERIAL true

Servo myservo1;
Servo myservo2;

DHT dht(DHTPIN, DHTTYPE);
const byte KARAKTER_DERAJAT = 0;
byte derajat[] = {
  B00111,
  B00101,
  B00111,
-  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
int hum = 0, temp = 0;
long duration,durasi; // variable for the duration of sound wave travel
int distance,jarak; // variable for the distance measurement

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  myservo1.attach(servopakan);
  myservo2.attach (servominum);
pinMode(trig, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echo, INPUT);
  pinMode (trig1,OUTPUT);
  pinMode (echo2,INPUT);
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
  pinMode (buzzer,OUTPUT);



}

void loop() {
  

  
 // Clears the trigPin condition
  digitalWrite(trig, LOW);
  digitalWrite (trig1,LOW);
  delay(500);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trig, HIGH);
  digitalWrite (trig1,HIGH);
  delay(500);
  digitalWrite(trig, LOW);
  digitalWrite(trig1,LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo, HIGH);
  durasi  = pulseIn (echo2,HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  jarak    = durasi    * 0.034/ 2;
  // Displays the distance on the Serial Monitor
hum  = dht.readHumidity();
temp = dht.readTemperature();

  if (isnan(hum) || isnan(temp)) {
#if TAMPIL_SERIAL
  Serial.println(F("Failed to read from DHT sensor!"));
#endif
    return;
  }

  //TAMPIL DI LCD 
 
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
  

//==================================================================
// penggerak lampu dan fan DHT 11
if (temp >=30 && temp <=32){

  delay(10000);
digitalWrite (relay_lamp,relay_on);
  digitalWrite (relay_fan1,relay_off);
digitalWrite (relay_fan2,relay_on);
Serial.println(" SUHU HANGAT( LAMPU PENGHANGAT MENYALA DAN FAN MENYALA) ");
Serial.println ("\n");
}
else if (temp >=33){
  delay(10000);
digitalWrite (relay_lamp,relay_off);  
digitalWrite (relay_fan1,relay_on);
digitalWrite (relay_fan2,relay_on);
Serial.println ("SUHU PANAS (LAMPU PENGHANGAT MATI DAN FAN MENYALA");

}


else if (temp <=29){
  delay(10000);
digitalWrite(relay_lamp,relay_on);
  digitalWrite (relay_fan1,relay_off);
digitalWrite (relay_fan2,relay_off);
Serial.println(" SUHU DINGIN( LAMPU PENGHANGAT MENYALA DAN FAN MATI) ");
}
  
// ========================================================================
delay (5000);
  lcd.clear ();
lcd.setCursor(0,0);
lcd.print ("PAKAN:");
lcd.setCursor (0,1);
lcd.print ("AIR :");

// ultrasonic pakan ayam
if(  (distance >=0) && (distance <= 7)   ) 
{
Serial.println("PAKAN MASIH PENUH");
lcd.setCursor (6,0);
lcd.print ("PENUH");
} 
else if(  (distance > 7) && (distance <= 13)  ) 
{
Serial.println (" pakan cukup penuh ");
 lcd.setCursor (6,0);
lcd.print ("SEDANG");

}
else if(  (distance > 13) && (distance <= 15)  ) 
{
 
Serial.println ("pakan sudah hampir habis ");
lcd.setCursor (6,0);
lcd.print ("SEDIKIT");
} 
else if (distance>16){

//parameter led menyala  
  digitalWrite(buzzer,HIGH);
  delay(10000);
digitalWrite (buzzer,LOW);


  Serial.println ("Pakan sudah habis ");
  lcd.setCursor (6,0);
lcd.print ("HABIS");
}

 //minum ayam 


 if ((jarak >=0) && (jarak <=3)){
   Serial.println ("AIR PENUH ");
   lcd.setCursor (6,1);
   lcd.print ("PENUH");
}

else if ((jarak>=4 ) && (jarak <=7)){
 lcd.setCursor (6,1);
   Serial.println ("AIR CUKUP");
   lcd.setCursor (6,1);
   lcd.print ("CUKUP");
}

else if (jarak >=8 && jarak<=14){

   Serial.println ("AIR HAMPIR HABIS");
   lcd.setCursor (6,1);
   lcd.print ("SEDIKIT");

 
  
}


else {


lcd.setCursor (6,1);
   lcd.print ("HABIS"); 

   digitalWrite(buzzer,HIGH);
delay(5000);
digitalWrite (buzzer,LOW);
}


delay(5000);
lcd.clear();



#if TAMPIL_SERIAL
  Serial.println("\n================");
  Serial.print(F("Humidity: ")); Serial.print(hum);
  Serial.println("%");
  Serial.print(F("Temperature: ")); Serial.print(temp);
  Serial.println(F("°C "));
#endif

}
