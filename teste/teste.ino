#include <DHT.h>
#include <DHT_U.h>
#define LED 13
#define DHT11_sensor A1
#define humity_sensor A0
#define DHTTYPE    DHT11
#define MAX_humity 50
#define MIN_humity 40
DHT_Unified dht(DHT11_sensor, DHTTYPE);
void setup()
{
  Serial.begin(9600);
  pinMode(humity_sensor, INPUT);
  pinMode(13, OUTPUT);
  dht.begin();

}
unsigned long sensor_period_time = millis(), LED_period_time = millis();
void loop()
{

  if (millis() - sensor_period_time >= 5000)
  {
    int humity = constrain(analogRead(humity_sensor),390,1023);
    humity = map (humity, 390, 1023, 100, 0);
    Serial.print("Umidade do solo: ");
    Serial.print(humity);
    Serial.print("%\n");
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.println(F("Error readingthe envairoment temperature!"));
    }
    else {
      Serial.print(F("Temperature: "));
      Serial.print(event.temperature);
      Serial.println(F("°C"));
    }
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println(F("Error reading the envairoment humidity!"));
    }
    else {
      Serial.print(F("Humidity: "));
      Serial.print(event.relative_humidity);
      Serial.println(F("%"));
    }
    if(humity <= MIN_humity)
    {
      digitalWrite(LED, HIGH);
    }
    else if(humity >= MAX_humity)
    {
      digitalWrite(LED, LOW);
    }
    
    sensor_period_time = millis();
    
  }
  
}
