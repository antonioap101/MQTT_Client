#ifndef GLOBAL_SENSORS_H
#define GLOBAL_SENSORS_H

#include <ArduinoJson.h>  // Asegúrate de incluir la biblioteca ArduinoJson

class GlobalSensors {
public:
  GlobalSensors();  // Constructor por defecto
  GlobalSensors(float initialTemperature, int initialHumidity,
              int initialGasDetection, int initialFireDetection,
              bool initialGardenLightStatus, int initialGardenSoilHumidity,
              bool initialParkingCarStatus, bool initialParkingAccessStatus, bool initialParkingLightStatus);
  
  // -------------------------- SETTERS
  void setTemperature(float temp);
  void setHumidity(int hum);
  void setGasDetection(int detected);
  void setFlameDetection(int detected);

  void setGardenLightStatus(bool isOpen);
  void setGardenSoilHumidity(int soilHumidity);

  void setParkingCarStatus(bool isCarInside);
  void setParkingAccessStatus(bool isLocked);
  void setParkingLightStatus(bool isOn);

  // -------------------------- GETTERS 
  float getTemperature() const;
  int getHumidity() const;
  int getGasDetection() const;
  int getFlameDetection() const;
  
  bool isGardenLightOn() const;
  int getGardenSoilHumidity() const;

  bool isCarInParking() const;
  bool isParkingLocked() const;
  bool isParkingLightOn() const;

  // -------------------------- SERIALIZADOR 
  String serialize() const;

private:
  float temperature;
  int humidity;
  int gasDetection;
  int fireDetection;

  bool gardenLightStatus;
  int gardenSoilHumidity; // FALTA

  bool parkingLightStatus; 
  bool parkingCarStatus;
  bool parkingAccessStatus;

};

// GlobalSensors myHome;

// Crear un nuevo objeto GlobalSensors con valores iniciales específicos
GlobalSensors myHome(25.5, 50, 0, 0, false, 50, false, false, false);

#endif  // GLOBAL_SENSORS_H
