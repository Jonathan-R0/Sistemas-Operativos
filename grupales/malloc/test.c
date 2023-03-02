#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "printfmt.h"

#define HELLO "hello from test"
#define TEST_STRING "FISOP malloc is working!"
#define TEST_STRING2 "FISOP malloc is actually working! :D yay!"

static char smaller_random_string[301];
static char random_string[501];
static char big_random_string[16501];
static char big_block_string[16000];
static char biggest_block_string[5000000];
static char big_medium_block_string[1000000];

static void
generate_smaller_random_string()
{
	for (int i = 0; i < 300; i++) {
		random_string[i] = 'A' + (random() % 26);
	}
	random_string[300] = 0;
}

static void
generate_random_string()
{
	for (int i = 0; i < 500; i++) {
		random_string[i] = 'A' + (random() % 26);
	}
	random_string[500] = 0;
}

static void
generate_big_random_string()
{
	for (int i = 0; i < 16500; i++) {
		big_random_string[i] = 'A' + (random() % 26);
	}
	big_random_string[16500] = 0;
}

static void
generate_biggest_random_string()
{
	for (int i = 0; i < 5000000; i++) {
		biggest_block_string[i] = 'A' + (random() % 26);
	}
	biggest_block_string[500000] = 0;
}

static void
generate_big_block_string()
{
	for (int i = 0; i < 15999; i++) {
		big_random_string[i] = 'A' + (random() % 26);
	}
	big_random_string[15999] = 0;
}

static void
generate_big_medium_block_string()
{
	for (int i = 0; i < 999999; i++) {
		big_medium_block_string[i] = 'A' + (random() % 26);
	}
	big_medium_block_string[999999] = 0;
}


static void
supports_basic_test(void)
{
	char *var1 = malloc(100);
	strcpy(var1, TEST_STRING);
	char *var2 = malloc(70);
	strcpy(var2, TEST_STRING2);
	assert(strcmp(var1, TEST_STRING) == 0);
	assert(strcmp(var2, TEST_STRING2) == 0);
	printfmt(GREEN "supports_basic_test: OK" RESET_COLOR "\n");
	free(var1);
	free(var2);
}

static void
supports_bigger_than_256(void)
{
	generate_random_string();
	char *var1 = malloc(sizeof(random_string));
	strcpy(var1, random_string);
	assert(strcmp(var1, random_string) == 0);
	printfmt(GREEN "supports_bigger_than_256: OK" RESET_COLOR "\n");
	free(var1);
}

static void
supports_big_block(void)
{
	generate_big_random_string();
	char *var1 = malloc(sizeof(big_random_string));
	strcpy(var1, big_random_string);
	assert(strcmp(var1, big_random_string) == 0);
	printfmt(GREEN "supports_big_block: OK" RESET_COLOR "\n");
	free(var1);
}

static void
supports_small_and_medium_blocks(void)
{
	generate_random_string();
	generate_big_random_string();
	char *var1 = malloc(sizeof(random_string));
	strcpy(var1, random_string);
	assert(strcmp(var1, random_string) == 0);
	char *var2 = malloc(sizeof(big_random_string));
	strcpy(var2, big_random_string);
	assert(strcmp(var2, big_random_string) == 0);
	printfmt(GREEN "supports_small_and_medium_blocks: OK" RESET_COLOR "\n");
	free(var1);
	free(var2);
}

static void
supports_small_medium_blocks_and_uses_the_remaining_region(void)
{
	generate_random_string();
	generate_big_random_string();
	char *var1 = malloc(sizeof(random_string));
	strcpy(var1, random_string);
	assert(strcmp(var1, random_string) == 0);
	char *var2 = malloc(sizeof(big_random_string));
	strcpy(var2, big_random_string);
	assert(strcmp(var2, big_random_string) == 0);
	generate_random_string();
	char *var3 = malloc(sizeof(random_string));
	strcpy(var3, random_string);
	assert(strcmp(var3, random_string) == 0);
	printfmt(GREEN "supports_small_medium_blocks_and_uses_the_remaining_"
	               "region: OK" RESET_COLOR "\n");
	free(var1);
	free(var2);
	free(var3);
}

