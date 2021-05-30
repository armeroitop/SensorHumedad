
#include <Arduino.h>
#include <Antena.h>
#include <Sensores.h>

unsigned long tiempo=0;
const int tiempoLectura = 30000;
const int tiempoApagado = 100000;
bool estaLeyendo = true;

Sensores miSensor(27,33);
Antena antenaLora;


void setup() {
     
    //initialize Serial Monitor
    Serial.begin(9600);
    miSensor.setup();
    antenaLora.setup();
 
}

void loop() {
    
  if( millis() > tiempo+tiempoApagado && !estaLeyendo){
      
      
      tiempo=millis();
      estaLeyendo=true;
      Serial.println("Encendemos lecturas");
  }
  if(millis() > tiempo+tiempoLectura && estaLeyendo){
      LoRa.sleep();
    
      tiempo = millis();
      estaLeyendo = false;    
      Serial.println("Apagamos lecturas");  
  }

  if (estaLeyendo)  {    
    delay(10000);
    //envioDatosLora();
    antenaLora.envioDatosLora(miSensor.lecturasDeTodo());
  } 
      
}