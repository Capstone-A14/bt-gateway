# Bluetooth Gateway
The bluetooth to mqtt gateway for CapstoneA14 project. Developed using AWS M5Stack Core2 board and PlatformIO framework

## How to run this code
### Run the mqtt server and node-red
- cd ./docker
- docker compose up -d
### Upload the code to the board
- pio run -t upload -e m5stack
- pio run -t upload -t monitor -e m5stack (optional: for monitoring the serial port)