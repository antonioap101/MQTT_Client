/* ---------------------------------------------------------------------
 *  Práctica 3: Ejercicio propuesto (ESCLAVO)
 *  Asignatura (GII-IoT)

 *  Autor: Antonio Aparicio González
 *  Autor: Daniel Díaz Martel
 *  Autor: Juan José Quesada Acosta
 *  Autor: Álvaro Lettieri Acosta
 *  
 *  Este ejemplo requiere de una versión modificada
 *  de la librería Arduino LoRa (descargable desde CV de la asignatura)
 *  
 *  También usa la librería Arduino_BQ24195 
 *  https://github.com/arduino-libraries/Arduino_BQ24195
 * ---------------------------------------------------------------------
 */

#include <SPI.h>             
#include <Arduino_PMIC.h>
#include "LoRa-config.h"
#include "message.h"
#include "PIR_sensor.h"


#define TX_LAPSE_MS          uint32_t(5000)
#define HOSTNAME             "#esclavo@ArduinoMKR1310: "
#define ADJUSTMENT_INTERVAL  uint32_t(10000)                     // Intervalo mínimo entre ajustes de la configuracion de LoRa (en milisegundos)
#define RESET_MULTIPLIER     uint8_t(3)                          // Multiplicador de tiempo para RESET_INTERVAL
#define SEND_CONFIG_MSG      true
#define SEND_RANDOM_MSG      false



void setup() { 
  initializeSerial();                            // Inicializa el serial  
  initializePMIC();                              // Inicializa el módulo de la batería PMIC 
  initializeLoRa();                              // Inicializa LoRa
  configureLoRa();                               // Configuración extra de LoRa
  setupLoRaCallbacks();                          // Configura las funciones de callback LoRa
  startLoRaReception();                          // Activa la recepción de mensajes de Lora
  setupPIRSensor();
  printNodeConfig("INITIAL THIS", thisNodeConf); // Muestra la configuración inicial del Nodo
  Serial.println();
}


void loop() {
  static uint32_t lastSendTime_ms = 0;                // Instante de tiempo en que se comenzó la transmisión previa a la actual
  static uint16_t msgCount = 0;                       // Conteo de mensajes enviados
  static uint32_t txInterval_ms = TX_LAPSE_MS;        // Intervalo de tiempo para enviar mensajes 
  static uint32_t tx_begin_ms = 0;                    // Instante de tiempo en que comienza la transmisión actual
  static uint32_t TxTime_ms;                          // Tiempo que tardó en completarse la transmisión actual (tiempo que se pretende minimizar)
  static uint32_t lastAdjustmentTime = 0;                 // Tiempo del último ajuste realizado
  uint32_t currentTime = millis();                    // Tiempo actual
  uint32_t RESET_INTERVAL = txInterval_ms * RESET_MULTIPLIER;    // Calcular RESET_INTERVAL en tiempo de ejecución

  if (shouldTransmit(currentTime, lastSendTime_ms, txInterval_ms)){
    Serial.print("Starting transmission with ");
    printNodeConfig("THIS", thisNodeConf); //printCurrentLoRaConfig();
    performTransmission(msgCount, tx_begin_ms, RND_MESSAGE);
  } 
  
  if (transmissionCompleted()) 
    TxTime_ms = processTransmissionCompletion(txInterval_ms, lastSendTime_ms, tx_begin_ms);

}


inline bool shouldTransmit(uint32_t currentTime, uint32_t lastSendTime, uint32_t txInterval) {
  return !transmitting && ((currentTime - lastSendTime) > txInterval);
}

inline bool transmissionCompleted() {
  return transmitting && txDoneFlag;
}


inline bool is_RSSI_InRange(int rssi) { // TODO: Cambiar RSSI a float
  return (rssi > MIN_RSSI) && (rssi < MAX_RSSI);
}

inline bool is_SNR_InRange(float snr) {
  return (snr > MIN_SNR) && (snr < MAX_SNR);
}

void performTransmission(uint16_t& msgCount, uint32_t& txBegin, byte messageType) {
    switch (messageType) {
          case ACK_MESSAGE: {// ACK        
            Message ACK(random(0, 20000), CREATE_ACK);
            Serial.println("This is master Node, should not send ACK messages!");
            break;
        } case NACK_MESSAGE: { // NACK
            Message NACK(random(0, 20000), CREATE_NACK);
            Serial.println("This is master Node, should not send NACK messages!");
            break;
        } case CFG_MESSAGE: { // CFG (Configuración)        
            Message configMessage(msgCount, remoteNodeConf);
            configMessage.print("[CFG] SENT");
            configMessage.send();
            break;
        } case RND_MESSAGE: { // OTHER (Mensaje aleatorio)
            int pirSensorValue = readPIRSensor();

            // Crear un mensaje personalizado con el valor del sensor PIR en el payload
            uint8_t customPayload[1] = {pirSensorValue};

            Message dataMessage(msgCount, customPayload, 1);
            dataMessage.print("[RND] SENT");
            dataMessage.send();
            
            //Serial.println("This is master Node, should not send RND messages!");
            break;
        } default:
            Serial.println("ERROR => Empty Message Queue.");
            return;
            break;
    }

  transmitting = true;
  txDoneFlag = false;
  txBegin = millis();               // Update the last send time to the current time
  
  Serial.print("Sending packet " + String(msgCount++) + ": ");
}

