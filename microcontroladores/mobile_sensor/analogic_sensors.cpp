#define PinA02 13//Pino utilizado pelo MQ2
#define PinA01 12//Pino utilizado pelo MQ131
#define PinNO2 10//Pino utilizado pelo MICS

void setup(){

pinMode(PinA02, INPUT); //Definição do pino como entrada do MQ2
pinMode(PinA01, INPUT); //Definição do pino como entrada do MQ131
pinMode(PinNO2, INPUT); //Definição do pino como entrada do MICS

Serial.begin(9600); //Iniciar Serial
}

void loop(){
int valor_analogico_mq2 = analogRead(PinA02); //Recebe o valor da porta analógica do MQ2
int valor_analogico_mq131 = analogRead(PinA01); //Recebe o valor da porta analógica do MQ131
int valor_analogico_mics = analogRead(PinNO2); //Recebe o valor da porta analógica do MICS

// Serial.println(valor_analogico_mq2);// Valor lido na porta analogico em ppm de fumaça
// Serial.println(valor_analogico_mq131);// Valor lido na porta analogico em ppm de ozonio
// Serial.println(valor_analogico_mics);// Valor lido na porta analogico em ppm de NO2

delay(100); //Intervalo

}
