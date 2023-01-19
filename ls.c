#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char** argv) {
	int i, option_flag = 0, a_flag = 0, i_flag = 0, number_directories = 0;
	char* directory_names[100];
	for	(i = 1; i < argc; i++) {
		if (argv[i][strlen(argv[i]) - 1] == '\n') 
			argv[i][strlen(argv[i]) - 1] = '\0';
		if (argv[i][0] == '-') {
			option_flag = 1;
			if (!strcmp(argv[i], "-a")) a_flag = 1;
			else if (!strcmp(argv[i], "-i")) i_flag = 1;
			else printf("Invalid option '%s'!!\n", argv[i]);
		}
		else {
			directory_names[number_directories++] = argv[i];
		}
	}
	
	if (number_directories == 0) {
		char directory[1000];
		getcwd(directory, sizeof(directory));
		char* wdirectory = strdup(directory);
		DIR* mydir = opendir(wdirectory);
		if (mydir == NULL) {
			printf("Directory '%s' not found!!\n", wdirectory);
		}
		else {
			struct dirent* dentry;
			while ((dentry = readdir(mydir)) != NULL) {
				if (!a_flag && dentry->d_name[0] == '.') {
					continue;
				}
				else {
					if (i_flag) printf("%lu ", dentry->d_ino);
					printf("%s\t", dentry->d_name);
					
				}
			}
			printf("\n");
		}
	}
	else {
		for (i = 0; i < number_directories; i++) {
			char* dirname = directory_names[i];
			DIR* mydir = opendir(dirname);

			if (mydir == NULL) {
			printf("Directory '%s' not found!!\n", dirname);
			}
			else {
				struct dirent* dentry;
				printf("%s:\n", dirname);
				while ((dentry = readdir(mydir)) != NULL) {
					if (!a_flag && dentry->d_name[0] == '.') {
						continue;
					}
					else {
						if (i_flag) printf("%lu ", dentry->d_ino);
						printf("%s\t", dentry->d_name);
					}
				}
				printf("\n");
			}
		}
	}
	
	return 0;
}