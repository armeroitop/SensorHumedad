/*********
  Modified from the examples of the Arduino LoRa library
  More resources: https://randomnerdtutorials.com
*********/

#include <SPI.h>
#include <LoRa.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

#define DHTPIN 26     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);

const int bateriaPin = 33;          // medidor de nivel de bateria
const int fotoresistorPin = 26;          // medidor de nivel de bateria
const int sueloPin = 32;          // medidor de humedad de suelo



//#define NUM_RELAYS  1// Set number of relays
const int relayGPIOs = 25;// Assign each GPIO to a relay
#define RELAY_NO false // Set to true to define Relay as Normally Open (NO)

const char* PARAM_INPUT_1 = "relay";  
const char* PARAM_INPUT_2 = "state";



int counter = 0;

unsigned long tiempo=0;
const int tiempoLectura = 15000;
const int tiempoApagado = 500;
bool estaLeyendo = true;


String LoRaMensaje = "";



String readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {    
    Serial.println("Failed Temperatura!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed Humedad!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}

String readSuelo() {
  
  float s = analogRead(sueloPin);
  if (isnan(s)) {
    Serial.println("Fallo Suelo!");
    return "--";
  }
  else {    
    Serial.println(s);
    return String(s);
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

String relayState(int numRelay){
  if(RELAY_NO){
    if(digitalRead(relayGPIOs)){
      return "";
    }
    else {
      return "checked";
    }
  }
  else {
    if(digitalRead(relayGPIOs)){
      return "checked";
    }
    else {
      return "";
    }
  }
  return "";
}

void envioDatosLora(){
    LoRaMensaje = readDHTTemperature() + "/" + readDHTHumidity() + "&" + readSuelo();
    LoRa.beginPacket();
    LoRa.print(LoRaMensaje);
    LoRa.endPacket();
    Serial.println(LoRaMensaje);
}

void setup() {
    
    //initialize Serial Monitor
    Serial.begin(9600);
    while (!Serial);
    Serial.println("LoRa Sender");

    //setup LoRa transceiver module
    LoRa.setPins(ss, rst, dio0);
    
    //replace the LoRa.begin(---E-) argument with your location's frequency 
    //433E6 for Asia
    //866E6 for Europe
    //915E6 for North America
    while (!LoRa.begin(866E6)) {
        Serial.println(".");
        delay(500);
    }
    // Change sync word (0xF3) to match the receiver
    // The sync word assures you don't get LoRa messages from other LoRa transceivers
    // ranges from 0-0xFF
    LoRa.setSyncWord(0xF3);
    Serial.println("LoRa Initializing OK!");

    // Set all relays to off when the program starts - if set to Normally Open (NO), the relay is off when you set the relay to HIGH
    
    pinMode(relayGPIOs, OUTPUT);
    if(RELAY_NO){  digitalWrite(relayGPIOs, LOW);}
    else{ digitalWrite(relayGPIOs, HIGH);}

    dht.begin();

}

void loop() {
    
  if( millis() > tiempo+tiempoApagado && !estaLeyendo){
      digitalWrite(relayGPIOs, HIGH);
      dht.begin();
      tiempo=millis();
      estaLeyendo=true;
      Serial.println("Encendemos lecturas");
  }
  if(millis() > tiempo+tiempoLectura && estaLeyendo){
     
      digitalWrite(relayGPIOs, LOW);
      tiempo=millis();
      estaLeyendo=false;    
      Serial.println("Apagamos lecturas");  
  }

  if (estaLeyendo)  {    
    delay(3000);
    envioDatosLora();
  }
  
    
    
    
}