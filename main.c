#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_OGRENCI 1000
#define DOSYA_ADI "ogrenciler.dat"

typedef struct {
    char numara[20];
    char ad[50];
    char soyad[50];
    int yas;
    char bolum[50];
    int sinif;
    char email[100];
    char telefon[20];
    double gano;
} Ogrenci;

Ogrenci ogrenciler[MAX_OGRENCI];
int ogrenciSayisi = 0;

// =====================================================
// YARDIMCI FONKSIYONLAR
// =====================================================

void satirTemizle(char *metin) {
    metin[strcspn(metin, "\n")] = '\0';
}

void metinOku(const char *mesaj, char *hedef, int boyut) {
    printf("%s", mesaj);

    if (fgets(hedef, boyut, stdin) != NULL) {
        satirTemizle(hedef);
    }
}

int sayiOku(const char *mesaj) {
    char buffer[100];
    int sayi;

    while (1) {
        printf("%s", mesaj);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return 0;
        }

        if (sscanf(buffer, "%d", &sayi) == 1) {
            return sayi;
        }

        printf("Hatali giris! Lutfen sayi giriniz.\n");
    }
}

double ondalikOku(const char *mesaj) {
    char buffer[100];
    double sayi;

    while (1) {
        printf("%s", mesaj);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return 0.0;
        }

        // Türkçe virgül girişini noktaya çevir
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (buffer[i] == ',') {
                buffer[i] = '.';
            }
        }

        if (sscanf(buffer, "%lf", &sayi) == 1 && sayi >= 0 && sayi <= 4.0) {
            return sayi;
        }

        printf("Hatali GANO! 0 ile 4 arasinda bir deger giriniz.\n");
    }
}

void bekle(void) {
    printf("\nDevam etmek icin Enter'a basin...");
    getchar();
}

void cizgi(void) {
    printf("------------------------------------------------------------\n");
}

// =====================================================
// DOSYA ISLEMLERI
// =====================================================

void verileriKaydet(void) {
    FILE *dosya = fopen(DOSYA_ADI, "wb");

    if (dosya == NULL) {
        printf("Veri dosyasi kaydedilemedi!\n");
        return;
    }

    fwrite(&ogrenciSayisi, sizeof(int), 1, dosya);
    fwrite(ogrenciler, sizeof(Ogrenci), ogrenciSayisi, dosya);

    fclose(dosya);
}

void verileriYukle(void) {
    FILE *dosya = fopen(DOSYA_ADI, "rb");

    if (dosya == NULL) {
        ogrenciSayisi = 0;
        return;
    }

    if (fread(&ogrenciSayisi, sizeof(int), 1, dosya) != 1) {
        ogrenciSayisi = 0;
        fclose(dosya);
        return;
    }

    if (ogrenciSayisi < 0 || ogrenciSayisi > MAX_OGRENCI) {
        ogrenciSayisi = 0;
        fclose(dosya);
        return;
    }

    fread(ogrenciler, sizeof(Ogrenci), ogrenciSayisi, dosya);

    fclose(dosya);
}

// =====================================================
// OGRENCI BULMA
// =====================================================

int numarayaGoreBul(const char *numara) {
    for (int i = 0; i < ogrenciSayisi; i++) {
        if (strcmp(ogrenciler[i].numara, numara) == 0) {
            return i;
        }
    }

    return -1;
}

// =====================================================
// OGRENCI BILGILERI
// =====================================================

void ogrenciBilgisiGoster(Ogrenci ogr) {
    cizgi();
    printf("Ogrenci No : %s\n", ogr.numara);
    printf("Ad         : %s\n", ogr.ad);
    printf("Soyad      : %s\n", ogr.soyad);
    printf("Yas        : %d\n", ogr.yas);
    printf("Bolum      : %s\n", ogr.bolum);
    printf("Sinif      : %d\n", ogr.sinif);
    printf("E-posta   : %s\n", ogr.email);
    printf("Telefon    : %s\n", ogr.telefon);
    printf("GANO       : %.2f\n", ogr.gano);
    cizgi();
}

// =====================================================
// OGRENCI EKLE
// =====================================================

