#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 4 
#define DHTTYPE DHT11

const char* ssid = "Anggrek 11";
const char* password = "anGGrek888";
const char* serverName = "http://192.168.0.180:5000/endpoint";

DHT dht(DHTPIN, DHTTYPE);
bool wifiConnected = false;

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi");
  wifiConnected = true;
}

void loop() {
  if (!wifiConnected) {
    Serial.println("WiFi not connected!");
    return;
  }

  delay(2000); // Delay between readings

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String postData = "{\"temperature\": " + String(t) + ", \"humidity\": " + String(h) + "}";
    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Error in WiFi connection");
    wifiConnected = false;
  }
}
