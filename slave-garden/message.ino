// ===================== CLASE MESSAGE =====================

// -----------------------------CONSTRUCTORES-----------------------------
Message::Message() {
  this->recipient = 0;
  this->sender = 0;
  this->messageId = 0;
  this->payloadLength = 0;
}

Message::Message(uint16_t messageId, uint8_t inputPayloadLength, uint8_t destination) {
    this->messageId = messageId;
    this->recipient = destination;
    prepareRandomPayload(this->payload, inputPayloadLength);
    this->payloadLength = inputPayloadLength;
}

Message::Message(uint16_t messageId, const LoRaConfig_t& config, uint8_t destination) {
    this->messageId = messageId;
    this->recipient = destination;
    this->payloadLength = prepareConfigPayload(config);
}

Message::Message(uint16_t messageId, const uint8_t* inputPayload, uint8_t inputPayloadLength, uint8_t destination) {
    this->messageId = messageId;
    this->recipient = destination;
    this->payloadLength = inputPayloadLength;
    memcpy(this->payload, inputPayload, inputPayloadLength * sizeof(uint8_t));
}

Message::Message(uint16_t messageId, bool isAck, uint8_t destination) {
    this->messageId = messageId;
    this->recipient = destination;
    if (isAck) this->payloadLength = prepareACKPayload();
    else this->payloadLength = prepareNACKPayload();
}

uint8_t Message::prepareACKPayload(){
    // Incluimos el RSSI y el SNR del último paquete recibido
    this->payload[0] = ACK_MESSAGE; 
    this->payload[1] = uint8_t(-LoRa.packetRssi() * 2); // RSSI puede estar en un rango de [0, -127] dBm
    this->payload[2] = uint8_t(148 + LoRa.packetSnr()); // SNR puede estar en un rango de [20, -148] dBm
    return 3;
}

uint8_t Message::prepareNACKPayload(){
    this->payload[0] = NACK_MESSAGE; 
    return 1;
}

void Message::prepareRandomPayload(uint8_t* payload, uint8_t payloadLength) {
    payload[0] = (RND_MESSAGE << 6) | random(0, 64);
  for (int i = 1; i < payloadLength; i++) {
    payload[i] = random(0, 256); // Generar un byte aleatorio
  }
}

uint8_t Message::prepareConfigPayload(const LoRaConfig_t config) {
    /* Primer byte:  M1 M0 A3 A2 A1 A0 C1 C0 
       Segundo byte: F2 F1 F0 T4 T3 T2 T1 T0 

       { M: Tipo de mensaje, A: Ancho de banda, C: Coding Rate, F: Spreading Factor, T: Tx Power }
    */
    uint8_t payloadLength = 0;
    uint8_t msgType = CFG_MESSAGE;

    // Empaquetar tipo de mensaje, índice de ancho de banda y tasa de codificación en el primer byte
    this->payload[payloadLength++] = (msgType << 6) | (config.bandwidth_index << 2) | (config.codingRate - 5);

    // Empaquetar factor de dispersión y potencia de transmisión en el segundo byte
    this->payload[payloadLength++] = ((config.spreadingFactor - 6) << 5) | (config.txPower - 2);

    return payloadLength;
}

bool Message::read(){
  int incomingRecipient = LoRa.read();                                            // Dirección del destinatario
  uint8_t incomingSender = LoRa.read();                                           // Dirección del remitente                                      
  uint16_t incomingMsgId = ((uint16_t)LoRa.read() << 7) | (uint16_t)LoRa.read();  // msg ID (High Byte first)   
  uint8_t incomingLength = LoRa.read();                                           // Longitud en bytes del mensaje

  // Leemos el payload del mensaje
  uint8_t buffer[20];                                                             // Buffer para almacenar el mensaje
  uint8_t receivedBytes = 0;
  while (LoRa.available() && (receivedBytes < uint8_t(sizeof(buffer)-1))) {            
    buffer[receivedBytes++] = (char)LoRa.read();
  }
  buffer[receivedBytes] = '\0';         // Terminamos la cadena

  // Comprobamos que el mensaje se haya recibido adecuadamente y que sea para este nodo
  if (!validateMessageLength(incomingLength, receivedBytes)) {
    printConflictiveMessage(incomingSender, incomingRecipient, incomingMsgId, incomingLength, buffer);
    return false;
  }
  if (!isMessageForThisNode(incomingRecipient)) return false;

  // Asignamos los valores a la instancia actual (this)
  this->recipient = incomingRecipient;
  this->sender = incomingSender;  
  this->messageId = incomingMsgId;
  memcpy(this->payload, buffer, receivedBytes);
  this->payloadLength = receivedBytes;
  return true;
}

