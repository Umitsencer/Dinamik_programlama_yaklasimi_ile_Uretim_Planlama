#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <windows.h>

// Matris yapısı
typedef struct {
    double** veri;
    int satir;
    int sutun;
} Matris;

// Üretim çizelgeleyici yapısı
typedef struct {
    int is_sayisi;
    int makine_sayisi;
    Matris islem_sureleri;
    Matris gecis_maliyetleri;
    Matris dp_tablosu;
    int* yol;
} UretimCizelgeleyici;

// Matris oluşturma fonksiyonu
Matris matris_olustur(int satir, int sutun) {
    Matris m;
    m.satir = satir;
    m.sutun = sutun;
    
    // Ana matris için bellek ayırma
    m.veri = (double**)malloc(satir * sizeof(double*));
    for (int i = 0; i < satir; i++) {
        m.veri[i] = (double*)calloc(sutun, sizeof(double));
    }
    
    return m;
}

// Matris temizleme fonksiyonu
void matris_temizle(Matris* m) {
    if (m == NULL || m->veri == NULL) return;
    
    for (int i = 0; i < m->satir; i++) {
        if (m->veri[i] != NULL) {
            free(m->veri[i]);
            m->veri[i] = NULL;
        }
    }
    free(m->veri);
    m->veri = NULL;
    m->satir = 0;
    m->sutun = 0;
}

// Üretim çizelgeleyici oluşturma
UretimCizelgeleyici* cizelgeleyici_olustur(int is_sayisi, int makine_sayisi) {
    UretimCizelgeleyici* c = (UretimCizelgeleyici*)malloc(sizeof(UretimCizelgeleyici));
    c->is_sayisi = is_sayisi;
    c->makine_sayisi = makine_sayisi;
    
    // Matrisleri oluştur
    c->islem_sureleri = matris_olustur(is_sayisi, makine_sayisi);
    c->gecis_maliyetleri = matris_olustur(makine_sayisi, makine_sayisi);
    c->dp_tablosu = matris_olustur(is_sayisi, makine_sayisi);
    
    // Yol dizisi için bellek ayırma
    c->yol = (int*)calloc(is_sayisi, sizeof(int));
    
    return c;
}

// Üretim çizelgeleyici temizleme
void cizelgeleyici_temizle(UretimCizelgeleyici* c) {
    if (c == NULL) return;
    
    matris_temizle(&c->islem_sureleri);
    matris_temizle(&c->gecis_maliyetleri);
    matris_temizle(&c->dp_tablosu);
    
    if (c->yol != NULL) {
        free(c->yol);
        c->yol = NULL;
    }
    
    free(c);
}

// İşlem sürelerini ayarlama
void islem_surelerini_ayarla(UretimCizelgeleyici* c, double** sureler) {
    for (int i = 0; i < c->is_sayisi; i++) {
        for (int j = 0; j < c->makine_sayisi; j++) {
            c->islem_sureleri.veri[i][j] = sureler[i][j];
        }
    }
}

// Geçiş maliyetlerini ayarlama
void gecis_maliyetlerini_ayarla(UretimCizelgeleyici* c, double** maliyetler) {
    for (int i = 0; i < c->makine_sayisi; i++) {
        for (int j = 0; j < c->makine_sayisi; j++) {
            c->gecis_maliyetleri.veri[i][j] = maliyetler[i][j];
        }
    }
}

// Dinamik programlama ile çözüm
void coz(UretimCizelgeleyici* c, double* min_toplam_sure) {
    // İlk işi başlat
    for (int j = 0; j < c->makine_sayisi; j++) {
        c->dp_tablosu.veri[0][j] = c->islem_sureleri.veri[0][j];
    }
    
    // DP tablosunu doldur
    for (int is_no = 1; is_no < c->is_sayisi; is_no++) {
        for (int mevcut_makine = 0; mevcut_makine < c->makine_sayisi; mevcut_makine++) {
            double min_sure = DBL_MAX;
            int en_iyi_onceki_makine = 0;
            
            for (int onceki_makine = 0; onceki_makine < c->makine_sayisi; onceki_makine++) {
                double toplam_sure = c->dp_tablosu.veri[is_no-1][onceki_makine] + 
                                   c->gecis_maliyetleri.veri[onceki_makine][mevcut_makine] +
                                   c->islem_sureleri.veri[is_no][mevcut_makine];
                if (toplam_sure < min_sure) {
                    min_sure = toplam_sure;
                    en_iyi_onceki_makine = onceki_makine;
                }
            }
            c->dp_tablosu.veri[is_no][mevcut_makine] = min_sure;
        }
    }
    
    // Optimal yolu bul
    *min_toplam_sure = DBL_MAX;
    int son_makine = 0;
    
    for (int makine = 0; makine < c->makine_sayisi; makine++) {
        if (c->dp_tablosu.veri[c->is_sayisi-1][makine] < *min_toplam_sure) {
            *min_toplam_sure = c->dp_tablosu.veri[c->is_sayisi-1][makine];
            son_makine = makine;
        }
    }
    
    c->yol[c->is_sayisi-1] = son_makine;
    
    for (int is_no = c->is_sayisi-2; is_no >= 0; is_no--) {
        double min_sure = DBL_MAX;
        int en_iyi_makine = 0;
        
        for (int makine = 0; makine < c->makine_sayisi; makine++) {
            double toplam_sure = c->dp_tablosu.veri[is_no][makine] + 
                               c->gecis_maliyetleri.veri[makine][c->yol[is_no+1]] +
                               c->islem_sureleri.veri[is_no+1][c->yol[is_no+1]];
            if (toplam_sure < min_sure) {
                min_sure = toplam_sure;
                en_iyi_makine = makine;
            }
        }
        c->yol[is_no] = en_iyi_makine;
    }
}

