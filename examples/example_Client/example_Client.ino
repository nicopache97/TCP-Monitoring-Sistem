#include <WiFi.h>

// Definiciones de pines
#define PIN_LED 2
#define PIN_READ 32 // GPIO32 usa el ADC1 que NO INTERFIERE CON EL WIFI

// Configuración de WiFi
char wifi_ssid[32] = "Lucky";   // SSID de la red WiFi
char wifi_password[32] = "123412341234";    // Contraseña de la red WiFi

// Configuración del servidor
char server_ip[16] = "192.168.1.65"; // IP del servidor
int server_port = 12345; // Puerto del servidor

uint16_t buffer;

// Cliente WiFi
WiFiClient client;

uint16_t time_update = 200; // tiempo de refresco general de tareas [ms]

// Funciones
void analog_read_task(void *);
void send_data_task(void *);

void init() {
  Serial.begin(115200); delay(10);
  Serial.println();
  Serial.println("Starting...");

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_LED, INPUT);

  // Conectar a WiFi
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println(".");
  }
  Serial.println("\n \t - Conectado a WiFi");

  // Señal visual de inicio completo
  for(int i=0;i<15;i++){
    digitalWrite(PIN_LED, HIGH); delay(35);
    digitalWrite(PIN_LED, LOW); delay(35);
  }
}

// Tarea para leer valores analógicos
void analog_read_task(void * param) {
  for (;;) {
    uint16_t localMedicion = analogRead(PIN_READ); // Leer valor analógico
    Serial.print("Medicion Local: ");
    Serial.print(localMedicion);
    buffer=localMedicion;
    vTaskDelay(time_update / portTICK_PERIOD_MS);
  }
}

// Tarea para enviar datos al servidor
void send_data_task(void * param) {
  for (;;) {
    if (client.connect(server_ip, server_port)) { // Conectar al servidor
      client.print(buffer);
    } else {
      Serial.println("/ fallo envio de datos");
    }
    vTaskDelay(time_update / portTICK_PERIOD_MS);
  }
}

// Configuración inicial
void setup() {
  init();

    // tarea Lectura Analogica + PID -> Core 0
  xTaskCreatePinnedToCore( analog_read_task,"", 10000,NULL,1,NULL,0);  

    // tarea Enviar datos UDP por WIFI -> Core 1
  xTaskCreatePinnedToCore( send_data_task,"", 10000,NULL,1,NULL,1);             
}

void loop() {
}