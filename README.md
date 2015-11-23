# README #

Particle Photon Sensors - DHT22

### What is this repository for? ###

This script allows to read data from a DHT22 (or other sensors supported by PietteTech_DHT library) using Particle Photon and publish retrieved data on the web and print it on serial

### How do I get set up? ###

* Copy the code contained into dht22.ino into Particle Build: https://build.particle.io/build/new
* Add the required PietteTech_DHT library to your project searching it in "Libraries", in your Particle Build project
* Flash it to your Particle Photon
* Cackle into the darkness

### Possible problems ###

If you get weird output from serial, in 99% of the cases, it is because of a bad connection or the use of a wrong resistor.

I had success with 4.7k resistor but your mileage may vary.

### Retrieve published variables using Particle API ###

* Temperature: https://api.particle.io/v1/devices/YOUR_DEVICE_ID/temperature/?access_token=YOUR_ACCESS_TOKEN
* Humidity: https://api.particle.io/v1/devices/YOUR_DEVICE_ID/humidity/?access_token=YOUR_ACCESS_TOKEN

Read Particle Documentation if you need further details: https://docs.particle.io/reference/api/

### Contribution guidelines ###

* If you have any idea or suggestion contact directly the Repo Owner

### Who do I talk to? ###

* ltpitt: Repo Owner
