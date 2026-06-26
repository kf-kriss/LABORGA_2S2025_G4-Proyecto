#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <Servo.h>

// --- Objetos ---
LiquidCrystal_I2C lcd_1(0x27, 16, 2);
SoftwareSerial bluetooth(A0, A1); // RX, TX
Servo puertaServo;

// --- Pines ---
const int luzSala = 2;
const int luzComedor = 3;
const int luzCocina = 4;
const int luzBano = 5;
const int luzHabitacion = 6;
const int ventilador = 7;
const int L1 = 8;  // Azul - Sistema Activo
const int L2 = 9;  // Verde - Éxito Configuración
const int L3 = 10; // Rojo - Error
const int pinServo = 11;
const int pinBoton = 12;

//---Servo---
const int cerrado = 10;
const int abierto = 100;
int i=0;
bool puertaAbierta;
bool estadoAnteriorBoton = HIGH;
unsigned long ultimoCambio = 0;
const unsigned long tiempoRebote = 250;


// --- Constantes y Direcciones de Memoria EEPROM ---
const int DIR_ESTADO_GLOBAL = 0;   // Para guardar el último estado manual
const int DIR_CUSTOM_1 = 100;      // Espacio para Modo Custom 1 (archivo .org)
const int DIR_CUSTOM_2 = 120;      // Espacio para Modo Custom 2 (archivo .org)

// --- Banderas y Variables Globales ---
bool modo_fiesta = false;
bool modo_custom_1_activo = false;
bool modo_custom_2_activo = false;
bool configurando_org = false;

String comando = "";
String comandoBluetooth = "";

// --- Variables para millis() (Temporizadores No Bloqueantes) ---
unsigned long tiempoAnteriorFiesta = 0;
const long intervaloFiesta = 300;
int pasoFiesta = 0;

unsigned long tiempoAnteriorVerde = 0;
int parpadeosVerde = 0;
bool estadoLedVerde = false;
bool accionParpadeoVerde = false;
bool hubo_error = false;

// --- Variables de Control para el Parser Multilínea ---
String nombre_custom_actual = "";
bool temp_sala = false;
bool temp_comedor = false;
bool temp_cocina = false;
bool temp_bano = false;
bool temp_habitacion = false;
bool temp_ventilador = false;
int direccion_destino_actual = DIR_CUSTOM_1; // Controla dónde guardar (Custom 1 o 2)

// --- Bloque 2: Estructuras de Memoria y Persistencia ---

// Estructura para almacenar el estado físico de todos los actuadores principales
struct EstadoCasa {
  bool estSala;
  bool estComedor;
  bool estCocina;
  bool estBano;
  bool estHabitacion;
  bool estVentilador;
  bool estAlternado;
};

// Estructura para los modos personalizados leídos desde el archivo .org (Custom 1 y Custom 2)
struct ModoConfiguracion {
  char nombre[16]; // Almacena el nombre asignado al modo (ej. "CUSTOM_1")
  bool estSala;
  bool estComedor;
  bool estCocina;
  bool estBano;
  bool estHabitacion;
  bool estVentilador;
};

// --- Funciones de Persistencia para el Estado Global ---

// Guarda el estado actual de la casa en la EEPROM de forma empaquetada
void guardarEstadoGlobal(bool sala, bool comed, bool cocin, bool bano, bool hab, bool vent, bool alt) {
  EstadoCasa e;
  e.estSala = sala;
  e.estComedor = comed;
  e.estCocina = cocin;
  e.estBano = bano;
  e.estHabitacion = hab;
  e.estVentilador = vent;
  e.estAlternado = alt;
  
  EEPROM.put(DIR_ESTADO_GLOBAL, e);
  Serial.println(F("EEPROM: Estado global de la casa actualizado con exito."));
}

// Recupera el estado guardado en la EEPROM
EstadoCasa obtenerEstadoGlobal() {
  EstadoCasa e;
  EEPROM.get(DIR_ESTADO_GLOBAL, e);
  return e;
}

// --- Funciones de Persistencia para Modos Personalizados (.org) ---

