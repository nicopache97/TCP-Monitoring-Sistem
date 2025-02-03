// Programa ejecucion multicore
// Core 0 : control PID iluminacion con tira led WS2812b y sensor LDR en ADC1
// Core 1 : comunicacion Wifi a servidor estatico TCP

#include <WiFi.h>
#include <PIDController.hpp>
#include <Adafruit_NeoPixel.h>

// Definiciones de pines
#define PIN_LED 2   // LED integrado
#define PIN_ADC 32  // Pin para lectura analógica GPIO32 usa el ADC1 que NO INTERFIERE CON EL WIFI
  // Parametros ws2812b
#define PIN_LED_WS 5   // LED WS2812b - GPIO5
#define LED_LENGTH  5 // tira led de 5 pixels
  // Parámetros del PID
#define DEFAULT_SET_POINT 1500   // valor objetivo de control

// Configuración de WiFi
char wifi_ssid[32] = "Lucky";   // SSID de la red WiFi
char wifi_password[32] = "123412341234";    // Contraseña de la red WiFi

// Configuración del servidor
char server_ip[16] = "192.168.1.65"; // IP del servidor
int server_port = 12345; // Puerto del servidor

// Configuración del controlador PID 
PID::PIDParameters<double> parameters(0.002, 0.08, 0.0011); // Kp, Ki, Kd
PID::PIDController<double> pidController(parameters);

// Objeto controlador Neopixel
Adafruit_NeoPixel strip(LED_LENGTH, PIN_LED_WS, NEO_GRB + NEO_KHZ800);

uint16_t value_ligth;
uint16_t value_output;

// Cliente WiFi
WiFiClient client;

#define time_update_PID  50 // tiempo de refresco PID [ms]
#define time_update_TCP  1000 // tiempo de refresco entre actualizacion al servidor TCP [ms]

// Funciones
void PID_task(void *);
void TCP_task(void *);

void init() {
  Serial.begin(115200); delay(10);
  Serial.println();
  Serial.println("Starting...");

  // Configuración de pines
  pinMode(PIN_LED_WS, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_ADC, INPUT);
  
  // Configuración inicial del PID
  pidController.Input = analogRead(PIN_ADC);
  pidController.Setpoint = DEFAULT_SET_POINT;
  pidController.TurnOn();

  // Conectar a WiFi
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println(".");
  }
  Serial.println("\n \t - Conectado a WiFi");
  
  WiFi.setAutoReconnect(true);

  // Señal visual de inicio completo
  for(int i=0;i<15;i++){
    digitalWrite(PIN_LED, HIGH); delay(35);
    digitalWrite(PIN_LED, LOW); delay(35);
  }
}

// prende tira led en blanco, con mayor brillo en el led central
void encenderBlanco(float brillo) {
  if(brillo>255) brillo=255;// trunca de 0 a 255
  if(brillo<0) brillo=0;

  int brillos[5];
  brillos[0] = brillos[4] = brillo;
  brillos[1] = brillos[3] = min(255, (int)(brillo * 3));
  brillos[2] = min(255, (int)(brillo * 10));

  for (int i = 0; i < LED_LENGTH; i++) {
    strip.setPixelColor(i, strip.Color(brillos[i], brillos[i], brillos[i]));
  }
  strip.show();
}


void PID_task(void * param) {
  for (;;) {
    uint16_t localMedicion = analogRead(PIN_ADC); // Leer valor analógico

    pidController.Input = localMedicion;    // Actualizar PID
    pidController.Update();

    encenderBlanco(pidController.Output); // ajusta con el error el brillo de los leds

    Serial.print("ligth value : "); // envia la medicion a consola
    Serial.print(localMedicion);
    Serial.print(", led value : ");
    Serial.println(pidController.Output);

    value_ligth=localMedicion;// actualiza variables globales
    value_output=pidController.Output;

    vTaskDelay(time_update_PID / portTICK_PERIOD_MS);
  }
}

void reconnectWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconectando a WiFi...");
    WiFi.disconnect();
    WiFi.begin(wifi_ssid, wifi_password);
    delay(5000);
  }
}

void reconnectTCP() {
  while (!client.connected()) {
    Serial.println("Reconectando al servidor TCP...");
    if (client.connect(server_ip, server_port)) {
      Serial.println("Reconectado al servidor TCP");
    } else {
      Serial.println("Fallo en la reconexión TCP. Reintentando en 5 segundos...");
      delay(5000);
    }
  }
}

void TCP_task(void * param) {
  for (;;) {
    if (WiFi.status() != WL_CONNECTED) {
      reconnectWiFi();
    }
    
    if (!client.connected()) {
      reconnectTCP();
    }
    
    if (client.connected()) {
      // Tu código existente para enviar datos
      client.print("light value : ");
      client.print(value_ligth);
      client.print(", led value : ");
      client.println(value_output);
      
      // Verificar respuesta del servidor
      if (client.available()) {
        String response = client.readStringUntil('\n');
        response.trim();
        int new_set_point = response.toInt();
        if (new_set_point > 0 && new_set_point <= 4095) {
          pidController.Setpoint = new_set_point;
          Serial.print("Nuevo set point recibido: ");
          Serial.println(new_set_point);
        }
      }
    }
    
    digitalWrite(PIN_LED, HIGH);
    delay(1);
    digitalWrite(PIN_LED, LOW);
    
    vTaskDelay(time_update_TCP / portTICK_PERIOD_MS);
  }
}

// Configuración inicial
void setup() {
  init();
    // tarea Lectura Analogica + PID + LED -> Core 0
  xTaskCreatePinnedToCore( PID_task,"", 10000,NULL,1,NULL,0);  
    // tarea Enviar datos TCP por WIFI -> Core 1
  xTaskCreatePinnedToCore( TCP_task,"", 10000,NULL,1,NULL,1);             
}

void loop() {
}