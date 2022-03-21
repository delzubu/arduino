#include <ETH.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiSTA.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

#include <dummy.h>

const int PORT = 13;
int level = LOW;

const char* ssid = "taliganetmb 3";
const char* password = "Miu321Miu";

WiFiServer server(80);
const long TIMEOUT_TIME = 2000;


void dumpAP() {
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
      Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");
}

void setupWifi() {
  // Connect to Wi-Fi network with SSID and password
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  WiFi.persistent(false);
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(false);
  WiFi.setAutoReconnect(true);
  WiFi.enableLongRange(true);
  WiFi.setTxPower(WIFI_POWER_19_5dBm);

  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
}

void connectWifi() {
  setupWifi();
//  dumpAP();
  
    
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int status;
  Serial.print("Status: ");
  while (status = WiFi.status() != WL_CONNECTED) {
    Serial.print(status);
    Serial.print(",");
    delay(250);
  }
  Serial.println(status);
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(PORT, OUTPUT);

  connectWifi();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {
     handleClient(client);
     digitalWrite(PORT, level);
  }
}

void processHeader(String header) {
  // turns the GPIOs on and off
  if (header.indexOf("GET /13/on") >= 0) {
    Serial.println("GPIO 13 on");
    level = HIGH;
  } else if (header.indexOf("GET /13/off") >= 0) {
    Serial.println("GPIO 13 off");
    level = LOW;
  }
}

void writeResponse(WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();

  // Display the HTML web page
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  // CSS to style the on/off buttons 
  // Feel free to change the background-color and font-size attributes to fit your preferences
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println(".button2 {background-color: #555555;}</style></head>");
            
  // Web Page Heading
  client.println("<body><h1>ESP32 Web Server</h1>");
            
  // Display current state, and ON/OFF buttons for GPIO 26  
  client.print("<p>GPIO 13 - State ");
  if(level == HIGH) {
    client.print("ON");
  } else {
    client.print("OFF");
  }
  client.println("</p>");
  // If the output26State is off, it displays the ON button       
  if (level==LOW) {
    client.println("<p><a href=\"/13/on\"><button class=\"button\">ON</button></a></p>");
  } else {
    client.println("<p><a href=\"/13/off\"><button class=\"button button2\">OFF</button></a></p>");
  } 
  client.println("</body></html>");
            
  // The HTTP response ends with another blank line
  client.println();
}

void handleClient(WiFiClient client)
{
  unsigned long currentTime = millis();
  unsigned long previousTime = currentTime;
  String header = "";
  Serial.println("New Client.");          // print a message out in the serial port
  String currentLine = "";                // make a String to hold incoming data from the client
  while (client.connected() && currentTime - previousTime <= TIMEOUT_TIME) {  // loop while the client's connected
    currentTime = millis();
    if (client.available()) {             // if there's bytes to read from the client,
      char c = client.read();             // read a byte, then
      Serial.write(c);                    // print it out the serial monitor
      header += c;
      if (c == '\n') {                    // if the byte is a newline character
        // if the current line is blank, you got two newline characters in a row.
        // that's the end of the client HTTP request, so send a response:
        if (currentLine.length() == 0) {
          // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
          // and a content-type so the client knows what's coming, then a blank line:
          processHeader(header);
          writeResponse(client);
          break;
        } else { // if you got a newline, then clear currentLine
          currentLine = "";
        }
      } else if (c != '\r') {  // if you got anything else but a carriage return character,
        currentLine += c;      // add it to the end of the currentLine
      }
    }
  }
  // Close the connection
  client.stop();
  Serial.println("Client disconnected.");
  Serial.println("");
}