// Guarda una configuración de modo personalizado en una dirección EEPROM específica
void guardarModoCustom(int direccion, String nombreModo, bool s, bool c, bool co, bool b, bool h, bool v) {
  ModoConfiguracion mc;
  
  // Limpiar y copiar el String al arreglo de chars de forma segura
  memset(mc.nombre, 0, sizeof(mc.nombre));
  nombreModo.toCharArray(mc.nombre, sizeof(mc.nombre) - 1);
  
  mc.estSala = s;
  mc.estComedor = c;
  mc.estCocina = co;
  mc.estBano = b;
  mc.estHabitacion = h;
  mc.estVentilador = v;
  
  EEPROM.put(direccion, mc);
  
  Serial.print(F("EEPROM: Configuración del modo '"));
  Serial.print(mc.nombre);
  Serial.println(F("' guardada correctamente."));
}

// Recupera un modo personalizado desde una dirección EEPROM específica
ModoConfiguracion obtenerModoCustom(int direccion) {
  ModoConfiguracion mc;
  EEPROM.get(direccion, mc);
  return mc;
}

// --- Bloque 3: Configuración Inicial (Setup) ---

void setup() {
  // Inicialización de comunicaciones
  Serial.begin(9600);
  bluetooth.begin(9600);
  Serial.setTimeout(500);
  bluetooth.setTimeout(500);

  // Inicialización de la pantalla LCD
  lcd_1.init();
  lcd_1.backlight();
  mostrarMensaje(F("Iniciando..."),F("SmartHome GT"));

  // Configuración de pines de salida
  pinMode(luzSala, OUTPUT);
  pinMode(luzComedor, OUTPUT);
  pinMode(luzCocina, OUTPUT);
  pinMode(luzBano, OUTPUT);
  pinMode(luzHabitacion, OUTPUT);
  pinMode(ventilador, OUTPUT);
  
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);

  // Inicialización del Servomotor (Puerta cerrada por defecto al iniciar)
  puertaServo.attach(pinServo);
  pinMode(pinBoton, INPUT_PULLUP);
  puertaServo.write(cerrado);
  puertaAbierta = false;

  // Estado inicial de los LEDs indicadores
  digitalWrite(L1, HIGH); // L1 Azul: Confirmación de sistema activo
  digitalWrite(L2, LOW);  // L2 Verde: Apagado
  digitalWrite(L3, LOW);  // L3 Rojo: Apagado

  // --- MAGIA DE PERSISTENCIA ---
  // Leemos la EEPROM para restaurar el último estado físico conocido
  EstadoCasa estadoGuardado = obtenerEstadoGlobal();
  
  if(estadoGuardado.estAlternado == true){
    modo_fiesta = true;
    digitalWrite(ventilador, estadoGuardado.estVentilador ? HIGH : LOW);
  } else {
    // Aplicamos el estado recuperado a los actuadores físicos
    digitalWrite(luzSala, estadoGuardado.estSala ? HIGH : LOW);
    digitalWrite(luzComedor, estadoGuardado.estComedor ? HIGH : LOW);
    digitalWrite(luzCocina, estadoGuardado.estCocina ? HIGH : LOW);
    digitalWrite(luzBano, estadoGuardado.estBano ? HIGH : LOW);
    digitalWrite(luzHabitacion, estadoGuardado.estHabitacion ? HIGH : LOW);
    digitalWrite(ventilador, estadoGuardado.estVentilador ? HIGH : LOW);
  }
  

  // Mensajes de confirmación finales
  delay(2000); // Breve pausa para visualizar el inicio en la LCD
  mostrarMensaje(F("Sistema listo"), F("Esperando cmd..."));
  responder("Sistema restaurado de EEPROM. Bluetooth HC-06 activo");
}

// --- Bloque 4: Ciclo Principal (Loop) ---

