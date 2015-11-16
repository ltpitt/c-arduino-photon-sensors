// Acquire and publish temperature and humidity from DHT22 sensor using Particle Photon
//
// To read variables from Particle Api use:
// https://api.particle.io/v1/devices/YOUR_DEVICE_ID/humidity/?access_token=YOUR_ACCESS_TOKEN
//
// Davide Nastri, 11/2015

#include "PietteTech_DHT/PietteTech_DHT.h"
#include "math.h"

#define DHTTYPE  DHT22       // Sensor type DHT11/21/22/AM2301/AM2302
#define DHTPIN   D2           // Digital pin for communications

// declaration
void dht_wrapper(); // must be declared before the lib initialization

// library instialization
PietteTech_DHT DHT(DHTPIN, DHTTYPE, dht_wrapper);

int n;      // sensor acquisition counter
double temp;  // temperature  
double umid;  // humidity
double t;   // needed in rounding

void dht_wrapper() {
    DHT.isrCallback();
}



void setup()
{
    Serial.begin(9600);

    Serial.println("Device starting...");

    Serial.println("DHT Example program using DHT.acquireAndWait");
    Serial.print("LIB version: ");
    Serial.println(DHTLIB_VERSION);
    Serial.println("---------------");
    
    pinMode(D7, OUTPUT);

    Particle.variable("humidity", umid); //write the value on the cloud variable called "Humidity"
    Particle.variable("temperature", temp); //write the value on the cloud variable called "Temperature"

}


void loop()
{
    Serial.print("\n");
    Serial.print(n);
    Serial.print(": Retrieving information from sensor: ");
    Serial.print("Read sensor: ");
    delay(100);

    int result = DHT.acquireAndWait();

    
    // Error handling
    switch (result) {
        case DHTLIB_OK:
            Serial.println("OK");
            break;
        case DHTLIB_ERROR_CHECKSUM:
            Serial.println("Error\n\r\tChecksum error");
            break;
        case DHTLIB_ERROR_ISR_TIMEOUT:
            Serial.println("Error\n\r\tISR time out error");
            break;
        case DHTLIB_ERROR_RESPONSE_TIMEOUT:
            Serial.println("Error\n\r\tResponse time out error");
            break;
        case DHTLIB_ERROR_DATA_TIMEOUT:
            Serial.println("Error\n\r\tData time out error");
            break;
        case DHTLIB_ERROR_ACQUIRING:
            Serial.println("Error\n\r\tAcquiring");
            break;
        case DHTLIB_ERROR_DELTA:
            Serial.println("Error\n\r\tDelta time to small");
            break;
        case DHTLIB_ERROR_NOTSTARTED:
            Serial.println("Error\n\r\tNot started");
            break;
        default:
            Serial.println("Unknown error");
            break;
    }


    // humidity rounding
    umid=DHT.getHumidity();
    t=umid-floor(umid);  // get only decimal part
    t=t*100; // multiply by 100 decimal part, get xx.xxxxxxxxx
    t=floor(t); // get only last two characters of decimal part, get yy
    umid=floor(umid)+(t/100);


    // temperature rounding
    temp=DHT.getCelsius();
    t=temp-floor(temp); // get only decimal part
    t=t*100; // multiply by 100 decimal part, get xx.xxxxxxxxx
    t=floor(t); // get only last two characters of decimal part, get yy
    temp=floor(temp)+(t/100);


    // print sensor data to serial
    Serial.print("Humidity (%): ");
    Serial.println(DHT.getHumidity(), 2);

    Serial.print("Temperature (oC): ");
    Serial.println(DHT.getCelsius(), 2);

    Serial.print("Temperature (oF): ");
    Serial.println(DHT.getFahrenheit(), 2);

    Serial.print("Temperature (K): ");
    Serial.println(DHT.getKelvin(), 2);

    Serial.print("Dew Point (oC): ");
    Serial.println(DHT.getDewPoint());

    Serial.print("Dew Point Slow (oC): ");
    Serial.println(DHT.getDewPointSlow());

    
    // Blink onboard led
    digitalWrite(D7, HIGH);
    delay(1000);
    digitalWrite(D7, LOW);


    // Increase acquisition counter
    n++;
    

    // Wait for 10 minutes
    delay(600000);
}

