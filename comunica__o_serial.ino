//inclusão das bibliotecas necessárias
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>

//definição das porta
#define DHT11_sensor A1
#define DHTTYPE    DHT11
#define humity_sensor A0
int rs = 12, e = 11, DB4 = 5, DB5 = 4, DB6 = 3, DB7 = 2;
LiquidCrystal lcd(rs, e, DB4, DB5, DB6, DB7);
DHT_Unified dht(DHT11_sensor, DHTTYPE);

//cabeçalhos das funções
void WriteTemperatureAndHumityFromEnvairoment();
void WriteHumityFromGround(int humity, int _Maximum_Ground_Humity, int _Minimum_Ground_Humity);

//seta as entradas e saídas e inicializa o LCD e o sensor de temperatura e umidade
void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  pinMode(humity_sensor, INPUT);
  digitalWrite(13, LOW);
  dht.begin();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);

}

//declaração das variáveis globais
int Maximum_Ground_Humity = -1, Minimum_Ground_Humity = -1;
char Maximum_Humity[10] = "111", Minimum_Humity[10] = "111";
int estado = '0';
bool LED = true;
unsigned long temporizer = millis();

//configuração de solo seco e molhado
int dry_ground = 1023, wet_ground = 390;


void loop() {
  //Escreve no dysplay LCD a umidade e a Temperatura do meio
  WriteTemperatureAndHumityFromEnvairoment();

  //estado 1 se as informações de máxima e mínima umidade do solo estiverem corretas

  if (Serial.available() > -1)
  {


    

    //lê do computador qual será a tarefa a ser feita
    if (estado == '1')
    {
      //atualiza a umidade do solo a cada 1 segundos. Pisca um LED para saber que está funcionando
      if ((millis() - temporizer) > 1000)
      {
        int humity;
        digitalWrite(13, HIGH);

        //captura qual é a umidade atual do solo em %
        humity = constrain(analogRead(humity_sensor), wet_ground, dry_ground);
        humity = map (humity, wet_ground, dry_ground, 100, 0);
        //Envia a umidade atual do solo e se precisa de mais água ou não para o computador
        WriteHumityFromGround(humity, Maximum_Ground_Humity, Minimum_Ground_Humity);

        digitalWrite(13, LED);
        LED = !LED;
        temporizer = millis();

      }


    }

    int aux = estado;
    estado = Serial.read();

    //estado 3 para quando o usuário quiser parar o circuito e depois voltar para o estado anterior
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
    //estado 2 para atualizar os dados de máxima e de mínima. Caso não estiverem corretos, o programa exibirá uma mensagem e não exibirá a umidade do solo
    if ((estado == '2'))
    {
      Maximum_Ground_Humity = Serial.parseInt();
      Minimum_Ground_Humity = Serial.parseInt();


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
//função que lê e mostra no LCD as informações de temperatura e umidade do meio
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

//função que envia os dados de umidade do solo e se precisa mais água para o computador
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
