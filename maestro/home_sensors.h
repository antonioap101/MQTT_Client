#ifndef HOME_SENSORS_H
#define HOME_SENSORS_H

#include <ArduinoJson.h>  // Asegúrate de incluir la biblioteca ArduinoJson

class HomeSensors {
public:
  HomeSensors();  // Constructor por defecto
  HomeSensors(double initialTemperature, double initialHumidity,
              bool initialGasDetection, bool initialFireDetection,
              bool initialGardenDoorStatus, bool initialParkingCarStatus,
              bool initialParkingAccessStatus);

  void setTemperature(double temp);
  void setHumidity(double hum);
  void setGasDetection(bool detected);
  void setFireDetection(bool detected);
  void setGardenDoorStatus(bool isOpen);
  void setParkingCarStatus(bool isOpen);
  void setParkingAccessStatus(bool isLocked);

  double getTemperature() const;
  double getHumidity() const;
  bool isGasDetected() const;
  bool isFireDetected() const;
  bool isGardenDoorOpen() const;
  bool isParkingDoorOpen() const;
  bool isParkingLocked() const;
  String serialize() const;

private:
  double temperature;
  double humidity;
  bool gasDetection;
  bool fireDetection;
  bool gardenDoorStatus;
  bool ParkingCarStatus;
  bool ParkingAccessStatus;
};

// HomeSensors myHome;

// Crear un nuevo objeto HomeSensors con valores iniciales específicos
HomeSensors myHome(25.5, 50.0, true, false, true, false, true);

#endif  // HOME_SENSORS_H
