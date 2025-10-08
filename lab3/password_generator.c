/*
 * V = 100 паролей в день
 * T = 10 дней
 * P = 10e-7
 * A^L = 100 * 10 / 10e-7 = 10e10
 * 
 * Пусть алфавит - английские символы (27)
 * тогда log27(10e10) = 7 (с округлением в большую)
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <time.h>

#define BUF_SIZE 100
#define L 7

struct unicode_range
{
	wchar_t start;
	wchar_t stop;
};

static const struct unicode_range basic_english_charset = {L'a', L'z'};

wchar_t generate_symbol(uint64_t hash_id) {
	static uint64_t state = 0;
	struct unicode_range range;

	range = basic_english_charset;
	if (state == 0) state = hash_id;
	state = state * 6364136223846793005ULL + 1442695040888963407ULL;

	wchar_t range_size = range.stop - range.start + 1;
	return range.start + (state % range_size);
}

int main() {
	wchar_t password[L + 1];
	time_t unix_timestamp;

	setlocale(LC_ALL, "ru_RU.UTF-8");
	unix_timestamp = time(NULL);

	for(int i = 0; i < L; i++)
		password[i] = generate_symbol((uint64_t)unix_timestamp);
	password[L] = L'\0';

	wprintf(L"Ваш пароль: %ls\n", password);
	return 0;
}
