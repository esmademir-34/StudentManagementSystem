#ifndef STUDENT_H
#define STUDENT_H

#define MAX_OGRENCI 1000

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

extern Ogrenci ogrenciler[MAX_OGRENCI];
extern int ogrenciSayisi;

void ogrenciEkle(void);
void ogrencileriListele(void);
void ogrenciAra(void);
void ogrenciGuncelle(void);
void ogrenciSil(void);
void bolumeGoreFiltrele(void);
void sinifaGoreFiltrele(void);
void ganoSirala(void);
void ogrenciBilgisiGoster(Ogrenci ogr);
int numarayaGoreBul(const char *numara);

#endif