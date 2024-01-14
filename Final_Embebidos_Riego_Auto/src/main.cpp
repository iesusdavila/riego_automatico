#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "RefurbishedF1";  // Nombre de la red Wi-Fi RefurbishedF1 notengo
const char* password = "DavilaAguilar2022"; // Contraseña de la red Wi-Fi DavilaAguilar2022 abcd1234

// Crear un objeto de servidor en el puerto 80
ESP8266WebServer server(80);

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int moisture_Pin = 0; // Soil Moisture Sensor input at Analog PIN A0
int moisture_value = 0, moisture_state = 0xFF;

#define RELAY 5
bool est_relay = false;
#define THRESHOLD_HUM_SOIL 30
#define THRESHOLD_HUM_AIR 40
#define THRESHOLD_TEMP 31

#define LED_VERDE 14
bool est_led_verde = false;
#define LED_ROJO 12
bool est_led_rojo = false;

float temp;
float hum;

void handleRoot() {
    server.send(200, "text/plain", "Bienvenido al servidor ESP8266");
}

void handleData() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    String json = "{";
    json += "\"moisture_value\":" + String(moisture_value) + ",";
    json += "\"temp\":" + String(temp) + ",";
    json += "\"hum\":" + String(hum) + ",";
    json += "\"LED_VERDE\":" + String((est_led_verde ? "true" : "false")) + ",";
    json += "\"LED_ROJO\":" + String((est_led_rojo ? "true" : "false")) + ",";
    json += "\"bomba\":" + String((est_relay ? "true" : "false"));
    json += "}";
    server.send(200, "application/json", json);
}

void setup()
{
    dht.begin();
    //Serial.begin(9600);
    pinMode(LED_VERDE, OUTPUT);
    pinMode(LED_ROJO, OUTPUT);
    pinMode(RELAY, OUTPUT);

    // ------------------ CONEXION WIFI ------------------
    Serial.begin(115200);
    delay(10);

    // Conexión a la red Wi-Fi
    Serial.println();
    Serial.println();
    Serial.print("Conectando a ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi conectado");  
    Serial.println("Dirección IP: ");
    Serial.println(WiFi.localIP());

    // ------------------ SERVIDOR WEB ------------------
    server.on("/", handleRoot);
    server.on("/data", handleData);
    server.begin();
    Serial.println("Servidor HTTP iniciado");
    // ------------------ FIN SERVIDOR WEB ------------------
}

void loop()
{
    server.handleClient();

    temp = dht.readTemperature();
    hum = dht.readHumidity();

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print(" C\n");
    Serial.print(" Humidity: ");
    Serial.print(hum);
    Serial.print(" %\n");

    Serial.print("MOISTURE LEVEL : ");
    moisture_value = (100.00 - ((analogRead(moisture_Pin) / 1023.00) * 100.00));
    Serial.print(moisture_value);
    Serial.print(" %\n");

    if ((moisture_value < THRESHOLD_HUM_SOIL) || (hum < THRESHOLD_HUM_AIR) || (temp > THRESHOLD_TEMP))
    {
        Serial.println("The soil moisture is DRY => activate pump");
        est_led_verde = false;
        est_led_rojo = true;
        est_relay = true;
        digitalWrite(RELAY, HIGH);
        digitalWrite(LED_VERDE, LOW);
        digitalWrite(LED_ROJO, HIGH);
    }
    else
    {
        Serial.println("The soil moisture is WET=> desactivate the pump");
        est_led_verde = true;
        est_led_rojo = false;
        est_relay = false;
        digitalWrite(RELAY, LOW);
        digitalWrite(LED_VERDE, HIGH);
        digitalWrite(LED_ROJO, LOW);
    }
    delay(1000);
    Serial.println("---------------------");
}