// Çözümü yazdırma
void cozumu_yazdir(UretimCizelgeleyici* c) {
    double min_toplam_sure;
    coz(c, &min_toplam_sure);
    
    printf("\n===========================================");
    printf("\n           OPTIMAL CIZELGE");
    printf("\n===========================================");
    printf("\nToplam Sure: %.1f dakika", min_toplam_sure);
    
    printf("\n\n-------------------------------------------");
    printf("\n              IS CIZELGESI");
    printf("\n-------------------------------------------");
    
    double toplam_islem_suresi = 0;
    double toplam_gecis_suresi = 0;
    
    for (int i = 0; i < c->is_sayisi; i++) {
        printf("\nIs %d -> Makine %d (Sure: %.1f dakika)", 
               i+1, c->yol[i]+1, c->islem_sureleri.veri[i][c->yol[i]]);
        toplam_islem_suresi += c->islem_sureleri.veri[i][c->yol[i]];
    }
    
    printf("\n\n-------------------------------------------");
    printf("\n           GECIS MALIYETLERI");
    printf("\n-------------------------------------------");
    
    for (int i = 0; i < c->is_sayisi-1; i++) {
        printf("\nMakine %d -> Makine %d: %.1f dakika", 
               c->yol[i]+1, c->yol[i+1]+1, 
               c->gecis_maliyetleri.veri[c->yol[i]][c->yol[i+1]]);
        toplam_gecis_suresi += c->gecis_maliyetleri.veri[c->yol[i]][c->yol[i+1]];
    }
    
    printf("\n\n-------------------------------------------");
    printf("\n           OZET BILGILER");
    printf("\n-------------------------------------------");
    printf("\nToplam Islem Suresi: %.1f dakika", toplam_islem_suresi);
    printf("\nToplam Gecis Suresi: %.1f dakika", toplam_gecis_suresi);
    printf("\nToplam Sure: %.1f dakika", min_toplam_sure);
    printf("\n===========================================\n");
}

// DP tablosunu yazdırma (debug için)
void dp_tablosunu_yazdir(UretimCizelgeleyici* c) {
    printf("\n-------------------------------------------");
    printf("\n           DP TABLOSU");
    printf("\n-------------------------------------------");
    for (int i = 0; i < c->is_sayisi; i++) {
        printf("\nIs %d: ", i+1);
        for (int j = 0; j < c->makine_sayisi; j++) {
            printf("%.1f ", c->dp_tablosu.veri[i][j]);
        }
    }
    printf("\n-------------------------------------------\n");
}

