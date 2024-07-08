#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <DHT.h>

#define FIREBASE_HOST "iot24-263f5-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "IVDcjDwevxVFYTzf0ZxoIVc2YGjpLyo3qrtatFVK3
"  

const char* ssid = "Bos Dian";
const char* password = "pakenanya";

#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define TRIGPIN D1
#define ECHOPIN D2

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi with IP: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  dht.begin();
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);

  long duration = pulseIn(ECHOPIN, HIGH);
  float distance = duration / 2 * 28.9;

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Firebase.setFloat("/temperature", temperature);
  if (Firebase.failed()) {
    Serial.print("Failed to update temperature: ");
    Serial.println(Firebase.error());
  } else {
    Serial.println("Temperature updated");
  }

  Firebase.setFloat("/humidity", humidity);
  if (Firebase.failed()) {
    Serial.print("Failed to update humidity: ");
    Serial.println(Firebase.error());
  } else {
    Serial.println("Humidity updated");
  }

  Firebase.setFloat("/feed_level", distance);
  if (Firebase.failed()) {
    Serial.print("Failed to update feed level: ");
    Serial.println(Firebase.error());
  } else {
    Serial.println("Feed level updated");
  }

  delay(2000); 
}
