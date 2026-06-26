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
| Eric | 202 |
| Randy Xavier Lone Peñate | 202341660 |
| Angie  | 202 |
| Kristhoper Franklin Ucelo Pirir | 202502428 |

---

**Fecha:** Viernes 26 de Junio del 2026  

---

## Introducción

El presente proyecto consiste en el diseño e implementación de una maqueta funcional de una casa inteligente denominada *SmartHome GT*, la cual integra conceptos de lógica combinacional, sistemas embebidos, comunicación serial e inalámbrica, así como el uso de memoria EEPROM en Arduino.

El sistema permite el control de distintos ambientes de una vivienda (sala, comedor, cocina, baño y habitación), mediante el encendido y apagado de luces LED, el control de un ventilador simulado con motor DC y la apertura de una puerta mediante servomotor.

Además, el sistema incorpora comunicación Bluetooth mediante un módulo HC-06, permitiendo el control remoto desde un dispositivo móvil, y la carga de configuraciones mediante archivos con extensión `.org`, los cuales son interpretados y almacenados en memoria no volátil.

---

## Descripción del problema

En las viviendas tradicionales, el control de la iluminación, ventilación y acceso se realiza de forma manual, lo que limita la eficiencia, comodidad y automatización del hogar. Cada dispositivo debe ser operado de manera independiente, lo que genera mayor consumo de tiempo y una menor integración entre los sistemas eléctricos.

Además, no existe una forma centralizada de controlar los diferentes ambientes de una vivienda, ni de mantener configuraciones personalizadas que puedan conservarse después de un corte de energía. Esto hace que el usuario deba reconfigurar manualmente el sistema cada vez que se reinicia.

También se identifica la falta de sistemas accesibles que permitan el control remoto de los dispositivos del hogar, ya que en muchos casos se requiere interacción física directa con interruptores o controles individuales.

Por esta razón, se propone el desarrollo de una maqueta de casa inteligente basada en Arduino, que permita automatizar el control de luces por ambientes, la ventilación y la apertura de puertas, integrando comunicación inalámbrica mediante Bluetooth y almacenamiento de configuraciones en memoria EEPROM, con el objetivo de simular un sistema domótico funcional y eficiente.

---

## Objetivos

### General

Diseñar e implementar un sistema de casa inteligente funcional utilizando Arduino, integrando control de dispositivos, comunicación inalámbrica y almacenamiento en EEPROM.

### Específicos

- Implementar control de iluminación por ambientes mediante salidas digitales.  
- Integrar comunicación Bluetooth para control remoto.  
- Almacenar configuraciones en memoria EEPROM.   
- Controlar actuadores como servomotores.  
- Mostrar estados del sistema en pantalla LCD I2C.

---

## Lógica del sistema

El sistema *SmartHome GT* funciona a partir de una lógica centralizada en la cual el Arduino UNO actúa como el controlador principal de toda la maqueta. Este se encarga de recibir, interpretar y ejecutar las diferentes instrucciones que permiten el funcionamiento de la casa inteligente.

Las órdenes pueden enviarse desde una computadora mediante comunicación serial o de forma inalámbrica a través del módulo Bluetooth HC-06. A partir de estos comandos, el sistema puede activar distintos modos de operación, controlar los ambientes de la vivienda o ejecutar configuraciones personalizadas almacenadas en la memoria EEPROM.

Cuando el Arduino recibe una instrucción válida, procesa la información y realiza las acciones correspondientes, como encender o apagar las luces de cada ambiente, controlar el ventilador o mover el servomotor que representa la puerta. Al mismo tiempo, se actualiza la pantalla LCD para mostrar al usuario el estado actual del sistema.

El funcionamiento se organiza en distintos modos, los cuales determinan el comportamiento de todos los dispositivos al mismo tiempo. De esta forma, cada modo define una combinación específica de luces, ventilador y demás componentes, logrando una operación coordinada y automática de toda la maqueta.

---

## Diagrama esquemático del circuito (Proteus)

El diagrama esquemático del sistema *SmartHome GT* fue realizado en Proteus, representando la conexión de todos los componentes del proyecto con el Arduino UNO como unidad central de control.

El sistema integra los siguientes elementos principales:

