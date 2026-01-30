#include <Wire.h>          
#include "DataBlynk.h"     
#include "SicaklikSensoru.h"
#include "HareketSensoru.h"
#include "NabizSensoru.h"
#include "Ekran.h"

// --- PIN TANIMLAMALARI ---
const int PIN_SICAKLIK = 15; 

// --- BLYNK SANAL PINLERİ ---
const int VPIN_NABIZ = V0;
const int VPIN_SICAKLIK = V1;
const int VPIN_ACI = V2;
const int VPIN_DOGRU = V3;
const int VPIN_YANLIS = V4;

// --- NESNELER ---
SicaklikSensoru termometre(PIN_SICAKLIK); 
HareketSensoru sporKocu;    
NabizSensoru nabizSensoru;  
EkranYoneticisi ekran;      
BlynkTimer veriZamanlayici; 

// --- ZAMANLAYICILAR (Performans İçin) ---
unsigned long sonEkranGuncelleme = 0; // Ekranı yavaşlatmak için sayaç

char wifiSsid[] = "BYY";
char wifiSifre[] = "salakzeyran";
char authToken[] = BLYNK_AUTH_TOKEN; 

void blynkGonder(); 

void setup() {
  Serial.begin(115200); 

  // --- I2C OTOBANI ---
  // NabizSensoru.cpp içinde de Wire.begin var ama burada da olması garantiye alır.
  Wire.begin(14, 13);  

  termometre.baslat();
  sporKocu.baslat();

  if(!nabizSensoru.baslat()) {
    Serial.println("UYARI: Nabiz Sensoru Baslatilamadi!");
  } else {
    Serial.println("SISTEM: Nabiz Sensoru Aktif!");
  }

  ekran.baslat();
  ekran.acilisMesaji(); 
  
  delay(1500); 

  // İnternet bağlantısı
  Blynk.begin(authToken, wifiSsid, wifiSifre);
  
  veriZamanlayici.setInterval(1000L, blynkGonder);
}

void loop() {
  // --- 1. SÜREKLİ YAPILACAKLAR  ---
   nabizSensoru.olc();
  sporKocu.hareketiTakipEt();
  nabizSensoru.olc();
  
  // --- 2. PERİYODİK YAPILACAKLAR (YAVAŞ OLABİLİR) ---
  // Ekranı her döngüde değil, saniyede 4 kere (250ms'de bir) güncelle.
  // Böylece işlemci nefes alır ve nabız ölçmeye vakit bulur.
  
  if (millis() - sonEkranGuncelleme > 250) {
    ekran.guncelle(
      termometre.sicaklikOku(), 
      nabizSensoru.getNabiz(), 
      sporKocu.getAnlikAci(), 
      sporKocu.getDogruSayisi(), 
      sporKocu.getYanlisSayisi()
    );
    sonEkranGuncelleme = millis(); // Saati kur
  }
 nabizSensoru.olc();
  // --- 3. ARKA PLAN İŞLERİ ---
  Blynk.run();         
  veriZamanlayici.run();
   nabizSensoru.olc();
}

void blynkGonder() {
  float anlikNabiz = nabizSensoru.getNabiz();
  
  if (anlikNabiz > 10) {
     Blynk.virtualWrite(VPIN_NABIZ, anlikNabiz);
  }
  
  Blynk.virtualWrite(VPIN_SICAKLIK, termometre.sicaklikOku());
  Blynk.virtualWrite(VPIN_ACI, sporKocu.getAnlikAci());
  Blynk.virtualWrite(VPIN_DOGRU, sporKocu.getDogruSayisi());
  Blynk.virtualWrite(VPIN_YANLIS, sporKocu.getYanlisSayisi());
}

BLYNK_WRITE(V5) {
  int butonDurumu = param.asInt(); 
  if (butonDurumu == 1) { 
    sporKocu.yeniSet();   
    Blynk.virtualWrite(VPIN_DOGRU, 0);
    Blynk.virtualWrite(VPIN_YANLIS, 0);
  }
}