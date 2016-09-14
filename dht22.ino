// Acquire sensor data from Particle Photon using DHT22 library
//
// This script will put Particle Photon in SLEEP_MODE_DEEP for the specified amount of time
// for a very low energy use (if you need to work with batteries)
//
// OTA update is enabled using a simple http get of a variable on any specified Webserver path
// When OTA mode is on the Particle Photon will not go to SLEEP_MODE_DEEP.
// Also the onboard led will be on and a notification will be send (using external code).
// Davide Nastri, 06/2016

// Libraries includes
#include "HttpClient/HttpClient.h" // HttpClient 
#include "PietteTech_DHT/PietteTech_DHT.h" // DHT22 (Temperature and Humidity sensor)
#include "math.h" // Math library for sensor values calculation

// Defines
#define DHTTYPE  DHT22 // Sensor type for DHT22 library (can be: DHT11/21/22/AM2301/AM2302)
#define DHTPIN   D2 // Digital pin for communications           

// Declaring the variables
void dht_wrapper(); // Must be declared before the lib initialization
unsigned int nextTime = 0; // Next time to contact the server
int n;      // Sensor acquisition counter
double temp;  // Temperature value
double umid;  // Humidity value
double t;   // Variable used in values rounding

// HttpClient initialization
HttpClient http;
// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
    //  { "Content-Type", "application/json" },
    //  { "Accept" , "application/json" },
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};
http_request_t request;
http_response_t response;


// DHT library initialization
PietteTech_DHT DHT(DHTPIN, DHTTYPE, dht_wrapper);

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

}


void loop()
{
    // Increase acquisition counter
    n++;
    
    // Check if OTA is enabled via http
    // Request path and body can be set at runtime or at setup.
    request.hostname = "192.168.178.25"; // Insert your Webserver IP
    request.port = 8080; // Insert your Webserver PORT
    request.path = "/ota"; // Insert the path where the string "on" or "off" can be retrieved

    // The library also supports sending a body with your request:
    //request.body = "{\"key\":\"value\"}";

    // Get request
    http.get(request, response, headers);
    Serial.print("Application>\tResponse status: ");
    Serial.println(response.status);
    Serial.print("Application>\tHTTP Response Body: ");
    Serial.println(response.body);
    
    // Temperature and Humidity 
    Serial.print("\n");
    Serial.print(n);
    Serial.print(": Retrieving information from sensor: ");
    Serial.print("Read sensor: ");
    delay(100);

    // Acquire data from sensor
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

    // Humidity value retrieval and rounding
    umid=DHT.getHumidity();
    t=umid-floor(umid);  // Get only decimal part
    t=t*100; // Multiply by 100 decimal part, get xx.xxxxxxxxx
    t=floor(t); // Get only last two characters of decimal part, get yy
    umid=floor(umid)+(t/100);

    // Temperature value retrieval and rounding
    temp=DHT.getCelsius();
    t=temp-floor(temp); // Get only decimal part
    t=t*100; // Multiply by 100 decimal part, get xx.xxxxxxxxx
    t=floor(t); // Get only last two characters of decimal part, get yy
    temp=floor(temp)+(t/100);

    // Print sensor data to serial
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

    // Uncomment or comment below rows accoring to the sensor you are going to flash
    //Particle.publish("1_floor_temp", String(temp,1), PRIVATE);
    //Particle.publish("1_floor_hum", String(umid, 1), PRIVATE);
    Particle.publish("2_floor_temp", String(temp,1), PRIVATE);
    Particle.publish("2_floor_hum", String(umid,1), PRIVATE);

    // Check if OTA update is enabled
    if (response.body=="off") {
        // Turn off onboard led
        digitalWrite(D7, LOW);
        // Put Particle Photon in deep sleep for 900 seconds (15 minutes)
        System.sleep(SLEEP_MODE_DEEP,900);
    } else {
        // Turn on onboard led
        digitalWrite(D7, HIGH);
        // Send notification
        request.hostname = "192.168.178.25"; // Insert your Webserver IP
        request.port = 8080; // Insert your Webserver PORT
        request.path = "/notification/ota"; // Webserver url that sends ota notification to user
        // Get request
        Serial.print("Application>\tSending OTA on notification to user");
        http.get(request, response, headers);
        Serial.print("Application>\tResponse status: ");
        Serial.println(response.status);
        Serial.print("Application>\tHTTP Response Body: ");
        Serial.println(response.body);
        // Delay 900000 milliseconds (15 minutes)
        delay(900000);
    }
    
}
