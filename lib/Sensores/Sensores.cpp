
#include <Sensores.h>

Sensores::Sensores(const int sueloPin, const int bateriaPin)
{
    _sueloPin = sueloPin;
    _bateriaPin = bateriaPin;
    // Set up oversampling and filter initialization
    //bme.setTemperatureOversampling(BME680_OS_8X);
    //bme.setHumidityOversampling(BME680_OS_2X);
    //bme.setPressureOversampling(BME680_OS_4X);
    //bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    //bme.setGasHeater(320, 150); // 320*C for 150 ms   

     //bmp280.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
     //                   Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
     //                   Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
     //                   Adafruit_BMP280::FILTER_X16,      /* Filtering. */
     //                   Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */ 
}

Sensores::~Sensores()
{
}

void Sensores::setup() 
{
     
    if (!bmp280.begin(0x76)) {
        Serial.println("Could not find a valid BMP280 sensor, check wiring!");
        //while (1)
    }        
}

String Sensores::lecturaTemperatura() 
{
    float t = bmp280.readTemperature();
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
    float h = bme680.humidity;
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
      float p = bmp280.readPressure()/100;
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
     float g = bme680.gas_resistance / 1000.0;
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
      float alt = bmp280.readAltitude(SEALEVELPRESSURE_HPA);
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
    if (! bme680.performReading()) {
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


