// Controlador PID que lee un valor analógico de un LDR y ajusta el brillo de una tira de 5 LEDs WS2812B para mantener el brillo constante. 
// El sistema muestra datos en formato CSV a través del puerto serial y utiliza el LED integrado para indicar cada actualización del PID.
// @ ING Nicolas Pacheco 02/02/2025

#include <PIDController.hpp>
#include <Adafruit_NeoPixel.h>

// Definición de pines
#define PIN_ADC 32  // Pin para lectura analógica
#define PIN_LED 2   // LED integrado
#define LED_PIN 5   // LED WS2812b - GPIO5

#define LED_LENGTH  5 // tira led de 5 pixels

// Parámetros del PID
#define SET_POINT 2000   // Punto objetivo
#define SAMPLE_TIME 200 // Tiempo entre mediciones (ms)

// Configuración del controlador PID 
PID::PIDParameters<double> parameters(0.1, 0.009, 0.0007); // Kp, Ki, Kd
PID::PIDController<double> pidController(parameters);

// Objeto controlador Neopixel
Adafruit_NeoPixel strip(LED_LENGTH, LED_PIN, NEO_GRB + NEO_KHZ800);

// Variables para timing
unsigned long lastTime = 0;

void encenderBlanco(int brillo);

void setup() {
  // Configuración inicial
  delay(1000); // Tiempo para que el serial se estabilice
  Serial.begin(115200);
  strip.begin();

  
  // Configuración de pines
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_ADC, INPUT);
  
  // Configuración inicial del PID
  pidController.Input = analogRead(PIN_ADC);
  pidController.Setpoint = SET_POINT;
  pidController.TurnOn();
  
  Serial.println("Sistema iniciado");
  Serial.println("Tiempo,SetPoint,Entrada,Salida,Error");
}

void loop() {
  unsigned long currentTime = millis();
  
  // Actualizar PID cada SAMPLE_TIME milisegundos
  if (currentTime - lastTime >= SAMPLE_TIME) {
    // Leer valor del sensor
    uint16_t sensorValue = analogRead(PIN_ADC);
    
    // Actualizar PID
    pidController.Input = sensorValue;
    pidController.Update();
    
    // Calcular error
    double error = pidController.Setpoint - pidController.Input;
    
    // Mostrar datos en formato CSV
    Serial.print(currentTime);
    Serial.print(",");
    Serial.print(pidController.Setpoint);
    Serial.print(",");
    Serial.print(pidController.Input);
    Serial.print(",");
    Serial.print(pidController.Output);
    Serial.print(",");
    Serial.println(error);
    
    encenderBlanco(pidController.Output); // ajusta con el error el brillo de los leds

    digitalWrite(PIN_LED, HIGH);
    delay(50);
    digitalWrite(PIN_LED, LOW);
    
    lastTime = currentTime;
  }
}

void encenderBlanco(int brillo) {
  if(brillo>255) brillo=255;// trunca de 0 a 255
  if(brillo<0) brillo=0; 
  for(int i=0; i<LED_LENGTH; i++) {
      strip.setPixelColor(i, strip.Color(brillo, brillo, brillo));
  }
  strip.show();
}