static void
supports_double_free_protection(void)
{
	generate_big_random_string();
	char *var = malloc(sizeof(big_random_string));
	strcpy(var, big_random_string);
	assert(strcmp(var, big_random_string) == 0);
	free(var);
	free(var);
	printfmt(GREEN "supports_double_free_protection: OK" RESET_COLOR "\n");
}

static void
supports_basic_calloc(void)
{
	char *var = calloc(512, 2);
	for (int i = 0; i < 1024; i++) {
		assert(var[i] == '\0');
	}
	printfmt(YELLOW "%s" RESET_COLOR "\n", var);
	free(var);
	printfmt(GREEN "supports_basic_calloc: OK" RESET_COLOR "\n");
}

static void
supports_big_enough_region_for_entire_block(void)
{
	generate_big_block_string();
	char *var = malloc(sizeof(big_block_string));
	strcpy(var, big_block_string);
	assert(strcmp(var, big_block_string) == 0);
	free(var);
	printfmt(GREEN
	         "supports_big_enough_region_for_entire_block: OK" RESET_COLOR
	         "\n");
}

static void
supports_big_enough_region_for_entire_medium_block(void)
{
	generate_big_medium_block_string();
	char *var = malloc(sizeof(big_medium_block_string));
	strcpy(var, big_medium_block_string);
	assert(strcmp(var, big_medium_block_string) == 0);
	free(var);
	printfmt(GREEN "supports_big_enough_region_for_entire_medium_block: "
	               "OK" RESET_COLOR "\n");
}

static void
supports_diff_sizes_with_sucesives_calls(void)
{
	generate_big_medium_block_string();
	char *var = malloc(sizeof(big_medium_block_string));
	strcpy(var, big_medium_block_string);
	assert(strcmp(var, big_medium_block_string) == 0);

	generate_big_random_string();
	char *var2 = malloc(sizeof(big_random_string));
	strcpy(var2, big_random_string);
	assert(strcmp(var2, big_random_string) == 0);

	generate_smaller_random_string();
	char *var3 = malloc(sizeof(smaller_random_string));
	strcpy(var3, smaller_random_string);
	assert(strcmp(var3, smaller_random_string) == 0);

	free(var);
	free(var2);
	free(var3);
	printfmt(GREEN "supports_diff_sizes_with_sucesives_calls: "
	               "OK" RESET_COLOR "\n");
}


static void
supports_diff_sizes_with_sucesives_calls_with_frees(void)
{
	generate_big_medium_block_string();
	char *var = malloc(sizeof(big_medium_block_string));
	strcpy(var, big_medium_block_string);
	assert(strcmp(var, big_medium_block_string) == 0);

	free(var);

	generate_big_random_string();
	char *var2 = malloc(sizeof(big_random_string));
	strcpy(var2, big_random_string);
	assert(strcmp(var2, big_random_string) == 0);

	free(var2);

	generate_smaller_random_string();
	char *var3 = malloc(sizeof(smaller_random_string));
	strcpy(var3, smaller_random_string);
	assert(strcmp(var3, smaller_random_string) == 0);

	free(var3);
	printfmt(GREEN "supports_diff_sizes_with_sucesives_calls_with_free: "
	               "OK" RESET_COLOR "\n");
}


