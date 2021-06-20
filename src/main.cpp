
#include <Arduino.h>
#include <Antena.h>
#include <Sensores.h>
#include <Estado.h>


unsigned long tiempo=0;
const int tiempoLectura = 30000;

bool estaLeyendo = true;

int contadorLecturas = 0;
const int numeroMaximoLecturas = 5;
const int tiempoEntreLecturas = 10000; //Son 10 segundos en milisencond, 
unsigned long tiempoUltimaLectura = 0; //Guardamos el tiempo en el que se toma la ultima lectura
const int tiempoApagado = 120; // En segundos

Estado estado;

Sensores miSensor(27,33);
Antena antenaLora;


void setup() {
     
    //initialize Serial Monitor
    Serial.begin(9600);
    miSensor.setup();
    antenaLora.setup();

    estado = Estado::Encendido;
 
}

void loop() {
    
  switch (estado)
  {
    case  Estado::Encendido:
        //¿Podemos tomar lecturas? ¿a pasado el tiempo entre lecturas necesario?
        if (millis()> tiempoUltimaLectura + tiempoEntreLecturas){
           estado = Estado::TomandoLecturas;
        }
      break;

    case  Estado::TomandoLecturas:
        
        Serial.print("Lectura numero: " );
        Serial.println(contadorLecturas);

        antenaLora.envioDatosLora(miSensor.lecturasDeTodo());
        contadorLecturas++;
        tiempoUltimaLectura = millis(); // guardamo el tiempo de esta ultima lectura
                  
        if(contadorLecturas >= numeroMaximoLecturas){
            estado = Estado::Dormido;
        }else{
            estado = Estado::Encendido;
        }
      break;

    case  Estado::Dormido :
        contadorLecturas = 0;
        Serial.println("Vamos a dormir " );
        
        esp_deep_sleep(tiempoApagado * 1000000);//Ponemos a dormir el esp32, tiempo en microsegundos 10^-6
        
      break;
    
    default:
      break;
  }
      
}