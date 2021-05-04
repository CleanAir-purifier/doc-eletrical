
#include <Wire.h>
#include <I2C.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define BME_SCK 36
#define BME_SDI 33

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme; // I2C

void setup() {
  Serial.begin(9600);
  while (!Serial);
  if (!bme.begin()) {
    while (1);
  }

  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); 

void loop() {
  if (! bme.performReading()) {
    return;
  }
  Serial.print(bme.temperature); // Valor de temperatura em Celsius

  Serial.print(bme.pressure / 100.0); // Valor de pressão em hectopascal
  
  Serial.print(bme.humidity); // Valor da taxa de humidade

  Serial.print(bme.gas_resistance / 1000.0); //Valor da resistencia de VOC em KOhms

  Serial.println();
  delay(2000);
}