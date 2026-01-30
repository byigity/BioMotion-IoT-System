#ifndef HAREKET_SENSORU_H
#define HAREKET_SENSORU_H

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

class HareketSensoru {
  private:
    Adafruit_MPU6050 mpu; // Sensör kütüphanesinin nesnesi
    
    // İstatistik Değişkenleri
    int dogruTekrarSayisi;
    int yanlisTekrarSayisi;
    
    // Hareket Mantığı Değişkenleri
    bool hareketBasladi;      // Hareketin başladığını anlamak için bayrak
    float maxAci;             // O tekrar içinde kolun çıktığı en tepe nokta
    unsigned long sonZaman;   

  public:
    HareketSensoru(); // Kurucu
    void baslat();    // Sensörü hazırlar
    
    // Sensörden açıyı okuyup tekrar sayan ana beyin
    void hareketiTakipEt();
    
    // Verileri okumak için "Getter" metodları (private metot gereği değişkenlere direkt erişilmez)
    int getDogruSayisi();
    int getYanlisSayisi();
    float getAnlikAci();
    
    // Set bittiğinde sayaçları sıfırlayan fonksiyon
    void yeniSet();
};
#endif