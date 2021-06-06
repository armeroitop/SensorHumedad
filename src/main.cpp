
#include <Arduino.h>
#include <Antena.h>
#include <Sensores.h>
#include <Estado.h>


unsigned long tiempo=0;
const int tiempoLectura = 30000;
const int tiempoApagado = 100000;
bool estaLeyendo = true;

Estado estado;

Sensores miSensor(27,33);
Antena antenaLora;


void setup() {
     
    //initialize Serial Monitor
    Serial.begin(9600);
    miSensor.setup();
    antenaLora.setup();

    estado = Estado::CalentadoMotores;
 
}

void loop() {
    
  if( millis() > tiempo+tiempoApagado && !estaLeyendo){
      
      
      tiempo=millis();
      estaLeyendo=true;
      Serial.println("Encendemos lecturas");
  }
  if(millis() > tiempo+tiempoLectura && estaLeyendo){
      LoRa.sleep();
    
      tiempo = millis();g
      estaLeyendo = false;    
      Serial.println("Apagamos lecturas");  
  }

  if (estaLeyendo)  {    
    delay(10000);
    //envioDatosLora();
    antenaLora.envioDatosLora(miSensor.lecturasDeTodo());
  } 

  switch (estado)
  {
    case  Estado::CalentadoMotores:
      /* code */
      break;

    case  Estado::TomandoLectoras :
        antenaLora.envioDatosLora(miSensor.lecturasDeTodo());
      break;

    case  Estado::Dormido :
      //Ponemos a dormir el esp32, tiempo en microsegundos 10^-6
      esp_deep_sleep(50000000);
      break;
    
    default:
      break;
  }
      
}