void loop() {
  bool estadoBoton = digitalRead(pinBoton);

  // Confirmación constante de sistema activo
  digitalWrite(L1, HIGH);

  // 1. Escuchar Puerto Serial (PC - Archivo .org o comandos manuales)
  if (Serial.available() > 0) {
    comando = Serial.readStringUntil('\n');
    comando.trim();

    if (comando.length() > 0) {

      if (hubo_error) {
        if (comando.indexOf("conf_ini") != -1) {
          hubo_error = false; // El usuario quiere volver a intentar, reactivamos
        } else {
          return; // Ignora todo y no dice "Comando no reconocido"
        }
      }
      // Usamos indexOf() != -1 para ignorar el BOM de Windows o basura invisible
      if (comando.indexOf("conf_ini") != -1) {
        configurando_org = true;
        mostrarMensaje(F("Configurando..."), F("Bloqueando BT"));
        digitalWrite(L3, LOW); // Apagamos LED Rojo de error por si estaba encendido
      } 
      else if (comando.indexOf("conf:fin") != -1) {
        configurando_org = false;
        direccion_destino_actual = DIR_CUSTOM_1;

        if (!hubo_error) {
          mostrarMensaje(F("Configuracion"), F("Guardada"));
          iniciarParpadeoVerde();
        } else {
          mostrarMensaje(F("Configuracion"), F("ERROR - GG"));
        }

        delay(2000);
        mostrarMensaje(F("Sistema listo"), F("Esperando cmd..."));
        comando = "";
        hubo_error = false;
      }
      else if (configurando_org) {
        parsearLineaOrg(comando); // Llama a la función del Bloque 5
      } 
      else if (!configurando_org && comando.indexOf("conf:") == -1 && comando.indexOf("MODO") == -1) {
        ejecutarComando(comando); // Llama a la función del Bloque 5
      }
    }
  }

  // 2. Escuchar Bluetooth (SOLO si no se está configurando por archivo)
  if (!configurando_org && bluetooth.available() > 0) {
    comandoBluetooth = bluetooth.readStringUntil('\n');
    comandoBluetooth.trim();

    if (comandoBluetooth.length() > 0) {
      ejecutarComando(comandoBluetooth); // Llama a la función del Bloque 5
    }
  }

  // 3. Ejecutar Lógica No Bloqueante (Temporizadores con millis)
  if (modo_fiesta) {
    ejecutarModoFiesta(); // Llama a la función del Bloque 6
  }
  
  if (accionParpadeoVerde) {
    actualizarParpadeoVerde(); // Llama a la función del Bloque 6
  }

  if (estadoAnteriorBoton == HIGH && estadoBoton == LOW) {
    if (millis() - ultimoCambio > tiempoRebote) {
      ultimoCambio = millis();

      if (puertaAbierta == false) {
        for(i = cerrado; i <= abierto; i++){
        puertaServo.write(i);
            delay(20);
     }
        puertaAbierta = true;
      } else {
            for(i = abierto; i >= cerrado; i--){
          puertaServo.write(i);
                delay(20);
      }
        puertaAbierta = false;
      }
    }
  }

  estadoAnteriorBoton = estadoBoton;
}

// --- Bloque 5: Procesador Central y Parser ---

