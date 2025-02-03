
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
> *si desea utilizar las configuraciones de depuracion, es recomendable conectar la ESP directamente a una computadora*

### Conecciones de los sensores
![Conexión ESP32S-Led-Ldr](/IMG/Esquema%20de%20Coneccion%20ESP32S-LED-LDR.jpg)

### Pines de la ESP32S utilizada
![pinout-ESP32S](/IMG/NodeMCU-ESP32S-pinout.jpg)
___

## Como instalar usar el programa
 
### 1. Server
   - copie la carpeta **`TCP_Server/`** en el directorio de su preferencia
   - instale node JS desde su [*pagina oficial*](https://nodejs.org/en/download)
   - ejecute el scrypt *`TCP_Server/run.cmd`*
   - debe abrirse una terminal con todos las IPv4 asociadas a su maquina, junto con el puerto de escucha por defecto `Port : 12345`

### 2. En la ESP32
   - copie la carpeta **`ESP-program/`** en el directorio de su preferencia
   - instale *Arduino IDE 2.3.4* desde su [*pagina oficial*](https://www.arduino.cc/en/software) segun su sistema operativo
   - instale el driver de comunicacion serie segun su modelo de placa: algunas disponen del controlador **`'CH340'`**  y otras **`'CP2102'`**
   -  desde ArduinoIDE abra las configuraciones, en la seccion de placas personalizadas agregue el siguiente link [*`https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`*](https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json)
   - desde Arduino IDE conectado a internet, abra el *`gestor de placas`*, e instale ***`esp32 v3.1.1`** de Espressif Systems*
   - con la ESP32 conectada, seleccione el puerto COM correspondiente, junto con la version de placa utilizada, nosotros utilizamos *ALKS ESP32*
   - cargue el programa *`ESP-program.ino`* a su ESP32, solo cuando haya cambiado las configuraciones de RED,y Servidor

### 3. Circuito Fisico y conecciones
   - debe tener conectado el servidor y la ESP32 en la misma red local. Esto puede lograrlo creando una zona wifi desde las configuraciones de la ESP, o dede un movile, o desde una computadora, o router. para que ambos dispositivos esten conectados de forma local. Para mejorar la latencia se recomienda no usar los wifi libres de Facultad. En nuestro caso, utilizamos un router conectdo por cable ethernet directo a la computadora, la esp32 se conecta al wifi del router como host.
   - ademas debe alimentar la ESP32, tanto desde el puerto USB o desde los pines RAW (con unas baterias). Se recomienda usar el puerto USB para conectarlo a la computadora de la misma manera que se programo, para ver los LOGS desde el monitor serie, pero no es indispensable
   - conecte el sensor y el actuador, en esta implementacion es un LDR con una resistencia de 1Kohm en serie, y la tira led ws2812b

### Demo short en [YouTube](https://youtu.be/_bBW_ashCTM?si=rxqPB7zVi3FnZUOu)

### Notas:

   > recuerde que la luz emitida por la tira led debe afectar **directamente al LDR**, para que el circuito de control quede realimentado, de no ser asi, el sistema es inestable y divergera

   > si conecta al reves el LDR el sistema puede quedar realimentado positivamente, se recomienda usar el LRD conectado entre los +5v y el pin de lectura analogico *GPIO32*

   > Se probaron diversas configuraciones de pines, el pin *`GPIO32`* no interfiere con las conecciones WIFI ya que usa el ADC1, por lo que es el mas apropiado para esta tarea, el pin *`GPIO5`* es el mas apropiado para salida digital, ya que no tiene pre-condiciones de estado que limitan el booteo de la placa. No todos los pines pueden ser configurados como Digitales, ni Analogicos 
