# Dinamik_programlama_yaklasimi_ile_Uretim_Planlama

Bu proje, n adet işin m adet makinede işlenmesi sırasında toplam işlem süresini (işlem süreleri ve makine geçiş maliyetleri dahil) minimize eden bir dinamik programlama çözümü sunmaktadır.

## 📚 Proje İçeriği
- **Dinamik Programlama Çözümü**: İşlem sürelerini ve makine geçiş maliyetlerini minimize eden optimal çözüm.
- **Matris Yapısı**: İki boyutlu veri dizileri için özel matris yapısı.
- **Bellek Yönetimi**: Dinamik bellek ayırma ve temizleme işlemleri.
- **Performans Ölçümü**: Algoritmanın çalışma süresini ölçme.

## 🔍 Problem Tanımı ve Matris Zinciri Çarpımı ile İlişkisi

Üretim çizelgeleme problemi, n adet işin m adet makinede işlenmesi sırasında toplam işlem süresini minimize etmeyi amaçlar. Bu problem, matris zinciri çarpımı problemi ile benzer bir yapıya sahiptir.

### Problem Tanımı:
- n adet iş ve m adet makine vardır
- Her iş her makinede farklı sürelerde işlenebilir
- Makineler arası geçişlerde ek maliyetler (süreler) vardır
- Amaç: Toplam işlem süresini (işlem süreleri + geçiş maliyetleri) minimize etmek

### Matris Zinciri Çarpımı ile İlişkisi:
Matris zinciri çarpımı problemi, bir dizi matrisin çarpımını en az sayıda çarpma işlemi ile gerçekleştirmeyi amaçlar. Üretim çizelgeleme problemi de benzer şekilde, işlerin makine atamalarını en az toplam süre ile gerçekleştirmeyi hedefler. Her iki problem de alt problemlerin çözümlerinin tekrar kullanılması prensibiyle dinamik programlama yaklaşımıyla çözülebilir.

## 🧮 Tablolama ve Hafızalama Algoritmalarının Adım Adım Açıklaması

### Dinamik Programlama Yaklaşımı:

1. **Alt Problemlerin Tanımlanması**:
   - `dp[i][j]`: i. işin j. makinede işlenmesi durumunda minimum toplam süre

2. **Tablolama Süreci**:
   - İlk iş için her makinedeki işlem süreleri doğrudan atanır
   - Her sonraki iş için, önceki işin hangi makinede işlendiğine bağlı olarak minimum süre hesaplanır
   - Her adımda, önceki işin tüm olası makine atamaları değerlendirilir

3. **Hafızalama (Memoization) Tekniği**:
   - Hesaplanan alt problem çözümleri `dp_tablosu` matrisinde saklanır
   - Aynı alt problem tekrar karşılaşıldığında, hesaplama yerine tablodan değer alınır

4. **Optimal Yolun Bulunması**:
   - Son iş için en düşük süreye sahip makine seçilir
   - Geriye doğru izleme yapılarak optimal makine atamaları belirlenir

## 💻 Kodların Açıklaması ve Test Sonuçları

### Ana Veri Yapıları:
- `Matris`: İki boyutlu veri dizileri için özel yapı
- `UretimCizelgeleyici`: Tüm problem verilerini ve çözüm sonuçlarını içeren yapı

### Önemli Fonksiyonlar:
- `matris_olustur`: İki boyutlu matris için bellek ayırma
- `matris_temizle`: Matris için ayrılan belleği temizleme
- `cizelgeleyici_olustur`: Üretim çizelgeleyici yapısını oluşturma
- `islem_surelerini_ayarla`: İşlem sürelerini ayarlama
- `gecis_maliyetlerini_ayarla`: Geçiş maliyetlerini ayarlama
- `coz`: Dinamik programlama ile optimal çözümü bulma
- `cozumu_yazdir`: Çözüm sonuçlarını ekrana yazdırma

### Test Sonuçları:
Program, 6 iş ve 4 makine için test edilmiştir. Örnek test verileri:
- İşlem süreleri: 45-38-52-41 dakika (İş 1 için)
- Geçiş maliyetleri: 0-15-20-12 dakika (Makine 1'den diğerlerine)

Test sonuçlarına göre, algoritma optimal çözümü başarıyla bulmuş ve toplam işlem süresini minimize etmiştir. Hesaplama süresi milisaniyeler içinde tamamlanmıştır.

## ⏱️ Zaman ve Uzay Karmaşıklığı Analizi

### Zaman Karmaşıklığı:
- **Dinamik Programlama Çözümü**: O(n·m²)
  - n: İş sayısı
  - m: Makine sayısı
  - Her iş için her makine kombinasyonu değerlendirilir
  - Her değerlendirme için önceki işin tüm makine olasılıkları kontrol edilir

### Uzay Karmaşıklığı:
- **Dinamik Programlama Tablosu**: O(n·m)
  - n satır (iş sayısı) × m sütun (makine sayısı) boyutunda matris
- **Giriş Verileri**: O(n·m + m²)
  - İşlem süreleri matrisi: n×m
  - Geçiş maliyetleri matrisi: m×m
- **Optimal Yol Dizisi**: O(n)
  - n elemanlı dizi (her iş için makine ataması)

### Performans İyileştirmeleri:
- Bellek yönetimi için özel temizleme fonksiyonları
- Gereksiz hesaplamaları önlemek için tablolama tekniği
- Çift işaretçi kullanımı ile verimli matris işlemleri

## 🛠 Derleme ve Çalıştırma
Projeyi derlemek ve çalıştırmak için aşağıdaki adımları takip edebilirsiniz:

### 1️⃣ Derleme
```bash
gcc uretim_cizelgeleyici.c -o uretim_cizelgeleyici
```

### 2️⃣ Çalıştırma
```bash
./uretim_cizelgeleyici
```

## 📌 Kullanım
Program çalıştırıldığında aşağıdaki işlemleri gerçekleştirir:
1. **Giriş Verilerini Göster**: İş sayısı, makine sayısı, işlem süreleri ve geçiş maliyetlerini gösterir.
2. **Optimal Çizelgeyi Hesapla**: Dinamik programlama ile optimal çözümü bulur.
3. **Sonuçları Göster**: Optimal makine atamaları, işlem süreleri ve toplam süreyi gösterir.
4. **Performans Bilgisi**: Algoritmanın çalışma süresini gösterir.

## 📊 Veri Yapıları
Proje iki ana veri yapısı kullanmaktadır:

### Matris Yapısı
```c
typedef struct {
    double** veri;  // İki boyutlu dizi için çift işaretçi
    int satir;      // Matrisin satır sayısı
    int sutun;      // Matrisin sütun sayısı
} Matris;
```

### Üretim Çizelgeleyici Yapısı
```c
typedef struct {
    int is_sayisi;              // Toplam iş sayısı
    int makine_sayisi;          // Toplam makine sayısı
    Matris islem_sureleri;      // İşlem süreleri matrisi
    Matris gecis_maliyetleri;   // Geçiş maliyetleri matrisi
    Matris dp_tablosu;          // Dinamik programlama tablosu
    int* yol;                   // Optimal yol dizisi
} UretimCizelgeleyici;
```
## ✅ Lisans
Bu proje eğitim amaçlı geliştirilmiştir.

 İyi çalışmalar! 😊 
