#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <unistd.h>

#define NO_CAPS_FLAG "-i"
#define INVALID_NUMBER_OF_ARGS_MSG                                             \
	"Illegal number of arguments. Should be either 2 or 3 instead of %d."
#define INVALID_ARG_USED                                                       \
	"Invalid arguments. Only \"-i\" is available as a flag before the "    \
	"filename."
#define MISSING_FILE_NAME "Must add a filename to look up."
#define UNABLE_TO_READ_DIR "Cannot read directory."
#define FILE_NAME_SIZE 256

int checkInputs(int argc, char *argv[]);
void stringToLower(char *str);
int readAllFiles(bool ignoreCaps, char *searched);
void fetchAndPrint(DIR *folder, bool ignoreCaps, char *searched, char *path);

int
checkInputs(int argc, char *argv[])
{
	if (argc != 3 && argc != 2) {
		fprintf(stderr, INVALID_NUMBER_OF_ARGS_MSG "\n", argc - 1);
		return -1;
	} else if (argc == 3 && strcmp(argv[1], NO_CAPS_FLAG) != 0) {
		fprintf(stderr, INVALID_ARG_USED "\n");
		return -2;
	} else if (argc == 2 && strcmp(argv[1], NO_CAPS_FLAG) == 0) {
		fprintf(stderr, MISSING_FILE_NAME "\n");
		return -3;
	}
	return 0;
}

void
stringToLower(char *str)
{
	for (char *c = str; *c; ++c)
		*c = tolower(*c);
}

void
fetchAndPrint(DIR *folder, bool ignoreCaps, char *searched, char *path)
{
	struct dirent *file;
	char *filename;
	char newPath[PATH_MAX];
	while ((file = readdir(folder))) {
		filename = file->d_name;
		if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0)
			continue;
		char toCompare[FILE_NAME_SIZE + 1];
		strcpy(toCompare, filename);
		if (ignoreCaps)
			stringToLower(toCompare);
		if (strstr(toCompare, searched))
			printf("%s%s\n", path, filename);
		if (file->d_type == DT_DIR) {
			int fd = openat(dirfd(folder), filename, O_DIRECTORY);
			if (fd != -1) {
				DIR *subFolder = fdopendir(fd);
				memset(newPath, 0, sizeof(newPath));
				strcpy(newPath, path);
				strcat(newPath, filename);
				newPath[strlen(newPath)] = '/';
				fetchAndPrint(
				        subFolder, ignoreCaps, searched, newPath);
				closedir(subFolder);
			}
		}
	}
}

int
readAllFiles(bool ignoreCaps, char *searched)
{
	if (ignoreCaps)
		stringToLower(searched);
	DIR *folder = opendir(".");
	if (!folder) {
		fprintf(stderr, UNABLE_TO_READ_DIR "\n");
		return -4;
	}
	char path[PATH_MAX];
	memset(path, 0, sizeof(path));
	fetchAndPrint(folder, ignoreCaps, searched, path);
	closedir(folder);
	return 0;
}

int
main(int argc, char *argv[])
{
	int validInputs = checkInputs(argc, argv);
	if (validInputs != 0)
		return validInputs;
	bool ignoreCaps = argc == 3;
	return readAllFiles(ignoreCaps, argv[ignoreCaps ? 2 : 1]);
}
