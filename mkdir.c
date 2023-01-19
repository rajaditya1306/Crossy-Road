#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char** argv) {
	int i, option_flag = 0, p_flag = 0, v_flag = 0, number_directories = 0;
	char* directory_names[100];
	for	(i = 1; i < argc; i++) {
		if (argv[i][strlen(argv[i]) - 1] == '\n') 
			argv[i][strlen(argv[i]) - 1] = '\0';
		if (argv[i][0] == '-') {
			option_flag = 1;
			if (!strcmp(argv[i], "-p")) p_flag = 1;
			else if (!strcmp(argv[i], "-v")) v_flag = 1;
			else printf("Invalid option '%s'\n", argv[i]);
		}
		else {
			directory_names[number_directories++] = argv[i];
		}
	}

	for (i = 0; i < number_directories; i++) {
		char* dirname;
		char actual_name[1000] = "";
		int count = 0;
		while ((dirname = strsep(&directory_names[i], "/")) != NULL) {
			if (!strcmp(dirname, ".")) {
				continue;
			}
			else {
				strcat(actual_name, dirname);
			}
			if (!strcmp(actual_name, "")) {
				strcat(actual_name, "/");
				count++;
				continue;
			}
			if (count > 0 && !p_flag) {
				printf("Cannot create directory '%s'\n", actual_name);
				break;
			}
			int result = mkdir(actual_name, 0777);
			if (result == -1 && !p_flag) {
				printf("Directory '%s' already exists!!\n", actual_name);
			}
			else if (result != -1 && v_flag) {
				printf("mkdir: created directory '%s'\n", actual_name);
			}
			strcat(actual_name, "/");
			count++;
		}
	}

	return 0;
}