/*********
  Modified from the examples of the Arduino LoRa library
  More resources: https://randomnerdtutorials.com
*********/

#include <SPI.h>
#include <LoRa.h>

#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

#define SEALEVELPRESSURE_HPA (1009.00)
Adafruit_BME680 bme; // I2C

const int bateriaPin = 33;          // medidor de nivel de bateria
const int fotoresistorPin = 26;          // medidor de nivel de bateria
const int sueloPin = 27;          // medidor de humedad de suelo


//#define NUM_RELAYS  1// Set number of relays
const int relayGPIOs = 25;// Assign each GPIO to a relay
#define RELAY_NO true // Set to true to define Relay as Normally Open (NO)


unsigned long tiempo=0;
const int tiempoLectura = 30000;
const int tiempoApagado = 100000;
bool estaLeyendo = true;


String LoRaMensaje = "";

String lecturaTemperatura() {
  
  float t = bme.temperature;
  if (isnan(t)) {
    Serial.println("Fallo de lectura Temperatura!");
    return "--";
  }
  else {
    Serial.print("Temperatura = ");
    Serial.print(t);
    Serial.println(" %");    
    return String(t);
  }
}

String lecturaHumedad() {
  
  float h = bme.humidity;
  if (isnan(h)) {
    Serial.println("Fallo de lectura Humedad!");
    return "--";
  }
  else {
    Serial.print("Humedad = ");
    Serial.print(h);
    Serial.println(" *C");    
    return String(h);
  }
}

String lecturaPresionAtmosferica() {
  
  float p = bme.pressure / 100.0;
  if (isnan(p)) {
    Serial.println("Fallo de lectura Presion Atmosferica!");
    return "--";
  }
  else {
    Serial.print("Presion Atmosferica = ");
    Serial.print(p);
    Serial.println(" hPa");    
    return String(p);
  }
}

String lecturaGas() {
  
  float g = bme.gas_resistance / 1000.0;
  if (isnan(g)) {
    Serial.println("Fallo de lectura Gas!");
    return "--";
  }
  else {
    Serial.print("Gas = ");
    Serial.print(g);
    Serial.println(" KOhms");    
    return String(g);
  }
}

String lecturaAltitud() {
  
  float alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
  if (isnan(alt)) {
    Serial.println("Fallo de lectura Altitude!");
    return "--";
  }
  else {
    Serial.print("Approx. Altitude = ");
    Serial.print(alt);
    Serial.println(" m");    
    return String(alt);
  }
}

String lecturaBateria() {
  
  float b = analogRead(bateriaPin);
  if (isnan(b)) {
    Serial.println("Fallo lectura de Bateria!");
    return "--";
  }
  else {        
    Serial.print("Carga de Bateria: ");
    Serial.print(b);
    Serial.println(" Volt");   
    return String(b);
  }
}

String readSol() {
  
  int y = analogRead(fotoresistorPin);
  if (isnan(y)) {
    Serial.println("Fallo fotoresistorPin!");
    return "--";
  }
  else {    
    Serial.println(y);
    return String(y);
  }
}

String lecturaSuelo() {
  
  float s = analogRead(sueloPin);
  if (isnan(s)) {
    Serial.println("Fallo Suelo!");
    return "--";
  }
  else {        
    Serial.print("Suelo Humedad: ");
    Serial.print(s);
    Serial.println("%");    
    return String(s);
  }
}

void envioDatosLora(){
    if (! bme.performReading()) {
        Serial.println("Failed to perform reading :(");
    }
    LoRaMensaje = "dgma temperatura"  +lecturaTemperatura() + 
                  "humedad" + lecturaHumedad() +
                  "suelo" + lecturaSuelo() +
                  "presion" + lecturaPresionAtmosferica() +
                  "gas" + lecturaGas() + 
                  "altitud" + lecturaAltitud() + 
                  "bateria" + lecturaBateria();
    LoRa.beginPacket();
    LoRa.print(LoRaMensaje);
    LoRa.endPacket();
    
    Serial.println(LoRaMensaje); 
}

void setup() {
    // Set all relays to off when the program starts - if set to Normally Open (NO), the relay is off when you set the relay to HIGH
    
    pinMode(relayGPIOs, OUTPUT);
    if(RELAY_NO){  digitalWrite(relayGPIOs, HIGH);}
    else{ digitalWrite(relayGPIOs, LOW);}
    
    //initialize Serial Monitor
    Serial.begin(9600);
    while (!Serial);
    Serial.println("LoRa Sender");

    //setup LoRa transceiver module
    LoRa.setPins(ss, rst, dio0);
    
   
    while (!LoRa.begin(866E6)) {
        Serial.println(".");
        delay(500);
    }
    // Change sync word (0xF3) to match the receiver
    // The sync word assures you don't get LoRa messages from other LoRa transceivers
    // ranges from 0-0xFF
    LoRa.setSyncWord(0xF3);
    Serial.println("LoRa Initializing OK!");

   
    // Set up oversampling and filter initialization
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms   

    

    if (!bme.begin()) {
        Serial.println("Could not find a valid BME680 sensor, check wiring!");
    //while (1);
    }

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