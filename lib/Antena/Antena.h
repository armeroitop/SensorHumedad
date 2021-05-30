#ifndef __ANTENA_H__
#define __ANTENA_H__

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>


//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2


class Antena
{
    private:
       String _mensaje;
    public:
        Antena(/* args */);
        ~Antena();
        void setup();
        void envioDatosLora(String mensaje);
};




#endif // __ANTENA_H__