void ejecutarComando(String cmd) {
  cmd.trim();
  
  // Reiniciamos banderas de modos al recibir un nuevo comando
  modo_fiesta = false;
  modo_custom_1_activo = false;
  modo_custom_2_activo = false;

  if (cmd == "LUZ_SALA_ON")
  {
    digitalWrite(luzSala, HIGH);
    responder(F("Luz de sala encendida"));
    mostrarMensaje(F("Luz de sala"), F("Encendida"));
  }
  else if (cmd == "LUZ_SALA_OFF")
  {
    digitalWrite(luzSala, LOW);
    responder(F("Luz de sala apagada"));
    mostrarMensaje(F("Luz de sala"), F("Apagada"));
  }
  else if (cmd == "LUZ_COMEDOR_ON")
  {
    digitalWrite(luzComedor, HIGH);
    responder(F("Luz de comedor encendida"));
    mostrarMensaje(F("Luz comedor"), F("Encendida"));
  }
  else if (cmd == "LUZ_COMEDOR_OFF")
  {
    digitalWrite(luzComedor, LOW);
    responder(F("Luz de comedor apagada"));
    mostrarMensaje(F("Luz comedor"), F("Apagada"));
  }
  else if (cmd == "LUZ_COCINA_ON")
  {
    digitalWrite(luzCocina, HIGH);
    responder(F("Luz de cocina encendida"));
    mostrarMensaje(F("Luz cocina"), F("Encendida"));
  }
  else if (cmd == "LUZ_COCINA_OFF")
  {
    digitalWrite(luzCocina, LOW);
    responder(F("Luz de cocina apagada"));
    mostrarMensaje(F("Luz cocina"), F("Apagada"));
  }
  else if (cmd == "LUZ_BANO_ON")
  {
    digitalWrite(luzBano, HIGH);
    responder(F("Luz de bano encendida"));
    mostrarMensaje(F("Luz del bano"), F("Encendida"));
  }
  else if (cmd == "LUZ_BANO_OFF")
  {
    digitalWrite(luzBano, LOW);
    responder(F("Luz de bano apagada"));
    mostrarMensaje(F("Luz del bano"), F("Apagada"));
  }
  else if (cmd == "LUZ_HABITACION_ON")
  {
    digitalWrite(luzHabitacion, HIGH);
    responder(F("Luz de habitacion encendida"));
    mostrarMensaje(F("Luz habitacion"), F("Encendida"));
  }
  else if (cmd == "LUZ_HABITACION_OFF")
  {
    digitalWrite(luzHabitacion, LOW);
    responder(F("Luz de habitacion apagada"));
    mostrarMensaje(F("Luz habitacion"), F("Apagada"));
  }
  else if (cmd == "VENTILADOR_ON")
  {
    digitalWrite(ventilador, HIGH);
    responder(F("Ventilador encendido"));
    mostrarMensaje(F("Ventilador"), F("Encendido"));
  }
  else if (cmd == "VENTILADOR_OFF")
  {
    digitalWrite(ventilador, LOW);
    responder(F("Ventilador apagado"));
    mostrarMensaje(F("Ventilador"), F("Apagado"));
  }
  else if (cmd == "modo_fiesta") {
    modo_fiesta = true;
    digitalWrite(ventilador, HIGH);
    pasoFiesta = 0;
    tiempoAnteriorFiesta = millis();
    responder(F("Modo Fiesta Activado"));
    mostrarMensaje(F("Modo: FIESTA"), F("V:ON, LEDS:ALT"));
    guardarEstadoGlobal(LOW, LOW, LOW, LOW, LOW, LOW, true);
  } 
  else if (cmd == "modo_relajado") {
    digitalWrite(ventilador, LOW);
    apagarLuces();
    responder(F("Modo Relajado Activado"));
    mostrarMensaje(F("Modo: RELAJADO"), F("V:OFF, LEDS:OFF"));
    guardarEstadoGlobal(LOW, LOW, LOW, LOW, LOW, LOW, false);
  } 
  else if (cmd == "modo_noche") {
    digitalWrite(ventilador, LOW);
    apagarLuces();
    responder(F("Modo Noche Activado"));
    mostrarMensaje(F("Modo: NOCHE"), F("V:OFF, LEDS:OFF"));
    guardarEstadoGlobal(LOW, LOW, LOW, LOW, LOW, LOW, false);
  } 
  else if (cmd == "apagar_todo") {
    digitalWrite(ventilador, LOW);
    apagarLuces();
    responder(F("Apagar todo"));
    mostrarMensaje(F("LEDS: OFF"), F("Ventilador: OFF"));
    guardarEstadoGlobal(LOW, LOW, LOW, LOW, LOW, LOW, false);
  } 
  else if (cmd == "encender_todo") {
    digitalWrite(ventilador, HIGH);
    encenderLuces();  
    responder(F("Encender todo"));
    mostrarMensaje(F("LEDS: ON"), F("Ventilador: ON"));
    guardarEstadoGlobal(HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, false);
  } 
  else if (cmd == "modo_custom_1") {
    activarModoCustom(DIR_CUSTOM_1);
  }
  else if (cmd == "modo_custom_2") {
    activarModoCustom(DIR_CUSTOM_2);
  }
  else if (cmd == "abrir_puerta") {
    puertaServo.write(abierto);
    puertaAbierta = true;
    responder(F("Puerta Abierta"));
  }
  else if (cmd == "cerrar_puerta") {
    puertaServo.write(cerrado);
    puertaAbierta = false;
    responder(F("Puerta Cerrada"));
  }
  else if (cmd == "estado") {
    responder(F("Estado: sistema activo"));
    mostrarMensaje(F("Sistema activo"), F("Esperando cmd"));
  } 
  else {
    responder(F("Comando no reconocido"));
    mostrarMensaje(F("Comando no"), F("reconocido"));
  }
}