static void
supports_a_lot_of_big_successive_calls(void)
{
	generate_big_random_string();

	char *var = malloc(sizeof(big_random_string));
	strcpy(var, big_random_string);
	assert(strcmp(var, big_random_string) == 0);

	char *var2 = malloc(sizeof(big_random_string));
	strcpy(var2, big_random_string);
	assert(strcmp(var2, big_random_string) == 0);

	char *var3 = malloc(sizeof(big_random_string));
	strcpy(var3, big_random_string);
	assert(strcmp(var3, big_random_string) == 0);

	char *var4 = malloc(sizeof(big_random_string));
	strcpy(var4, big_random_string);
	assert(strcmp(var4, big_random_string) == 0);

	char *var5 = malloc(sizeof(big_random_string));
	strcpy(var5, big_random_string);
	assert(strcmp(var5, big_random_string) == 0);

	char *var6 = malloc(sizeof(big_random_string));
	strcpy(var6, big_random_string);
	assert(strcmp(var6, big_random_string) == 0);

	char *var7 = malloc(sizeof(big_random_string));
	strcpy(var7, big_random_string);
	assert(strcmp(var7, big_random_string) == 0);


	free(var);
	free(var2);
	free(var3);
	free(var4);
	free(var5);
	free(var6);
	free(var7);

	printfmt(GREEN "supports_a_lot_of_big_successive_calls: "
	               "OK" RESET_COLOR "\n");
}

static void
supports_biggest(void)
{
	generate_biggest_random_string();

	char *var = malloc(sizeof(biggest_block_string));
	strcpy(var, biggest_block_string);
	assert(strcmp(var, biggest_block_string) == 0);

	free(var);

	printfmt(GREEN "supports_biggest: "
	               "OK" RESET_COLOR "\n");
}


static void
supports_biggest_successive_calls(void)
{
	generate_biggest_random_string();

	char *var = malloc(sizeof(biggest_block_string));
	strcpy(var, biggest_block_string);
	assert(strcmp(var, biggest_block_string) == 0);

	char *var2 = malloc(sizeof(biggest_block_string));
	strcpy(var2, biggest_block_string);
	assert(strcmp(var2, biggest_block_string) == 0);

	char *var3 = malloc(sizeof(biggest_block_string));
	strcpy(var3, biggest_block_string);
	assert(strcmp(var3, biggest_block_string) == 0);

	char *var4 = malloc(sizeof(biggest_block_string));
	strcpy(var4, biggest_block_string);
	assert(strcmp(var4, biggest_block_string) == 0);

	char *var5 = malloc(sizeof(biggest_block_string));
	strcpy(var5, biggest_block_string);
	assert(strcmp(var5, biggest_block_string) == 0);

	char *var6 = malloc(sizeof(biggest_block_string));
	strcpy(var6, biggest_block_string);
	assert(strcmp(var6, biggest_block_string) == 0);

	char *var7 = malloc(sizeof(biggest_block_string));
	strcpy(var7, biggest_block_string);
	assert(strcmp(var7, biggest_block_string) == 0);

	generate_big_block_string();
	char *var8 = malloc(sizeof(big_block_string));
	strcpy(var8, big_block_string);
	assert(strcmp(var8, big_block_string) == 0);

	free(var);
	free(var2);
	free(var3);
	free(var4);
	free(var5);
	free(var6);
	free(var7);
	free(var8);

	printfmt(GREEN "supports_biggest_successive_calls: "
	               "OK" RESET_COLOR "\n");
}

static void
supports_realloc_when_size_is_bigger(void)
{
	generate_random_string();
	generate_big_random_string();
	char *var1 = malloc(sizeof(random_string));
	strcpy(var1, random_string);
	assert(strcmp(var1, random_string) == 0);
	char *var2 = realloc(var1, sizeof(big_random_string));
	for (size_t i = 0; i < strlen(random_string); i++) {
		assert(var2[i] == random_string[i]);
	}
	strcpy(var2, big_random_string);
	assert(strcmp(var2, big_random_string) == 0);
	free(var2);
	printfmt(GREEN "supports_realloc_when_size_is_bigger: "
	               "OK" RESET_COLOR "\n");
}

