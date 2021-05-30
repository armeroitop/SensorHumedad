
#include <Arduino.h>
#include <Antena.h>
#include <Sensores.h>

unsigned long tiempo=0;
const int tiempoLectura = 30000;
const int tiempoApagado = 100000;
bool estaLeyendo = true;




void setup() {
     
    //initialize Serial Monitor
    Serial.begin(9600);
 
}

void loop() {
    
  if( millis() > tiempo+tiempoApagado && !estaLeyendo){
      digitalWrite(relayGPIOs, HIGH);
      
      tiempo=millis();
      estaLeyendo=true;
      Serial.println("Encendemos lecturas");
  }
  if(millis() > tiempo+tiempoLectura && estaLeyendo){
      LoRa.sleep();
      digitalWrite(relayGPIOs, LOW);
      tiempo = millis();
      estaLeyendo = false;    
      Serial.println("Apagamos lecturas");  
  }

  if (estaLeyendo)  {    
    delay(10000);
    envioDatosLora();
  } 
      
}