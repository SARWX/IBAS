#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define DEBUG

#define WORD_EQ(command) (strcmp(word, command) == 0)
static enum command {
        REGISTER,
        WHOAMI,
        AUTH,
        CHANGE_PASSWORD,

        /* add commands above */
        NOT_KNOWN
};

static int registration() {
        return 0;
}

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
                printf("команда %s не известна\n", word);
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
                case 1:
                        registration();
                        break;
                default:
                        break;
        }

        return 0;
}