static void
supports_realloc_when_size_is_bigger_and_catches_invalid_free_from_old_ptr(void)
{
	generate_random_string();
	generate_big_random_string();
	char *var1 = malloc(sizeof(random_string));
	strcpy(var1, random_string);
	assert(strcmp(var1, random_string) == 0);
	char *var2 = realloc(var1, sizeof(big_random_string));
	for (size_t i = 0; i < strlen(random_string); i++) {
		assert(var2[i] == random_string[i]);
	}
	strcpy(var2, big_random_string);
	assert(strcmp(var2, big_random_string) == 0);
	free(var1);
	free(var2);
	printfmt(GREEN "supports_realloc_when_size_is_bigger: "
	               "OK" RESET_COLOR "\n");
}

static void
supports_basic_realloc_when_size_is_smaller(void)
{
	generate_smaller_random_string();
	generate_random_string();
	char *var1 = malloc(sizeof(random_string));
	strcpy(var1, random_string);
	assert(strcmp(var1, random_string) == 0);
	char *var2 = realloc(var1, sizeof(smaller_random_string));
	for (size_t i = 0; i < strlen(smaller_random_string); i++) {
		assert(var2[i] == random_string[i]);
	}
	strcpy(var2, smaller_random_string);
	assert(strcmp(var2, smaller_random_string) == 0);
	free(var2);
	printfmt(GREEN "supports_basic_realloc_when_size_is_smaller: "
	               "OK" RESET_COLOR "\n");
}

static void
supports_invalid_free_for_ptr_to_stack(void)
{
	char *var = TEST_STRING;
	free(var);
	printfmt(GREEN "supports_invalid_free_for_ptr_to_stack: "
	               "OK" RESET_COLOR "\n");
}

static void
supports_realloc_with_null_pointer(void){
	generate_random_string();
	char *var = realloc(NULL, sizeof(random_string));
	assert(var != NULL);
	free(var);
	printfmt(GREEN "supports_realloc_with_null_pointer: "
	               "OK" RESET_COLOR "\n");
}

static void
supports_realloc_with_null_pointer_and_catches_invalid_free(void){
	generate_random_string();
	char *var = realloc(NULL, sizeof(random_string));
	assert(var != NULL);
	free(var);
	free(var);
	printfmt(GREEN "supports_realloc_with_null_pointer: "
	               "OK" RESET_COLOR "\n");
}

static void
supports_realloc_when_size_is_zero(void){
	generate_random_string();
	char *var = malloc(sizeof(random_string));
	strcpy(var, random_string);
	assert(strcmp(var, random_string) == 0);
	char *var2 = realloc(var, 0);
	assert(var2 == NULL);
	printfmt(GREEN "supports_realloc_when_size_is_zero: "
	               "OK" RESET_COLOR "\n");
}

static void
supports_realloc_when_size_is_zero_and_catches_invalid_free(void){
	generate_random_string();
	char *var = malloc(sizeof(random_string));
	strcpy(var, random_string);
	assert(strcmp(var, random_string) == 0);
	char *var2 = realloc(var, 0);
	assert(var2 == NULL);
	free(var);
	printfmt(GREEN "supports_realloc_when_size_is_zero: "
	               "OK" RESET_COLOR "\n");
}

int
main(void)
{
	supports_basic_test();
	supports_bigger_than_256();
	supports_big_block();
	supports_small_and_medium_blocks();
	supports_small_medium_blocks_and_uses_the_remaining_region();
	supports_double_free_protection();
	supports_basic_calloc();
	supports_big_enough_region_for_entire_block();
	supports_big_enough_region_for_entire_medium_block();
	supports_realloc_when_size_is_bigger();
	supports_realloc_when_size_is_bigger_and_catches_invalid_free_from_old_ptr();
	supports_basic_realloc_when_size_is_smaller();
	supports_invalid_free_for_ptr_to_stack();
	supports_diff_sizes_with_sucesives_calls();
	supports_diff_sizes_with_sucesives_calls_with_frees();
	supports_a_lot_of_big_successive_calls();
	supports_biggest();
	supports_biggest_successive_calls();
	supports_realloc_with_null_pointer();
	supports_realloc_with_null_pointer_and_catches_invalid_free();
	supports_realloc_when_size_is_zero();
	supports_realloc_when_size_is_zero_and_catches_invalid_free();
	return 0;
}
