#ifndef QUEUE_MESSAGE_H
#define QUEUE_MESSAGE_H


// Clase para representar una cola de mensajes
class QueueMessage {
public:

  // Constructor por defecto
  QueueMessage();

  // Constructor que permite especificar el tamaño máximo de la cola
  QueueMessage(int maxQueueSize);

  // Agrega un mensaje a la cola
  bool enqueue(const Message &msg);

  // Desencola y devuelve el mensaje más antiguo de la cola
  Message dequeue();

  // Verifica si la cola está vacía
  bool isEmpty() const;

private:
  Message *messages; // Usaremos asignación dinámica de memoria según el tamaño especificado
  int maxSize;
  int size;
};

QueueMessage messageQueue(10);

#endif // QUEUE_MESSAGE_H
