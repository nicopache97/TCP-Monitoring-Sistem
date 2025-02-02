
#include <WiFi.h>


// Definición de la estructura de configuración
struct Config {
    // WiFi
    char wifi_ssid[32];
    char wifi_password[32];
    // Server
    char server_ip[16];
    int server_port;
};

// Valores por defecto de la configuración
const Config DEFAULT_CONFIG = {
    // WiFi
    "Lucky",           // wifi_ssid
    "123412341234",    // wifi_password
    // Server
    "192.168.1.10",    // server_ip
    8266,              // server_port

};



//constructor
WiFiClient client;

void setup() {
  Serial.begin(115200); delay(10);
  Serial.println();
  Serial.println("Starting...");


  pinMode(config.pin_led, OUTPUT);

    // Conectar a WiFi
  WiFi.begin(config.wifi_ssid, config.wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println(".");
  }
  Serial.println("\n \t - Conectado a WiFi");

  for(int i=0;i<15;i++){ // señal visual de inicio completo
  digitalWrite(config.pin_led, HIGH);delay(35);
  digitalWrite(config.pin_led, LOW); delay(35);
  }

}

void loop() {


      float medicion = LoadCell.getData(); // lectura del peso
      Serial.print("medicion : ");
      Serial.println(medicion);

      // Conectar al servidor
      if (client.connect(config.server_ip, config.server_port)) {


          // convierte la lectura a string 2 decimales y lo envia por TCP
          client.println(String(medicion,2));
          
      }
  }
}






