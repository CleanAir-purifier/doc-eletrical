#include<stdio.h>
#include "leitura_sensores.h"
#include <Wire.h>
#include <I2C.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <Math.h>

int faz_leitura_ozonio(int PinA01){
	int valor_analogico_mq131 = analogRead(PinA01); //Recebe o valor da porta analógica do MQ131
	return valor_analogico_mq131;
}

int faz_leitura_fumaca(int PinA02){
	int valor_analogico_mq2 = analogRead(PinA02); //Recebe o valor da porta analógica do MQ2
	return valor_analogico_mq2;
}

int faz_leitura_no2(int PinNO2){
	int valor_analogico_mics = analogRead(PinNO2); //Recebe o valor da porta analógica do MICS
	return valor_analogico_mics;
}

float faz_leitura_covs() {

  if (! bme.performReading()) {
  
  	return;
  }
	bme_ppm = (bme.gas_resistance/1000)*pow(10, 9)*0.64; // calculo de concetração de ohms para ppm

	return bme_ppm;
	delay(2000);

}

float faz_leitura_mp25(){
  PM25_AQI_Data data;
  
  if (! aqi.read(&data)) {
    delay(500);
    return;
  }
	return data.pm25_standard; // Retornar valor medido de MP de 10.0um
	delay(1000);
}
float faz_leitura_mp100(){

PM25_AQI_Data data;
  
  if (! aqi.read(&data)) {
    delay(500);
    return;
  }
	return data.pm100_standard; // Retornar valor medido de MP de 10.0um
	delay(1000);
}
float faz_leitura_so2(){

	dgsSO2.read(); // Valor medido de SO2 em ppb

	dgsSO2ppm = dgsSO2.read() * 0.001; //Conversão de ppb para ppm
	return dgsSO2ppm; //Retorna valor do sensor em ppm de SO2
}

float faz_leitura_temp(){
	if (! bme.performReading()) {
  
  	return;
  }
	return bme.temperature; // Retorna valor de temperatura em Celsius
	delay(2000);
}
float faz_leitura_umi(){
	if (! bme.performReading()) {
  
  	return;
  }
	return bme.humidity; // Retorna valor da taxa de humidade
	delay(2000);
}
float faz_leitura_pres(){
	if (! bme.performReading()) {
  
  	return;
  }
	return (bme.pressure/100); // Retorna valor de pressão em hectopascal
	delay(2000);
}

void bateria(int nivel)
{
  nivel = analogRead(9); 
  
  if(nivel < 50)
  {
    digitalWrite(24, HIGH);
    digitalWrite(25, HIGH);
    digitalWrite(26, HIGH);
    digitalWrite(27, HIGH);
    digitalWrite(28, HIGH); 
    digitalWrite(29,  LOW);
 
  } 
  
  else if(nivel < 205)
  {
    digitalWrite(24,  LOW);
    digitalWrite(25, HIGH);
    digitalWrite(26, HIGH);
    digitalWrite(27, HIGH);
    digitalWrite(28, HIGH);
    digitalWrite(29, !digitalRead(7));
    delay(90);
    
  } 

  else if(nivel < 410)
  {
    digitalWrite(24,  LOW);
    digitalWrite(25,  LOW);
    digitalWrite(26, HIGH);
    digitalWrite(27, HIGH);
    digitalWrite(28, HIGH);
    digitalWrite(29, !digitalRead(7));
    delay(150);

    } 

  else if(nivel < 615)
  {
    digitalWrite(24,  LOW);
    digitalWrite(25,  LOW);
    digitalWrite(26,  LOW);
    digitalWrite(27, HIGH);
    digitalWrite(28, HIGH);
    digitalWrite(29, HIGH);
    
  } 

  else if(nivel < 820)
  {
    digitalWrite(24,  LOW);
    digitalWrite(25,  LOW);
    digitalWrite(26,  LOW);
    digitalWrite(27,  LOW);
    digitalWrite(28, HIGH);
    digitalWrite(29, HIGH);
    
  } 

  else if(nivel < 1023)
  {
    digitalWrite(24,  LOW);
    digitalWrite(25,  LOW);
    digitalWrite(26,  LOW);
    digitalWrite(27,  LOW);
    digitalWrite(28,  LOW);
    digitalWrite(29, HIGH);
    
  } 
  
} 