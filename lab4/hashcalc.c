#include <stdio.h>
#include <string.h>

// Параметры для гаммирования
#define A 51
#define B 13
#define C 256
#define T0 102
#define MAX_VAL 255

// Метод контрольных сумм (простая сумма по модулю 256)
unsigned char control_sum_method(const char* message) {
    unsigned int sum = 0;
    int len = strlen(message);
    
    for (int i = 0; i < len; i++) {
        sum += (unsigned char)message[i];
    }
    
    return sum % (MAX_VAL + 1);
}

// Метод хеширования с применением гаммирования
unsigned char hash_with_gamma_method(const char* message) {
    int len = strlen(message);
    unsigned char t = T0;
    unsigned int sum = 0;
    
    for (int i = 0; i < len; i++) {
        t = (A * t + B) % C;
        unsigned char y = message[i] ^ t;
        sum += y;
    }
    return sum % (MAX_VAL + 1);
}

int main() {
    // Тестовые сообщения
    const char* messages[] = {
        "12121212",
        "21212121", 
        "90000009",
        "99000000"
    };
    
    int num_messages = sizeof(messages) / sizeof(messages[0]);
    
    printf("Результаты вычисления контрольных сумм:\n");
    printf("=========================================\n\n");
    
    for (int i = 0; i < num_messages; i++) {
        const char* message = messages[i];
        
        // Вычисляем контрольные суммы обоими методами
        unsigned char ksumm = control_sum_method(message);
        unsigned char summkodbukvotkr = hash_with_gamma_method(message);
        
        printf("Сообщение: '%s'\n", message);
        printf("KSumm (метод контрольных сумм): %u\n", ksumm);
        printf("SummKodBukvOtkr (с гаммированием): %u\n", summkodbukvotkr);
        printf("---\n");
    }
    
    return 0;
}
