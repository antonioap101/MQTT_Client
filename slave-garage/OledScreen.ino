#include "OledScreen.h"

void setupOledScreen(){
    #if RST_PIN >= 0
      oled.begin(&Adafruit128x64, SCREEN_I2C_ADDRESS, RST_PIN);
    #else   // RST_PIN >= 0
      oled.begin(&Adafruit128x64, SCREEN_I2C_ADDRESS);
    #endif  // RST_PIN >= 0  

    oled.setFont(Adafruit5x7);


}

void sensorStartScreen() {
  oled.clear(); // Limpia la pantalla antes de mostrar el mensaje
  oled.set2X(); // Aumenta el tamaño de la fuente para el título
  oled.println("SRF02<addr>");

  oled.set1X(); // Regresa a tamaño normal para el resto del texto
  oled.println();

  oled.println("Waiting for data...");
  oled.println();

  // Opcional: Puedes agregar más información o líneas decorativas
  oled.println("________________");
}


void printOnOLEDScreen(String title, String arg, int resultStringSize, const uint8_t* font){
  // Limpiamos el contenido de la pantalla
  oled.clear();

  // Ajustamos la fuente
  oled.setFont(font);

  // Mostramos el titulo del mensaje a mostrar
  oled.set2X();
  oled.print(title);

  // Mostramos el contenido
  if (resultStringSize == 1) oled.set1X();
  else if (resultStringSize == 2) oled.set2X();
  else oled.set1X();

  oled.println(arg);
  
}