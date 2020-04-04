#include <ArduinoJson.h>
#include <Wire.h>

const byte I2C_SLAVE_ADDR = 0x8;
const byte I2C_LENGTH_LIMIT = 32;
const byte ASK_FOR_LENGTH = 0x0;
const byte ASK_FOR_DATA = 0x1;

String json;
StaticJsonDocument<300> doc;
char request = ' ';
byte requestIndex = 0;

void serializeObject()
{
    doc["this"] = 123.456;
    doc["is"] = "hello";
    doc["a"] = "word!";
    doc["json"] = "python";
    doc["example"] = 789;
    serializeJson(doc, json);
}

void receiveEvent(int nBytes) {
    while (Wire.available()) {
        request = (char) Wire.read();
    }
}

void requestEvent() {

    if (request == ASK_FOR_LENGTH) {
        Wire.write(json.length());
        requestIndex = 0;
    }

    if (request == ASK_FOR_DATA) {
        if (requestIndex < (json.length() / I2C_LENGTH_LIMIT)) {
            Wire.write(json.c_str() + requestIndex * I2C_LENGTH_LIMIT, I2C_LENGTH_LIMIT);
            requestIndex++;
        }
        else {
            Wire.write(json.c_str() + requestIndex * I2C_LENGTH_LIMIT, json.length() % I2C_LENGTH_LIMIT);
            requestIndex = 0;
        }
    }
}

void setup() {

    Serial.begin(9600);
    serializeObject();
    Wire.begin(I2C_SLAVE_ADDR);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);

    Serial.print("Json size: "); Serial.print(json.length()); Serial.println(" bytes");
    Serial.print("Json: "); Serial.print(json.c_str()); Serial.println("");
}

void loop() {

}