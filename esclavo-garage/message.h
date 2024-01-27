#ifndef MESSAGE_H
#define MESSAGE_H

#include <Arduino.h>
#include "LoRa-config.h" // Asegúrate de incluir todas las dependencias necesarias

#define CREATE_ACK true
#define CREATE_NACK false

class Message {
public:

    Message();                                                                              // Constructor por defecto
    Message(uint16_t messageId, uint8_t inputPayloadLength, uint8_t destination = 0xFF);    // Constructor para mensajes con payload aleatorio
    Message(uint16_t messageId, const LoRaConfig_t& config, uint8_t destination = 0xFF);    // Constructor para mensaje de configuración
    Message(uint16_t messageId, const uint8_t* inputPayload, uint8_t inputPayloadLength,
             uint8_t destination = 0xFF);                                                   // Constructor para mensajes con payload personalizado
    Message(uint16_t messageId, bool isAck, uint8_t destination = 0xFF);                    // Constructor para mensaje de ACK o NACK
    
    bool read();
    void send();
    void print(String received_or_sent);
    static void printBinaryPayload(uint8_t* payload, uint8_t payloadLength);
    uint8_t getMessageType();
    const uint8_t* getPayload() const;
    uint8_t getPayloadLength() const;
    uint8_t getSender() const;
    

private:
    uint8_t recipient;
    uint8_t sender = localAddress;
    uint16_t messageId;
    uint8_t payload[50];
    uint8_t payloadLength;

    bool validateMessageLength(uint8_t incomingLength, uint8_t receivedBytes);
    bool isMessageForThisNode(int recipient);
    uint8_t prepareConfigPayload(const LoRaConfig_t config);
    static void prepareRandomPayload(uint8_t* payload, uint8_t payloadLength);
    uint8_t prepareACKPayload();
    uint8_t prepareNACKPayload();

};

#endif // MESSAGE_H
