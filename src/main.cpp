#include <M5Core2.h>
#include <EspMQTTClient.h>
#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

EspMQTTClient mqttClient(
        "Grande Space",
        "Grande_ccdk",
        "192.168.1.78",  // MQTT Broker server ip
        "m5stack",     // Client name that uniquely identify your device
        1883              // The MQTT port, default to 1883. this line can be omitted
);

void onConnectionEstablished(){
    mqttClient.subscribe("/m5stack/sub", [](const String & payload) {
        Serial.println(payload);
    });
    mqttClient.publish("/m5stack/pub", "Hello World"); // You can activate the retain flag by setting the third parameter to true
}

void setup() {
    Serial.begin(115200);
    M5.begin();
    M5.Lcd.printf("START");

    SerialBT.begin("ESP32test"); //Bluetooth device name
    Serial.println("The device started, now you can pair it with bluetooth!");

    mqttClient.enableDebuggingMessages();
}

void loop() {
    mqttClient.loop();
    if (SerialBT.available()) {
        String message = SerialBT.readString();
        Serial.println(message);
        mqttClient.publish("/m5stack/pub", message);
    }
    delay(20);
}