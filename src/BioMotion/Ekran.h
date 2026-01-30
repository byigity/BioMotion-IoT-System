#ifndef EKRAN_H
#define EKRAN_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class EkranYoneticisi {
  private:
    // ona özel "TwoWire" türünde ikinci bir I2C hattı oluşturuyorum.
    TwoWire ozelI2C; 
    
    // Ekran kütüphanesinin nesnesi
    Adafruit_SSD1306 oled;

    // --- SİYAH KARTIN ÖZEL PİNLERİ ---
    // Bu pinler kartın içindeki ekrana bağlıdır
    const int PIN_SDA = 5;  
    const int PIN_SCL = 4; 
    const int PIN_RST = 16; 
    
    // Ekranı çok sık yenileyip titretmemek için zaman tutucu
    unsigned long sonGuncelleme;

  public:
    EkranYoneticisi(); // Kurucu: Ayarları yükler
    void baslat();     // Ekranı elektriksel olarak açar
    void acilisMesaji(); // Logoyu gösterir
    
    // Bütün verileri alıp ekrana güzelce yerleştiren fonksiyon
    void guncelle(float sicaklik, int nabiz, float aci, int dogru, int yanlis);
};

#endif