int main() {
    // Türkçe karakter desteği için
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    // Örnek kullanım
    const int is_sayisi = 6;  // İş sayısını artırdık
    const int makine_sayisi = 4;  // Makine sayısını artırdık
    
    UretimCizelgeleyici* cizelgeleyici = cizelgeleyici_olustur(is_sayisi, makine_sayisi);
    if (cizelgeleyici == NULL) {
        printf("Bellek ayırma hatası!\n");
        return 1;
    }
    
    // Daha gerçekçi işlem süreleri (dakika cinsinden)
    double** islem_sureleri = (double**)malloc(is_sayisi * sizeof(double*));
    if (islem_sureleri == NULL) {
        printf("Bellek ayırma hatası!\n");
        cizelgeleyici_temizle(cizelgeleyici);
        return 1;
    }
    
    for (int i = 0; i < is_sayisi; i++) {
        islem_sureleri[i] = (double*)malloc(makine_sayisi * sizeof(double));
        if (islem_sureleri[i] == NULL) {
            printf("Bellek ayırma hatası!\n");
            for (int j = 0; j < i; j++) {
                free(islem_sureleri[j]);
            }
            free(islem_sureleri);
            cizelgeleyici_temizle(cizelgeleyici);
            return 1;
        }
    }
    
    // İşlem sürelerini ayarla
    islem_sureleri[0][0] = 45; islem_sureleri[0][1] = 38; islem_sureleri[0][2] = 52; islem_sureleri[0][3] = 41;
    islem_sureleri[1][0] = 32; islem_sureleri[1][1] = 48; islem_sureleri[1][2] = 35; islem_sureleri[1][3] = 42;
    islem_sureleri[2][0] = 55; islem_sureleri[2][1] = 42; islem_sureleri[2][2] = 38; islem_sureleri[2][3] = 45;
    islem_sureleri[3][0] = 38; islem_sureleri[3][1] = 50; islem_sureleri[3][2] = 45; islem_sureleri[3][3] = 35;
    islem_sureleri[4][0] = 42; islem_sureleri[4][1] = 35; islem_sureleri[4][2] = 48; islem_sureleri[4][3] = 40;
    islem_sureleri[5][0] = 48; islem_sureleri[5][1] = 42; islem_sureleri[5][2] = 35; islem_sureleri[5][3] = 38;
    
    // Daha gerçekçi geçiş maliyetleri (dakika cinsinden)
    double** gecis_maliyetleri = (double**)malloc(makine_sayisi * sizeof(double*));
    if (gecis_maliyetleri == NULL) {
        printf("Bellek ayırma hatası!\n");
        for (int i = 0; i < is_sayisi; i++) {
            free(islem_sureleri[i]);
        }
        free(islem_sureleri);
        cizelgeleyici_temizle(cizelgeleyici);
        return 1;
    }
    
    for (int i = 0; i < makine_sayisi; i++) {
        gecis_maliyetleri[i] = (double*)malloc(makine_sayisi * sizeof(double));
        if (gecis_maliyetleri[i] == NULL) {
            printf("Bellek ayırma hatası!\n");
            for (int j = 0; j < i; j++) {
                free(gecis_maliyetleri[j]);
            }
            free(gecis_maliyetleri);
            for (int j = 0; j < is_sayisi; j++) {
                free(islem_sureleri[j]);
            }
            free(islem_sureleri);
            cizelgeleyici_temizle(cizelgeleyici);
            return 1;
        }
    }
    
    // Geçiş maliyetlerini ayarla
    gecis_maliyetleri[0][0] = 0;  gecis_maliyetleri[0][1] = 15; gecis_maliyetleri[0][2] = 20; gecis_maliyetleri[0][3] = 12;
    gecis_maliyetleri[1][0] = 15; gecis_maliyetleri[1][1] = 0;  gecis_maliyetleri[1][2] = 18; gecis_maliyetleri[1][3] = 10;
    gecis_maliyetleri[2][0] = 20; gecis_maliyetleri[2][1] = 18; gecis_maliyetleri[2][2] = 0;  gecis_maliyetleri[2][3] = 15;
    gecis_maliyetleri[3][0] = 12; gecis_maliyetleri[3][1] = 10; gecis_maliyetleri[3][2] = 15; gecis_maliyetleri[3][3] = 0;
    
    islem_surelerini_ayarla(cizelgeleyici, islem_sureleri);
    gecis_maliyetlerini_ayarla(cizelgeleyici, gecis_maliyetleri);
    
    printf("\n===========================================");
    printf("\n           GIRIS VERILERI");
    printf("\n===========================================");
    printf("\nIs Sayisi: %d", is_sayisi);
    printf("\nMakine Sayisi: %d", makine_sayisi);
    
    printf("\n\n-------------------------------------------");
    printf("\n           ISLEM SURELERI (dakika)");
    printf("\n-------------------------------------------");
    for (int i = 0; i < is_sayisi; i++) {
        printf("\nIs %d: ", i+1);
        for (int j = 0; j < makine_sayisi; j++) {
            printf("%.1f ", islem_sureleri[i][j]);
        }
    }
    
    printf("\n\n-------------------------------------------");
    printf("\n           GECIS MALIYETLERI (dakika)");
    printf("\n-------------------------------------------");
    for (int i = 0; i < makine_sayisi; i++) {
        printf("\nMakine %d: ", i+1);
        for (int j = 0; j < makine_sayisi; j++) {
            printf("%.1f ", gecis_maliyetleri[i][j]);
        }
    }
    printf("\n===========================================\n");
    
    clock_t baslangic = clock();
    cozumu_yazdir(cizelgeleyici);
    clock_t bitis = clock();
    
    double hesaplama_suresi = (double)(bitis - baslangic) / CLOCKS_PER_SEC;
    printf("Hesaplama suresi: %.6f saniye\n", hesaplama_suresi);
    
    // Belleği temizle
    for (int i = 0; i < is_sayisi; i++) {
        free(islem_sureleri[i]);
    }
    free(islem_sureleri);
    
    for (int i = 0; i < makine_sayisi; i++) {
        free(gecis_maliyetleri[i]);
    }
    free(gecis_maliyetleri);
    
    cizelgeleyici_temizle(cizelgeleyici);
    return 0;
} 