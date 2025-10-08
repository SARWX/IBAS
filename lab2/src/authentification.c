/*
 * Задание к лабораторной работе № 2
 * Разработать программу, представляющую собой форму доступа к определённым
 * информационным ресурсам на основе пароля.
 * • В качестве информационного ресурса использовать любой файл или приложение.
 * • Доступ к ресурсу должен быть разрешен только санкционированным пользователям.
 * Для этого в программе должны храниться имена пользователей и их пароли. При 
 * попытке доступа пользователя к ресурсу проверяется наличие его идентификатора
 * (имени) в системе и соответствие введенного пароля паролю, который хранится в
 * системе.
 * • В системе должна храниться следующая информация о пользователе: ID или имя 
 * пользователя, пароль, ФИО, дата рождения, место рождения (город), номер 
 * телефона.
 * • Пользователь должен иметь возможность поменять пароль
 * 
 * Вариант 9
 * Кириллица(прописные буквы)
 * При смене пароля: проверка на совпадение пароля с отчеством пользователя
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/xattr.h>
#include "user.h"
#include "registration.h"

#define CURRENT_USER_FILE "/home/tnovikov/study/iu6/IBAS/lab2/.auth_shell/.current_user"

#define WORD_EQ(command) (strcmp(word, command) == 0)
enum command {
        REGISTER,
        WHOAMI,
        AUTH,
        CHANGE_PASSWORD,
        ALLOWED_COMMAND,
        AUTH_ONLY_COMMAND,

        /* add commands above */
        NOT_KNOWN
};

enum ret_code {
        OK,
        NOT_OK,
        EXECUTE_COMMAND,
        FILE_NOT_ALLOWED,
        NOT_FULL_COMMAND,
};

char *whoami(void);
int auth(void);
void execute_command(const char *cmd, const char *arg);
int is_file_allowed(const char *filename);
int change_password(void);

static enum command word_to_command(char *word) {
        enum command ret = 0;
        if (WORD_EQ("register"))
                ret = REGISTER;
        else if (WORD_EQ("whoami"))
                ret = WHOAMI;
        else if (WORD_EQ("auth"))
                ret = AUTH;
        else if (WORD_EQ("change_password"))
                ret = CHANGE_PASSWORD;
        else if (WORD_EQ("ls"))
                ret = ALLOWED_COMMAND;
        else if (WORD_EQ("cd"))
                ret = ALLOWED_COMMAND;
        else if (WORD_EQ("cat"))
                ret = AUTH_ONLY_COMMAND;
        else if (WORD_EQ("vim"))
                ret = AUTH_ONLY_COMMAND;
        else {
                ret = NOT_KNOWN;
        }
        return ret;
}

int main(int argc, char *argv[])
{
        enum ret_code ret_code = NOT_OK;
        User user;
#ifdef DEBUG
        for(int i = 0; i < argc; i++) 
                printf("%d argument: %s\n", i, argv[i]);
#endif
        switch (word_to_command(argv[1])) {
                case REGISTER:
                        user = registration();
                        print_user(&user);
                        break;
                case WHOAMI:
                        printf("Текущий пользователь: %s\n", whoami());
                        break;
                case AUTH:
                        ret_code = auth();
                        break;
                case CHANGE_PASSWORD:
                        change_password();
                        break;
                case ALLOWED_COMMAND:
                        // execute_command(argv[1], argv[2]);
                        ret_code = EXECUTE_COMMAND;
                        break;
                case AUTH_ONLY_COMMAND:
                        if (argc < 2) {
                            ret_code = NOT_FULL_COMMAND;
                            break;
                        }
                        if (is_file_allowed(argv[2]))
                                ret_code = EXECUTE_COMMAND;
                        else
                                ret_code = FILE_NOT_ALLOWED;
                        break;
                default:        // NOT_KNOWN
                        printf("команда %s не известна\n", argv[1]);
                        break;
        }
        return (int)ret_code;
}

char *whoami(void) {
    FILE *file;
    char *filename = "/home/tnovikov/study/iu6/IBAS/lab2/.auth_shell/.current_user";
    char *username = NULL;
    
    file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }
    
    // Получаем размер файла
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (file_size > 0) {
        // Выделяем память для имени пользователя
        username = (char *)malloc(file_size + 1);
        if (username != NULL) {
            // Читаем содержимое файла
            size_t read_size = fread(username, 1, file_size, file);
            username[read_size] = '\0';
            
            // Убираем символ новой строки, если есть
            char *newline = strchr(username, '\n');
            if (newline != NULL) {
                *newline = '\0';
            }
        }
    }
    
    fclose(file);
    return username;
}

