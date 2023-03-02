#ifndef _PS_H_
#define _PS_H_

#define RED "\033[0;31m"
#define RESET "\033[0m"
#define OPEN_DIR_ERROR RED "Error: dir could not be opened" RESET
#define CLOSE_DIR_ERROR RED "Error: dir could not be closed" RESET
#define LOG_READING_ERROR(name)                                                \
	fprintf(stderr, RED "Error reading file: %s\n" RESET, name)

int is_digit(char *str);

#endif  // _PS_H_
