#define _GNU_SOURCE
#include <stdio.h>
#include <dirent.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "ps.h"

int
is_digit(char *str)
{
	for (char c = *str; c != 0; c = *++str)
		if (!isdigit(c))
			return 0;
	return 1;
}

int
main(void)
{
	DIR *procs;
	if ((procs = opendir("/proc")) == NULL) {
		perror(OPEN_DIR_ERROR "\n");
		return 1;
	}
	struct dirent *dir;
	printf("      DIR COMMAND\n");
	while ((dir = readdir(procs))) {
		if (dir->d_type == DT_DIR && is_digit(dir->d_name)) {
			char line[256];
			char stats[sizeof(dir->d_name) + 12];
			snprintf(stats, sizeof(stats), "/proc/%s/stat", dir->d_name);
			int fd = open(stats, DT_REG);
			if (read(fd, &line, sizeof(line)) == -1) {
				LOG_READING_ERROR(dir->d_name);
				continue;
			}
			char *token = strtok(line, " ");
			token = strtok(NULL, ") ");
			printf("\t%s %s\n", dir->d_name, token + 1);
			close(fd);
		}
	}
	printf("\n");
	if (closedir(procs) == -1) {
		perror(CLOSE_DIR_ERROR "\n");
		return 1;
	}
	return 0;
}
