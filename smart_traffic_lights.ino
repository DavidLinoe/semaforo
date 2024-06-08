#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>


const char* ssid     = "4g_Chacara_Sao_Jose";
const char* password = "nthm1233";

const int redLedPin = 4;    // Pino do LED vermelho
const int yellowLedPin = 2; // Pino do LED amarelo
const int greenLedPin = 15;  // Pino do LED verde

int semaphoreState = 0; // 0: vermelho, 1: amarelo, 2: verde
unsigned long previousMillis = 0;
const long redInterval = 2000; // Tempo de espera no vermelho (em milissegundos)
const long yellowInterval = 1000; // Tempo de espera no amarelo (em milissegundos)
const long greenInterval = 2000; // Tempo de espera no verde (em milissegundos)

String currentLine = "";  

WebServer server(80);

//Check if header is present and correct
bool is_authentified() {
  Serial.println("Enter is_authentified");
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {
      Serial.println("Authentification Successful");
      return true;
    }
  }
  Serial.println("Authentification Failed");
  return false;
}

void ligarSemaforo() {
painel_html();


 }
 
  
//login page, also called for disconnect
void handleLogin() {
  String msg;
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("DISCONNECT")) {
    Serial.println("Disconnection");
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Set-Cookie", "ESPSESSIONID=0");
    server.send(301);
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) {
    if (server.arg("USERNAME") == "David" &&  server.arg("PASSWORD") == "david123") {
      server.sendHeader("Location", "/home1");
      server.sendHeader("Cache-Control", "no-cache");
      server.sendHeader("Set-Cookie", "ESPSESSIONID=1");
      server.send(301);
      Serial.println("Log in Successful");
      return;
    }
    msg = "Usuario ou Senha Invalidos!";
    Serial.println("Log in Failed");
  }

   index_html();

}

//root page can be accessed only if authentification is ok
void handleRoot() {
  Serial.println("Enter handleRoot");
  String header;
  if (!is_authentified()) {
    server.sendHeader("Location", "/home");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
dashboard_html();
  
}

//no need authentification
void handleNotFound() {
  
page_not_found_html();

}

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.on("/home", ligarSemaforo);
  server.on("/home1", ligarSemaforo);

  server.on("/inline", []() {
  server.send(200, "text/plain", "this works without need of authentification");
  });

  server.onNotFound(handleNotFound);
  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize);
  server.begin();
  Serial.println("HTTP server started");


   Serial.begin(115200);
     pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  
  // Desligar todos os LEDs inicialmente
  digitalWrite(redLedPin, LOW);
  digitalWrite(yellowLedPin, LOW);
  digitalWrite(greenLedPin, LOW);
    delay(10);
  
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
//---
// Verifica o estado atual do semáforo e liga o LED correspondente
 unsigned long currentMillis = millis();

  // Verifica o estado atual do semáforo e liga o LED correspondente
  switch (semaphoreState) {
    case 0: // Vermelho
      if (currentMillis - previousMillis >= redInterval) {
        previousMillis = currentMillis;
        semaphoreState++; // Muda para amarelo
        digitalWrite(redLedPin, HIGH);
        digitalWrite(yellowLedPin, LOW);
        digitalWrite(greenLedPin, LOW);
      }
      break;
    case 1: // Amarelo
      if (currentMillis - previousMillis >= yellowInterval) {
        previousMillis = currentMillis;
        semaphoreState++; // Muda para verde
        digitalWrite(redLedPin, LOW);
        digitalWrite(yellowLedPin, HIGH);
        digitalWrite(greenLedPin, LOW);
      }
      break;
    case 2: // Verde
      if (currentMillis - previousMillis >= greenInterval) {
        previousMillis = currentMillis;
        semaphoreState = 0; // Muda para vermelho
        digitalWrite(redLedPin, LOW);
        digitalWrite(yellowLedPin, LOW);
        digitalWrite(greenLedPin, HIGH);
      }
      break;
  }
}
