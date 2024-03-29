#ifndef SENSORES_H
#define SENSORES_H


#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME680.h>
#include <Adafruit_BMP280.h>

#define SEALEVELPRESSURE_HPA (1009.00)


class Sensores
{
    private:
        Adafruit_BMP280 bmp280; // I2C
        Adafruit_BME680 bme680; // I2C
        int _sueloPin;          // medidor de humedad de suelo 27
        int _bateriaPin;          // medidor de nivel de bateria 33
    public:
        Sensores(const int sueloPin, const int bateriaPin);
        ~Sensores();

        void setup();
        String lecturaTemperatura();
        String lecturaHumedad();
        String lecturaPresionAtmosferica();
        String lecturaGas();
        String lecturaAltitud();
        String lecturaSuelo();
        String lecturaBateria();
        String lecturasDeTodo();
};




#endif