void ogrenciEkle(void) {
    if (ogrenciSayisi >= MAX_OGRENCI) {
        printf("\nKayit kapasitesi doldu!\n");
        return;
    }

    Ogrenci yeni;

    printf("\n============== OGRENCI EKLE ==============\n");

    metinOku("Ogrenci numarasi: ", yeni.numara, sizeof(yeni.numara));

    if (numarayaGoreBul(yeni.numara) != -1) {
        printf("\nBu numara zaten kayitli!\n");
        return;
    }

    metinOku("Ad: ", yeni.ad, sizeof(yeni.ad));
    metinOku("Soyad: ", yeni.soyad, sizeof(yeni.soyad));

    yeni.yas = sayiOku("Yas: ");

    metinOku("Bolum: ", yeni.bolum, sizeof(yeni.bolum));

    yeni.sinif = sayiOku("Sinif (1-4): ");

    metinOku("E-posta: ", yeni.email, sizeof(yeni.email));
    metinOku("Telefon: ", yeni.telefon, sizeof(yeni.telefon));

    yeni.gano = ondalikOku("GANO (0-4): ");

    ogrenciler[ogrenciSayisi] = yeni;
    ogrenciSayisi++;

    verileriKaydet();

    printf("\nOgrenci basariyla kaydedildi!\n");
}

// =====================================================
// OGRENCILERI LISTELE
// =====================================================

void ogrencileriListele(void) {
    printf("\n================ OGRENCI LISTESI ================\n");

    if (ogrenciSayisi == 0) {
        printf("Kayitli ogrenci bulunmuyor.\n");
        return;
    }

    printf("%-15s %-15s %-15s %-5s %-20s %-5s %-5s\n",
           "NUMARA", "AD", "SOYAD", "YAS", "BOLUM", "SINIF", "GANO");

    cizgi();

    for (int i = 0; i < ogrenciSayisi; i++) {
        printf("%-15s %-15s %-15s %-5d %-20s %-5d %-5.2f\n",
               ogrenciler[i].numara,
               ogrenciler[i].ad,
               ogrenciler[i].soyad,
               ogrenciler[i].yas,
               ogrenciler[i].bolum,
               ogrenciler[i].sinif,
               ogrenciler[i].gano);
    }

    cizgi();
    printf("Toplam kayit: %d\n", ogrenciSayisi);
}

// =====================================================
// OGRENCI ARA
// =====================================================

void numarayaGoreAra(void) {
    char numara[20];

    printf("\n============= NUMARAYA GORE ARAMA =============\n");

    metinOku("Ogrenci numarasi: ", numara, sizeof(numara));

    int index = numarayaGoreBul(numara);

    if (index == -1) {
        printf("\nOgrenci bulunamadi.\n");
        return;
    }

    printf("\nOgrenci bulundu!\n");
    ogrenciBilgisiGoster(ogrenciler[index]);
}

void ismeGoreAra(void) {
    char aranan[50];
    int bulundu = 0;

    printf("\n================ ISME GORE ARAMA ================\n");

    metinOku("Aranacak ad: ", aranan, sizeof(aranan));

    for (int i = 0; i < ogrenciSayisi; i++) {
        if (strstr(ogrenciler[i].ad, aranan) != NULL) {
            ogrenciBilgisiGoster(ogrenciler[i]);
            bulundu = 1;
        }
    }

    if (!bulundu) {
        printf("\nEslesen ogrenci bulunamadi.\n");
    }
}

void ogrenciAra(void) {
    printf("\n=============== OGRENCI ARAMA ===============\n");
    printf("1. Numara ile ara\n");
    printf("2. Isim ile ara\n");
    printf("0. Geri\n");

    int secim = sayiOku("Seciminiz: ");

    switch (secim) {
        case 1:
            numarayaGoreAra();
            break;

        case 2:
            ismeGoreAra();
            break;

        case 0:
            return;

        default:
            printf("Gecersiz secim!\n");
    }
}

// =====================================================
// OGRENCI GUNCELLE
// =====================================================

void ogrenciGuncelle(void) {
    char numara[20];

    printf("\n============== OGRENCI GUNCELLE ==============\n");

    metinOku("Guncellenecek ogrenci numarasi: ", numara, sizeof(numara));

    int index = numarayaGoreBul(numara);

    if (index == -1) {
        printf("\nOgrenci bulunamadi!\n");
        return;
    }

    printf("\nMevcut bilgiler:\n");
    ogrenciBilgisiGoster(ogrenciler[index]);

    printf("\nYeni bilgileri giriniz:\n");

    metinOku("Yeni ad: ", ogrenciler[index].ad, sizeof(ogrenciler[index].ad));
    metinOku("Yeni soyad: ", ogrenciler[index].soyad, sizeof(ogrenciler[index].soyad));

    ogrenciler[index].yas = sayiOku("Yeni yas: ");

    metinOku("Yeni bolum: ", ogrenciler[index].bolum, sizeof(ogrenciler[index].bolum));

    ogrenciler[index].sinif = sayiOku("Yeni sinif: ");

    metinOku("Yeni e-posta: ", ogrenciler[index].email, sizeof(ogrenciler[index].email));
    metinOku("Yeni telefon: ", ogrenciler[index].telefon, sizeof(ogrenciler[index].telefon));

    ogrenciler[index].gano = ondalikOku("Yeni GANO: ");

    verileriKaydet();

    printf("\nOgrenci bilgileri guncellendi!\n");
}

