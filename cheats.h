/*
Copyright (c) 2012 Guillermo "Tordek" Freschi
Copyright (c) 2013 Sampsa "Tuplanolla" Kiiskinen
Copyright (c) 2021 Clement Foyer

This is free software, and you are welcome to redistribute it
under certain conditions; see the LICENSE file for details.
*/

#ifndef CHEAT_H
	#error "the header file \"cheat.h\" is not available"
#endif

/*
Identifiers starting with CHEATS_ and cheats_ are reserved for internal use.
*/

#ifndef CHEATS_H
#define CHEATS_H

#include <string.h>

/*
These prevent having to compile with -lm since
some assertions make use of fabs() and friends.
*/

#ifndef CHEAT_NO_MATH
#include <math.h>
#endif

#ifdef CHEAT_MODERN
#include <inttypes.h>
#include <stdint.h>
#ifndef CHEAT_NO_MATH
#include <complex.h>
#endif
#endif

/*
This computes an upper bound for the string length of a floating point type.
*/
#define CHEAT_FLOATING_LENGTH(type) \
	(CHEAT_LIMIT) /* This is based on the assumption that
		the preprocessor can convert floating point literals into strings;
		the standard leaves the length unbounded. */

#define CHEAT_GENERATE_INTEGER(name, type, specifier) \
	__attribute__ ((__io__, __nonnull__ (1, 5), __unused__)) \
	static void cheat_check_##name(struct cheat_suite* const suite, \
			bool const negate, \
			type const actual, \
			type const expected, \
			char const* const file, \
			size_t const line) { \
		if (cheat_further(suite->outcome) && (actual == expected) != !negate) { \
			char const* comparator; \
			char* expression; \
			\
			suite->outcome = CHEAT_FAILED; \
			\
			if (negate) \
				comparator = "!="; \
			else \
				comparator = "=="; \
			\
			expression = CHEAT_CAST(char*, cheat_allocate_total(4, \
						CHEAT_INTEGER_LENGTH(actual), strlen(comparator), \
						CHEAT_INTEGER_LENGTH(expected), (size_t )3)); \
			if (expression == NULL) \
				cheat_death("failed to allocate memory", errno); \
			\
			if (cheat_print_string(expression, \
						specifier " %s " specifier, \
						3, actual, comparator, expected) < 0) \
				cheat_death("failed to build a string", errno); \
			\
			cheat_print_failure(suite, expression, file, line); \
		} \
	}

CHEAT_GENERATE_INTEGER(char, char, "%c")
CHEAT_GENERATE_INTEGER(short_int, short int, "%hd")
CHEAT_GENERATE_INTEGER(short_unsigned_int, short unsigned int, "%hu")
CHEAT_GENERATE_INTEGER(int, int, "%d")
CHEAT_GENERATE_INTEGER(unsigned_int, unsigned int, "%u")
CHEAT_GENERATE_INTEGER(long_int, long int, "%ld")
CHEAT_GENERATE_INTEGER(long_unsigned_int, long unsigned int, "%lu")