int auth() {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    char filename[256];
    int auth_success = 1;
    int auth_failed = 0;
    
    printf("=== АУТЕНТИФИКАЦИЯ ===\n");
    printf("Введите username: ");
    scanf("%100s", username);
    printf("Введите пароль: ");
    scanf("%100s", password);
    
    snprintf(filename, sizeof(filename), "/home/tnovikov/study/iu6/IBAS/lab2/.auth_shell/%s.user", username);
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Пользователь не найден\n");
        return auth_failed;
    }
    
    char line[256];
    char stored_password[MAX_LENGTH] = {0};
    
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "password: ", 10) == 0) {
            sscanf(line + 10, "%100s", stored_password);
            break;
        }
    }
    fclose(file);
    
    if (strcmp(password, stored_password) == 0) {
        printf("Аутентификация успешна!\n");
        
        // Сохраняем текущего пользователя
        FILE *current = fopen("/home/tnovikov/study/iu6/IBAS/lab2/.auth_shell/.current_user", "w");
        if (current) {
            fprintf(current, "%s\n", username);
            fclose(current);
        }
        return auth_success;
    } else {
        printf("Неверный пароль!\n");
        return auth_failed;
    }
}

int is_file_allowed(const char *filename) {
    char *attr_name;
    char *current_user = whoami();
   
    if (current_user == NULL) {
        fprintf(stderr, "Вы не авторизировались\n");
        return 0;
    }
    
    if (strlen(current_user) == 0) {
        fprintf(stderr, "Имя пользователя пустое\n");
        free(current_user);
        return 0;
    }
    
    // Проверяем существование файла
    if (access(filename, F_OK) != 0) {
        fprintf(stderr, "Файл %s не существует\n", filename);
        free(current_user);
        return 0;
    }

    attr_name = malloc(strlen(current_user) + 6); // "user." + строка + нулевой байт
    if (attr_name == NULL)
        return 1;
    sprintf(attr_name, "user.%s", current_user);
    free(current_user);

    // Проверяем расширенный атрибут
    int result = (getxattr(filename, attr_name, NULL, 0) >= 0);
    
    free(attr_name);
    return result;
}

int change_password() {
    char username[MAX_LENGTH];
    char current_password[MAX_LENGTH];
    char new_password[MAX_LENGTH];
    char confirm_password[MAX_LENGTH];
    char filename[250];
    
    printf("=== СМЕНА ПАРОЛЯ ===\n");
    
    // Получаем текущего пользователя (если он аутентифицирован)
    char *current_user = whoami();
    if (current_user) {
        strncpy(username, current_user, MAX_LENGTH);
        printf("Пользователь: %s\n", username);
    } else {
        printf("Введите username: ");
        scanf("%100s", username);
    }
    
    printf("Введите текущий пароль: ");
    scanf("%100s", current_password);
    
    // Проверяем текущий пароль
    snprintf(filename, sizeof(filename), "/home/tnovikov/study/iu6/IBAS/lab2/.auth_shell/%s.user", username);
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Пользователь не найден\n");
        return 0;
    }
    
    char line[256];
    char stored_password[MAX_LENGTH] = {0};
    char middle_name[MAX_LENGTH] = {0};
    int password_found = 0;
    int middle_name_found = 0;
    
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "password: ", 10) == 0) {
            sscanf(line + 10, "%100s", stored_password);
            password_found = 1;
        } else if (strncmp(line, "middle_name: ", 13) == 0) {
            sscanf(line + 13, "%100s", middle_name);
            middle_name_found = 1;
        }
    }
    fclose(file);
    
    if (!password_found || strcmp(current_password, stored_password) != 0) {
        printf("Неверный текущий пароль!\n");
        return 0;
    }
    
    // Запрашиваем новый пароль
    printf("Введите новый пароль: ");
    scanf("%100s", new_password);
    
    printf("Подтвердите новый пароль: ");
    scanf("%100s", confirm_password);
    
    if (strcmp(new_password, confirm_password) != 0) {
        printf("Пароли не совпадают!\n");
        return 0;
    }
    
    if (strcmp(new_password, current_password) == 0) {
        printf("Новый пароль должен отличаться от текущего!\n");
        return 0;
    }

    if (middle_name_found && strcmp(new_password, middle_name) == 0) {
        printf("Пароль не должен совпадать с отчеством!\n");
        return 0;
    }
    if (strlen(new_password) < 9) {
        printf("Пароль должен быть не меньше 9 символов\n");
        return 0;
    }
        
    
    // Обновляем файл с новым паролем
    char temp_filename[256];
    snprintf(temp_filename, sizeof(temp_filename), "%s.tmp", filename);
    
    FILE *input = fopen(filename, "r");
    FILE *output = fopen(temp_filename, "w");
    
    if (!input || !output) {
        printf("Ошибка при открытии файлов\n");
        if (input) fclose(input);
        if (output) fclose(output);
        return 0;
    }
    
    // Копируем все строки, заменяя пароль
    while (fgets(line, sizeof(line), input)) {
        if (strncmp(line, "password: ", 10) == 0) {
            fprintf(output, "password: %s\n", new_password);
        } else {
            fputs(line, output);
        }
    }
    
    fclose(input);
    fclose(output);
    
    // Заменяем старый файл новым
    if (remove(filename) != 0) {
        printf("Ошибка при удалении старого файла\n");
        remove(temp_filename);
        return 0;
    }
    
    if (rename(temp_filename, filename) != 0) {
        printf("Ошибка при переименовании файла\n");
        return 0;
    }
    
    printf("Пароль успешно изменен!\n");
    return 1;
}