- **Iluminación por ambientes:** cinco zonas (sala, comedor, cocina, baño y habitación) controladas mediante salidas digitales del Arduino.  
- **Módulo Bluetooth HC-06:** permite el control remoto del sistema desde un dispositivo móvil mediante comandos inalámbricos.  
- **Pantalla LCD I2C (16x2):** muestra el modo activo, el estado del ventilador y mensajes de error.  
- **Motor DC (ventilador):** simula la ventilación de la habitación y se activa según el modo seleccionado.  
- **Servomotor:** representa la puerta principal y permite su apertura y cierre.  
- **LEDs de estado:** indican el funcionamiento del sistema (activo, éxito o error).

<img width="1542" height="848" alt="image" src="https://github.com/user-attachments/assets/76411e05-7b45-4d80-b581-b3c13aaffdcd" />


---

## Funciones booleanas

El sistema *SmartHome GT* puede representarse mediante funciones booleanas, ya que cada salida del sistema solo puede tener dos estados posibles: encendido (1) o apagado (0). Esto permite simplificar el comportamiento de cada uno de los elementos del sistema dentro de la lógica digital.

Cada ambiente de la vivienda se define como una variable booleana independiente. De esta forma, la sala (S), comedor (C), cocina (K), baño (B), habitación (H) y el ventilador (V) pueden tomar valores de 0 o 1 dependiendo del modo seleccionado.

Los diferentes modos del sistema se representan como combinaciones de estas variables. Por ejemplo, en el modo “encender todo”, todas las variables toman valor de 1, mientras que en el modo “apagar todo”, todas las salidas se establecen en 0. En el modo “relajado” y “noche”, todas las luces y el ventilador se mantienen apagados.

En el modo “fiesta”, las salidas de iluminación cambian de forma secuencial, lo que puede representarse como una función de activación alternada entre los ambientes, mientras el ventilador permanece encendido. Los modos personalizados almacenados en la memoria EEPROM también corresponden a combinaciones específicas de estas variables.

En general, el sistema puede expresarse como un conjunto de funciones booleanas donde cada salida depende directamente del estado del modo activo, permitiendo una representación clara del comportamiento lógico del sistema domótico.

---

## Explicación del formato `.org`

- `conf_ini`: indica el inicio del archivo de configuración.
- `MODO_CUSTOM`: define el nombre del modo.
- `VENTILADOR`: indica el estado del motor (ON/OFF).
- `LED'S`: define el estado de cada ambiente de la casa.
- `conf:fin`: indica el final del archivo.

Este formato permite almacenar múltiples configuraciones en un solo archivo y facilita su interpretación dentro del sistema embebido.

---

## Tabla de direcciones de EEPROM (cómo se almacenan los modos)

El sistema SmartHome GT utiliza la memoria EEPROM del Arduino para almacenar de forma permanente la configuración de los diferentes modos de funcionamiento. Esto permite que la información no se pierda al apagar o reiniciar el sistema, manteniendo guardados los estados de los ambientes, el ventilador y la configuración general del sistema.

### Asignación de memoria EEPROM

| Dirección EEPROM | Datos almacenados |
|------------------|------------------|
| 0   | Estado global del sistema (último modo activo y configuración general) |
| 100 | Modo personalizado 1 (nombre, estados de luces y ventilador) |
| 120 | Modo personalizado 2 (nombre, estados de luces y ventilador) |

El uso de la memoria EEPROM permite que el sistema recupere automáticamente la última configuración utilizada, brindando mayor estabilidad y autonomía al sistema de casa inteligente.

---

## Configuración Bluetooth

El sistema *SmartHome GT* utiliza un módulo Bluetooth HC-06 para permitir la comunicación inalámbrica entre el usuario y la maqueta. Este módulo se conecta al Arduino UNO mediante comunicación serial, utilizando los pines de transmisión (TX) y recepción (RX).

La configuración del Bluetooth permite el envío de comandos desde un dispositivo móvil mediante una aplicación de terminal Bluetooth. Estos comandos son interpretados por el Arduino para ejecutar acciones específicas dentro del sistema, como el control de luces por ambiente, la activación de modos predefinidos o el manejo del ventilador y la puerta.

