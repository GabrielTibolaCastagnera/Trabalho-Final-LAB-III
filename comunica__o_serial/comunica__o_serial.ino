#include <DHT.h>
#include <DHT_U.h>
#define DHT11_sensor A1
#define DHTTYPE    DHT11
DHT_Unified dht(DHT11_sensor, DHTTYPE);
void WriteTemperatureAndHumityFromEnvairoment();
void setup() {
  Serial.begin(57600);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  dht.begin();

}
void loop() {
  if (Serial.available() > 0)
  {
    char estado = Serial.parseInt();
    if (estado == 1)
    {
      char agora =1;
      int cont = 0;
      while (agora != '0')
      {
        digitalWrite(13, HIGH);
        WriteTemperatureAndHumityFromEnvairoment();
        delay(500);
        digitalWrite(13, LOW);
        delay(500);
        agora = Serial.read();

      }
    }
    //
    //      delay(1000);
  }
}
void WriteTemperatureAndHumityFromEnvairoment()
{
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading the envairoment temperature!"));
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
}
