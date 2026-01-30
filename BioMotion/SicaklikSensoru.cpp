#include "SicaklikSensoru.h"

// --- KURUCU METOT ---
// pin numarasını alıp OneWire nesnesini ona bağlıyoruz.
SicaklikSensoru::SicaklikSensoru(int pin) : wire(pin), sensor(&wire) { }

void SicaklikSensoru::baslat() {
  sensor.begin(); 
}

float SicaklikSensoru::sicaklikOku() {
  // 1. Sensöre "Ölçüm yap" emri ver
  sensor.requestTemperatures(); 
  
  // 2. Sonucu Santigrat (C) olarak al
  float sicaklikC = sensor.getTempCByIndex(0);
  
  // --- HATA KONTROLÜ ---
  // Eğer sensör takılı değilse -127 gönderir.
  // Ekranda -127 yazmasın diye 0.0 gönderiyoruz.
  if(sicaklikC == -127.00) {
    return 0.0; 
  }
  
  return sicaklikC; 
}