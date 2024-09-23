#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>

#define WIFI_NAME "your-wifi-name"
#define WIFI_PWD "your-wifi-password"
#define FIREBASE_HOST "your-firebase-url"  // Your Firebase URL
#define FIREBASE_AUTH "your-firebase-secret-key" // Your Firebase Authentication Key

// Wi-Fi credentials
const char *ssid = WIFI_NAME;
const char *password = WIFI_PWD;

// Firebase details
FirebaseConfig config;  // Firebase configuration
FirebaseAuth auth;      // Firebase authentication
FirebaseData firebaseData; // Firebase setup

#define DHTPIN 4        // DHT sensor: Pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT11 or DHT22 sensor type
DHT dht(DHTPIN, DHTTYPE);

void connectToWiFi() {
  Serial.print("[WiFi] Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  int tryDelay = 500;
  int numberOfTries = 20;
  
  while (WiFi.status() != WL_CONNECTED && numberOfTries > 0) {
    delay(tryDelay);
    Serial.print(".");
    numberOfTries--;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WiFi] Connected!");
    Serial.print("[WiFi] IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("[WiFi] Failed to connect.");
  }
}

void sendDataToFirebase(float temperature, float humidity) {
  String path = "/sensorData"; // Firebase path where data will be stored

  if (Firebase.setFloat(firebaseData, path + "/temperature", temperature)) {
    Serial.println("Temperature data sent to Firebase");
  } else {
    Serial.println("Failed to send temperature data to Firebase");
    Serial.println(firebaseData.errorReason());
  }

  if (Firebase.setFloat(firebaseData, path + "/humidity", humidity)) {
    Serial.println("Humidity data sent to Firebase");
  } else {
    Serial.println("Failed to send humidity data to Firebase");
    Serial.println(firebaseData.errorReason());
  }
}

void setup() {
  Serial.begin(9600);
  dht.begin();  // Initialize the DHT sensor
  
  connectToWiFi();  // Attempt to connect to Wi-Fi

  if (WiFi.status() == WL_CONNECTED) {
    // Setup Firebase config and authentication
    config.host = FIREBASE_HOST;
    config.signer.tokens.legacy_token = FIREBASE_AUTH;

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Read humidity and temperature from DHT sensor
    float humidity = dht.readHumidity();
    float temperatureC = dht.readTemperature();

    if (isnan(humidity) || isnan(temperatureC)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Print readings to Serial Monitor
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%  Temperature: ");
    Serial.print(temperatureC);
    Serial.println("°C");

    // Send data to Firebase
    sendDataToFirebase(temperatureC, humidity);

  } else {
    Serial.println("WiFi not connected. Attempting to reconnect...");
    connectToWiFi();
  }

  delay(5000);  // Delay between sensor readings
}
