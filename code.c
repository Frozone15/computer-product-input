#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int LANG = 0;

#define TXT(id, en) (LANG == 0 ? (id) : (en))

typedef struct Part {
    char kategori[30];
    char nama_produk[50];
    long harga;
    int stok;
    struct Part* next;
} Part;

Part* createNode(char* kat, char* nama, long harga, int stok) {
    Part* newNode = (Part*)malloc(sizeof(Part));
    strcpy(newNode->kategori, kat);
    strcpy(newNode->nama_produk, nama);
    newNode->harga = harga;
    newNode->stok = stok;
    newNode->next = NULL;
    return newNode;
}

Part* loadFromFile(Part* head) {
    FILE* file = fopen("gudang.txt", "r");
    if (file == NULL) {
        printf("[!] %s\n", TXT("File gudang.txt tidak ditemukan. Membuat database baru...", 
                               "gudang.txt not found. Creating new database..."));
        return NULL;
    }

    char kat[30], nama[50];
    long harga;
    int stok;

    while (fscanf(file, " %[^,],%[^,],%ld,%d\n", kat, nama, &harga, &stok) != EOF) {
        Part* newNode = createNode(kat, nama, harga, stok);
        if (head == NULL) {
            head = newNode;
        } else {
            Part* temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }
    fclose(file);
    printf("[+] %s\n", TXT("Berhasil memuat data dari gudang.txt", 
                           "Successfully loaded data from gudang.txt"));
    return head;
}

void appendToFile(char* kat, char* nama, long harga, int stok) {
    FILE* file = fopen("gudang.txt", "a");
    if (file == NULL) {
        printf("[!] %s\n", TXT("Gagal membuka file untuk menyimpan data.", 
                               "Failed to open file to save data."));
        return;
    }
    fprintf(file, "%s,%s,%ld,%d\n", kat, nama, harga, stok);
    fclose(file);
}

void saveAllToFile(Part* head) {
    FILE* file = fopen("gudang.txt", "w");
    if (file == NULL) return;

    Part* temp = head;
    while (temp != NULL) {
        fprintf(file, "%s,%s,%ld,%d\n", temp->kategori, temp->nama_produk, temp->harga, temp->stok);
        temp = temp->next;
    }
    fclose(file);
}

void displayList(Part* head) {
    if (head == NULL) {
        printf("\n=== %s ===\n", TXT("GUDANG KOSONG", "INVENTORY EMPTY"));
        return;
    }
    printf("\n=========================================================================\n");
    printf("%-15s | %-25s | %-12s | %-5s\n", 
           TXT("KATEGORI", "CATEGORY"), 
           TXT("NAMA PRODUK", "PRODUCT NAME"), 
           TXT("HARGA (Rp)", "PRICE (Rp)"), 
           TXT("STOK", "STOCK"));
    printf("=========================================================================\n");
    
    Part* temp = head;
    while (temp != NULL) {
        printf("%-15s | %-25s | Rp%-10ld | %-5d\n", temp->kategori, temp->nama_produk, temp->harga, temp->stok);
        temp = temp->next;
    }
    printf("=========================================================================\n");
}

Part* addPart(Part* head) {
    char kat[30], nama[50];
    long harga;
    int stok;

    printf("\n--- %s ---\n", TXT("TAMBAH BARANG BARU", "ADD NEW PART"));
    printf("%s : ", TXT("Masukkan Kategori (CPU/GPU/RAM)", "Enter Category (CPU/GPU/RAM)"));
    scanf(" %[^\n]", kat);
    printf("%s : ", TXT("Masukkan Nama Produk", "Enter Product Name"));
    scanf(" %[^\n]", nama);
    printf("%s : ", TXT("Masukkan Harga (Rupiah)", "Enter Price (IDR)"));
    scanf("%ld", &harga);
    printf("%s : ", TXT("Masukkan Jumlah Stok", "Enter Stock Quantity"));
    scanf("%d", &stok);

    Part* newNode = createNode(kat, nama, harga, stok);

    if (head == NULL) {
        head = newNode;
    } else {
        Part* temp = head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }

    appendToFile(kat, nama, harga, stok);
    printf("[+] %s\n", TXT("Barang berhasil ditambahkan dan disimpan!", 
                           "Part successfully added and saved!"));
    return head;
}

void sortInventory(Part* head, int pilihanSort) {
    if (head == NULL || head->next == NULL) return;

    int swapped;
    Part* ptr1;
    Part* lptr = NULL;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            int kondisiTukar = 0;

            if (pilihanSort == 1) {
                if (ptr1->harga > ptr1->next->harga) kondisiTukar = 1;
            } else if (pilihanSort == 2) {
                if (ptr1->stok > ptr1->next->stok) kondisiTukar = 1;
            }

            if (kondisiTukar) {
                char tKat[30], tNama[50];
                strcpy(tKat, ptr1->kategori); strcpy(ptr1->kategori, ptr1->next->kategori); strcpy(ptr1->next->kategori, tKat);
                strcpy(tNama, ptr1->nama_produk); strcpy(ptr1->nama_produk, ptr1->next->nama_produk); strcpy(ptr1->next->nama_produk, tNama);

                long tHarga = ptr1->harga; ptr1->harga = ptr1->next->harga; ptr1->next->harga = tHarga;
                int tStok = ptr1->stok; ptr1->stok = ptr1->next->stok; ptr1->next->stok = tStok;

                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);

    printf("[+] %s\n", TXT("Inventaris berhasil di-sortir!", "Inventory successfully sorted!"));
    displayList(head);
}

void updateStok(Part* head) {
    char namaCari[50];
    int statusKetemu = 0, stokBaru;

    printf("\n--- %s ---\n", TXT("UPDATE STOK BARANG", "UPDATE STOCK QUANTITY"));
    printf("%s: ", TXT("Masukkan Nama Produk", "Enter Product Name"));
    scanf(" %[^\n]", namaCari);

    Part* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->nama_produk, namaCari) == 0) {
            statusKetemu = 1;
            printf(TXT("Produk Ditemukan Stok saat ini: %d\n", "Product Found! Current stock: %d\n"), temp->stok);
            printf("%s : ", TXT("Masukkan Jumlah Stok Baru", "Enter New Stock Quantity"));
            scanf("%d", &stokBaru);
            
            temp->stok = stokBaru;
            saveAllToFile(head);
            printf("[+] %s\n", TXT("Stok berhasil diperbarui di database!", "Stock updated in database!"));
            break;
        }
        temp = temp->next;
    }

    if (!statusKetemu) {
        printf("[!] %s\n", TXT("Produk tidak ditemukan.", "Product not found."));
    }
}

