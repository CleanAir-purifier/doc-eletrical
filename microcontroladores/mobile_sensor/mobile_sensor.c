#include <stdio.h>
#include "leitura_sensores.h"
#include <Wire.h>
#include <I2C.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <Math.h>
#include <esp_now.h>
#include <WiFi.h>

#define ID "mobile_1" //Modifique para cada mobile que for usar
#define CHANNEL 6  //Canal usado para conexão 
#define INTERVAL 500 //Intervalo entre os envios dos dados do sensores

#define PinA02 13//Pino utilizado pelo MQ2
#define PinA01 12//Pino utilizado pelo MQ131
#define PinNO2 10//Pino utilizado pelo MICS

#define BME_SCK 36 // Porta SCL do I2C
#define BME_SDI 33 // Porta SDA do I2C
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme; // I2C

float bme_ppm;

SteamDGSS02 dgsS02; 

float dgsSO2ppm;

Adafruit_PM25AQI aqi = Adafruit_PM25AQI();

uint32_t lastTimeSent = 0; //Marca quando foi feito o último envio os dados

uint8_t gatewayMacAdress[6] = {0x3C,0x71,0xBF,0x44,0x81,0x81}; // Endereço da ESP do mobile sensor
esp_now_peer_info_t gateway;

//Estrutura de comunicação entre os ESPs com DHT e o gateway
//Os ESPs com DHT irão enviar esta estrutura
//Para que o gateway receba é necessário que no código do gateway tenha outra struct igual a esta
typedef struct {
	char id[30];
	int faz_leitura_ozonio;
	int faz_leitura_no2;
	int faz_leitura_fumaca;
	float faz_leitura_so2;
	float faz_leitura_mp25;
	float faz_leitura_mp100;
	float faz_leitura_covs;
	float faz_leitura_tem;
	float faz_leitura_umi;
	float faz_leitura_pres;
	void bateria;
}SensorData;

//Guarda os valores dos dispositivos lidos para serem enviados para o gateway
SensorData sensorData;

void setup(){

Serial.begin(115200);//Iniciar Serial


setupWiFi(); //Inicializa a WiFi

setupEspNow(); //Inicializa o ESPNow e adiciona o callback de envio


addGatewayAsPeer();//Adiciona o gateway como um peer do ESPNOW

pinMode(PinA02, INPUT); //Definição do pino como entrada do MQ2
pinMode(PinA01, INPUT); //Definição do pino como entrada do MQ131
pinMode(PinNO2, INPUT); //Definição do pino como entrada do MICS

	while (!Serial);
	if (!bme.begin()) {
	while (1);
	}

bme.setTemperatureOversampling(BME680_OS_8X);
bme.setHumidityOversampling(BME680_OS_2X);
bme.setPressureOversampling(BME680_OS_4X);
bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
bme.setGasHeater(320, 150); 

dgs.begin();

	if (! aqi.begin_I2C()) { // Conexão do sensor com I2C 
	while (1) delay(10);
	}

	for(int i=24; i<30; i++) pinMode(i, OUTPUT);
	for(int i=24; i<30; i++) digitalWrite(i, HIGH);

}

void setupWiFi() {
  
  WiFi.disconnect(true, true); //Reseta as configurações da WiFi
  
  WiFi.mode(WIFI_AP); //Colocamos o ESP em modo AP
 
  bool hidden = true; //Faz o AP utilizar o canal passado em "CHANNEL"
  WiFi.softAP(ID, "12345678", CHANNEL, hidden);
}

void setupEspNow() {
  
  if (esp_now_init() == ESP_OK) {
    esp_now_register_send_cb(onDataSent);
  }
  else {
    ESP.restart();
  }
}

void addGatewayAsPeer() {
gateway.channel = CHANNEL; //Canal de comunicação
gateway.encrypt = 0; //0 para não usar criptografia
gateway.ifidx = WIFI_IF_AP; //Interface utilizada na comunicação do ESPNOW

  memcpy(gateway.peer_addr, gatewayMacAdress, sizeof(gatewayMacAdress)); //Copia o endereço do array para a estrutura
  esp_now_add_peer(&gateway);  //Adiciona gateway como peer
}

//Função que serve de callback
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {

Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");

}

void loop()
{
  uint32_t now = millis();

  
  if(now - lastTimeSent > INTERVAL)
  {
    //Faz a leitura dos dispositivos
    readSensor();

    //Envia dados dos dispositivos
    sendSensorData();

    //Marca o tempo do último envio como agora
    lastTimeSent = now;
  }
}

void readSensor() {

//Faz a leitura da temperatura e umidade
int oz, no2, fumaca, bat;
float so2, mp25, mp100, covs, tem, umi, pres;

//Guarda na struct a id e os valores da umidade e da temperatura
strcpy(sensorData.id, ID);
sensorData.faz_leitura_ozonio = oz;
sensorData.faz_leitura_no2 = no2;
sensorData.faz_leitura_fumaca = fumaca;
sensorData.faz_leitura_so2 = so2;
sensorData.faz_leitura_mp25 = mp25;
sensorData.faz_leitura_mp100 = mp100;
sensorData.faz_leitura_covs = covs;
sensorData.faz_leitura_tem = tem;
sensorData.faz_leitura_umi = umi;
sensorData.faz_leitura_pres = pres;
sensorData.bateria = bat;

}

void sendSensorData() {
//Envia a struct para o gateway
esp_err_t result = esp_now_send(gatewayMacAdress, (uint8_t*) &sensorData, sizeof(sensorData));
// Serial.print("Send Status: ");
// //Se o envio foi bem sucedido
// if (result == ESP_OK) {
// Serial.println("Success");
// }
// //Se aconteceu algum erro no envio
// else {
// Serial.println("Error");
//   }
}