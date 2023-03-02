#ifndef PRINTFMT_H
#define PRINTFMT_H

int printfmt(char *format, ...);

#define EXIT_LINE "\n+--------------------- EXIT ---------------------+\n"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define RESET_COLOR "\033[0m"
#define STRINGIFY_BLOCK(b) "[%d]\n", (b)->size
#define STRINGIFY_REGION(r)                                                    \
	" -> [%s | %ld]\n", (r)->free ? "FREE" : "USED", (r)->size

#endif  // PRINTFMT_H
