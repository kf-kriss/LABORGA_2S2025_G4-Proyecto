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

**Fecha:** Viernes 25 de Junio del 2026  

---

## Introducción

El presente proyecto consiste en el diseño e implementación de una maqueta funcional de una casa inteligente denominada *SmartHome GT*, la cual integra conceptos de lógica combinacional, sistemas embebidos, comunicación serial e inalámbrica, así como el uso de memoria EEPROM en Arduino.

El sistema permite el control de distintos ambientes de una vivienda (sala, comedor, cocina, baño y habitación), mediante el encendido y apagado de luces LED, el control de un ventilador simulado con motor DC y la apertura de una puerta mediante servomotor.

Además, el sistema incorpora comunicación Bluetooth mediante un módulo HC-06, permitiendo el control remoto desde un dispositivo móvil, y la carga de configuraciones mediante archivos con extensión `.org`, los cuales son interpretados y almacenados en memoria no volátil.

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



---

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
