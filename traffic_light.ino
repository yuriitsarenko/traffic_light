#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>

MDNSResponder mdns;

const char* ssid = "wifi";
const char* password = "12345678";

byte arduino_mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192,168,1,100);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

int D0_pin = 16;
int D2_pin = 4;
int D1_pin = 5;
int D3_pin = 0;

//String city = "Cherkasy";
//String city = "Chernihiv";
//String city = "Chernivtsi";
//String city = "Crimea";
//String city = "Dnipropetrovs'k";
//String city = "Donets'k";
//String city = "Ivano-Frankivs'k";
//String city = "Kharkiv";
//String city = "Kherson";
//String city = "Khmel'nyts'kyy";
//String city = "Kirovohrad";
//String city = "Kyiv City";
//String city = "Kyiv";
//String city = "Luhans'k";
//String city = "L'viv";
//String city = "Mykolayiv";
//String city = "Odessa";
//String city = "Poltava";
//String city = "Rivne";
//String city = "Sevastopol";
//String city = "Sumy";
//String city = "Ternopil'";
//String city = "Transcarpathia";
//String city = "Vinnytsya";
//String city = "Volyn";
//String city = "Zaporizhzhya";
//String city = "Zhytomyr";

void setup() {
  pinMode(D0_pin, OUTPUT);
  digitalWrite(D0_pin, HIGH);
  pinMode(D2_pin, OUTPUT);
  digitalWrite(D2_pin, HIGH);
  pinMode(D1_pin, OUTPUT);
  digitalWrite(D1_pin, HIGH);
  pinMode(D3_pin, OUTPUT);
  digitalWrite(D3_pin, HIGH);

  delay(100);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);

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

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/", HTTP_GET, [](){
    server.send(200, "text/html", webPage());
  });

  server.on("/socket1On", HTTP_GET, [](){
    digitalWrite(D0_pin, LOW);
    server.send(200, "text/html", webPage());
    delay(100);
  });

  server.on("/socket1Off", HTTP_GET, [](){
    digitalWrite(D0_pin, HIGH);
    server.send(200, "text/html", webPage());
    delay(100);
  });

  server.on("/socket2On", HTTP_GET, [](){
    digitalWrite(D1_pin, LOW);
    server.send(200, "text/html", webPage());
    delay(100);
  });

  server.on("/socket2Off", HTTP_GET, [](){
    digitalWrite(D1_pin, HIGH);
    server.send(200, "text/html", webPage());
    delay(100);
  });

  server.on("/socket3On", HTTP_GET, [](){
    digitalWrite(D2_pin, LOW);
    server.send(200, "text/html", webPage());
    delay(100);
  });

  server.on("/socket3Off", HTTP_GET, [](){
    digitalWrite(D2_pin, HIGH);
    server.send(200, "text/html", webPage());
    delay(100);
  });

  server.on("/socket4On", HTTP_GET, [](){
    digitalWrite(D3_pin, LOW);
    server.send(200, "text/html", webPage());
    delay(100);
  });

  server.on("/socket4Off", HTTP_GET, [](){
    digitalWrite(D3_pin, HIGH);
    server.send(200, "text/html", webPage());
    delay(100);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  if (WiFi.status() == WL_CONNECTED) {
    ConnectAPI();
    delay(2000);
  } else {
    Serial.println("not wifi or json error");
  }
}

String webPage() {
  String web; 
  web += "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  web += "<title>Fractured Byte</title><style>body{display:flex;flex-direction:column;align-items:center;justify-content:center;height:100vh;margin:0;background-color:#f0f0f0;}.header{font-size:24px;margin-bottom:20px;}.traffic-light{display:flex;flex-direction:column;align-items:center;border:2px solid #000;padding:10px;border-radius:10px;background-color:#C0C0C0;}.light{position:relative;width:150px;height:150px;border-radius:50%;margin:10px;border:1px solid #000;display:flex;align-items:center;justify-content:center;}.button-container{display:flex;justify-content:space-around;align-items:center;margin-top:5px;}.button-container a+a{margin-left:7px;}.relay-label{font-size:16px;position:relative;}.relay-label::before{content:\"\";position:absolute;top:-5px;right:-5px;bottom:-5px;left:-5px;border:2px solid #000;border-radius:5px;pointer-events:none;box-sizing:border-box;}.button{padding:5px 10px;font-size:12px;background-color:#fff;border:1px solid #000;border-radius:5px;cursor:pointer;margin:5px;}";
  
  if (digitalRead(D0_pin) == 1) {
    web += ".red { background-color: #800000; }";
  } else {
    web += ".red { background-color: #FF0000; }";
  }

  if (digitalRead(D1_pin) == 1) {
    web += ".yellow { background-color: #D2691E; }";
  } else {
    web += ".yellow { background-color: #FFD700; }";
  }

  if (digitalRead(D2_pin) == 1) {
    web += ".green { background-color: #008000; }</style></head><body>";
  } else {
    web += ".green { background-color: #00FF00; }</style></head><body>";
  }

  web += "<br /><div class=\"traffic-light\"><div class=\"light red\">";
  web += "<div class=\"button-container\"><a href=\"socket1On\"><button style=\"z-index: 1; width: 49px; height: 24px;\">ON</button></a><a href=\"socket1Off\"><button style=\"z-index: 1; width: 49px; height: 24px;\">OFF</button></a></div></div>";

  web += "<div class=\"light yellow\">";
  web += "<div class=\"button-container\"><a href=\"socket2On\"><button style=\"z-index: 1; width: 49px; height: 24px;\">ON</button></a><a href=\"socket2Off\"><button style=\"z-index: 1; width: 49px; height: 24px;\">OFF</button></a></div></div>";

  web += "<div class=\"light green\">";
  web += "<div class=\"button-container\"><a href=\"socket3On\"><button style=\"z-index: 1; width: 49px; height: 24px;\">ON</button></a><a href=\"socket3Off\"><button style=\"z-index: 1; width: 49px; height: 24px;\">OFF</button></a></div></div></div><br />";

  web += "<div style=\"text-align:center;margin-top:20px;\"><a href=\"/\"><button style=\"width:158px;\">REFRESH</button></a></div><br />";
  web += "<footer>© Fractured Byte</footer></body></html>";

  return web;
}

boolean ConnectAPI() {
  WiFiClient client;
  client.setTimeout(3000);
  if (!client.connect("68.183.208.54", 80)) {   // sirens.in.ua
    Serial.println(F("Error connecting to sirens.in.ua"));
    return false;
  }

  client.println(F("GET /api/v1/ HTTP/1.0"));
  client.println(F("Host: sirens.in.ua"));
  client.println(F("Connection: close"));
  if (client.println() == 0) {
    Serial.println(F("Error sending request"));
    client.stop();
    return false;
  }

  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status + 9, "200 OK") != 0) {
    Serial.println(status);
    client.stop();
    return false;
  }

  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Incorrect answer"));
    client.stop();
    return false;
  }

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    Serial.print(F("JSON processing error: "));
    Serial.println(error.f_str());
    client.stop();
    return false;
  }

  client.stop();

  String result = doc[city].as<String>();
  Serial.print(F("Response: "));
  Serial.println(result);

  if (result == "full") {
    // Switch off all lamps
    digitalWrite(D0_pin, HIGH);  // Switch off red
    digitalWrite(D1_pin, HIGH);  // Switch off yellow
    digitalWrite(D2_pin, HIGH);  // Switch off green
    // Red alarm signal
    digitalWrite(D0_pin, LOW);  // Turn on red
  } else if (result == "partial" || result == "no_data") {
    // Yellow flashing in case data is not received
    // Switch off all lamps
    digitalWrite(D0_pin, HIGH);  // Switch off red
    digitalWrite(D1_pin, HIGH);  // Switch off yellow
    digitalWrite(D2_pin, HIGH);  // Switch off green
    // Switch on all lamps
    digitalWrite(D0_pin, LOW);  // Switch on red
    digitalWrite(D1_pin, LOW);  // Switch on yellow
    digitalWrite(D2_pin, LOW);  // Switch on green
  } else {
    // Switch off all lamps
    digitalWrite(D0_pin, HIGH);  // Switch off red
    digitalWrite(D1_pin, HIGH);  // Switch off yellow
    digitalWrite(D2_pin, HIGH);  // Switch off green
    // Green alarm signal
    digitalWrite(D2_pin, LOW);  // Turn on green
  }
  return true;
}
