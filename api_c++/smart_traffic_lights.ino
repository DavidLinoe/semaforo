#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

const char* ssid     = "A14D";
const char* password = "nthm1233";

const int greenLedPins[] = {13, 15, 5};    // Pins for the green LEDs
const int yellowLedPins[] = {12, 2, 18};   // Pins for the yellow LEDs
const int redLedPins[] = {14, 4, 19};     // Pins for the red LEDs

long redInterval = 8000;    // Red light duration (in milliseconds)
long yellowInterval = 2000; // Yellow light duration (in milliseconds)
long greenInterval = 8000;  // Green light duration (in milliseconds)

unsigned long previousMillis = 0; // Store the last time the LEDs were updated
int phase = 0; // Current phase of the traffic light


String currentLine = "";

WebServer server(80);

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

// teste de rota 
  if (server.hasArg("TIMER")) {
    if (server.arg("TIMER") == "2") {
      // server.sendHeader("Location", "/home1");
      server.send(200);
      Serial.println("Tempo Recebido Da Api");

      return;
    }
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

void handleNotFound() {

  page_not_found_html();

}


void loop(void) {


}

void Task1(void *pvParameters);
void Task2(void *pvParameters);

void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  for (int i = 0; i < 3; i++) {
    pinMode(redLedPins[i], OUTPUT);
    pinMode(yellowLedPins[i], OUTPUT);
    pinMode(greenLedPins[i], OUTPUT);
  }

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(4);

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






  // Cria a tarefa 1 no núcleo 0
  xTaskCreatePinnedToCore(
    Task1,           // Função da tarefa
    "Task1",         // Nome da tarefa
    10000,           // Tamanho da pilha
    NULL,            // Parâmetro para a tarefa
    1,               // Prioridade da tarefa
    NULL,            // Handle da tarefa
    0);              // Núcleo que vai rodar a tarefa

  // Cria a tarefa 2 no núcleo 1
  xTaskCreatePinnedToCore(
    Task2,           // Função da tarefa
    "Task2",         // Nome da tarefa
    10000,           // Tamanho da pilha
    NULL,            // Parâmetro para a tarefa
    1,               // Prioridade da tarefa
    NULL,            // Handle da tarefa
    1);              // Núcleo que vai rodar a tarefa
}


void Task1(void *pvParameters) {
  while (true) {
    server.handleClient();
    delay(2);
  }
}


void Task2(void *pvParameters) {
  while (true) {
 unsigned long currentMillis = millis();
  long interval = 0;

  // Determine the interval based on the current phase
  switch (phase) {
    case 0:
    case 3:
      interval = greenInterval;
      break;
    case 1:
    case 4:
      interval = yellowInterval;
      break;
    case 2:
    case 5:
      interval = redInterval;
      break;
  }

  if (currentMillis - previousMillis >= interval) {
    // Update the previousMillis to the current time
    previousMillis = currentMillis;

    // Update the traffic light phase
    phase = (phase + 1) % 6;

    // Update the LED states based on the new phase
    updateTrafficLights();
  }

  // Example of dynamically adjusting the intervals
  // You can replace this part with actual conditions or inputs
  redInterval = 6000; // Example adjustment
  yellowInterval = 3000; // Example adjustment
  greenInterval = 8000; // Example adjustment

    
  }

}
void updateTrafficLights() {
  switch (phase) {
    case 0:
      // Phase 1: Two directions green, one red
      for (int i = 0; i < 2; i++) {
        digitalWrite(redLedPins[i], LOW);
        digitalWrite(yellowLedPins[i], LOW);
        digitalWrite(greenLedPins[i], HIGH);
      }
      digitalWrite(redLedPins[2], HIGH);
      digitalWrite(yellowLedPins[2], LOW);
      digitalWrite(greenLedPins[2], LOW);
      break;
    case 1:
      // Phase 2: Transition to yellow for direction 1 and 2
      for (int i = 0; i < 2; i++) {
        digitalWrite(redLedPins[i], LOW);
        digitalWrite(yellowLedPins[i], HIGH);
        digitalWrite(greenLedPins[i], LOW);
      }
      break;
    case 2:
      // Phase 3: All red
      for (int i = 0; i < 3; i++) {
        digitalWrite(redLedPins[i], HIGH);
        digitalWrite(yellowLedPins[i], LOW);
        digitalWrite(greenLedPins[i], LOW);
      }
      break;
    case 3:
      // Phase 4: Direction 3 green, direction 1 and 2 red
      digitalWrite(redLedPins[0], HIGH);
      digitalWrite(yellowLedPins[0], LOW);
      digitalWrite(greenLedPins[0], LOW);

      digitalWrite(redLedPins[1], HIGH);
      digitalWrite(yellowLedPins[1], LOW);
      digitalWrite(greenLedPins[1], LOW);

      digitalWrite(redLedPins[2], LOW);
      digitalWrite(yellowLedPins[2], LOW);
      digitalWrite(greenLedPins[2], HIGH);
      break;
    case 4:
      // Phase 5: Transition to yellow for direction 3
      digitalWrite(redLedPins[2], LOW);
      digitalWrite(yellowLedPins[2], HIGH);
      digitalWrite(greenLedPins[2], LOW);
      break;
    case 5:
      // Phase 6: All red
      for (int i = 0; i < 3; i++) {
        digitalWrite(redLedPins[i], HIGH);
        digitalWrite(yellowLedPins[i], LOW);
        digitalWrite(greenLedPins[i], LOW);
      }
      break;
  }
}