#include "HareketSensoru.h"
#include <math.h> 

HareketSensoru::HareketSensoru() {
  // Değişkenleri güvenli başlangıç değerlerine çekiyoruz
  dogruTekrarSayisi = 0;
  yanlisTekrarSayisi = 0;
  hareketBasladi = false;
  maxAci = 0;
}

void HareketSensoru::baslat() {
  // Sensörü başlat. Eğer kabloda sorun varsa burada hata verir.
  // Not: Wire.begin(14,12) ana dosyada yapıldığı için burası o hattı kullanır.
  if (!mpu.begin()) {
    Serial.println("MPU6050 Hatasi! Kablolari kontrol et.");
    // while(1); // Sistemi kilitlememek için burayı iptal ettim, sadece uyarı versin.
  }
  
  // --- SPOR MODU AYARLARI ---
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G); // Hızlı kol hareketlerini yakalamak için 8G
  mpu.setGyroRange(MPU6050_RANGE_500_DEG); 
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ); // Titreşimleri filtrele
}

float HareketSensoru::getAnlikAci() {
  sensors_event_t a, g, temp; 
  mpu.getEvent(&a, &g, &temp); // Ham veriyi çek
  
  // --- AÇI HESAPLAMA ---
  // Trigonometri kullanarak (atan2) yerçekimi vektörünün açısını buluyoruz.
  // Bu bize kolun yere göre ne kadar kalktığını söyler.
  float aci = atan2(a.acceleration.y, a.acceleration.z) * 180 / PI; 
  return abs(aci); // Negatif açıları pozitife çevirip gönderiyorum
}

void HareketSensoru::hareketiTakipEt() {
  float anlikAci = getAnlikAci();
  
  // --- DURUM MAKİNESİ (STATE MACHINE) ---
  
  // 1. AŞAMA: Hareket Başlangıcı
  // Eğer kol 15 dereceyi geçerse "Hareket Başladı" diyoruz.
  if (anlikAci > 15 && !hareketBasladi) {
    hareketBasladi = true;
    maxAci = 0; // Yeni hareket için zirve noktasını sıfırla
  }
  
  // 2. AŞAMA: Hareketin Takibi
  if (hareketBasladi) {
    // Kolun çıktığı en yüksek noktayı hafızada tut
    if (anlikAci > maxAci) {
      maxAci = anlikAci;
    }
    
    // 3. AŞAMA: Hareketin Bitişi (Kol indi)
    // Kol 10 derecenin altına inince hareket bitmiş sayılır.
    if (anlikAci < 10) {
      // Karar Anı: Hareket doğru muydu?
      
      if (maxAci >= 75) { 
        // Kol tam kalktıysa (75 derece üstü) -> BAŞARILI
        dogruTekrarSayisi++;
        Serial.println(">>> BASARILI TEKRAR! <<<");
      } 
      else if (maxAci > 30 && maxAci < 75) {
        // Kol yarım kalktıysa -> HATALI
        yanlisTekrarSayisi++;
        Serial.println(">>> HATALI / YARIM TEKRAR! <<<");
      }
      
      hareketBasladi = false; // Sistemi sıfırla, yeni hareketi bekle
    }
  }
}

// Private değişkenleri dışarıya okutmak için köprü fonksiyonlar
int HareketSensoru::getDogruSayisi() { return dogruTekrarSayisi; }
int HareketSensoru::getYanlisSayisi() { return yanlisTekrarSayisi; }

void HareketSensoru::yeniSet() {
  // Telefondan "Sıfırla" denince burası çalışır
  dogruTekrarSayisi = 0;
  yanlisTekrarSayisi = 0;
  hareketBasladi = false;
  Serial.println("--- YENI SET BASLADI ---");
}