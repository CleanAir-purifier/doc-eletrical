#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_now.h>


#define PWM 31               //pino da ESP32 para ligar driver de motor
#define TOPICO_MOTOR         "topico_controle_pwm"

#define TOPICO_OZONIO         "topico_ozonio_ppm"
#define TOPICO_NO2            "topico_no2_ppm"
#define TOPICO_MP25           "topico_mp25_ppm"
#define TOPICO_MP100          "topico_mp100_ppm"
#define TOPICO_COVS           "topico_covs_ppm"
#define TOPICO_FUMACA         "topico_fumaca_ppm"
#define TOPICO_SO2            "topico_so2_ppm"
#define TOPICO_TEM            "topico_tem"
#define TOPICO_UMI            "topico_umi"
#define TOPICO_PRES           "topico_pres"
#define TOPICO_BAT            "topico_bat"
 
#define ID_MQTT  "esp32_mqtt" 

//Nome do tópico que devemos enviar os dados
//para que eles apareçam nos gráficos
#define TOPIC_NAME "iot-2/evt/status/fmt/json" //Software


const char* SSID = " "; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = " "; // Senha da rede WI-FI que deseja se conectar
 
const char* BROKER_MQTT = "rabbitmq.com"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 15672; // Porta do Broker MQTT

int valor_pwm = 0;  //variavel que armazena o valor do PWM (0 = 0% e 1023 = 100% da rotação do motor) 
int nivel_bateria;

WiFiClient espClient; // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient

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
}SensorData;

void setupWiFi(void);
void setupEspNow(void);
void initMQTT(void);
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void reconnectMQTT(void);
void reconnectWiFi(void);
void VerificaConexoesWiFIEMQTT(void);

/* Função: inicializa e conecta-se na rede WI-FI desejada */

void setupWiFi(void) 
{
    //Desconecta e reseta as configurações da WiFi
    WiFi.disconnect(true, true);

    //O modo deve ser tanto AP quanto Station
    WiFi.mode(WIFI_AP_STA);

    //Mandamos conectar na rede
    WiFi.begin(SSID, PASSWORD);
    Serial.println("");

    //Enquanto não estiver conectado à rede WiFi
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
}

}

void setupEspNow() {
  //Se a inicialização foi bem sucedida
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
    //Registra a função que será executada quando chegarem dados dos ESPs com DHT
    esp_now_register_recv_cb(OnDataRecv);
  }
  //Se houve erro na inicialização
  else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
}

/* Função: inicializa parâmetros de conexão MQTT(endereço do broker, porta e seta função de callback) */

void initMQTT(void) 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}

//Função que avisar quando chegou algo das ESPs
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  SensorData* sensorData = (SensorData*) data;
  //Envia para o Server MQTT

    char oz_str[10]     = {0};
    char no2_str[10]    = {0};
    char mp25_str[10]   = {0};
    char mp100_str[10]  = {0};
    char covs_str[10]   = {0};
    char fumaca_str[10] = {0};
    char so2_str[10]    = {0};
    char tem_str[10]    = {0};
    char pres_str[10]   = {0};
    char umi_str[10]    = {0};
    char bat_str[10]    = {0};

  /* Compoe as strings a serem enviadas pro dashboard (campos texto) */
    sprintf(oz_str,"%.2d", SensorData.ozonio);
    sprintf(no2_str,"%.2d", SensorData.no2);
    sprintf(fumaca_str,"%.2d", SensorData.fumaca);
    sprintf(covs_str,"%.2d", SensorData.covs);
    sprintf(mp25_str,"%.2d", SensorData.mp25);
    sprintf(mp100_str,"%.2d", SensorData.mp100);
    sprintf(so2_str,"%.2d", SensorData.so2);
    sprintf(pres_str,"%.2d", SensorData.pres);
    sprintf(tem_str,"%.2d", SensorData.tem);
    sprintf(umi_str,"%.2d", SensorData.umi);
    sprintf(bat_str,"%.2d", SensorData.bat);

  /*  Envia as strings ao dashboard MQTT */

    MQTT.publish(TOPICO_OZONIO, oz_str);
    MQTT.publish(TOPICO_NO2, no2_str);
    MQTT.publish(TOPICO_MP25, mp25_str);
    MQTT.publish(TOPICO_MP100, mp100_str);
    MQTT.publish(TOPICO_COVS, covs_str);           
    MQTT.publish(TOPICO_FUMACA, fumaca_str);         
    MQTT.publish(TOPICO_SO2, so2_str);    
    MQTT.publish(TOPICO_TEM, tem_str);           
    MQTT.publish(TOPICO_PRES, pres_str);         
    MQTT.publish(TOPICO_UMI, umi_str);  
    MQTT.publish(TOPICO_BAT, bat_str);   

}

/* Função: função de callback é chamada toda vez que uma informação de um dos tópicos subescritos chega) */
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
    String msg;
  
    /* obtem a string do payload recebido */
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }

  /* Ação dependente da string recebida */
    if (msg.equals("vel_1"))
    {
        valor_pwm = 256;  //Velocidade 1(25%) 
        analogWrite(PINO_PWM, valor_pwm);
    }

    else if (msg.equals("vel_2"))
    {
        valor_pwm = 512;  //Velocidade 2(50%) 
        analogWrite(PINO_PWM, valor_pwm);
    }
    else if (msg.equals("vel_3"))
    {
        valor_pwm = 1023; //Velocidade 3(100%)
        analogWrite(PINO_PWM, valor_pwm);
    }
    else {
        valor_pwm = 0; 
        analogWrite(PINO_PWM, valor_pwm); //Motor em modo de repouso
    }
}


void setup()
{   
  Serial.begin(115200); //Inicia serial
  setupWiFi();   //Inicia WiFi  
  setupEspNow(); //Inicia ESPNOW
  connectToMQTTServer(); //Conecta ao server MQTT

  pinMode(PINO_PWM, OUTPUT); //configura como saída pino do motor
}

void loop()
{   /* garante funcionamento das conexões WiFi e ao broker MQTT */
    VerificaConexoesWiFIEMQTT();
 
    /* keep-alive da comunicação com broker MQTT */
    MQTT.loop();
 
    /* Refaz o ciclo após 2 segundos */
    delay(2000);
}

