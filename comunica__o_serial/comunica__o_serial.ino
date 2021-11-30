#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>
#define DHT11_sensor A1
#define DHTTYPE    DHT11
#define humity_sensor A0
int rs = 12, e = 11, DB4 = 5, DB5 = 4, DB6 = 3, DB7 = 2;
LiquidCrystal lcd(rs, e, DB4, DB5, DB6, DB7);
DHT_Unified dht(DHT11_sensor, DHTTYPE);
void WriteTemperatureAndHumityFromEnvairoment();
void WriteHumityFromGround(int humity, int _Maximum_Ground_Humity, int _Minimum_Ground_Humity);
void setup() {
  Serial.begin(57600);
  pinMode(13, OUTPUT);
  pinMode(humity_sensor, INPUT);
  digitalWrite(13, LOW);
  dht.begin();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);

}
int Maximum_Ground_Humity = -1, Minimum_Ground_Humity = -1;
char Maximum_Humity[10] = "111", Minimum_Humity[10] = "111";
char estado = '0';
bool LED = true;
unsigned long temporizer = millis();
void loop() {

  WriteTemperatureAndHumityFromEnvairoment();
  if (estado == '1')
  {
    if ((millis() - temporizer) > 1000)
    {
      int humity;
      digitalWrite(13, HIGH);
      WriteTemperatureAndHumityFromEnvairoment();
      humity = constrain(analogRead(humity_sensor), 390, 1023);
      humity = map (humity, 390, 1023, 100, 0);
      WriteHumityFromGround(humity, Maximum_Ground_Humity, Minimum_Ground_Humity);
      digitalWrite(13, LED);
      LED = !LED;
      temporizer = millis();

    }


  }
  if (Serial.available() > 0)
  {
    char aux = estado;
    estado = Serial.read();
    
    if (estado == '3')
    {
      estado = aux;
      while (estado != '3')
      {
        Serial.println("PARADO!");
        digitalWrite(13, HIGH);
        delay(250);
        digitalWrite(13, LOW);
        delay(250);
        
        estado = Serial.read();
      }
      estado = aux;
    }
    if ((estado == '2'))
    {
      Maximum_Ground_Humity = Serial.read();
      Minimum_Ground_Humity = Serial.read();
      
      
      if (Maximum_Ground_Humity >= 0 && Maximum_Ground_Humity <= 100 && Minimum_Ground_Humity <= Maximum_Ground_Humity && Minimum_Ground_Humity >= 0)
      {
        estado = '1';
      }
      else
      {
        estado = '0';
      }
    }
    if (estado == '0')
    {
      Serial.println("Error on reading maximum and minimum ground humity");
    }





  }


}
void WriteTemperatureAndHumityFromEnvairoment()
{
  sensors_event_t eventT, eventH;
  dht.temperature().getEvent(&eventT);
  dht.humidity().getEvent(&eventH);
  lcd.setCursor(0, 0);
  if (isnan(eventT.temperature)) {
    lcd.println(F("Error reading the envairoment temperature!"));
  }
  else {

    lcd.print("Temperature C:");
    lcd.println(int(eventT.temperature));
  }
  if (isnan(eventH.relative_humidity)) {
    lcd.println(F("Error reading the envairoment humidity!"));
  }
  else {
    lcd.setCursor(0, 1);
    lcd.print("Humity %:");
    lcd.print(int(eventH.relative_humidity));
  }
}
void WriteHumityFromGround(int humity, int _Maximum_Ground_Humity, int _Minimum_Ground_Humity)
{
  Serial.print("Ground Humity: ");
  Serial.print(humity);
  Serial.print("%\n");
  if (humity <= _Minimum_Ground_Humity)
  {
    Serial.println("Too few Humity!");
  }
  else if (humity >= _Maximum_Ground_Humity)
  {
    Serial.println("Too much Humity!");
  }
  else
  {
    Serial.println(" Ideal Humity!");
  }
}
