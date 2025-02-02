#include <WiFi.h>

  // WiFi
char wifi_ssid[32] = "Lucky";   // wifi_ssid
char wifi_password[32] = "123412341234";    // wifi_password
  // Server
char server_ip[16] = "192.168.1.10";
int server_port = 12345;

#define PIN_LED 2
#define PIN_READ 0

//constructor
WiFiClient client;

void setup() {
  Serial.begin(115200); delay(10);
  Serial.println();
  Serial.println("Starting...");


  pinMode(PIN_LED, OUTPUT);

    // Conectar a WiFi
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println(".");
  }
  Serial.println("\n \t - Conectado a WiFi");

  for(int i=0;i<15;i++){ // señal visual de inicio completo
    digitalWrite(PIN_LED, HIGH);delay(35);
    digitalWrite(PIN_LED, LOW); delay(35);
  }

}

void loop() {
  uint16_t medicion = analogRead(PIN_READ); // lectura potenciometro

  Serial.print("medicion : ");// envia valores por puerto serie
  Serial.println(medicion);

  if (client.connect(server_ip, server_port)) { // envia valores a servidor
    client.println(medicion);
  }else{
    Serial.println("/ fallo envio de datos");
  }
  
  for(int i=0;i<15;i++){ // señal visual de inicio completo
  digitalWrite(PIN_LED, HIGH);delay(35);
  digitalWrite(PIN_LED, LOW); delay(35);
  }

  delay(1000);
}






