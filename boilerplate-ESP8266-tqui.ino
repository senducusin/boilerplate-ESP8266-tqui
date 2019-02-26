// Libraries
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Pin definition
#define lm35Pin A0
#define appliancePin 2
#define readPin 0

// Replace with your network credentials
const char* ssid = "home";//"<Your WiFI SSID>";
const char* password = "Pqwe^&8rtYu54B7*@@#";//"<Your WiFI Password>";

// Millis Timer
unsigned long previousMillis = 0;
const long interval = 50;

// Web UI variables
String page = "";
String text = "";

// Logic variables
int tempValue = 0;
float cel;
String appStatus = "OFF";

//instantiate server at port 80 (http port)
ESP8266WebServer server(80);

void setup() {
  // Pin Initialization
  pinMode(appliancePin, OUTPUT);
  pinMode(readPin, INPUT);

  Serial.begin(9600);
  delay(1);
  WiFi.begin(ssid, password); //begin WiFi connection

  // Wait for connection
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/data.txt", []() {
    text = (String)cel + "|" + appStatus;
    server.send(200, "text/html", text);
  });

  server.on("/", []() {

    page = "<h1>ESP8266 Web UI + AJAX Boilerplate</h1>\n";
    page += "<h2>Analog Check</h2>\n";
    page += "<h3 style=\"display:inline-block; margin-right:5px;\">Temperature:</h3>\n";
    page += "<h3 style=\"display:inline-block;\" id=\"analog-data\">""</h3>\r\n";
    page += "<a href=\"toggleLink\">Toggle Appliance</a>\n";
    page += "<h2>Digital Check</h2>\n";
    page += "<h3 style=\"display:inline-block; margin-right:5px;\">Appliance:</h3>\n";
    page += "<h3 style=\"display:inline-block;\" id=\"digital-data\">""</h3>\r\n";

    page += "<script>\r\n";
    page += "var x = setInterval(function() {loadData(\"data.txt\",updateData)}, 1000);\r\n";

    page += "function loadData(url, callback){\r\n";
    page += "var xhttp = new XMLHttpRequest();\r\n";
    page += "xhttp.onreadystatechange = function(){\r\n";
    page += " if(this.readyState == 4 && this.status == 200){\r\n";
    page += " callback.apply(xhttp);\r\n";
    page += " }\r\n";
    page += "};\r\n";
    page += "xhttp.open(\"GET\", url, true);\r\n";
    page += "xhttp.send();\r\n";
    page += "}\r\n";

    page += "function updateData(){\r\n";
    page += "let data = this.responseText.split('|')\n";
    page += "let temp = data[0]\n";
    page += "document.getElementById(\"analog-data\").innerHTML = temp;\r\n";
    page += "let relay = data[1]\n";
    page += "document.getElementById(\"digital-data\").innerHTML = relay;\r\n";
    page += "}\r\n";
    page += "</script>\r\n";

    server.send(200, "text/html", page);
  });

  server.on("/toggleLink", TogglePin);
  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    int analogValue = analogRead(lm35Pin);
    cel = getCelsius(analogValue);

    int digitalValue = digitalRead(appliancePin);

    if (digitalValue == HIGH) {
      appStatus = "ON";
    } else {
      appStatus = "OFF";
    }

    server.handleClient();
  }
}
