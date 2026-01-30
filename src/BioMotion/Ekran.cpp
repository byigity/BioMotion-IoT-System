#include "Ekran.h"

// --- KURUCU METOT ---
// Burada siyah karta özel bir ayar yapıyoruz:
// ozelI2C(1) -> 1 numaralı donanım kanalını seç (0'ı sensörlere verdik).
// &ozelI2C -> Ekran kütüphanesine "Sen bu özel hattı kullan" diyoruz.
EkranYoneticisi::EkranYoneticisi() : ozelI2C(1), oled(128, 64, &ozelI2C, PIN_RST) {
  sonGuncelleme = 0; // Sayacı sıfırla
}

void EkranYoneticisi::baslat() {
  // 1. Ekranın özel hattını (Pin 5 ve 4) 400kHz hızında başlatıyorum.
  ozelI2C.begin(PIN_SDA, PIN_SCL, 400000);

  // 2. Ekranı Resetle
  pinMode(PIN_RST, OUTPUT);
  digitalWrite(PIN_RST, LOW);  
  delay(20);                   
  digitalWrite(PIN_RST, HIGH); 

  // 3. Kütüphaneyi Başlat (0x3C ekranın adresidir)
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("HATA: OLED Ekran Bulunamadı!");
  } else {
    Serial.println("OLED Ekran Baslatildi.");
  }
  
  oled.clearDisplay(); // Ekranın hafızasındaki çöpleri temizle
  oled.display();      
}

void EkranYoneticisi::acilisMesaji() {
  oled.clearDisplay();           
  oled.setTextSize(1);           // Küçük yazı
  oled.setTextColor(SSD1306_WHITE); 
  oled.setCursor(10, 20);        // İmleci konumlandır
  oled.println("BioMotion");     
  oled.setCursor(10, 40);
  oled.println("Sistem Aciliyor...");
  oled.display();                // Çizilenleri ekrana yansıt
}

void EkranYoneticisi::guncelle(float sicaklik, int nabiz, float aci, int dogru, int yanlis) {
  // --- ZAMANLAMA KONTROLÜ ---
  // Ekranı her milisaniyede güncellersek sistem yavaşlar.
  // Saniyede 5 kare (200ms) insan gözü için yeterli ve akıcıdır.
  if (millis() - sonGuncelleme < 200) return; 
  sonGuncelleme = millis();

  oled.clearDisplay(); // Temiz sayfa aç
  
  // --- ÜST KISIM ---
  oled.setCursor(0, 0);
  oled.print("Nabiz: "); oled.print(nabiz); 
  
  oled.setCursor(70, 0); 
  oled.print(sicaklik, 1); oled.println(" C"); 

  // Görsel ayıraç çizgisi
  oled.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  // --- HAREKET VERİLERİ ---
  oled.setCursor(0, 15);
  oled.print("Aci: "); oled.print((int)aci); oled.println(" deg");

  oled.setCursor(0, 30);
  oled.print("DOGRU: "); 
  oled.setTextSize(2); // Skoru büyük yazalım ki sporcu rahat görsün
  oled.print(dogru); 
  oled.setTextSize(1); 
  
  oled.setCursor(0, 50);
  oled.print("YANLIS: "); oled.print(yanlis);

  oled.display(); // Hepsini yansıt
}