Entre los comandos principales que el sistema reconoce se encuentran: `modo_fiesta`, `modo_relajado`, `modo_noche`, `encender_todo`, `apagar_todo`, así como los modos personalizados `modo_custom_1` y `modo_custom_2` almacenados en la memoria EEPROM.

La comunicación es en tiempo real, por lo que cada comando recibido es procesado inmediatamente, actualizando el estado del sistema y mostrando la información correspondiente en la pantalla LCD. Además, el sistema envía una respuesta de confirmación al dispositivo móvil para indicar que la acción se ejecutó correctamente.

---

## Equipo y componentes utilizados

| Tipo | Elementos |
|------|----------|
| Componentes electrónicos | Arduino UNO, Módulo Bluetooth HC-06, Pantalla LCD 16x2 I2C, Servomotor, Motor DC, LEDs, Resistencias 220Ω, Transistor NPN (2N2222), Cables jumper, Protoboard |
| Materiales de maqueta | Cartón, silicón caliente, pinturas / acuarelas, cartulina, elementos decorativos |
| Herramientas de desarrollo | Arduino IDE, Proteus, editor de texto, App Bluetooth Terminal |

## Presupuesto 

| Material | Cantidad | Costo unitario (Q) | Subtotal (Q) |
|----------|----------|---------------------|--------------|
| Cartón para estructura de la maqueta | 1 | 28.00 | 28.00 |
| Acuarelas | 1 set | 25.00 | 25.00 |
| Cable para protoboard | 10 metros | 1.50 | 15.00 |
| LEDs | 8 | 2.00 | 16.00 |
| Resistencias (220Ω) | 10 | 0.50 | 5.00 |
| Barras de silicón | 5 | 2.00 | 10.00 |
| Módulo Bluetooth HC-06 | 1 | 85.00 | 85.00 |
| Otros materiales | — | — | 20.00 |

**Total general: Q224.00**

---

## Fotos
### Durante el desarrollo del proyecto

<img width="1285" height="auto" alt="image" src="https://github.com/user-attachments/assets/48be868e-1e94-42d8-9e61-c8524cec7389" />

<img width="2000" height="1600" alt="Beige   Grey Aesthetic Moodboard Photo Collage" src="https://github.com/user-attachments/assets/ac7c122c-87c0-47e9-9904-6a9bc8c12215" />

<img width="2000" height="auto" alt="Neutral Earthy Clean Grid Photography Photo Collage Facebook Cover" src="https://github.com/user-attachments/assets/f4113e15-d2b1-4761-a03c-68a56040a539" />

### Terminado

<img width="375" height="496" alt="image" src="https://github.com/user-attachments/assets/aae18481-6d69-4d32-8382-6984b60d9af8" />

[Ver video](./SmartHouse_Finalizado.mp4)
[Ver video](./SmartHouse_Funcionamiento.mp4)

---

## Conclusiones

El proyecto *SmartHome GT* permitió aplicar de forma práctica distintos conceptos vistos en el curso, como la lógica digital, la programación en Arduino y el uso de sistemas embebidos. Mediante la construcción de una maqueta funcional, se logró representar una vivienda inteligente capaz de controlar luces, ventilación y acceso de manera automatizada.

Asimismo, se destacó la utilidad de la memoria EEPROM para conservar la configuración del sistema, lo que garantiza que los modos de funcionamiento se mantengan guardados incluso después de un reinicio. De igual forma, el uso de comunicación Bluetooth facilitó el control remoto, demostrando la importancia de las tecnologías inalámbricas en sistemas de automatización modernos.

Por último, el desarrollo del proyecto contribuyó al fortalecimiento de habilidades técnicas como el diseño de circuitos, la programación estructurada y la integración de hardware con software. Esto permitió comprender mejor el funcionamiento de sistemas electrónicos reales, logrando una solución funcional, estable y adaptable a diferentes condiciones de uso.

---

## Anexos

### Explicación y cortometraje
https://youtu.be/or0pGBAJmtU?si=cD5MwrecTMHobJ4A 

### Soundtrack
https://www.youtube.com/watch?v=ZyhrYis509A&list=RDZyhrYis509A&start_radio=1

---



---
