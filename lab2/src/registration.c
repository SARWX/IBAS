#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include "user.h"

// Макрос для безопасного ввода строк
#define INPUT_FIELD(field, prompt) \
    do { \
        printf(prompt); \
        fgets(field, MAX_LENGTH, stdin); \
        field[strcspn(field, "\n")] = '\0'; \
    } while (0)

// Макрос для ввода с проверкой
#define INPUT_VALIDATED(field, prompt, validation_func, error_msg) \
    do { \
        do { \
            printf(prompt); \
            fgets(field, MAX_LENGTH, stdin); \
            field[strcspn(field, "\n")] = '\0'; \
            if (!validation_func(field)) { \
                printf(error_msg); \
            } \
        } while (!validation_func(field)); \
    } while (0)

// Функция для проверки номера телефона
static int is_valid_phone(const char *phone) {
    if (strlen(phone) != 11) return 0;
    for (int i = 0; i < 11; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

// Функция для проверки даты
static int is_valid_date(const char *date) {
    if (strlen(date) != 10) return 0;
    if (date[2] != '.' || date[5] != '.') return 0;
    
    // Проверяем, что все символы кроме точек - цифры
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && !isdigit(date[i])) {
            return 0;
        }
    }
    return 1;
}

// Функция для вывода информации о пользователе
void print_user(const User *user) {
    printf("\n=== ИНФОРМАЦИЯ О ПОЛЬЗОВАТЕЛЕ ===\n");
    printf("Username: %s\n", user->username);
    printf("ФИО: %s %s %s\n", user->last_name, user->first_name, user->middle_name);
    printf("Дата рождения: %s\n", user->birth_date);
    printf("Место рождения: %s\n", user->birth_place);
    printf("Телефон: %s\n", user->phone_number);
}

// REGISTRATION
User registration_get_data() {
    User user;
    printf("=== РЕГИСТРАЦИЯ ПОЛЬЗОВАТЕЛЯ ===\n");
    INPUT_FIELD(user.username, "Введите username: ");
    INPUT_FIELD(user.password, "Введите пароль: ");
    INPUT_FIELD(user.first_name, "Введите имя: ");
    INPUT_FIELD(user.last_name, "Введите фамилию: ");
    INPUT_FIELD(user.middle_name, "Введите отчество: ");
    INPUT_VALIDATED(user.birth_date, 
                   "Введите дату рождения (дд.мм.гггг): ",
                   is_valid_date, 
                   "Неверный формат даты! Используйте дд.мм.гггг\n");
    INPUT_FIELD(user.birth_place, "Введите место рождения: ");
    INPUT_VALIDATED(user.phone_number,
                   "Введите номер телефона (11 цифр, например 89501864620): ",
                   is_valid_phone,
                   "Неверный формат номера! Должно быть 11 цифр\n");
    return user;
}

void registration_put_data(User user) {
    struct stat st = {0};
    if (stat("/tmp/.auth_shell", &st) == -1) {
        mkdir("/tmp/.auth_shell", 0700);
    }
    char filename[256];
    snprintf(filename, sizeof(filename), "/tmp/.auth_shell/%s.user", user.username);
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Ошибка при создании файла пользователя");
        return;
    }
    // fprintf(file, "username: %s\n", user.username);
    fprintf(file, "password: %s\n", user.password);
    fprintf(file, "first_name: %s\n", user.first_name);
    fprintf(file, "last_name: %s\n", user.last_name);
    fprintf(file, "middle_name: %s\n", user.middle_name);
    fprintf(file, "birth_date: %s\n", user.birth_date);
    fprintf(file, "birth_place: %s\n", user.birth_place);
    fprintf(file, "phone_number: %s\n", user.phone_number);
    
    fclose(file);
    printf("Данные пользователя %s успешно сохранены в %s\n", user.username, filename);
}


User registration() {
    User user = registration_get_data();
    registration_put_data(user);
    return(user);
}
