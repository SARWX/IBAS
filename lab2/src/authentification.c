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
#include "user.h"
#include "registration.h"

#define WORD_EQ(command) (strcmp(word, command) == 0)
static enum command {
        REGISTER,
        WHOAMI,
        AUTH,
        CHANGE_PASSWORD,

        /* add commands above */
        NOT_KNOWN
};

static int word_to_command(char *word) {
        int ret = 0;
        if (WORD_EQ("register"))
                ret = REGISTER;
        else if (WORD_EQ("whoami"))
                ret = WHOAMI;
        else if (WORD_EQ("auth"))
                ret = AUTH;
        else if (WORD_EQ("change_password"))
                ret = CHANGE_PASSWORD;
        else {
                ret = NOT_KNOWN;
        }
        return ret;
}

int main(int argc, char *argv[])
{
#ifdef DEBUG
        for(int i = 0; i < argc; i++) 
                printf("%d argument: %s\n", i, argv[i]);
#endif
        switch (word_to_command(argv[1])) {
                case REGISTER:
                        User user = registration();
                        print_user(&user);
                        break;
                case WHOAMI:
                        // whoami();
                        break;
                case AUTH:
                        // auth();
                        break;
                case CHANGE_PASSWORD:
                        // change_password();
                        break;
                default:        // NOT_KNOWN
                        printf("команда %s не известна\n", argv[1]);
                        break;
        }
        return 0;
}
