# Bluetooth Gateway
The Bluetooth gateway for CapstoneA14 project. This gateway works by consuming data from the sensor transmitted using Bluetooth protocol and then re-transmits it using MQTT protocol. Developed using AWS M5Stack Core2 board and PlatformIO framework


## How to run this code
### Run the mqtt server and node-red
- cd ./docker
- docker compose up -d
### Upload the code to the board
- pio run -t upload -e m5stack
- pio run -t upload -t monitor -e m5stack (optional: for monitoring the serial port)