// =====================================================
// OGRENCI SIL
// =====================================================

void ogrenciSil(void) {
    char numara[20];

    printf("\n================ OGRENCI SIL ================ \n");

    metinOku("Silinecek ogrenci numarasi: ", numara, sizeof(numara));

    int index = numarayaGoreBul(numara);

    if (index == -1) {
        printf("\nOgrenci bulunamadi!\n");
        return;
    }

    printf("\nSilinecek ogrenci:\n");
    ogrenciBilgisiGoster(ogrenciler[index]);

    char onay[10];
    metinOku("Silmek istiyor musunuz? (E/H): ", onay, sizeof(onay));

    if (toupper((unsigned char)onay[0]) != 'E') {
        printf("Silme islemi iptal edildi.\n");
        return;
    }

    for (int i = index; i < ogrenciSayisi - 1; i++) {
        ogrenciler[i] = ogrenciler[i + 1];
    }

    ogrenciSayisi--;

    verileriKaydet();

    printf("\nOgrenci silindi!\n");
}

// =====================================================
// FILTRELEME
// =====================================================

void bolumeGoreFiltrele(void) {
    char bolum[50];
    int bulundu = 0;

    printf("\n============= BOLUM FILTRESI =============\n");

    metinOku("Bolum adi: ", bolum, sizeof(bolum));

    for (int i = 0; i < ogrenciSayisi; i++) {
        if (strcmp(ogrenciler[i].bolum, bolum) == 0) {
            ogrenciBilgisiGoster(ogrenciler[i]);
            bulundu = 1;
        }
    }

    if (!bulundu) {
        printf("\nBu bolumde ogrenci bulunamadi.\n");
    }
}

void sinifaGoreFiltrele(void) {
    int sinif;
    int bulundu = 0;

    printf("\n============= SINIF FILTRESI =============\n");

    sinif = sayiOku("Sinif (1-4): ");

    for (int i = 0; i < ogrenciSayisi; i++) {
        if (ogrenciler[i].sinif == sinif) {
            ogrenciBilgisiGoster(ogrenciler[i]);
            bulundu = 1;
        }
    }

    if (!bulundu) {
        printf("\nBu sinifta ogrenci bulunamadi.\n");
    }
}

void filtreMenu(void) {
    printf("\n=============== FILTRELE ===============\n");
    printf("1. Bolume gore filtrele\n");
    printf("2. Sinifa gore filtrele\n");
    printf("0. Geri\n");

    int secim = sayiOku("Seciminiz: ");

    switch (secim) {
        case 1:
            bolumeGoreFiltrele();
            break;

        case 2:
            sinifaGoreFiltrele();
            break;

        case 0:
            return;

        default:
            printf("Gecersiz secim!\n");
    }
}

// =====================================================
// SIRALAMA
// =====================================================

void ganoSirala(void) {
    if (ogrenciSayisi < 2) {
        printf("\nSiralamak icin yeterli ogrenci yok.\n");
        return;
    }

    for (int i = 0; i < ogrenciSayisi - 1; i++) {
        for (int j = 0; j < ogrenciSayisi - i - 1; j++) {
            if (ogrenciler[j].gano < ogrenciler[j + 1].gano) {
                Ogrenci temp = ogrenciler[j];
                ogrenciler[j] = ogrenciler[j + 1];
                ogrenciler[j + 1] = temp;
            }
        }
    }

    printf("\nOgrenciler GANO'ya gore siralandi (Yuksekten dusuge).\n");
    ogrencileriListele();
}

// =====================================================
// ISTATISTIK
// =====================================================

