#include <WiFi.h>

// WiFi
char wifi_ssid[32]="Lucky";   // wifi_ssid
char wifi_password[32]"123412341234";    // wifi_password
// Server
char server_ip[16]="192.168.1.10";
int server_port=12345;

//constructor
WiFiClient client;

void setup() {
  Serial.begin(115200); delay(10);
  Serial.println();
  Serial.println("Starting...");


  pinMode(pin_led, OUTPUT);

    // Conectar a WiFi
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println(".");
  }
  Serial.println("\n \t - Conectado a WiFi");

  for(int i=0;i<15;i++){ // señal visual de inicio completo
  digitalWrite(pin_led, HIGH);delay(35);
  digitalWrite(pin_led, LOW); delay(35);
  }

}

void loop() {
    uint16_t medicion = analogRead(GPIO); // lectura potenciometro

    Serial.print("medicion : ");
    Serial.println(medicion);

    // Conectar al servidor
    if (client.connect(config.server_ip, config.server_port)) {
        // convierte la lectura a string 2 decimales y lo envia por TCP
        client.println(medicion);
    }
    delay(1000);

}






