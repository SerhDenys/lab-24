#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура для опису квартири
typedef struct {
    char street[50];
    int building;
    int apartment;
    float price;
} Apartment;

// Функція для введення даних про квартири
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

// Функція для оновлення цін квартир
void update_prices(Apartment *apts, int n) {
    printf("\nОновлення цін квартир:\n");
    for (int i = 0; i < n; i++) {
        printf("Квартира %d (вул. %s, буд. %d, кв. %d): ", 
               i+1, apts[i].street, apts[i].building, apts[i].apartment);
        printf("Поточна ціна: %.2f. Введіть нову ціну: ", apts[i].price);
        scanf("%f", &apts[i].price);
    }
}

// Функція для виведення даних про квартири
void print_apartments(Apartment *apts, int n) {
    printf("\nСписок квартир:\n");
    for (int i = 0; i < n; i++) {
        printf("Вулиця: %s, буд. %d, кв. %d, ціна: %.2f\n",
               apts[i].street, apts[i].building, 
               apts[i].apartment, apts[i].price);
    }
}

int main() {
    int n;
    printf("Введіть кількість квартир: ");
    scanf("%d", &n);
    
    // Виділення пам'яті для масиву структур
    Apartment *apartments = (Apartment*)malloc(n * sizeof(Apartment));
    
    // Введення даних про квартири
    input_apartments(apartments, n);
    
    // Відкриття файлу для запису у бінарному режимі
    FILE *file = fopen("apartments.dat", "wb");
    if (file == NULL) {
        printf("Помилка відкриття файлу для запису!\n");
        free(apartments);
        return 1;
    }
    
    // Запис даних у файл за допомогою fwrite
    int written = fwrite(apartments, sizeof(Apartment), n, file);
    if (written != n) {
        printf("Помилка запису у файл!\n");
    }
    fclose(file);
    
    // Оновлення цін квартир
    update_prices(apartments, n);
    
    // Відкриття файлу для оновлення даних
    file = fopen("apartments.dat", "r+b");
    if (file == NULL) {
        printf("Помилка відкриття файлу для оновлення!\n");
        free(apartments);
        return 1;
    }
    
    // Оновлення даних у файлі
    for (int i = 0; i < n; i++) {
        // Перехід до потрібної позиції у файлі
        fseek(file, i * sizeof(Apartment), SEEK_SET);
        
        // Запис оновлених даних
        fwrite(&apartments[i], sizeof(Apartment), 1, file);
    }
    
    // Закриття файлу
    fclose(file);
    
    // Відкриття файлу для читання та перевірки результатів
    file = fopen("apartments.dat", "rb");
    if (file == NULL) {
        printf("Помилка відкриття файлу для читання!\n");
        free(apartments);
        return 1;
    }
    
    // Читання даних з файлу за допомогою fread
    Apartment *read_apartments = (Apartment*)malloc(n * sizeof(Apartment));
    int read = fread(read_apartments, sizeof(Apartment), n, file);
    if (read != n) {
        printf("Помилка читання з файлу!\n");
    }
    fclose(file);
    
    // Виведення оновлених даних
    printf("\nОновлені дані з файлу:\n");
    print_apartments(read_apartments, n);
    
    // Звільнення пам'яті
    free(apartments);
    free(read_apartments);
    
    return 0;
}