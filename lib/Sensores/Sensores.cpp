
#include <Sensores.h>

Sensores::Sensores(const int sueloPin, const int bateriaPin)
{
    _sueloPin = sueloPin;
    _bateriaPin = bateriaPin;
    // Set up oversampling and filter initialization
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms   
}

Sensores::~Sensores()
{
}

void Sensores::setup() 
{
    if (!bme.begin()) {
        Serial.println("Could not find a valid BME680 sensor, check wiring!");
    //while (1);
    }

}

String Sensores::lecturaTemperatura() 
{
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

String Sensores::lecturaHumedad() 
{
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

String Sensores::lecturaPresionAtmosferica() 
{
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

String Sensores::lecturaGas() 
{
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

String Sensores::lecturaAltitud() 
{
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

String Sensores::lecturaSuelo() 
{
      float s = analogRead(_sueloPin);
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

String Sensores::lecturaBateria() 
{
     float b = analogRead(_bateriaPin);
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

String Sensores::lecturasDeTodo() 
{
    if (! bme.performReading()) {
        Serial.println("Failed to perform reading :(");
    }
    String mensaje;
    mensaje =   "dgma temperatura"  +lecturaTemperatura() + 
                "humedad" + lecturaHumedad() +
                "suelo" + lecturaSuelo() +
                "presion" + lecturaPresionAtmosferica() +
                "gas" + lecturaGas() + 
                "altitud" + lecturaAltitud() + 
                "bateria" + lecturaBateria();
    return mensaje;   
}


