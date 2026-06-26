# Universidad de San Carlos de Guatemala  
## Facultad de Ingeniería  

**Primer Semestre Vacaciones 2026**  
**Laboratorio Organización Computacional**  
**Sección A**  

**Ing. Carlos Lozano**  
**Aux. Carlos Blanco**  

---

## Proyecto 1
### SmartHome GT: Sistema de Control Inteligente


---

| Estudiantes | Carnet |
|------------|--------|
| José Daniel Salguero Saldivar | 202444320 |
| Adrián Obed Coronado Bueso | 202444480 |
| Eric Esteban Ramos Sagastume | 202445229 |
| Randy Xavier Lone Peñate | 202341660 |
| Angie Renata Godoy Portillo  | 202441178 |
| Kristhoper Franklin Ucelo Pirir | 202502428 |

---

**Fecha:** Viernes 26 de Junio del 2026  

---
---
---

# Manual de usuario

El sistema *SmartHome GT* es una maqueta de casa inteligente que permite el control de diferentes dispositivos electrónicos mediante comunicación Bluetooth o conexión USB con una computadora. Su objetivo es simular un entorno domótico donde el usuario puede gestionar luces, ventilación y acceso de forma sencilla e intuitiva.

---

## Encendido del sistema

Para iniciar el funcionamiento del sistema, se debe conectar el Arduino UNO a una fuente de alimentación o a una computadora mediante cable USB. Una vez encendido, el sistema realiza una inicialización automática de todos sus componentes, activando los indicadores de estado y mostrando en la pantalla LCD el mensaje de inicio del sistema.

---

## Conexión Bluetooth

El módulo Bluetooth HC-06 permite la comunicación inalámbrica con dispositivos móviles. Para utilizarlo, el usuario debe vincular su teléfono con el módulo desde la configuración de Bluetooth. Luego, se utiliza una aplicación de terminal Bluetooth para enviar comandos directamente al sistema.

Una vez establecida la conexión, el sistema queda listo para recibir instrucciones en tiempo real, sin necesidad de conexión física con el Arduino.

---

## Comandos de control

El sistema interpreta una serie de comandos que permiten controlar todas las funciones de la maqueta:

- `modo_fiesta` → Activa iluminación alternada en los ambientes y enciende el ventilador.  
- `modo_relajado` → Apaga todas las luces y desactiva el ventilador.  
- `modo_noche` → Mantiene el sistema en estado de iluminación mínima o apagado.  
- `encender_todo` → Activa todas las luces de la casa y el ventilador.  
- `apagar_todo` → Desactiva completamente luces y ventilador.  
- `modo_custom_1` → Ejecuta la primera configuración personalizada almacenada en EEPROM.  
- `modo_custom_2` → Ejecuta la segunda configuración personalizada almacenada en EEPROM.  
- `abrir_puerta` → Activa el servomotor para abrir la puerta principal.  
- `cerrar_puerta` → Regresa el servomotor a la posición de cierre.

---

## Indicadores de estado

El sistema cuenta con tres LEDs que informan el estado de operación:

- LED azul → Sistema encendido y listo para recibir comandos.  
- LED verde → Ejecución correcta de un comando o carga exitosa de configuración.  
- LED rojo → Error en comando o en la lectura de configuración.

---

## Pantalla LCD

La pantalla LCD 16x2 muestra información en tiempo real sobre el estado del sistema, incluyendo el modo activo, el estado del ventilador y mensajes de confirmación o error, lo que permite al usuario tener control visual del funcionamiento de la maqueta.

---
---
---

# Manual técnico

El sistema *SmartHome GT* está desarrollado sobre la plataforma Arduino UNO, el cual actúa como unidad central de control encargada de gestionar la lógica del sistema, la comunicación con dispositivos externos y el control de los actuadores electrónicos.

---

## Arquitectura del sistema

El sistema está compuesto por tres niveles principales:

- **Nivel de entrada:** conformado por el módulo Bluetooth HC-06 y la comunicación serial USB, a través de los cuales se reciben los comandos del usuario.  
- **Nivel de procesamiento:** representado por el Arduino UNO, encargado de interpretar los comandos, ejecutar la lógica del sistema y gestionar la memoria EEPROM.  
- **Nivel de salida:** compuesto por los LEDs de iluminación, el motor DC (ventilador), el servomotor (puerta) y la pantalla LCD.

---

## Componentes y conexión

El Arduino UNO controla todos los dispositivos mediante pines digitales y comunicación I2C para la pantalla LCD. El módulo Bluetooth se conecta mediante pines de comunicación serial (TX y RX), mientras que los actuadores como LEDs, motor DC y servomotor se conectan a salidas digitales configuradas según su función.

El motor DC puede requerir un transistor NPN como interruptor electrónico para proteger el microcontrolador, mientras que el servomotor es controlado mediante señal PWM.

---

## Gestión de memoria EEPROM

El sistema utiliza la memoria EEPROM interna del Arduino para almacenar configuraciones de los modos de funcionamiento. Esto permite conservar los datos incluso después de un reinicio.

Cada modo se guarda en una dirección específica de memoria, permitiendo recuperar configuraciones como el estado de luces, ventilador y modos personalizados.

---

## Lógica de funcionamiento

El programa se basa en la recepción de comandos que son interpretados mediante estructuras condicionales. Dependiendo del comando recibido, el sistema activa o desactiva los diferentes componentes, actualiza la pantalla LCD y envía una respuesta de confirmación por Bluetooth.

Además, el sistema incluye manejo de errores, validación de comandos y control de estados para asegurar un funcionamiento estable.

---

## Comunicación

La comunicación con el sistema se realiza mediante dos métodos:

- **USB Serial:** utilizado para carga de configuraciones y depuración.  
- **Bluetooth HC-06:** utilizado para control remoto desde dispositivos móviles en tiempo real.

---

## Consideraciones de funcionamiento

El sistema está diseñado para evitar bloqueos utilizando retardos controlados y lógica secuencial. Se recomienda una alimentación estable de 5V para el correcto funcionamiento de todos los módulos.

## Recomendaciones de uso

Se recomienda enviar los comandos de forma correcta para evitar errores en la ejecución del sistema. También es importante no desconectar el sistema mientras se está cargando una configuración desde el archivo `.org`, ya que esto podría generar fallos en la memoria EEPROM. Además, se debe asegurar que la conexión Bluetooth esté correctamente establecida antes de iniciar el control remoto.