#define cheat_assert_char(actual, expected) \
	cheat_check_char(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_char(actual, expected) \
	cheat_check_char(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_short_int(actual, expected) \
	cheat_check_short_int(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_short_int(actual, expected) \
	cheat_check_short_int(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_short_unsigned_int(actual, expected) \
	cheat_check_short_unsigned_int(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_short_unsigned_int(actual, expected) \
	cheat_check_short_unsigned_int(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_int(actual, expected) \
	cheat_check_int(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_int(actual, expected) \
	cheat_check_int(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_unsigned_int(actual, expected) \
	cheat_check_unsigned_int(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_unsigned_int(actual, expected) \
	cheat_check_unsigned_int(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_long_int(actual, expected) \
	cheat_check_long_int(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_long_int(actual, expected) \
	cheat_check_long_int(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_long_unsigned_int(actual, expected) \
	cheat_check_long_unsigned_int(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_long_unsigned_int(actual, expected) \
	cheat_check_long_unsigned_int(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#ifndef CHEAT_NO_MATH

#define CHEAT_GENERATE_FLOATING(name, type, abs, specifier) \
	__attribute__ ((__io__, __nonnull__ (1, 6), __unused__)) \
	static void cheat_check_##name(struct cheat_suite* const suite, \
			bool const negate, \
			type const tolerance, \
			type const actual, \
			type const expected, \
			char const* const file, \
			size_t const line) { \
		if (cheat_further(suite->outcome) \
				&& (abs(actual - expected) <= tolerance) != !negate) { \
			char const* comparator; \
			char* expression; \
			\
			suite->outcome = CHEAT_FAILED; \
			\
			if (negate) \
				comparator = "~!="; \
			else \
				comparator = "~=="; \
			\
			expression = CHEAT_CAST(char*, cheat_allocate_total(4, \
						CHEAT_FLOATING_LENGTH(actual), strlen(comparator), \
						CHEAT_FLOATING_LENGTH(expected), (size_t )3)); \
			if (expression == NULL) \
				cheat_death("failed to allocate memory", errno); \
			\
			if (cheat_print_string(expression, \
						specifier " %s " specifier, \
						3, actual, comparator, expected) < 0) \
				cheat_death("failed to build a string", errno); \
			\
			cheat_print_failure(suite, expression, file, line); \
		} \
	}

CHEAT_GENERATE_FLOATING(double, double, fabs, "%g")

#define cheat_assert_double(actual, expected, tolerance) \
	cheat_check_double(&cheat_suite, false, tolerance, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_double(actual, expected, tolerance) \
	cheat_check_double(&cheat_suite, true, tolerance, actual, expected, \
		__FILE__, __LINE__)

#endif

#define CHEAT_GENERATE_SPECIAL(name, type, cast, specifier) \
	__attribute__ ((__io__, __nonnull__ (1, 5), __unused__)) \
	static void cheat_check_##name(struct cheat_suite* const suite, \
			bool const negate, \
			type const actual, \
			type const expected, \
			char const* const file, \
			size_t const line) { \
		if (cheat_further(suite->outcome) && (actual == expected) != !negate) { \
			cast cast_actual; \
			cast cast_expected; \
			char const* comparator; \
			char* expression; \
			\
			suite->outcome = CHEAT_FAILED; \
			\
			cast_actual = (cast )actual;\
			cast_expected = (cast )expected;\
			\
			if (negate) \
				comparator = "!="; \
			else \
				comparator = "=="; \
			\
			expression = CHEAT_CAST(char*, cheat_allocate_total(4, \
						CHEAT_INTEGER_LENGTH(cast_actual), strlen(comparator), \
						CHEAT_INTEGER_LENGTH(cast_expected), (size_t )3)); \
			if (expression == NULL) \
				cheat_death("failed to allocate memory", errno); \
			\
			if (cheat_print_string(expression, \
						specifier " %s " specifier, \
						3, cast_actual, comparator, cast_expected) < 0) \
				cheat_death("failed to build a string", errno); \
			\
			cheat_print_failure(suite, expression, file, line); \
		} \
	}

CHEAT_GENERATE_SPECIAL(size, size_t, CHEAT_SIZE_TYPE, CHEAT_SIZE_FORMAT)
CHEAT_GENERATE_SPECIAL(ptrdiff, ptrdiff_t,
		CHEAT_POINTER_TYPE, CHEAT_POINTER_FORMAT)

#define cheat_assert_size(actual, expected) \
	cheat_check_size(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_size(actual, expected) \
	cheat_check_size(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_ptrdiff(actual, expected) \
	cheat_check_ptrdiff(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_ptrdiff(actual, expected) \
	cheat_check_ptrdiff(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#ifdef CHEAT_MODERN

CHEAT_GENERATE_INTEGER(long_long_int, long long int, "%lld")
CHEAT_GENERATE_INTEGER(long_long_unsigned_int, long long unsigned int, "%llu")

#define cheat_assert_long_long_int(actual, expected) \
	cheat_check_long_long_int(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_long_long_int(actual, expected) \
	cheat_check_long_long_int(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_long_long_unsigned_int(actual, expected) \
	cheat_check_long_long_unsigned_int(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_long_long_unsigned_int(actual, expected) \
	cheat_check_long_long_unsigned_int(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#ifndef CHEAT_NO_MATH

CHEAT_GENERATE_FLOATING(float, float, fabsf, "%hg")
CHEAT_GENERATE_FLOATING(long_double, long double, fabsl, "%lg")

#define cheat_assert_float(actual, expected, tolerance) \
	cheat_check_float(&cheat_suite, false, tolerance, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_float(actual, expected, tolerance) \
	cheat_check_float(&cheat_suite, true, tolerance, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_long_double(actual, expected, tolerance) \
	cheat_check_long_double(&cheat_suite, false, tolerance, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_long_double(actual, expected, tolerance) \
	cheat_check_long_double(&cheat_suite, true, tolerance, actual, expected, \
		__FILE__, __LINE__)

#endif

/*
These alone would take up 839 lines without preprocessor generators.
*/
CHEAT_GENERATE_INTEGER(int8, int8_t, "%" PRId8)
CHEAT_GENERATE_INTEGER(uint8, uint8_t, "%" PRIu8)
CHEAT_GENERATE_INTEGER(int16, int16_t, "%" PRId16)
CHEAT_GENERATE_INTEGER(uint16, uint16_t, "%" PRIu16)
CHEAT_GENERATE_INTEGER(int32, int32_t, "%" PRId32)
CHEAT_GENERATE_INTEGER(uint32, uint32_t, "%" PRIu32)
CHEAT_GENERATE_INTEGER(int64, int64_t, "%" PRId64)
CHEAT_GENERATE_INTEGER(uint64, uint64_t, "%" PRIu64)
CHEAT_GENERATE_INTEGER(int_fast8, int_fast8_t, "%" PRIdFAST8)
CHEAT_GENERATE_INTEGER(uint_fast8, uint_fast8_t, "%" PRIuFAST8)
CHEAT_GENERATE_INTEGER(int_fast16, int_fast16_t, "%" PRIdFAST16)
CHEAT_GENERATE_INTEGER(uint_fast16, uint_fast16_t, "%" PRIuFAST16)
CHEAT_GENERATE_INTEGER(int_fast32, int_fast32_t, "%" PRIdFAST32)
CHEAT_GENERATE_INTEGER(uint_fast32, uint_fast32_t, "%" PRIuFAST32)
CHEAT_GENERATE_INTEGER(int_fast64, int_fast64_t, "%" PRIdFAST64)
CHEAT_GENERATE_INTEGER(uint_fast64, uint_fast64_t, "%" PRIuFAST64)
CHEAT_GENERATE_INTEGER(int_least8, int_least8_t, "%" PRIdLEAST8)
CHEAT_GENERATE_INTEGER(uint_least8, uint_least8_t, "%" PRIuLEAST8)
CHEAT_GENERATE_INTEGER(int_least16, int_least16_t, "%" PRIdLEAST16)
CHEAT_GENERATE_INTEGER(uint_least16, uint_least16_t, "%" PRIuLEAST16)
CHEAT_GENERATE_INTEGER(int_least32, int_least32_t, "%" PRIdLEAST32)
CHEAT_GENERATE_INTEGER(uint_least32, uint_least32_t, "%" PRIuLEAST32)
CHEAT_GENERATE_INTEGER(int_least64, int_least64_t, "%" PRIdLEAST64)
CHEAT_GENERATE_INTEGER(uint_least64, uint_least64_t, "%" PRIuLEAST64)
CHEAT_GENERATE_INTEGER(intmax, intmax_t, "%" PRIdMAX)
CHEAT_GENERATE_INTEGER(uintmax, uintmax_t, "%" PRIuMAX)
CHEAT_GENERATE_INTEGER(intptr, intptr_t, "%" PRIdPTR)
CHEAT_GENERATE_INTEGER(uintptr, uintptr_t, "%" PRIuPTR)

#define cheat_assert_int8(actual, expected) \
	cheat_check_int8(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_int8(actual, expected) \
	cheat_check_int8(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_uint8(actual, expected) \
	cheat_check_uint8(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_uint8(actual, expected) \
	cheat_check_uint8(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_int16(actual, expected) \
	cheat_check_int16(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_int16(actual, expected) \
	cheat_check_int16(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_uint16(actual, expected) \
	cheat_check_uint16(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_uint16(actual, expected) \
	cheat_check_uint16(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_int32(actual, expected) \
	cheat_check_int32(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_int32(actual, expected) \
	cheat_check_int32(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_uint32(actual, expected) \
	cheat_check_uint32(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_uint32(actual, expected) \
	cheat_check_uint32(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_int64(actual, expected) \
	cheat_check_int64(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_int64(actual, expected) \
	cheat_check_int64(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_uint64(actual, expected) \
	cheat_check_uint64(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_uint64(actual, expected) \
	cheat_check_uint64(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_int_fast8(actual, expected) \
	cheat_check_int_fast8(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_int_fast8(actual, expected) \
	cheat_check_int_fast8(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_uint_fast8(actual, expected) \
	cheat_check_uint_fast8(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_uint_fast8(actual, expected) \
	cheat_check_uint_fast8(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_int_fast16(actual, expected) \
	cheat_check_int_fast16(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_int_fast16(actual, expected) \
	cheat_check_int_fast16(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_uint_fast16(actual, expected) \
	cheat_check_uint_fast16(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_uint_fast16(actual, expected) \
	cheat_check_uint_fast16(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_int_fast32(actual, expected) \
	cheat_check_int_fast32(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_int_fast32(actual, expected) \
	cheat_check_int_fast32(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_uint_fast32(actual, expected) \
	cheat_check_uint_fast32(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_uint_fast32(actual, expected) \
	cheat_check_uint_fast32(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_int_fast64(actual, expected) \
	cheat_check_int_fast64(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_int_fast64(actual, expected) \
	cheat_check_int_fast64(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_uint_fast64(actual, expected) \
	cheat_check_uint_fast64(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_uint_fast64(actual, expected) \
	cheat_check_uint_fast64(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_int_least8(actual, expected) \
	cheat_check_int_least8(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_int_least8(actual, expected) \
	cheat_check_int_least8(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_uint_least8(actual, expected) \
	cheat_check_uint_least8(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_uint_least8(actual, expected) \
	cheat_check_uint_least8(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_int_least16(actual, expected) \
	cheat_check_int_least16(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_int_least16(actual, expected) \
	cheat_check_int_least16(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_uint_least16(actual, expected) \
	cheat_check_uint_least16(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_uint_least16(actual, expected) \
	cheat_check_uint_least16(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_int_least32(actual, expected) \
	cheat_check_int_least32(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_int_least32(actual, expected) \
	cheat_check_int_least32(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_uint_least32(actual, expected) \
	cheat_check_uint_least32(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_uint_least32(actual, expected) \
	cheat_check_uint_least32(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_int_least64(actual, expected) \
	cheat_check_int_least64(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_int_least64(actual, expected) \
	cheat_check_int_least64(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_uint_least64(actual, expected) \
	cheat_check_uint_least64(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_uint_least64(actual, expected) \
	cheat_check_uint_least64(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_intmax(actual, expected) \
	cheat_check_intmax(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_intmax(actual, expected) \
	cheat_check_intmax(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_uintmax(actual, expected) \
	cheat_check_uintmax(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_uintmax(actual, expected) \
	cheat_check_uintmax(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_intptr(actual, expected) \
	cheat_check_intptr(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_intptr(actual, expected) \
	cheat_check_intptr(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_uintptr(actual, expected) \
	cheat_check_uintptr(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_uintptr(actual, expected) \
	cheat_check_uintptr(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#ifndef CHEAT_NO_MATH

#define CHEAT_GENERATE_COMPLEX(name, type, abs, real, imag, specifier) \
	__attribute__ ((__io__, __nonnull__ (1, 6), __unused__)) \
	static void cheat_check_##name(struct cheat_suite* const suite, \
			bool const negate, \
			type const tolerance, \
			type complex const actual, \
			type complex const expected, \
			char const* const file, \
			size_t const line) { \
		if (cheat_further(suite->outcome) \
				&& (abs(actual - expected) <= tolerance) != !negate) { \
			char const* comparator; \
			char* expression; \
			\
			suite->outcome = CHEAT_FAILED; \
			\
			if (negate) \
				comparator = "~!="; \
			else \
				comparator = "~=="; \
			\
			expression = CHEAT_CAST(char*, cheat_allocate_total(4, \
						CHEAT_FLOATING_LENGTH(actual), strlen(comparator), \
						CHEAT_FLOATING_LENGTH(expected), (size_t )7)); \
			if (expression == NULL) \
				cheat_death("failed to allocate memory", errno); \
			\
			if (cheat_print_string(expression, \
						specifier "+" specifier "i" \
						" %s " specifier "+" specifier "i", \
						5, real(actual), imag(actual), comparator, \
						real(expected), imag(expected)) < 0) \
				cheat_death("failed to build a string", errno); \
			\
			cheat_print_failure(suite, expression, file, line); \
		} \
	}

CHEAT_GENERATE_COMPLEX(float_complex, float, cabsf, crealf, cimagf, "%g")
CHEAT_GENERATE_COMPLEX(double_complex, double, cabs, creal, cimag, "%g")
CHEAT_GENERATE_COMPLEX(long_double_complex, long double,
		cabsl, creall, cimagl, "%lg")

#define cheat_assert_float_complex(actual, expected, tolerance) \
	cheat_check_float_complex(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_float_complex(actual, expected, tolerance) \
	cheat_check_float_complex(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_double_complex(actual, expected, tolerance) \
	cheat_check_double_complex(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_double_complex(actual, expected, tolerance) \
	cheat_check_double_complex(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_long_double_complex(actual, expected, tolerance) \
	cheat_check_long_double_complex(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_long_double_complex(actual, expected, tolerance) \
	cheat_check_long_double_complex(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#endif

CHEAT_GENERATE_INTEGER(signed_char, signed char, "%hhd")
CHEAT_GENERATE_INTEGER(unsigned_char, unsigned char, "%hhu")

#else

CHEAT_GENERATE_INTEGER(signed_char, signed char, "%hd")
CHEAT_GENERATE_INTEGER(unsigned_char, unsigned char, "%hu")

#endif

#define cheat_assert_signed_char(actual, expected) \
	cheat_check_signed_char(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_signed_char(actual, expected) \
	cheat_check_signed_char(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_unsigned_char(actual, expected) \
	cheat_check_unsigned_char(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_unsigned_char(actual, expected) \
	cheat_check_unsigned_char(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

CHEAT_GENERATE_INTEGER(pointer, void const*, "%p")

#define cheat_assert_pointer(actual, expected) \
	cheat_check_pointer(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_pointer(actual, expected) \
	cheat_check_pointer(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

__attribute__ ((__io__, __nonnull__ (1, 5), __unused__))
static void cheat_check_string(struct cheat_suite* const suite,
		bool const negate,
		char const* const actual,
		char const* const expected,
		char const* const file,
		size_t const line) {
	if (cheat_further(suite->outcome)
			&& (actual == expected
				|| (actual != NULL && expected != NULL
					&& strcmp(actual, expected) == 0)) != !negate) {
		char const* comparator;
		char* expression;

		suite->outcome = CHEAT_FAILED;

		if (negate)
			comparator = "!=";
		else
			comparator = "==";

		expression = CHEAT_CAST(char*, cheat_allocate_total(4,
					strlen(actual), strlen(comparator),
					strlen(expected), (size_t )7));
		if (expression == NULL)
			cheat_death("failed to allocate memory", errno);

		if (cheat_print_string(expression, "\"%s\" %s \"%s\"",
					3, actual, comparator, expected) < 0)
			cheat_death("failed to build a string", errno);

		cheat_print_failure(suite, expression, file, line);
	}
}

#define cheat_assert_string(actual, expected) \
	cheat_check_string(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_string(actual, expected) \
	cheat_check_string(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

__attribute__ ((__io__, __nonnull__ (1, 5), __unused__))
static void cheat_check_bool(struct cheat_suite* const suite,
		bool const negate,
		bool const actual,
		bool const expected,
		char const* const file,
		size_t const line) {
	if (cheat_further(suite->outcome) && (actual == expected) != !negate) {
		char const* comparator;
		char* expression;

		suite->outcome = CHEAT_FAILED;

		if (negate)
			comparator = "!=";
		else
			comparator = "==";

		expression = CHEAT_CAST(char*, cheat_allocate_total(4,
					(size_t)5, strlen(comparator), (size_t)5, (size_t)3));
		if (expression == NULL)
			cheat_death("failed to allocate memory", errno);

		if (cheat_print_string(expression,
					"%s" " %s " "%s", 3, actual ? "true" : "false",
					comparator, expected ? "true" : "false") < 0)
			cheat_death("failed to build a string", errno);

		cheat_print_failure(suite, expression, file, line);
	}
}

#define cheat_assert_bool(actual, expected) \
	cheat_check_bool(&cheat_suite, false, actual, expected, \
		__FILE__, __LINE__)

#define cheat_assert_not_bool(actual, expected) \
	cheat_check_bool(&cheat_suite, true, actual, expected, \
		__FILE__, __LINE__)

#endif
