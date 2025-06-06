#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char street[50];
    int building;
    int apartment;
    float price;
} Apartment;

void input_apartments(Apartment *apts, int n) {
    for (int i = 0; i < n; i++) {
        printf("\nКвартира #%d:\n", i+1);
        printf("Вулиця: ");
        scanf("%s", apts[i].street);
        printf("Номер будинку: ");
        scanf("%d", &apts[i].building);
        printf("Номер квартири: ");
        scanf("%d", &apts[i].apartment);
        printf("Вартість: ");
        scanf("%f", &apts[i].price);
    }
}

void update_prices(Apartment *apts, int n) {
    printf("\nОновлення цін квартир:\n");
    for (int i = 0; i < n; i++) {
        printf("Квартира %d (вул. %s, буд. %d, кв. %d): ", 
               i+1, apts[i].street, apts[i].building, apts[i].apartment);
        printf("Поточна ціна: %.2f. Введіть нову ціну: ", apts[i].price);
        scanf("%f", &apts[i].price);
    }
}

void print_apartments(Apartment *apts, int n) {
    printf("\nСписок квартир:\n");
    for (int i = 0; i < n; i++) {
        printf("Вулиця: %s, буд. %d, кв. %d, ціна: %.2f\n",
               apts[i].street, apts[i].building, 
               apts[i].apartment, apts[i].price);
    }
}

int main() {
    system("chcp 65001");
    int n;
    printf("Введіть кількість квартир: ");
    scanf("%d", &n);

    Apartment *apartments = (Apartment*)malloc(n * sizeof(Apartment));

    input_apartments(apartments, n);

    FILE *file = fopen("apartments.dat", "wb");
    if (file == NULL) {
        printf("Помилка відкриття файлу для запису!\n");
        free(apartments);
        return 1;
    }

    int written = fwrite(apartments, sizeof(Apartment), n, file);
    if (written != n) {
        printf("Помилка запису у файл!\n");
    }
    fclose(file);

    update_prices(apartments, n);

    file = fopen("apartments.dat", "r+b");
    if (file == NULL) {
        printf("Помилка відкриття файлу для оновлення!\n");
        free(apartments);
        return 1;
    }

    for (int i = 0; i < n; i++) {

        fseek(file, i * sizeof(Apartment), SEEK_SET);

        fwrite(&apartments[i], sizeof(Apartment), 1, file);
    }

    fclose(file);

    file = fopen("apartments.dat", "rb");
    if (file == NULL) {
        printf("Помилка відкриття файлу для читання!\n");
        free(apartments);
        return 1;
    }
 
    Apartment *read_apartments = (Apartment*)malloc(n * sizeof(Apartment));
    int read = fread(read_apartments, sizeof(Apartment), n, file);
    if (read != n) {
        printf("Помилка читання з файлу!\n");
    }
    fclose(file);

    printf("\nОновлені дані з файлу:\n");
    print_apartments(read_apartments, n);

    free(apartments);
    free(read_apartments);
    
    return 0;
}