void freeList(Part* head) {
    Part* temp;
    while (head != NULL) {
        temp = head; head = head->next; free(temp);
    }
}

int main() {
    Part* inventoryHead = NULL;
    int pilihan;

    printf("======================================\n");
    printf("  SELECT LANGUAGE / PILIH BAHASA\n");
    printf("======================================\n");
    printf("1. Bahasa Indonesia\n");
    printf("2. English\n");
    printf("-> ");
    int awalLang;
    scanf("%d", &awalLang);
    LANG = (awalLang == 2) ? 1 : 0;

    inventoryHead = loadFromFile(inventoryHead);

    do {
        printf("\n===== %s =====\n", TXT("SISTEM INVENTARIS TOKO KOMPUTER", "PC STORE INVENTORY SYSTEM"));
        printf("%s\n", TXT("1. Lihat Semua Inventaris Barang", "1. View All Inventory"));
        printf("%s\n", TXT("2. Tambah Barang Baru", "2. Add New Part"));
        printf("%s\n", TXT("3. Urutkan Harga Termurah", "3. Sort by Lowest Price"));
        printf("%s\n", TXT("4. Urutkan Sisa Stok Tersedikit", "4. Sort by Lowest Stock"));
        printf("%s\n", TXT("5. Update Jumlah Stok", "5. Update Stock Quantity"));
        printf("%s\n", TXT("6. Ganti interface ke Bahasa Inggris", "6. Switch interface to Bahasa Indonesia"));
        printf("%s\n", TXT("7. Keluar Program", "7. Exit Program"));
        
        printf("%s", TXT("Pilih Menu (1-7): ", "Select Menu (1-7): "));
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1: displayList(inventoryHead); break;
            case 2: inventoryHead = addPart(inventoryHead); break;
            case 3: sortInventory(inventoryHead, 1); break;
            case 4: sortInventory(inventoryHead, 2); break;
            case 5: updateStok(inventoryHead); break;
            case 6:
                LANG = (LANG == 0) ? 1 : 0;
                printf("\n[+] %s\n", TXT("Bahasa diubah ke Indonesia!", "Language switched to English!"));
                break;
            case 7:
                printf("%s\n", TXT("Keluar dari program. Terima kasih!", "Exiting program. Thank you!"));
                break;
            default:
                printf("[!] %s\n", TXT("Pilihan tidak valid!", "Invalid option!"));
        }
    } while (pilihan != 7);

    freeList(inventoryHead);
    return 0;
}
