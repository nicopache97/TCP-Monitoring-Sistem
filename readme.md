
# TCP-Monitoring-Ligth-Sistem-PID

## Introducción
Este proyecto consiste en un sistema de monitoreo y control de iluminación utilizando un ESP32S y un servidor TCP basado en Node.js. El sistema permite la gestión remota de la iluminación a través de una tira LED WS2812b y un sensor LDR.

## Componentes del Sistema

1. **ESP32S Program**:
   - Proyecto que se ejecuta en el ESP32S y utiliza dos núcleos del microcontrolador.
     - Core 0: Control de la iluminación a través de PID (Proportional Integral Derivative) utilizando una tira LED WS2812b y un sensor LDR conectado al ADC1.
     - Core 1: Maneja la conexión WiFi al servidor TCP estático.
   - Utiliza las bibliotecas `WiFi.h`, `PIDController.hpp`, y `Adafruit_NeoPixel.h` para controlar la tira LED.

2. **Server.js**:
   - Servidor TCP basado en Node.js que recibe datos de iluminación desde el ESP32S.
   - Permite al usuario ingresar nuevos niveles de iluminación a través de la consola.
   - Envía los nuevos niveles de iluminación al dispositivo ESP32S cuando estén disponibles.
   - Almacena el último nivel de iluminación ingresado en un buffer y lo envía al dispositivo cuando se conecte.

## Dependencias

1. **Dependencias del ESP32S**:
   - `WiFi.h`: Biblioteca para la comunicación WiFi.
   - `PIDController.hpp`: Biblioteca personalizada para el control PID.
   - `Adafruit_NeoPixel.h`: Biblioteca para controlar la tira LED.

2. **Dependencias del Servidor.js**:
   - Exige las siguientes dependencias de Node.js: `net`, `chalk`, `os`, y `readline`.
   - Opcional: `express` para crear servidores HTTP.

## Configuración

### Para el ESP32S **`ESP-program.ino`**
- Definir los pines correspondientes para el LED integrado, la lectura analógica y la tira de LEDs WS2812b.
- Configurar los parámetros del PID (Kp, Ki, Kd).
- Establecer las credenciales de red WiFi y la dirección IP y puerto del servidor TCP.

### Para el **`Servidor.js`**
- Definir el puerto en el que el servidor TCP escuchará las conexiones entrantes.
- Configurar el manejo de la consola (username/password o acceso público).

## Ejemplos de Proyectos

### 1. **`Example_Client.ino`**
   - Prueba la funcionalidad de comunicar procesos FreeRTOS, conexión WiFi y envío de datos al servidor TCP.

### 2. **`Example_Neopixel.ino`**
 - Prueba la configuración de la tira LED WS2812b, pines de salida y el correcto funcionamiento de la librería.
 - Este programa simple gestiona el manejo de la tira LED y sus niveles de brillo.

### 3. **`Example_PID.ino`**
   - Prueba la biblioteca suministrada por la catedra para el control PID y lectura del LDR y control de brillo.
   - Este programa se enfoca en el control PID de la iluminación, para mantener constante la lectura de luz.
___

### TAGS

#TCP-Monitoring-Sistem #ESP32S #Node.js #Arduino #WS2812b #PID_Control #TCP_Server

#WiFi.h #Adafruit_NeoPixel.h #PIDController.hpp #net

#Remote_Monitoring #Lighting_Control #Automated_Systems #IoT_Automation #Education_Project

#Project_Monitoring #Smart_Lighting_System #Wireless_Monitoring #Data_Transmission
#Sensor_Networks #Educational_IoT_Application

## Coneccion

### Server
La maquina server debe estar conectado en la misma red que la ESP32, esto se puede lograr conectando por cable Ethernet a un router con wifi, o conectando ambos dispositivos por wifi para que esten localmente conectados

### ESP32
Previamente se debe cargar el programa *`ESP-program.ino`* a la placa, luego debe conectar el sondor LDR, la tira LED WS2812b y finalmente la alimentacion, como se muestra en la siguiente imagen 
*si desea utilizar las configuraciones de depuracion, es recomendable conectar la ESP directamente a una computadora*

![Conexión ESP32S-Led-Ldr](/IMG/Esquema%20de%20Coneccion%20ESP32S-LED-LDR.jpg)