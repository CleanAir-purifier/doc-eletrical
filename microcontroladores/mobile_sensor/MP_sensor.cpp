#include "Adafruit_PM25AQI.h"
 
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();
 
void setup() {
  Serial.begin(115200); //Iniciar Serial
  while (!Serial) delay(10);
   delay(1000);
 
  if (! aqi.begin_I2C()) {      // Conexão do sensor com I2C 
    while (1) delay(10);
  }
}
 
void loop() {
  PM25_AQI_Data data;
  
  if (! aqi.read(&data)) {
    delay(500);
    return;
  }

  Serial.print(data.pm25_standard); // Valor medido de MP de 2.5um
  Serial.println(data.pm100_standard); // Valor medido de MP de 10.0um
   
  delay(1000);
}