void istatistikGoster(void) {
    if (ogrenciSayisi == 0) {
        printf("\nIstatistik icin kayitli ogrenci bulunmuyor.\n");
        return;
    }

    double toplamGano = 0;
    double enYuksek = ogrenciler[0].gano;
    double enDusuk = ogrenciler[0].gano;

    int toplamYas = 0;

    int basarili = 0;
    int akademikUyari = 0;

    for (int i = 0; i < ogrenciSayisi; i++) {
        toplamGano += ogrenciler[i].gano;
        toplamYas += ogrenciler[i].yas;

        if (ogrenciler[i].gano > enYuksek) {
            enYuksek = ogrenciler[i].gano;
        }

        if (ogrenciler[i].gano < enDusuk) {
            enDusuk = ogrenciler[i].gano;
        }

        if (ogrenciler[i].gano >= 2.0) {
            basarili++;
        } else {
            akademikUyari++;
        }
    }

    printf("\n================ ISTATISTIKLER ================ \n");
    cizgi();

    printf("Toplam ogrenci       : %d\n", ogrenciSayisi);
    printf("Ortalama GANO        : %.2f\n", toplamGano / ogrenciSayisi);
    printf("En yuksek GANO       : %.2f\n", enYuksek);
    printf("En dusuk GANO        : %.2f\n", enDusuk);
    printf("Ortalama yas         : %.1f\n", (double)toplamYas / ogrenciSayisi);
    printf("Basarili ogrenci     : %d\n", basarili);
    printf("Akademik uyari       : %d\n", akademikUyari);

    cizgi();
}

// =====================================================
// CSV DISARI AKTAR
// =====================================================

void csvAktar(void) {
    FILE *dosya = fopen("ogrenci_raporu.csv", "w");

    if (dosya == NULL) {
        printf("\nCSV dosyasi olusturulamadi!\n");
        return;
    }

    fprintf(dosya, "Numara,Ad,Soyad,Yas,Bolum,Sinif,Eposta,Telefon,GANO\n");

    for (int i = 0; i < ogrenciSayisi; i++) {
        fprintf(dosya, "%s,%s,%s,%d,%s,%d,%s,%s,%.2f\n",
                ogrenciler[i].numara,
                ogrenciler[i].ad,
                ogrenciler[i].soyad,
                ogrenciler[i].yas,
                ogrenciler[i].bolum,
                ogrenciler[i].sinif,
                ogrenciler[i].email,
                ogrenciler[i].telefon,
                ogrenciler[i].gano);
    }

    fclose(dosya);

    printf("\nRapor olusturuldu: ogrenci_raporu.csv\n");
}

// =====================================================
// MENULER
// =====================================================

void ogrenciMenu(void) {
    int secim;

    do {
        printf("\n");
        printf("=============== OGRENCI ISLEMLERI ===============\n");
        printf("1. Ogrenci Ekle\n");
        printf("2. Ogrencileri Listele\n");
        printf("3. Ogrenci Ara\n");
        printf("4. Ogrenci Guncelle\n");
        printf("5. Ogrenci Sil\n");
        printf("6. Ogrenci Filtrele\n");
        printf("7. GANO'ya Gore Sirala\n");
        printf("8. CSV Raporu Olustur\n");
        printf("0. Ana Menuye Don\n");
        cizgi();

        secim = sayiOku("Seciminiz: ");

        switch (secim) {
            case 1:
                ogrenciEkle();
                break;

            case 2:
                ogrencileriListele();
                break;

            case 3:
                ogrenciAra();
                break;

            case 4:
                ogrenciGuncelle();
                break;

            case 5:
                ogrenciSil();
                break;

            case 6:
                filtreMenu();
                break;

            case 7:
                ganoSirala();
                break;

            case 8:
                csvAktar();
                break;

            case 0:
                break;

            default:
                printf("\nGecersiz secim!\n");
        }

        if (secim != 0) {
            bekle();
        }

    } while (secim != 0);
}

void anaMenu(void) {
    int secim;

    do {
        printf("\n");
        printf("============================================================\n");
        printf("                 STUDENT MANAGEMENT SYSTEM                    \n");
        printf("                    OGRENCI YONETIMI                         \n");
        printf("============================================================\n");
        printf(" [1] Ogrenci Islemleri\n");
        printf(" [2] Akademik Istatistikler\n");
        printf(" [3] Verileri Kaydet\n");
        printf(" [0] Programdan Cikis\n");
        printf("============================================================\n");

        secim = sayiOku("Seciminiz: ");

        switch (secim) {
            case 1:
                ogrenciMenu();
                break;

            case 2:
                istatistikGoster();
                bekle();
                break;

            case 3:
                verileriKaydet();
                printf("\nVeriler kaydedildi.\n");
                bekle();
                break;

            case 0:
                verileriKaydet();
                printf("\nVeriler kaydedildi. Program kapatiliyor...\n");
                break;

            default:
                printf("\nGecersiz secim!\n");
                bekle();
        }

    } while (secim != 0);
}

// =====================================================
// MAIN
// =====================================================

int main(void) {
    verileriYukle();

    printf("\nSistem baslatiliyor...\n");
    printf("%d ogrenci kaydi yuklendi.\n", ogrenciSayisi);

    anaMenu();

    return 0;
}