#include "messageQueue.h"

// Clase para representar una cola de mensajes
QueueMessage::QueueMessage() : maxSize(10), size(0) {
    messages = new Message[maxSize];
}

// Constructor que permite especificar el tamaño máximo de la cola
QueueMessage::QueueMessage(int maxQueueSize) : maxSize(maxQueueSize), size(0) {
  messages = new Message[maxSize];
}

// Agrega un mensaje a la cola
bool QueueMessage::enqueue(const Message &msg) {
  if (size < maxSize) {
    messages[size++] = msg;
    return true;
  } else {
    Serial.println("Cola de mensajes llena, no se pudo agregar el mensaje.");
    return false;
  }
}

// Desencola y devuelve el mensaje más antiguo de la cola
Message QueueMessage::dequeue() {
  if (size > 0) {
    Message msg = messages[0];
    // Mover los mensajes restantes hacia la izquierda
    for (int i = 0; i < size - 1; i++) {
      messages[i] = messages[i + 1];
    }
    size--;
    return msg;
  } else {
    // La cola está vacía, devuelve un mensaje vacío
    return Message();
  }
}

// Verifica si la cola está vacía
bool QueueMessage::isEmpty() const {
  return size == 0;
}