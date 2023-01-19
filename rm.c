#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char** argv) {
	int i, option_flag = 0, f_flag = 0, v_flag = 0, number_files = 0;
	char* file_names[100];
	if (argc < 2) {
		printf("Invalid command!!\n");
		return 0;
	}

	for	(i = 1; i < argc; i++) {
		if (argv[i][strlen(argv[i]) - 1] == '\n') 
			argv[i][strlen(argv[i]) - 1] = '\0';
		if (argv[i][0] == '-') { 
			option_flag = 1;
			if (!strcmp(argv[i], "-f")) f_flag = 1;
			else if (!strcmp(argv[i], "-v")) v_flag = 1;
			else {
				printf("Invalid option '%s'!!\n", argv[i]);
			}
		}
		else {
			file_names[number_files++] = argv[i];
		}
	}

	for (i = 0; i < number_files; i++) {
		char* file_name = file_names[i];
		int result = remove(file_name);

		if (!result && v_flag) 
			printf("removed '%s'\n", file_name);
		else if (result && !f_flag) {
			printf("File '%s' does not exist!!\n", file_name);
		}
	}

	return 0;
}