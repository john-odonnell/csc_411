#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "assert.h"
#include "except.h"

Except_T Bitpack_Overflow = { "Overflow Packing Bits" };

uint64_t shl(uint64_t word, unsigned bits) {
	assert(bits <= 64);
	if (bits == 64) {
		return 0;
	} else {
		return (word << bits);
	}
}

uint64_t shr(uint64_t word, unsigned bits) {
	if (bits == 64) {
		return 0;
	} else {
		return (word >> bits);
	}
}

int64_t sra(int64_t word, unsigned bits) {
	assert(bits <= 64);
	
	if (bits <= 63) {
		word = (word >> bits);
	} else if (word >= 0) {
		word = 0;
	} else {
		word = ~0;
	}

	return word;
}

bool Bitpack_fitsu(uint64_t n, unsigned width) {
	assert(width <= 64);

	uint64_t limit = ~0;

	limit = shl(limit, width);

	if (n < limit) {
		return true;
	} else {
		return false;
	}
}

bool Bitpack_fitss(int64_t n, unsigned width) {
	assert(width <= 64);

	int64_t limit = ~0;

	limit = shl(limit, width-1);

	if (limit <= n && n <= ~limit) {
		return true;
	} else {
		return false;
	}
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb) {
	assert(width <= 64);

	uint64_t n = ~0;
	n = shl(n, (64-width));
	n = shr(n, (64-width-lsb));

	word = (word & n);
	word = shr(word, lsb);

	return word;
}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb) {
	assert(width <= 64);

	uint64_t n = ~0;
	n = shl(n, (64-width));
	n = shr(n, (64-width-lsb));

	word = (word & n);
	word = shl(word, (64-width-lsb));
	word = sra(word, (64-width));

	return (uint64_t)word;
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value) {
	assert(width <= 64);

	if (Bitpack_fitsu(value, width)) {
		int64_t n = ~0;
		n = shl(n, (64-width));
		n = shr(n, (64-width-lsb));

		value = shl(value, lsb);
		value = (value & n);

		word = (word & ~n);
		word = (word | value);
	} else {
		RAISE(Bitpack_Overflow);
	}

	return word;
}

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, int64_t value) {
	assert(width <= 64);

	if (Bitpack_fitss(value, width)) {
		int64_t n = ~0;
		n = shl(n, (64-width));
		n = shr(n, (64-width-lsb));

		value = shl(value, lsb);
		value = (value & n);

		word = (word & ~n);
		word = (word | value);
	} else {
		RAISE(Bitpack_Overflow);
	}

	return word;
}
