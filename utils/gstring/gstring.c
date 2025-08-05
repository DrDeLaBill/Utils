/* Copyright © 2024 Georgy E. All rights reserved. */

#include "gstring.h"

#include <stdio.h>
#include <string.h>

#include "gutils.h"


void util_add_char(char* phrase, size_t max_len, char symbol, size_t target_len, ALIGN_MODE mode)
{
	BEDUG_ASSERT(IS_ALIGN_MODE(mode), "Unknown align mode");
	if (!IS_ALIGN_MODE(mode)) {
		mode = ALIGN_MODE_CENTER;
	}

	max_len -= 1;
	size_t len = strlen(phrase);
	size_t need_len = __min(max_len, target_len);
	if (len >= need_len) {
		return;
	}

	size_t symbols_count = need_len - len;
	if (symbols_count % 2) {
		symbols_count--;
	}
	switch (mode) {
	case ALIGN_MODE_LEFT:
		memset((void*)(phrase + len), symbol, symbols_count);
		break;
	case ALIGN_MODE_RIGHT:
		for (size_t i = len; i > 0 ; i--) {
			phrase[i - 1 + symbols_count] = phrase[i - 1];
		}
		memset(phrase, symbol, symbols_count);
		break;
	case ALIGN_MODE_CENTER:
		for (size_t i = len; i > 0 ; i--) {
			phrase[i - 1 + symbols_count / 2] = phrase[i - 1];
		}
		memset(phrase, symbol, symbols_count / 2);
		memset(phrase + symbols_count / 2 + len, symbol, symbols_count - symbols_count / 2);
		break;
	default:
		BEDUG_ASSERT(false, "Unknown align mode");
		return;
	};

	phrase[need_len] = 0;
}

// TODO: point_count = 100 fix
void util_int_to_str_with_point(char* target, unsigned size, int value, unsigned div, unsigned point_count, char point)
{
	snprintf(
		target,
		size - 1,
		"%s%lu",
		value < 0 ? "-" : "",
		(((long unsigned)__abs(value)) / div)
	);
	if (!point_count) {
		return;
	}
	size_t len = strlen(target);
	target[len++] = point;
	target[len]   = 0;
	BEDUG_ASSERT(
		size > len + 1 && div % 10 == 0 && div > 1 && point_count > 0,
		"util_int_to_str_with_point bad size"
	);
	if (size < len + 1) {
		return;
	}
	if (div % 10) {
		target[len++] = '0';
		target[len]   = 0;
		return;
	}
	unsigned dec = util_small_pow(10, point_count);
	value = (int)(((unsigned)__abs(value)) % div);
	if (div > dec) {
		value /= (int)(div / dec);
	}
	char format[16] = "";
	snprintf(
		format,
		sizeof(format) - 1,
		"%c0%uld",
		'%',
		point_count
	);
	snprintf(
		target + len,
		size - len - 1,
		format,
		value
	);
}

char* util_u64_to_str(uint64_t value)
{
	static char str[22] = "";
	memset(str, 0, sizeof(str));
	size_t counter = 0;
	do {
		for (size_t i = counter + 1; i > 0; i--) {
			str[i] = str[i-1];
		}
		str[0] = (uint8_t)(value % 10) + '0';
		value /= 10;
		counter++;
	} while (value);
	return str;
}
