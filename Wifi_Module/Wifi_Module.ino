#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
 
const char* ssid     = "twguest";  
const char* password = "novo torch cycad ellen aqua";
IPAddress ip(10, 136, 127, 205); // where xx is the desired IP Address
IPAddress gateway(10,136,127,254); // set gateway/router to match your network IP
IPAddress subnet(255, 255, 252, 0); // set subnet mask to match your network 

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
int status = LOW;
const int pin = 2;
void setup() {
  delay(3000); //Check every 3 sec
  Serial.begin(115200);
  delay(100);

  // prepare GPIO2
  pinMode(pin, OUTPUT);
  pinMode(pin, HIGH);
  
  
  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){ 
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('$');
  Serial.println("Received req=>" + req);
  client.flush();
  
  // Match the request
  if (req.indexOf("status") != -1 || req.indexOf("/status") != -1)
    Serial.println("Switch is: "+ status);
  else if (req.indexOf("on") != -1 || req.indexOf("/on") != -1)
    status = HIGH;
  else if (req.indexOf("off") != -1 || req.indexOf("/off") != -1)
    status = LOW;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set GPIO2 according to the request
  digitalWrite(pin, status);
  // Prepare the response
  String response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nSwitch is now ";
  response += status;
  response += "</html>\n";
  // Send the response to the client
  client.print(response);
  client.flush();

  delay(1);
  Serial.println("Client disonnected"+ status);

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

