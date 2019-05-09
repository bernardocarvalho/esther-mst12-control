// Relay control using the ESP8266 WiFi chip
// https://www.norwegiancreations.com/2017/03/state-machines-and-arduino-implementation
//
// Import required libraries
#include <ESP8266WiFi.h>
#include <aREST.h>
#include <aREST_UI.h>

// Create aREST instance
aREST_UI rest = aREST_UI();

// WiFi parameters
// const char *ssid = "Cabovisao-E30F";
// const char *password = "e0cec31ae30f";
const char *ssid = "Vodafone-EDB45A";
const char *password = "71F62F063E";

enum State_enum { STOP, FORWARD, ROTATE_RIGHT, ROTATE_LEFT };
enum Sensors_enum { NONE, SENSOR_RIGHT, SENSOR_LEFT, BOTH };

uint8_t state = STOP;

// The port to listen for incoming TCP connections
#define LISTEN_PORT 80
#define LED_WEMOOS_D1_MINI 2 // Pin D4 GPIO2

// Create an instance of the server
WiFiServer server(LISTEN_PORT);
void printWifiStatus();

void setup(void) {
  // Start Serial

  Serial.begin(115200);
  Serial.println("Trying WiFi ....");

  // Create UI
  rest.title("Relay Control");
  rest.button(4);
  rest.variable("state", &state);
  rest.label("state");

  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("esp8266");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  // Serial.println(WiFi.localIP());
  // Print out the status
  printWifiStatus();
}

void loop() {

  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while (!client.available()) {
    delay(1);
  }
  rest.handle(client);
  // Serial.print(".");
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
