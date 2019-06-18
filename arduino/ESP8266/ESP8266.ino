#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK  "your-password"
#endif

const char* ssid     = "AAAA";
const char* password = "77777777";
const char* host = "140.126.21.185";
const uint16_t port = 80;

float temperature = 30;
float humidity = 20;
float windspeed = 10;
void setup() {
  Serial.begin(9600);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //==========
  temperature += 1;
  humidity += 2;
  windspeed += 3;
  //==========
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  //DB/script/ins.php?a=4&b=4&c=4
  String url = "/DB/script/ins.php?";
  url += "temperature=";
  url += temperature;
  url += "&humidity=";
  url += humidity;
  url += "&windspeed=";
  url += windspeed;
  
  // This will send a string to the server
  Serial.print("URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n"+"Connection: close\r\n\r\n");

  // wait for data to be available
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Close the connection
  Serial.println();
  Serial.println("closing connection");

  delay(5000); // execute once every 5 minutes, don't flood remote service
}
