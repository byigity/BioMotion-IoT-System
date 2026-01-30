#include "NabizSensoru.h"

NabizSensoru::NabizSensoru() {
  guncelNabiz = 0.0f;  
  sonOkumaZamani = 0;
  sensorHazir = false;
}

bool NabizSensoru::baslat() {
  Wire.begin(14, 13);
  // Sensörü başlatmaya çalış. (Hızlı modda)
  sensorHazir = sensor.begin(Wire, I2C_SPEED_FAST);
  
  if (sensorHazir) {
    sensorAyarlariYukle(); // Bağlantı varsa ayarları yükle
  }
  return sensorHazir;
}

void NabizSensoru::sensorAyarlariYukle() {
  // --- AYAR GÜNCELLEMESİ ---
  // 0x0A (10) az geldi (20k okudu).
  // 0x1F (31) çok geldi (136k okudu).
  // Ortasını bulalım: 0x15 (21). Hedefimiz 50k-70k arası temiz sinyal.
  
  byte ledBrightness = 0x15; 
  byte sampleAverage = 4; 
  byte ledMode = 2; 
  int sampleRate = 100; 
  int pulseWidth = 411; 
  int adcRange = 4096;

  sensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); 
  sensor.enableFIFORollover(); 
}

void NabizSensoru::olc() {
  if (!sensorHazir) return; // Sensör bozuksa işlem yapma

  long irDegeri = sensor.getIR(); // Parmağın yansıttığı ışık miktarını oku
  Serial.print ("Ham Ir:");
  Serial.println (irDegeri);

  // --- PARMAK KONTROLÜ (EŞİK GÜNCELLEMESİ) ---
  // IR değerin 20.000 civarında olduğu için eşiği 10.000'e çektik.
  if (irDegeri < 10000) { 
    guncelNabiz = 0; // Parmak yoksa nabız 0'dır
    sonOkumaZamani = millis(); // Zamanlayıcıyı resetle
    return; 
  }

  // --- KALP ATIŞI TESPİTİ ---
  if (checkForBeat(irDegeri)) { 
    unsigned long simdi = millis(); 
    unsigned long fark = simdi - sonOkumaZamani; 
    sonOkumaZamani = simdi; 
    
    // --- FİLTRELEME ---
    // 20 BPM ile 250 BPM arası mantıklı değerlerdir
    if (fark > 240 && fark < 3000) { 
      float yeniNabiz = 60000.0f / (float)fark;
      
      // --- MATEMATİKSEL DÜZELTME (JUMPSTART) ---
      // Eğer nabız daha önce 0 idiyse (ilk ölçümse), ortalama alma!
      // Direkt yeni değeri kabul et. Yoksa 0'dan 70'e çıkması dakikalar sürer.
      if (guncelNabiz < 10) {
         guncelNabiz = yeniNabiz;
      } else {
         // Nabız zaten varsa yumuşak geçiş yap
         guncelNabiz = (guncelNabiz * 0.9) + (yeniNabiz * 0.1); 
      }
    }
  }
}

float NabizSensoru::getNabiz() {
  return guncelNabiz;
}