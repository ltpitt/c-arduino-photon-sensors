#include "HttpClient/HttpClient.h"
#include "InternetButton/InternetButton.h"

/**
* Declaring the variables.
*/
unsigned int nextTime = 0;    // Next time to contact the server

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
InternetButton b = InternetButton();
int i;

void setup() {
    Serial.begin(9600);
    b.begin();
    b.allLedsOff();
    RGB.control(true); 
    RGB.color(0, 0, 0);
}

void loop() {

    if(b.buttonOn(1)){
        b.ledOn(1, 15, 0, 0);
        b.ledOn(11, 15, 0, 0);
        checkService(1);
    }
     
    if(b.buttonOn(2)){
        b.ledOn(3, 15, 0, 0);
        checkService(2);
    }

    if(b.buttonOn(3)){
        b.ledOn(6, 15, 0, 0);
        checkService(3);
    }
    
    if(b.buttonOn(4)){
        b.ledOn(9, 15, 0, 0);
        checkService(4);
    }

}

void okLed(int led){
    
    if (led == 1) {
            b.ledOn(1, 0, 15, 0);
            b.ledOn(11, 0, 15, 0);
            delay(1000);
            b.ledOff(1);
            b.ledOff(11);
    }
    else {
        b.ledOn(led, 0, 15, 0);
        delay(1000);
        b.ledOff(led);
    }
}

void koLed(){
    for (i = 0; i < 3; i++) {
        b.allLedsOn(15, 0, 0);
        delay(500);
        b.allLedsOff();
        delay(500);
    }
    
}

String restCall(int button){
    
    static const uint16_t TIMEOUT = 95000; // Allow maximum 95s between data packets.

    request.hostname = "YOURSERVERIP";
    request.port = YOURSERVERPORT;

    // Check if user already requested the restCall less than 2 seconds ago
    if (nextTime > millis()) {
        koLed();
        return "Nothing done: button pressed too soon after the previous request";
    }

    // Specified paths are just customizable examples
    if (button == 1){
        request.path = "/internet_button_1/1/unreal";
    }
    if (button == 2){
        request.path = "/internet_button_1/2/unreal";
    }
    if (button == 3){
        request.path = "/internet_button_1/3/unreal";
    }
    if (button == 4){
        request.path = "/internet_button_1/4/unreal";
    }
    
    // Get request
    http.get(request, response, headers);

    // Print response.status and response.body to serial
    Serial.println(response.status);
    Serial.println(response.body);

    // Update nextTime execution
    nextTime = millis() + 500;

    // If request went well blink led to inform user
    // Response status and body can be customized to match
    // The ones given by your REST API
    if (response.status==200 and response.body=="done") {
        
        if (button==1){
            okLed(1);
        }
        
        if (button==2){
            okLed(3);
        }

        
        if (button==3){
            okLed(6);
        }
        
        if (button==4){
            okLed(9);
        }
        
    }
    
    if (response.status=-1 and button==1){
 
        okLed(1);
        
    }
    
    if (response.status=-1 and button==2){
        
        okLed(3);
    
    }
    
    if (response.status=-1 and button==4){
        
        okLed(9);
           
    }

    return response.body;
    
}

String checkService(int button) {

    request.hostname = "YOURSERVERIP";
    request.port = YOURPORT;
    request.path = "/ota";
    // Get request
    http.get(request, response, headers);

    if (response.status==200) {
        b.playSong("C1,4,C1,4,C1,4");
        restCall(button);
    } else {
        koLed();
    }

}