void Message::send() {
  while(!LoRa.beginPacket()) delay(10);           // Comenzamos el empaquetado del mensaje                                
  
  LoRa.write(this->recipient);                          // Añadimos el ID del destinatario
  LoRa.write(this->sender);                             // Añadimos el ID del remitente
  LoRa.write((uint8_t)(this->messageId >> 7));          // Añadimos el Id del mensaje (MSB primero)
  LoRa.write((uint8_t)(this->messageId & 0xFF)); 
  LoRa.write(this->payloadLength);                      // Añadimos la longitud en bytes del mensaje
  LoRa.write(this->payload, (size_t)this->payloadLength);     // Añadimos el mensaje/payload 
  LoRa.endPacket(true);                           // Finalizamos el paquete, pero no esperamos a que finalice su transmisión
}


void Message::print(String received_or_sent){
    Serial.print(received_or_sent + " MESSAGE { From: 0x" + String(sender, HEX));
    Serial.print(", To: 0x" + String(recipient, HEX));
    Serial.print(", Message ID: " + String(messageId));
    Serial.print(", Payload length: " + String(payloadLength));
    Serial.print(", Payload: ");
    printBinaryPayload(payload, payloadLength);
    int rssi = LoRa.packetRssi();
    float snr = LoRa.packetSnr();
    Serial.print(", RSSI: " + String(rssi) + " dBm");
    Serial.println(", SNR: " + String(snr) + " dB } \n");   
}

void printConflictiveMessage(uint8_t sen, int rec, uint16_t id, uint8_t length, uint8_t* buffer){
    Serial.print(" MESSAGE { From: 0x" + String(sen, HEX));
    Serial.print(", To: 0x" + String(rec, HEX));
    Serial.print(", Message ID: " + String(id));
    Serial.print(", Payload length: " + String(length));
    Serial.print(", Payload: ");
    Message::printBinaryPayload(buffer, length);
    Serial.println(" }");
}


bool Message::validateMessageLength(uint8_t incomingLength, uint8_t receivedBytes) {
    if (incomingLength != receivedBytes) {
        Serial.println("\nReceiving error: declared message length " + String(incomingLength) + 
                       " does not match length " + String(receivedBytes));
        return false;
    }
    return true;
}

bool Message::isMessageForThisNode(int recipient) {
  /* Verificamos si se trata de un mensaje en broadcast o es un mensaje dirigido específicamente
   * a este dispositivo. Nótese que este mecanismo es complementario al uso de la misma SyncWord y 
   * solo tiene sentido si hay más de dos receptores activos compartiendo la misma palabra de sincronización
   */
    if ((recipient & localAddress) != localAddress) {
        Serial.println("\nReceiving error: This message is not for me.");
        return false;
    }
    return true;
}


uint8_t Message::getMessageType() {
    return this->payload[0] >> 6;
}

// Getter para obtener el puntero al payload
const uint8_t* Message::getPayload() const {
    return this->payload;
}
// Getter para obtener la longitud del payload
uint8_t Message::getPayloadLength() const {
    return this->payloadLength;
}

// Agrega este método al final de la implementación de la clase
uint8_t Message::getSender() const {
    return this->sender;
}

void Message::printBinaryPayload(uint8_t * payload, uint8_t payloadLength){
  Serial.print("[");
  for (int i = 0; i < payloadLength; i++) {
    Serial.print((payload[i] & 0xF0) >> 4, HEX);
    Serial.print(payload[i] & 0x0F, HEX);
    if (i<payloadLength-1) Serial.print(" ");
  }
  Serial.print("]");
}