uint32_t processTransmissionCompletion(uint32_t& txInterval, uint32_t& lastSendTime, uint32_t& txBegin) {
  // Obtenemos el tiempo que tardó la transmisión y  ajustamos el intervalo si es necsario  
  uint32_t TxTime_ms = adjustTxIntervalIfNeeded(txInterval, lastSendTime, txBegin);

  // Ajustamos el flag de transmisión a false, ya que la transmisión ya ha finalizado
  transmitting = false;

  // Reactivamos la recepción de mensajes, que se desactiva mientras se transmite
  LoRa.receive();    

  return TxTime_ms; 
}

uint32_t adjustTxIntervalIfNeeded(uint32_t& txInterval, uint32_t& lastSendTime, uint32_t txBegin) {
  // Tiempo que tardó en completarse la transmisión actual
  uint32_t TxTime_ms = millis() - txBegin;    
  Serial.println(" ----> TX completed in " + String(TxTime_ms) + " ms");

  // Lapso de tiempo entre el comienzo de la transmisión actual  y el comienzo de la transmisión previa a la actual
  uint32_t lapse_ms = txBegin - lastSendTime; 

  // El instante de comienzo de la transmision actual, pasa a ser el previo a la actual.
  lastSendTime = txBegin;                     

  // Se calcula el duty cycle y se modifica txInterval en caso necesario para respetar el duty_cycle <= 1%
  float duty_cycle = (100.0f * TxTime_ms) / lapse_ms;
  Serial.println("Duty cycle: " + String(duty_cycle, 1) + "%");
  Serial.println("Last txInterval: " + String(txInterval) + " ms");
  Serial.println("New txInterval to: " + String(txInterval) + " ms\n");        
  
  return TxTime_ms;
}

// --------------------------------------------------------------------
// Receiving message function
// --------------------------------------------------------------------
void onReceive(int packetSize) {
    if (transmitting && !txDoneFlag) txDoneFlag = true;
    if (packetSize == 0) return;

    // Si se ha recibido un mensaje de confirmación se registra
    
    Message receivedMessage = Message();
    if (receivedMessage.read()){
      // En caso de recibir correctamente el mensaje, se imprime y se procesa
      lastReceptionTime = millis();
      processReceivedMessage(receivedMessage);      
    } else {
      Serial.println("Error while reading received message!");
    }
}


void processReceivedMessage(Message& message) {
    // Imprimimos el mensaje
    uint8_t messageType = message.getMessageType();

    switch (messageType) {
        case ACK_MESSAGE: // ACK
            message.print("[ACK] RECEIVED");
            prevNodeConf = thisNodeConf;                 // Guardamos una copia de la configuración anterior
            updateThisNodeConfiguration(remoteNodeConf); // Actualizamos la configuración a la nueva
            break;
        case NACK_MESSAGE: // NACK
            message.print("[NACK] RECEIVED");            
            break;
        case CFG_MESSAGE: // CFG (Configuración)
            message.print("[CFG] RECEIVED");
            Serial.println("This is master Node, should not send CFG messages to me!");
            break;
        case RND_MESSAGE: // OTHER (Mensaje aleatorio)
            message.print("[RND] RECEIVED");
            break;
        default:
            Serial.println("ERROR => UNKNOWN MSG TYPE RECEIVED.");
            message.print("[UNK] RECEIVED");
            break;
    }    
}

void extractRSSIandSNRfromPayLoad(const uint8_t* payload, int& remoteRSSI, float& remoteSNR) {
    // Extracción del RSSI
    remoteRSSI = -int(payload[1]) / 2.0f; // Dividir entre -2 para revertir la operación original empaquetado
    // Extracción del SNR
    remoteSNR = int(payload[2]) - 148; // Restar 148 para revertir la operación original de empaquetado
}

bool updateThisNodeConfiguration(LoRaConfig_t& config) {
  // Aplica la nueva configuración recibida por el maestro
  applyLoRaConfiguration(config);
  thisNodeConf = config;

  // Muestra la nueva configuración aplicada
  Serial.println("············ New LoRa configuration set ············");
  printNodeConfig("NEW", thisNodeConf);
  Serial.println("····················································");
  return true;
}








