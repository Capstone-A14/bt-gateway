#include <M5Core2.h>
#include <EspMQTTClient.h>
#include <ArduinoJson.h>

#define ECG_BUFFER 100


/*EspMQTTClient mqttClient(
        "Herdi",
        "127127127",
        "192.168.1.12",
        "m5stack",
        1883
);*/

EspMQTTClient mqttClient(
        "UGM SECURE 2",
        "127127127",
        "192.168.91.102",
        "m5stack",
        1883
);


void onConnectionEstablished(){
    mqttClient.publish("/time/dev0/pat0", "100");
    mqttClient.publish("/time/dev1/pat1", "101");
}

void setup() {
    Serial.begin(115200);
    M5.begin();

    mqttClient.setMaxPacketSize(1024*5);
    mqttClient.enableDebuggingMessages(); // Enable debugging messages sent to serial output

    M5.Lcd.printf("START");
}

//one ecg cycle. source: https://www.hl7.org/fhir/observation-example-sample-data.html
int ecgMock[116] = {2041,2043,2037,2047,2060,2062,2051,2023,2014,2027,2034,2033,2040,2047,2047,2053,2058,2064,2059,2063,2061,2052,2053,2038,1966,1885,1884,2009,2129,2166,2137,2102,2086,2077,2067,2067,2060,2059,2062,2062,2060,2057,2045,2047,2057,2054,2042,2029,2027,2018,2007,1995,2001,2012,2024,2039,2068,2092,2111,2125,2131,2148,2137,2138,2128,2128,2115,2099,2097,2096,2101,2101,2091,2073,2076,2077,2084,2081,2088,2092,2070,2069,2074,2077,2075,2068,2064,2060,2062,2074,2075,2074,2075,2063,2058,2058,2064,2064,2070,2074,2067,2060,2062,2063,2061,2059,2048,2052,2049,2048,2051,2059,2059,2066,2077,2073};

int samplingFreq = 20; //Hz
int samplingInterval = 5; //sampling windows in second. (create new sampling batch every 5 second)
int ecgBuffer[ECG_BUFFER];
String payload;
int buffItr = 0;
int mockItr = 0;
int tempItr = 0;

void SerializeJson(int input[ECG_BUFFER], String &output){
    StaticJsonDocument<24576> doc;
    JsonArray data = doc.createNestedArray("data");
    for(int i=0; i< ECG_BUFFER; i++){
        data.add(input[i]);
    }
    serializeJson(doc, output);
}




void loop() {
    mqttClient.loop();
    if(mqttClient.isConnected()){
        ecgBuffer[buffItr] = ecgMock[mockItr];// membaca detak "jantung"
        Serial.printf("adding %d on buffer at %d itteration\n",ecgBuffer[buffItr], buffItr);
        buffItr++;mockItr++;
        if(mockItr==116){
            mockItr=0;
        }
        if(buffItr==(samplingFreq*samplingInterval)){
            SerializeJson(ecgBuffer, payload);
            Serial.println("paload: ");
            Serial.println(payload);
            if(tempItr%2==0){
                mqttClient.publish("/ecg/lead1", payload); //publish the buffer
//                mqttClient.publish("/imu/dev0/pat0", payload);
            } else{
                mqttClient.publish("/ecg/lead2", payload); //publish the buffer
//                mqttClient.publish("/imu/dev0/pat0", payload);
            }
            payload.clear();
            tempItr++;
            buffItr=0;
        }
        delay(1000/samplingFreq);
    }
}