void parsearLineaOrg(String linea) {
  if (!configurando_org) return;

  linea.trim();

  // 1. Descartar líneas vacías o comentarios puros
  if (linea.length() == 0 || linea.startsWith("//")) {
    return;
  }

  // 2. Limpiar comentarios inline (ej: "conf_ini // validar que venga")
  int indiceComentario = linea.indexOf("//");
  if (indiceComentario != -1) {
    linea = linea.substring(0, indiceComentario);
    linea.trim();
  }

  // Crear una copia en mayúsculas para facilitar las comparaciones de comandos
  String lineaUpper = linea;
  lineaUpper.toUpperCase();

  // 3. Validar filtros de errores sintácticos preexistentes
  if (lineaUpper.indexOf("SALON:") != -1 || lineaUpper.indexOf("HABITACION:ABC") != -1) {
    abortarConfiguracion("Sintaxis Invalida");
    return;
  }

  // 4. Procesar directivas de Modos Fijos (Fiesta, etc.) si es necesario registrar algo
  if (lineaUpper.startsWith("MODO_FIESTA")) {
    // Si tu archivo solo avisa que viene el modo fiesta, puedes elegir omitir 
    // o preconfigurar estados globales aquí.
    return;
  }

  // 5. DETECTAR NUEVO BLOQUE PERSONALIZADO: modo_custom: "Cena"
  if (lineaUpper.startsWith("MODO_CUSTOM:")) {
    // Extraer el nombre que se encuentra entre comillas
    int primeraComilla = linea.indexOf('"');
    int ultimaComilla = linea.lastIndexOf('"');
    
    if (primeraComilla != -1 && ultimaComilla != -1 && ultimaComilla > primeraComilla) {
      nombre_custom_actual = linea.substring(primeraComilla + 1, ultimaComilla);
    } else {
      nombre_custom_actual = "Custom_Modo";
    }

    // Resetear las variables de acumulación para este nuevo bloque
    temp_sala = temp_comedor = temp_cocina = temp_bano = temp_habitacion = temp_ventilador = false;
    return;
  }

  // 6. ACUMULAR ESTADO DEL VENTILADOR: Ventilador: OFF
  if (lineaUpper.startsWith("VENTILADOR:")) {
    if (lineaUpper.indexOf("ON") != -1) {
      temp_ventilador = true;
    } else {
      temp_ventilador = false;
    }
    return;
  }

  // 7. ACUMULAR LEDS Y DISPARAR GUARDADO: LED'S: sala:ON, comedor:ON...
  if (lineaUpper.startsWith("LED'S:") || lineaUpper.startsWith("LEDS:")) {
    temp_sala       = (lineaUpper.indexOf("SALA:ON") != -1);
    temp_comedor   = (lineaUpper.indexOf("COMEDOR:ON") != -1);
    temp_cocina    = (lineaUpper.indexOf("COCINA:ON") != -1);
    temp_bano      = (lineaUpper.indexOf("BANO:ON") != -1);
    temp_habitacion = (lineaUpper.indexOf("HABITACION:ON") != -1);

    // Como la línea de LED'S cierra la declaración del bloque según la estructura,
    // procedemos a realizar la persistencia física en la EEPROM en este instante.
    guardarModoCustom(direccion_destino_actual, nombre_custom_actual, 
                      temp_sala, temp_comedor, temp_cocina, temp_bano, temp_habitacion, temp_ventilador);
    
    // Alternar la dirección de memoria para el siguiente modo personalizado (máximo 2)
    if (direccion_destino_actual == DIR_CUSTOM_1) {
      direccion_destino_actual = DIR_CUSTOM_2;
    } else {
      direccion_destino_actual = DIR_CUSTOM_1;
    }
    return;
  }
}

// --- Bloque 6: Funciones Auxiliares, Modos y Controladores ---

// --- Funciones de Comunicación y UI ---

void mostrarMensaje(const __FlashStringHelper* linea1, const __FlashStringHelper* linea2) {
  lcd_1.clear();
  lcd_1.setCursor(0, 0);
  lcd_1.print(linea1);
  lcd_1.setCursor(0, 1);
  lcd_1.print(linea2);
}

void mostrarMensaje(const String& linea1, const String& linea2) {
  lcd_1.clear();
  lcd_1.setCursor(0, 0);
  lcd_1.print(linea1);
  lcd_1.setCursor(0, 1);
  lcd_1.print(linea2);
}

void responder(const __FlashStringHelper* mensaje) {
  Serial.println(mensaje);
  bluetooth.println(mensaje);
}

void responder(const String& mensaje) {
  Serial.println(mensaje);
  bluetooth.println(mensaje);
}

