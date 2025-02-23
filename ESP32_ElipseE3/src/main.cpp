//#define BLYNK_TEMPLATE_ID "ID Do template"
//#define BLYNK_TEMPLATE_NAME "nome do template blynk"
//#define BLYNK_AUTH_TOKEN "Token blink"
//Caso queira usar o BLYNK para um monitoramento pelo celular, descomente esta parte e adicione suas credenciais

#include <WiFi.h>
#include <ModbusIP_ESP8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BlynkSimpleEsp32.h>
#include <math.h>

unsigned long previousMillis = 0;
const long interval = 5000;
const char* ssid = "SUA REDE";
const char* password = "SUA SENHA DE REDE";

// Configuração do ModbusTCP
ModbusIP mb;
const int REG_TEMPERATURA = 40001;
const int REG_CO2 = 40002;
const int REG_RECONHECIMENTO = 40003; // Botão de reconhecimento no Elipse

// Configuração do DS18B20
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Configuração do MQ-135
#define PIN_MQ135 11
#define RLOAD 10
#define RZERO 80
#define PARA 116.6020682
#define PARB 2.769034857

// Configuração do buzzer passivo
#define PIN_BUZZER 41
#define BUZZER_CHANNEL 0 //Canal PWM para o buzzer
bool buzzerAtivado = false; // Estado do alarme
bool reconhecimentoFeito = false; // Estado do reconhecimento

void setup() {
  Serial.begin(115200);

  // Conectar ao WiFi
  Serial.print("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
  Serial.print("IP da ESP32: ");
  Serial.println(WiFi.localIP());

  // Inicializar Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  // Inicializar Modbus como servidor
  mb.server();
  mb.addHreg(REG_TEMPERATURA, 0);
  mb.addHreg(REG_CO2, 0);
  mb.addHreg(REG_RECONHECIMENTO, 0); // Botão de reconhecimento

  // Inicializar sensor de temperatura
  sensors.begin();
  sensors.setResolution(12);

  // Configurar canal PWM para o buzzer
  ledcSetup(BUZZER_CHANNEL, 2000, 8); // Frequência de 2kHz, resolução de 8 bits
  ledcAttachPin(PIN_BUZZER, BUZZER_CHANNEL);
  ledcWrite(BUZZER_CHANNEL, 0); // Inicialmente desligar o buzzer
}

void loop() {
  Blynk.run();
  sensors.begin();
  // Obter temperatura
  sensors.requestTemperatures();
  float temperatura = sensors.getTempCByIndex(0);

  if (temperatura == DEVICE_DISCONNECTED_C) {
    Serial.println("Sensor de temperatura desconectado!");
  } else {
    Serial.printf("Temperatura: %.2f °C\n", temperatura);
    mb.Hreg(REG_TEMPERATURA, static_cast<int>(temperatura * 100));
    Blynk.virtualWrite(V1, temperatura);

    // Lógica para ativar o buzzer
    if (temperatura > 30 && !reconhecimentoFeito && !buzzerAtivado) {
      ledcWrite(BUZZER_CHANNEL, 128); // Ativar buzzer
      buzzerAtivado = true;
    }

    // Verificar botão de reconhecimento no Elipse
    if (mb.Hreg(REG_RECONHECIMENTO) == 1) {
      ledcWrite(BUZZER_CHANNEL, 0); // Desligar buzzer
      reconhecimentoFeito = true;   // Registrar que o reconhecimento foi feito
      buzzerAtivado = false;        // Resetar estado do buzzer
      mb.Hreg(REG_RECONHECIMENTO, 0); // Resetar registrador no Elipse
    }

    // Liberar alarme apenas quando a temperatura abaixar para menos de 30
    if (temperatura <= 30) {
      reconhecimentoFeito = false; // Permitir que o alarme seja reativado no futuro
      buzzerAtivado = false;       // Resetar o estado do buzzer
    }
  }

  // Leitura do MQ-135
  int sensorValue = analogRead(PIN_MQ135);
  float sensorVoltage = (sensorValue / 4095.0) * 3.3;
  float sensorResistance = ((3.3 - sensorVoltage) / sensorVoltage) * RLOAD;
  float ppm = PARA * pow((sensorResistance / RZERO), -PARB);

  Serial.printf("CO₂: %.2f ppm\n", ppm);
  mb.Hreg(REG_CO2, static_cast<int>(ppm));
  Blynk.virtualWrite(V2, ppm);

  // Atualizar ModbusTCP
  mb.task();
}
