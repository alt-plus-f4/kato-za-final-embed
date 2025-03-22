#include <ESP8266WiFi.h>

// WiFi credentials
const char* ssid = "AndroidAP";
const char* password = "";  // No password for open network

// Server details
const char* server = "192.168.157.1";  // Replace with your server's IP
const int port = 5000;  // Flask default port

WiFiClient client;

const char* machine_id = "machine_001";  // Unique machine identifier

float money = 0.00;

void setup() {
    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");

    Serial.println("Ready to receive UART commands...");
}

void sendRequest(const char* path) {
    Serial.print("Connecting to server...");
    if (client.connect(server, port)) {
        Serial.println("Connected!");

        // Send HTTP GET request
        client.print(String("GET ") + path + " HTTP/1.1\r\n" +
                     "Host: " + server + "\r\n" +
                     "Connection: close\r\n\r\n");

        // Wait for response
        while (client.connected() || client.available()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);  // Print server response
            }
        }
        client.stop();  // Close connection
    } else {
        Serial.println("Connection failed");
    }
}

void loop() {
    if (Serial.available() > 0) {  // Check if data is received via UART
        String receivedData = Serial.readStringUntil('\n');  // Read UART data
        receivedData.trim();  // Remove any whitespace or newline characters
        Serial.println("Received: " + receivedData);

        // Increment money based on the received message
        if (receivedData == "INC 20") {
            money += 0.20;
        } else if (receivedData == "INC 50") {
            money += 0.50;
        } else if (receivedData == "INC 100") {
            money += 1.00;
        }

        Serial.println("Money incremented: " + String(money));

        // Send update request to server
        String updatePath = "/update?value=" + String(money) + "&machine_id=" + String(machine_id);
        sendRequest(updatePath.c_str());
    }
}
