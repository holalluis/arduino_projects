// llibreria - sensor temperatura i humitat
#include <DHT.h>

// llibreria - pantalla LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 5
#define DHTTYPE DHT11
#define LLUM_PIN 0
#define HUM_TERRA_PIN 1
#define SMS_VCC 7
#define SMS_GND 6

int tempDHT;
int humDHT;
int llum;
int humterra;

//inic sensor DHT (temperatura i humitat)
DHT dht(DHTPIN, DHTTYPE);

//inici pantalla LCD amb 20 caràcters i 4 línies útils
LiquidCrystal_I2C lcd(0x27, 20, 4);

//display serial monitor
void setup(){
  Serial.begin(9600);
  Serial.println("Hola, Lluís, començo a mesurar");
  pinMode(SMS_VCC,OUTPUT);
  pinMode(SMS_GND,OUTPUT);
  dht.begin();
  lcd.begin();
}

void loop(){
  delay(2000);
  readSensors();
  printData();
  showDataLCD();
}

//lectura sensors
void readSensors(void){
  tempDHT = dht.readTemperature();
  humDHT = dht.readHumidity();
  llum = getllum(LLUM_PIN);
  humterra = gethumterra();
}

int getllum(int anaPin){
  int anaValue = 0;
  for(int i = 0; i < 10; i++) {
    anaValue += analogRead(anaPin);
    delay(50);
  }
  anaValue = anaValue/10;
  anaValue = map(anaValue, 1023, 0, 0, 100); //fosc:0%  ==> llum 100%
  return anaValue;
}

int gethumterra(){
  int anaValue = 0;
  for(int i = 0; i < 10; i++) {
    digitalWrite(SMS_VCC,LOW);   // drive a current through the divider in one direction
    digitalWrite(SMS_GND,HIGH);
    delay(1000);   // wait a moment for capacitance effects to settle
    anaValue += analogRead(HUM_TERRA_PIN);

    digitalWrite(SMS_VCC,HIGH);   // reverse the current
    digitalWrite(SMS_GND,LOW);
    delay(1000);   // give as much time in 'reverse' as in 'forward'
    digitalWrite(SMS_VCC,LOW);   // stop the current
  }
  anaValue = anaValue/10;
  anaValue = map(anaValue, 1023, 0, 0, 100);
  return anaValue;
}

//escriure valors mesurats al serial
void printData(void){
  Serial.print("   Temperatura DHT ==> ");
  Serial.print(tempDHT);
  Serial.print("oC  Humitat DHT ==> ");
  Serial.print(humDHT);
  Serial.print("%  Llum ==> ");
  Serial.print(llum);
  Serial.print("%  Humitat terra ==> ");
  Serial.print(humterra);
  Serial.println("%");
}

//escriure valors mesurats pantalla LCD
void showDataLCD(void){
  lcd.setCursor(0,0);
  lcd.print("Hola, Lluis");
  lcd.setCursor(0,1);
  lcd.print("                    ");
  lcd.setCursor(0,2);
  lcd.print("Temp:");
  lcd.print(tempDHT);
  lcd.print("oC Humit:");
  lcd.print(humDHT);
  lcd.print("%  ");
  lcd.setCursor(0,3);
  lcd.print("Llum:");
  lcd.print(llum);
  lcd.print("%");
  lcd.print("  Terra:");
  lcd.print(humterra);
  lcd.print("%");
  lcd.setCursor(0,0);
}
