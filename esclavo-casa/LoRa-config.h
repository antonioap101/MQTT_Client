
#ifndef LORA_CONFIG_H
#define LORA_CONFIG_H

#include <LoRa.h>

#define ACK_MESSAGE     byte(0)   // 0b00 para ACK
#define NACK_MESSAGE    byte(1)   // 0b01 para NACK
#define CFG_MESSAGE     byte(2)   // 0b10 para CFG  
#define RND_MESSAGE     byte(3)   // 0b11 para OTHER



// =============================== VALORES FIJOS ===============================

// Received Signal Strength Indicator [0, -127] (dBm): Medida de la potencia que tiene la señal recibida. 
const int16_t MIN_RSSI = -127; // -127 dBm (señal muy débil)
const int16_t MAX_RSSI = 0;    // 0 dBm (señal fuerte)

// Signal-to-Noise Ratio [-148, 20] (dB) : Compara el nivel de la señal deseada con el nivel de ruido de fondo. Un SNR más alto indica una señal más clara.
const int16_t MIN_SNR = -148;  // -148 dB (SNR bajo)
const int16_t MAX_SNR = 20;    // 20 dB (SNR alto)

// Bandwitdth
const uint8_t MIN_BW_INDEX = 0;
const uint8_t MAX_BW_INDEX = 9;

// Spreading-Factor
const uint8_t MIN_SF = 6;
const uint8_t MAX_SF = 12;

// Coding-Rate
const uint8_t MIN_CR = 5;  
const uint8_t MAX_CR = 8;  

// Transmission-Power
const uint8_t MIN_TX_POWER = 2;  
const uint8_t MAX_TX_POWER = 20;  

// =============================== VARIABLES ===============================
// NOTA: Ajustar estas variables 
const uint8_t localAddress = 0x10;           // Dirección de este dispositivo
const uint8_t destination = 0x40;            // Dirección de destino, 0xFF es la dirección de broadcast

volatile bool txDoneFlag = true;             // Flag para indicar cuando ha finalizado una transmisión
volatile bool transmitting = false;          // Flag para indicar la transmisión de un mensaje de datos
volatile uint32_t lastReceptionTime = 0;     // Momento en que se recibio el último mensaje del esclavo

// Estructura para almacenar la configuración de la radio
typedef struct {
  uint8_t bandwidth_index;    // Ancho de banda [0,9]: Como norma general, multiplicarlo por dos supone reducir a la mitad el tiempo de transmisión.
                              // Reducir el ancho de banda permite reducir el impacto de interferencias y obtener distancias de enlace mayores.

  uint8_t spreadingFactor;    // Factor de dispersión [6,12]: Aumentar el Spf permite hacer que la señal aumente su inmunidad al ruido y puedan 
                              // lograrse distancias de enlace mayores. Sin embargo, el uso de factores de dispersión mayores incrementa de forma
                              // muy significativa el tiempo de transmisión de un dato, lo que puede provocar un aumento importante en el consumo
                              // de energía o exceder el tiempo actividad o duty-cycle permitido.
  
  uint8_t codingRate;         // Tasa de codificación [5, 8]:  Esta estrategia permite detectar y corregir, hasta cierto punto, los errores que 
                              // se produzcan en la recepción. Como contrapartida, aumentar la tasa de codificación incrementa el número de símbolos
                              // a transmitir y, por tanto, el tiempo de transmisión
  
  uint8_t txPower;            // Potencia de transmisión [2,20]: Potencia deseada expresada en dBm. . Para pruebas en interior es aconsejable poner 
                              // poner este valor al mínimo y evitar así saturar los receptores, ya que de lo contrario fallarán las transmisiones.
} LoRaConfig_t;


double bandwidth_kHz[10] = {7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, //  [7800.000000, 10400.000000, 15600.000000, 20800.000000, 31250.000000,
                           41.7E3, 62.5E3, 125E3, 250E3, 500E3 };   //   41700.000000, 62500.000000, 125000.000000, 250000.000000, 500000.000000]

LoRaConfig_t thisNodeConf   = {6, 10, 5, 2};
LoRaConfig_t remoteNodeConf = {6, 10, 5, 2};
LoRaConfig_t prevNodeConf   = {6, 10, 5, 2};

int remoteRSSI = 0;
float remoteSNR = 0;

// =============================== FUNCIONES ===============================
void initializeSerial();
void initializePMIC();
void initializeLoRa();
void setupLoRaCallbacks();
void startLoRaReception();
void applyLoRaConfiguration(const LoRaConfig_t config);
void configureLoRa();
void TxFinished();

void onReceive(int packetSize);
uint8_t readPacketHeader(int& recipient, uint8_t& sender, uint16_t& incomingMsgId);
uint8_t readMessage(uint8_t* buffer, size_t bufferSize);
bool validateMessageLength(uint8_t incomingLength, uint8_t receivedBytes);
bool isMessageForThisNode(int recipient);
void processReceivedMessage(uint8_t sender, int recipient, uint16_t incomingMsgId, uint8_t incomingLength, const uint8_t* buffer, uint8_t receivedBytes);
void updateRemoteNodeConfiguration(const uint8_t* buffer);
void printNodeConfig(String whichNode, const LoRaConfig_t& config);

#endif