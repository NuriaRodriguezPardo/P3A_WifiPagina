#include <WiFi.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <WebServer.h>
#include <Arduino.h>

const char* ssid = "RedmiNuria";
const char* password = "Patata123";


WebServer server;

String webContent; 

void handle_root() {
  server.send(200, "text/html", webContent);
}

String loadFile(String fileName) { 
  File file = SPIFFS.open(fileName);
  String content = "";

  if (!file) {
    Serial.println("Error al abrir el archivo");
    return content;
  }

  while (file.available()) {
    char c = file.read();
    content += c;
  }

  file.close();
  return content;
}

void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("Error al montar SPIFFS");
    return;
  }

  webContent = loadFile("/PaginaWeb.html");
  delay(1000);

  Serial.println("Conectando a la red WiFi...");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conexión WiFi establecida");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Servidor HTTP iniciado");
  
  server.on("/", handle_root);
}

void loop() {
  server.handleClient();
}