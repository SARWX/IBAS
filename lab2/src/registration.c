#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "user.h"

#define MAX_LENGTH 100

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
User registration() {
    User user;
    printf("=== РЕГИСТРАЦИЯ ПОЛЬЗОВАТЕЛЯ ===\n");
    INPUT_FIELD(user.username, "Введите username: ");
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
