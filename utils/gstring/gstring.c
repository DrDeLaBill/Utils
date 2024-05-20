/* Copyright © 2024 Georgy E. All rights reserved. */

#include "gstring.h"

#include <string.h>


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
