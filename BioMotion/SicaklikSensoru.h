#ifndef SICAKLIK_SENSORU_H
#define SICAKLIK_SENSORU_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

class SicaklikSensoru {
  private:
    // Bu sensör OneWire protokolünü kullanır.
    OneWire wire; 
    DallasTemperature sensor; 

  public:
    // Kurucu: Hangi pin
    SicaklikSensoru(int pin);

    void baslat();
    float sicaklikOku();
};

#endif