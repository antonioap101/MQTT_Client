// ========================== CLASE HOME_SENSORS ==========================

#include "home_sensors.h"

// Constructor por defecto
HomeSensors::HomeSensors() {
    this->temperature = 0.0;
    this->humidity = 0.0;
    this->gasDetection = false;
    this->fireDetection = false;
    this->gardenDoorStatus = false;
    this->ParkingCarStatus = false;
    this->ParkingAccessStatus = false;
}

// Constructor con parámetros iniciales utilizando asignación directa
HomeSensors::HomeSensors(double initialTemperature, double initialHumidity, bool initialGasDetection, bool initialFireDetection,
                          bool initialGardenDoorStatus, bool initialParkingCarStatus, bool initialParkingAccessStatus) {
    this->temperature = initialTemperature;
    this->humidity = initialHumidity;
    this->gasDetection = initialGasDetection;
    this->fireDetection = initialFireDetection;
    this->gardenDoorStatus = initialGardenDoorStatus;
    this->ParkingCarStatus = initialParkingCarStatus;
    this->ParkingAccessStatus = initialParkingAccessStatus;
}


// Métodos para establecer los datos del sensor
void HomeSensors::setTemperature(double temp) {
    temperature = temp;
}

void HomeSensors::setHumidity(double hum) {
    humidity = hum;
}

void HomeSensors::setGasDetection(bool detected) {
    gasDetection = detected;
}

void HomeSensors::setFireDetection(bool detected) {
    fireDetection = detected;
}

void HomeSensors::setGardenDoorStatus(bool isOpen) {
    gardenDoorStatus = isOpen;
}

void HomeSensors::setParkingCarStatus(bool isOpen) {
    ParkingCarStatus = isOpen;
}

void HomeSensors::setParkingAccessStatus(bool isLocked) {
    ParkingAccessStatus = isLocked;
}

// Métodos para obtener los datos del sensor
double HomeSensors::getTemperature() const {
    return temperature;
}

double HomeSensors::getHumidity() const {
    return humidity;
}

bool HomeSensors::isGasDetected() const {
    return gasDetection;
}

bool HomeSensors::isFireDetected() const {
    return fireDetection;
}

bool HomeSensors::isGardenDoorOpen() const {
    return gardenDoorStatus;
}

bool HomeSensors::isParkingDoorOpen() const {
    return ParkingCarStatus;
}

bool HomeSensors::isParkingLocked() const {
    return ParkingAccessStatus;
}

String HomeSensors::serialize() const {
    // Crear un objeto DynamicJsonDocument    
    DynamicJsonDocument jsonDoc(256);

    // Agregar los datos al documento JSON
    jsonDoc["Temperature"] = temperature;
    jsonDoc["Humidity"] = humidity;
    jsonDoc["GasDetection"] = gasDetection;
    jsonDoc["FireDetection"] = fireDetection;
    jsonDoc["GardenDoorStatus"] = gardenDoorStatus;
    jsonDoc["ParkingCarStatus"] = ParkingCarStatus;
    jsonDoc["ParkingAccessStatus"] = ParkingAccessStatus;

    // Crear una cadena para almacenar el JSON serializado
    String serializedData;
    serializeJson(jsonDoc, serializedData);

    return serializedData;
}
