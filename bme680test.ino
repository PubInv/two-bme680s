/***************************************************************************
  This is a library for the BME680 gas, humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME680 Breakout
  ----> http://www.adafruit.com/products/3660

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

/***************************************************************************
  Hacked by Robert L. Read, 2020

  ... in order to support two breakout boards on the same Arduinol
  Modififications released under BSD to avoid complication (I prefer GPL).

 ***************************************************************************/


#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

// It would be nice to support more than one,
// but the BME680 only has two addresses (0x77, and 0x76 when SD0 tied to GND).
Adafruit_BME680 bme[2]; // I2C

bool found_bme[2] = { false, false}; // an abundance of caution to init

uint8_t addr[2] = {0x76,0x77};

//Adafruit_BME680 bme(BME_CS); // hardware SPI
//Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println(F("BME680 test"));
  seekBME(0);
  seekBME(1);
}

void seekUnfoundBME() {
  if (!found_bme[0]) {
    seekBME(0);
  }
  if (!found_bme[1]) {
    seekBME(1);
  }
}

void seekBME(int idx) {
  found_bme[idx] = bme[idx].begin(addr[idx]);
  if (!found_bme[idx]) {
    Serial.println("Could not find a valid BME680 sensor, check wiring for:");
    Serial.println(addr[idx],HEX);
  } else {
    // Set up oversampling and filter initialization
    bme[idx].setTemperatureOversampling(BME680_OS_8X);
    bme[idx].setHumidityOversampling(BME680_OS_2X);
    bme[idx].setPressureOversampling(BME680_OS_4X);
    bme[idx].setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme[idx].setGasHeater(320, 150); // 320*C for 150 ms
  }
}


void loop() {
  seekUnfoundBME();
  if (found_bme[0])
    readBME(0);
  if (found_bme[1])
    readBME(1);
}
void readBME(int idx) 
{
  if (! bme[idx].performReading()) {
    Serial.println("Failed to perform reading :( for:");
    Serial.println(addr[idx],HEX);
    found_bme[idx] = false;
    return;
  }
  Serial.print("Data For:");
  Serial.println(addr[idx],HEX);
  Serial.print("Temperature = ");
  Serial.print(bme[idx].temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bme[idx].pressure / 100.0);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(bme[idx].humidity);
  Serial.println(" %");

  Serial.print("Gas = ");
  Serial.print(bme[idx].gas_resistance / 1000.0);
  Serial.println(" KOhms");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme[idx].readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.println();
  delay(3000);
}