void abortarConfiguracion(const String& mensajeError) {
  // 1. Resetear indicadores físicos
  digitalWrite(L3, HIGH); 
  delay(1000);            
  digitalWrite(L3, LOW);  
  
  // 2. Resetear banderas lógicas
  configurando_org = false;
  hubo_error = true;
  direccion_destino_actual = DIR_CUSTOM_1;
  
  // --- LIMPIEZA CRÍTICA ---
  comando = ""; // Limpiamos el String global
  while (Serial.available() > 0) { Serial.read(); } // Vaciamos cualquier basura restante en el buffer serie
  // ------------------------
  
  // 3. Avisar al usuario
  mostrarMensaje(F("Error: Abortado"), mensajeError);
  delay(2000);
  mostrarMensaje(F("Sistema listo"), F("Esperando cmd..."));
}

// --- Controladores de Hardware Básicos ---

void apagarLuces() {
  digitalWrite(luzSala, LOW);
  digitalWrite(luzComedor, LOW);
  digitalWrite(luzCocina, LOW);
  digitalWrite(luzBano, LOW);
  digitalWrite(luzHabitacion, LOW);
}

void encenderLuces() {
  digitalWrite(luzSala, HIGH);
  digitalWrite(luzComedor, HIGH);
  digitalWrite(luzCocina, HIGH);
  digitalWrite(luzBano, HIGH);
  digitalWrite(luzHabitacion, HIGH);
}

// Activa un modo personalizado leyendo directamente desde la EEPROM
void activarModoCustom(int direccion) {
  ModoConfiguracion conf = obtenerModoCustom(direccion);

  digitalWrite(luzSala, conf.estSala ? HIGH : LOW);
  digitalWrite(luzComedor, conf.estComedor ? HIGH : LOW);
  digitalWrite(luzCocina, conf.estCocina ? HIGH : LOW);
  digitalWrite(luzBano, conf.estBano ? HIGH : LOW);
  digitalWrite(luzHabitacion, conf.estHabitacion ? HIGH : LOW);
  digitalWrite(ventilador, conf.estVentilador ? HIGH : LOW);

  String estadoVent = conf.estVentilador ? "V: ON" : "V: OFF";
  mostrarMensaje("Modo: " + String(conf.nombre), estadoVent);
  responder("Activado " + String(conf.nombre));
  
  // Guardamos este nuevo estado físico como el estado global actual para la persistencia
  guardarEstadoGlobal(conf.estSala, conf.estComedor, conf.estCocina, conf.estBano, conf.estHabitacion, conf.estVentilador, false);
}

// --- Lógica de Control No Bloqueante (Uso de millis) ---

// Secuencia de luces para el Modo Fiesta sin usar delay()
void ejecutarModoFiesta() {
  unsigned long tiempoActual = millis();
  
  if (tiempoActual - tiempoAnteriorFiesta >= intervaloFiesta) {
    tiempoAnteriorFiesta = tiempoActual;
    apagarLuces();
    
    switch (pasoFiesta) {
      case 0: digitalWrite(luzSala, HIGH); pasoFiesta = 1; break;
      case 1: digitalWrite(luzComedor, HIGH); pasoFiesta = 2; break;
      case 2: digitalWrite(luzCocina, HIGH); pasoFiesta = 3; break;
      case 3: digitalWrite(luzBano, HIGH); pasoFiesta = 4; break;
      case 4: digitalWrite(luzHabitacion, HIGH); pasoFiesta = 0; break;
    }
  }
}

// Prepara las variables para iniciar el parpadeo del LED Verde
void iniciarParpadeoVerde() {
  accionParpadeoVerde = true;
  parpadeosVerde = 0;
  estadoLedVerde = false;
  tiempoAnteriorVerde = millis();
}

// Ejecuta el parpadeo del LED Verde sin usar delay()
void actualizarParpadeoVerde() {
  unsigned long tiempoActual = millis();
  
  if (tiempoActual - tiempoAnteriorVerde >= 250) {
    tiempoAnteriorVerde = tiempoActual;
    estadoLedVerde = !estadoLedVerde; // Cambia de apagado a encendido o viceversa
    digitalWrite(L2, estadoLedVerde ? HIGH : LOW);
    
    if (!estadoLedVerde) {
      parpadeosVerde++; // Contamos un parpadeo completo cada vez que se apaga
    }
    
    if (parpadeosVerde >= 3) {
      accionParpadeoVerde = false; // Detenemos la rutina
      digitalWrite(L2, LOW);       // Aseguramos que el LED quede apagado
    }
  }
}
