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

// Definición del tamaño del buffer circular. Esto se hace para evitar problemas de concurrencia y bloqueos entre las tareas definidas
#define BUFFER_SIZE 10
uint16_t buffer[BUFFER_SIZE];
int readIndex = 0; // Índice de lectura del buffer
int writeIndex = 0; // Índice de escritura del buffer

// Cliente WiFi
WiFiClient client;

// Mutex para proteger el acceso al buffer
SemaphoreHandle_t xMutex;

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
    if (xSemaphoreTake(xMutex, portMAX_DELAY)) { // Tomar el mutex (semáforo que protege el acceso al buffer)
      buffer[writeIndex] = localMedicion; // Escribir valor en el buffer
      writeIndex = (writeIndex + 1) % BUFFER_SIZE; // Actualizar índice de escritura
      xSemaphoreGive(xMutex); // Liberar el mutex
    }
    Serial.print("medicion : ");
    Serial.println(localMedicion);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Tarea para enviar datos al servidor
void send_data_task(void * param) {
  for (;;) {
    if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
      if (readIndex != writeIndex) { // Verificar si hay datos para enviar
        if (client.connect(server_ip, server_port)) { // Conectar al servidor
          client.print(buffer[readIndex]);
          readIndex = (readIndex + 1) % BUFFER_SIZE;
        } else {
          Serial.println("/ fallo envio de datos");
        }
      }
      xSemaphoreGive(xMutex);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Configuración inicial
void setup() {
  init();
  xMutex = xSemaphoreCreateMutex();
  xTaskCreatePinnedToCore( // tarea Lectura Analogica + PID -> Core 0
    analog_read_task,  
    "AnalogReadTask",
    10000,           
    NULL,           
    1,              
    NULL,            
    0);

  xTaskCreatePinnedToCore( // tarea Enviar datos UDP por WIFI -> Core 1
    send_data_task,   
    "SendDataTask", 
    10000,          
    NULL,            
    1,              
    NULL,           
    1);             
}

// Bucle principal vacío ya que las tareas se ejecutan en paralelo
void loop() {
  // El loop se deja vacío ya que las tareas se ejecutan en paralelo
}