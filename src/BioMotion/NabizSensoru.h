#ifndef NABIZ_SENSORU_H 
#define NABIZ_SENSORU_H 

#include <Arduino.h> 
#include <Wire.h> 
#include "MAX30105.h" // SparkFun kütüphanesini kullanıyoruz
#include "heartRate.h" // Nabız algoritması

class NabizSensoru { 
  public: 
    NabizSensoru(); 
    bool baslat(); // Sensörü başlatır
    void olc();    // Sürekli çalışıp nabzı hesaplar
    float getNabiz(); // Sonucu verir
    bool hazirMi(); 

  private: 
    MAX30105 sensor; 
    float guncelNabiz; 
    unsigned long sonOkumaZamani; 
    bool sensorHazir; 
    void sensorAyarlariYukle(); // İnce ayarlar
};

#endif