// ========================== CLASE HOME_SENSORS ==========================

#include "global_sensors.h"

// Constructor por defecto
GlobalSensors::GlobalSensors() {
    this->temperature = 0.0;
    this->humidity = 0;
    this->gasDetection = 0;
    this->fireDetection = 0;
    this->gardenLightStatus = false;
    this->gardenLightStatus = false;
    this->parkingCarStatus = false;
    this->parkingAccessStatus = false;
    this->parkingLightStatus = false;
}


// Constructor con parámetros iniciales utilizando asignación directa
GlobalSensors::GlobalSensors(float initialTemperature, int initialHumidity, int initialGasDetection, int initialFireDetection,
                          bool initialGardenLightStatus, int initialGardenSoilHumidity,
                          bool initialParkingCarStatus, bool initialParkingAccessStatus, bool initialParkingLightStatus) {
    this->temperature = initialTemperature;
    this->humidity = initialHumidity;
    this->gasDetection = initialGasDetection;
    this->fireDetection = initialFireDetection;
    this->gardenLightStatus = initialGardenLightStatus;
    this->gardenSoilHumidity = initialGardenSoilHumidity;
    this->parkingCarStatus = initialParkingCarStatus;
    this->parkingAccessStatus = initialParkingAccessStatus;
    this->parkingLightStatus = initialParkingLightStatus;
}


// Métodos para establecer los datos del sensor
void GlobalSensors::setTemperature(float temp) {
    temperature = temp;
}

void GlobalSensors::setHumidity(int hum) {
    humidity = hum;
}

// Cambiar bool a int y renombrar los métodos para setGasDetection y setFlameDetection
void GlobalSensors::setGasDetection(int detected) {
    gasDetection = detected;
}

void GlobalSensors::setFlameDetection(int detected) {
    fireDetection = detected;
}

void GlobalSensors::setGardenLightStatus(bool isOpen) {
    gardenLightStatus = isOpen;
}


void GlobalSensors::setGardenSoilHumidity(int soilHumidity){
  gardenSoilHumidity = soilHumidity;
}

void GlobalSensors::setParkingCarStatus(bool isCarInside) {
    parkingCarStatus = isCarInside;
}

void GlobalSensors::setParkingAccessStatus(bool isLocked) {
    parkingAccessStatus = isLocked;
}

void GlobalSensors::setParkingLightStatus(bool isOn){
  parkingLightStatus = isOn;
}

// Métodos para obtener los datos del sensor
float GlobalSensors::getTemperature() const {
    return temperature;
}

int GlobalSensors::getHumidity() const {
    return humidity;
}

int GlobalSensors::getGasDetection() const {
    return gasDetection;
}

int GlobalSensors::getFlameDetection() const {
    return fireDetection;
}

bool GlobalSensors::isGardenLightOn() const {
    return gardenLightStatus;
}

int GlobalSensors::getGardenSoilHumidity() const {
    return gardenSoilHumidity;
}

bool GlobalSensors::isCarInParking() const {
    return parkingCarStatus;
}

bool GlobalSensors::isParkingLocked() const {
    return parkingAccessStatus;
}

bool GlobalSensors::isParkingLightOn() const {
  return parkingLightStatus;
}

String GlobalSensors::serialize() const {
    // Crear un objeto DynamicJsonDocument    
    DynamicJsonDocument jsonDoc(256);

    // Agregar los datos al documento JSON
    jsonDoc["Temperature"] = temperature;
    jsonDoc["Humidity"] = humidity;
    jsonDoc["GasDetection"] = gasDetection;
    jsonDoc["FireDetection"] = fireDetection;
    jsonDoc["GardenLightStatus"] = gardenLightStatus;
    jsonDoc["GardenSoilHumidity"] = gardenSoilHumidity;
    jsonDoc["ParkingCarStatus"] = parkingCarStatus;
    jsonDoc["ParkingAccessStatus"] = parkingAccessStatus;
    jsonDoc["ParkingLightStatus"] = parkingLightStatus;

    // Crear una cadena para almacenar el JSON serializado
    String serializedData;
    serializeJson(jsonDoc, serializedData);

    return serializedData;
}
