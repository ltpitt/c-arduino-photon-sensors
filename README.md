# README #

Particle Photon DHT22 (or other sensor)

### What is this repository for? ###

This script allows to read data from a DHT22 (or other sensor) using Particle Photon, publish it on the web and print it on serial

### How do I get set up? ###

* Copy the code contained into dht22.ino into Particle Build
* Add the required PietteTech_DHT library to your project using Particle Build
* Flash it to your Particle Photon
* Cackle into the darkness

### Possible problems ###

If you get weird output from serial it is 99% of the cases bad connection or use of wrong resistor.
I had success with 4.7k resistor but your mileage may vary.

### Retrieve published variables using Particle API ###

* Temperature: https://api.particle.io/v1/devices/YOUR_DEVICE_ID/temperature/?access_token=YOUR_ACCESS_TOKEN
* Humidity: https://api.particle.io/v1/devices/YOUR_DEVICE_ID/humidity/?access_token=YOUR_ACCESS_TOKEN

Read Particle Documentation if you need further details

### Contribution guidelines ###

* If you have any idea or suggestion contact directly the Repo Owner

### Who do I talk to? ###

* ltpitt: Repo Owner
