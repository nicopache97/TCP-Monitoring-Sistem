
#include <WiFi.h>

  // WiFi
char wifi_ssid[32] = "Lucky";   // wifi_ssid
char wifi_password[32] = "123412341234";    // wifi_password
  // Server
char server_ip[16] = "192.168.1.65";
int server_port = 12345;

#define PIN_LED 2
#define PIN_READ 32 // GPIO32 usa el ADC1 que NO INTERFIERE CON EL WIFI

int medicion;

//constructor
WiFiClient client;

void setup() {
  Serial.begin(115200); delay(10);
  Serial.println();
  Serial.println("Starting...");

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_READ, INPUT);

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
  medicion = analogRead(PIN_READ); // lectura potenciometro
  Serial.print("medicion : ");// envia valores por puerto serie
  Serial.print(medicion);
  Serial.println();

  if (client.connect(server_ip, server_port)) { // envia valores a servidor
    client.print(medicion);
  }else{
    Serial.println("/ fallo envio de datos");
  }


  for(int i=0;i<15;i++){ // señal visual de inicio completo
    digitalWrite(PIN_LED, HIGH); delay(35);
    digitalWrite(PIN_LED, LOW);  delay(35);
  }

  delay(1000); 
}