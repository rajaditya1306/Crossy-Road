#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>

int main(int argc, char** argv) {
	time_t T = time(NULL);
	struct tm* present_time;
	int i;
	for	(i = 1; i < argc; i++) {
		if (argv[i][strlen(argv[i]) - 1] == '\n') 
			argv[i][strlen(argv[i]) - 1] = '\0';
	}

	int option_flag = 0;
	if (argc > 1) {
		option_flag = 1;
	}
	if (argc > 2 && strcmp(argv[1], "-r") || 
		argc > 3 && !strcmp(argv[1], "-r")) {
		printf("Too many arguments!!\n");
		return 0;
	}

	if (!option_flag) {
		present_time = localtime(&T);
		printf("%s", asctime(present_time));
	}
	else {
		if (!strcmp(argv[1], "-u")) {
			present_time = gmtime(&T);
			printf("%s", asctime(present_time));
		}
		else if (!strcmp(argv[1], "-r")) {
			char* file_path = argv[2];
			struct stat attr;
			int result = stat(file_path, &attr);
			if (!result) printf("%s", ctime(&attr.st_mtime));
			else printf("File '%s' doesn't exist!!\n", file_path);
		}
		else {
			printf("Invalid option '%s'!!\n", argv[1]);
		}
	}

	return 0;
}