#include <Antena.h>

Antena::Antena()
{
    //setup LoRa transceiver module
    LoRa.setPins(ss, rst, dio0);
}

Antena::~Antena()
{
}

void Antena::setup() 
{
    while (!LoRa.begin(866E6)) {
        Serial.println(".");
        delay(500);
    }
    // Change sync word (0xF3) to match the receiver
    // The sync word assures you don't get LoRa messages from other LoRa transceivers
    // ranges from 0-0xFF
    LoRa.setSyncWord(0xF3);
    Serial.println("LoRa Initializing OK!");
}

void Antena::envioDatosLora(String mensaje) 
{
    LoRa.beginPacket();
    LoRa.print(mensaje);
    LoRa.endPacket();
    
    Serial.println(mensaje); 
}
