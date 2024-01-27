#include "LoRa-config.h"

void initializeSerial() {
  Serial.begin(115200);  
  while (!Serial); 
  Serial.println("Serial initialization succeded!");
}


void initializePMIC() {
  if (!init_PMIC()) {
    Serial.println("Initialization of BQ24195L failed!");
    while(true);
  }  
  Serial.println("Initialization of BQ24195L succeeded!");
}

void initializeLoRa() {
  if (!LoRa.begin(868E6)) {
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                
  }
  Serial.println("Initialization of LoRa succeeded!");
}

void setupLoRaCallbacks() {
  LoRa.onReceive(onReceive);
  LoRa.onTxDone(TxFinished);
}

void startLoRaReception() {
  LoRa.receive(); // La recepción está activada a partir de este punto
  Serial.println("LoRa Duplex with TxDone and Receive callbacks");
  Serial.println("Using binary packets");
  Serial.print(HOSTNAME);
}

void applyLoRaConfiguration(const LoRaConfig_t config){
  LoRa.setSignalBandwidth(long(bandwidth_kHz[config.bandwidth_index])); 
                                  // 7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3
                                  // 41.7E3, 62.5E3, 125E3, 250E3, 500E3 
                                  // Multiplicar por dos el ancho de banda
                                  // supone dividir a la mitad el tiempo de Tx
                                  
  LoRa.setSpreadingFactor(config.spreadingFactor);     
                                  // [6, 12] Aumentar el spreading factor incrementa 
                                  // de forma significativa el tiempo de Tx
                                  // SPF = 6 es un valor especial
                                  // Ver tabla 12 del manual del SEMTECH SX1276
  
  LoRa.setCodingRate4(config.codingRate);         
                                  // [5, 8] 5 da un tiempo de Tx menor
                                  
  LoRa.setTxPower(config.txPower, PA_OUTPUT_PA_BOOST_PIN); 
                                  // Rango [2, 20] en dBm
                                  // Importante seleccionar un valor bajo para pruebas
                                  // a corta distancia y evitar saturar al receptor
}

void configureLoRa() {
  // Configuramos algunos parámetros de la radio
  applyLoRaConfiguration(thisNodeConf);   // Configuramos el BW, SF, CR y PW

  LoRa.setSyncWord(0x12);         // Palabra de sincronización privada por defecto para SX127X 
                                  // Usaremos la palabra de sincronización para crear diferentes
                                  // redes privadas por equipos
  LoRa.setPreambleLength(8);      // Número de símbolos a usar como preámbulo
}


void TxFinished() {
  txDoneFlag = true;
}

void printNodeConfig(String whichNode, const LoRaConfig_t& config){
    Serial.print(whichNode + " CONFIG { BW: ");
    Serial.print(bandwidth_kHz[config.bandwidth_index]);
    Serial.print(" kHz, SPF: ");
    Serial.print(config.spreadingFactor);
    Serial.print(", CR: ");
    Serial.print(config.codingRate);
    Serial.print(", TxPwr: ");
    Serial.print(config.txPower);
    Serial.println(" dBm }");
}


// =========================================== DEBUGGING FUNCTIONS ===========================================
/* Para poder usar hay que modificar la libreria LoRa y poner públicos los métodos que se utilizan
void printCurrentLoRaConfig() {
    float bandwidth = LoRa.getSignalBandwidth();   // Obtiene el ancho de banda en Hz
    int spreadingFactor = LoRa.getSpreadingFactor(); // Obtiene el factor de expansión
    int codingRate = LoRa.getCodingRate4();        // Obtiene la tasa de codificación
    float txPower = LoRa.getTxOutputPower();       // Obtiene la potencia de transmisión en dBm

    // Convertir el ancho de banda a kHz para mostrarlo
    bandwidth /= 1000.0;

    Serial.print("CURRENT CONFIG { BW: ");
    Serial.print(bandwidth, 2); // Muestra el ancho de banda con dos decimales
    Serial.print(" kHz, SPF: ");
    Serial.print(spreadingFactor);
    Serial.print(", CR: ");
    Serial.print(codingRate);
    Serial.print(", TxPwr: ");
    Serial.print(txPower);
    Serial.println(" dBm }");
}
*/

/* Funcion para introducir modificaciones por serial
inline void checkSerialCommands(){
// 1: Aumentar SF   2: Reducir BW     3: Aumentar CR    4: Reducir PW
// 5: Reducir SF    6: Aumentar BW    7: Reducir CR     8: Aumentar PW   

    if (Serial.available() > 0) {
        char c = Serial.read(); // Lee el carácter entrante
        int number = c - '0'; // Convierte el carácter a un número entero

        if (number >= 1 && number <= 8) {
            switch (number) {
                case 1:                    
                    Serial.println("Increasing SF...");
                    remoteNodeConf.spreadingFactor = min(MAX_SF, remoteNodeConf.spreadingFactor + 1);
                    break;
                case 2:
                    Serial.println("Reducing BW...");
                    remoteNodeConf.bandwidth_index = max(MIN_BW_INDEX, remoteNodeConf.bandwidth_index - 1);
                    break;
                case 3:
                    Serial.println("Increasing CR...");
                    remoteNodeConf.codingRate = min(MAX_CR, remoteNodeConf.codingRate + 1);
                    break;
                case 4:
                    Serial.println("Reducing PW");
                    remoteNodeConf.txPower = max(MIN_TX_POWER, remoteNodeConf.txPower - 1);
                    break;
                case 5:
                    Serial.println("Reducing SF");
                    remoteNodeConf.spreadingFactor = max(MIN_SF, remoteNodeConf.spreadingFactor - 1);
                    break;
                case 6:
                    Serial.println("Increasing BW");
                    remoteNodeConf.bandwidth_index = min(MAX_BW_INDEX, remoteNodeConf.bandwidth_index + 1);
                    break;
                case 7:
                    Serial.println("Reducing CR");
                    remoteNodeConf.codingRate = max(MIN_CR, remoteNodeConf.codingRate - 1);
                    break;
                case 8:
                    Serial.println("Increasing PW");
                    remoteNodeConf.txPower = min(MAX_TX_POWER, remoteNodeConf.txPower + 1);
                    break;                        
                default:
                    Serial.println("Número no reconocido.");
                    break;
            }
            applyLoRaConfiguration(remoteNodeConf);
            
        }
    }
}
*/