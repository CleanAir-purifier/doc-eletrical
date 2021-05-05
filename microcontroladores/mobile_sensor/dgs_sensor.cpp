#include <DGS.h> //Biblioteca interna

SteamDGSS02 dgsS02; 

float dgsSO2ppm;

void setup(){
	Serial.begin(9600); //Iniciar Serial
	dgs.begin(); //Iniciar clock do sensor
}

void loop() {
	dgsSO2.read(); // Valor medido de SO2 em ppb

	dgsSO2ppm = dgsSO2.read() * 0.001; //Conversão de ppb para ppm

	// dgsSO2ppm.display(); //Valor do sensor em ppm de SO2
}