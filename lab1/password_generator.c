#include <stdio.h>
#include <stdint.h>
#include <locale.h>
#include <wchar.h>

#define BUF_SIZE 100
#define M 8

struct unicode_range
{
	wchar_t start;
	wchar_t stop;
};

static const struct unicode_range group_1_3 = {L'0', L'9'};
static const struct unicode_range group_4_5 = {L'!', L'*'};
static const struct unicode_range group_6_7 = {L'А', L'Я'};
static const struct unicode_range group_8   = {L'а', L'я'};

uint64_t hash_function(wchar_t *str) {
	uint64_t hash = 0;
	while (*str) {
		hash = hash * 31 + *str;
		str++;
	}
	return hash;
}

wchar_t generate_symbol(uint64_t hash_id, int group_type) {
	static uint64_t state = 0;
	struct unicode_range range;
	
	switch (group_type) {
		case 1: // 0-9
			range = group_1_3;
			break;
		case 2: // !-*
			range = group_4_5;
			break;
		case 3: // А-Я
			range = group_6_7;
			break;
		case 4: // а-я
			range = group_8;
			break;
		default:
			range.start = L'A';
			range.stop = L'Z';
	}
	
	if (state == 0) state = hash_id;
	state = state * 6364136223846793005ULL + 1442695040888963407ULL;

	wchar_t range_size = range.stop - range.start + 1;
	return range.start + (state % range_size);
}

uint64_t compute_P(uint64_t N) {
	uint64_t N_squared_mod = (N * N) % 15;
	uint64_t N_cubed_mod = (N * N * N) % 15;
	return N_squared_mod + N_cubed_mod + 1;
}

wchar_t generate_last_symbol(size_t N) {
	uint64_t digit;
	
	digit = compute_P(N);
	digit = digit % (group_8.stop - group_8.start);
	return group_8.start + (wchar_t)digit;

}

int main() {
	wchar_t buf[BUF_SIZE];
	wchar_t password[M + 1];
	uint64_t hash_id = 0;

	setlocale(LC_ALL, "ru_RU.UTF-8");

	wprintf(L"Введите идентификатор пользователя\n");
	wscanf(L"%99ls", buf);
	hash_id = hash_function(buf);

	for(int i = 0; i < 3; i++)
		password[i] = generate_symbol(hash_id, 1);
	for(int i = 3; i < 5; i++)
		password[i] = generate_symbol(hash_id, 2);
	for(int i = 5; i < 7; i++)
		password[i] = generate_symbol(hash_id, 3);
	password[7] = generate_last_symbol(strlen(buf));
	password[8] = L'\0';

	wprintf(L"Ваш пароль: %ls\n", password);
	return 0;
}
