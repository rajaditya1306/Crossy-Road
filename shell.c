#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

void* exec_command(void* arg) {
	char* arguments = (char*) arg;
	system(arguments);
}

int main() {
	while (1) {
		char working_directory[1000];
		getcwd(working_directory, sizeof(working_directory));
		printf("%s$ ", working_directory);

		char input[1000];
		fgets(input, sizeof(input), stdin);
		char* input_dup = strdup(input);

		char* command = strsep(&input_dup, " ");
		if (command[strlen(command) - 1] == '\n')
			command[strlen(command) - 1] = '\0';
		int number_args = 1, thread_flag = 0;
		char* args[100];
		char* arg;
		while ((arg = strsep(&input_dup, " ")) != NULL) {
			if (strcmp(arg, "&t\n")) 
				args[number_args++] = arg;
			else thread_flag = 1;
		}

		//Internal Commands
		if (!strcmp(command, "echo")) {
			int i, error_flag = 0, help_flag = 0, n_flag = 0;

			for	(i = 1; i < number_args; i++) {
				if (args[i][strlen(args[i]) - 1] == '\n') 
					args[i][strlen(args[i]) - 1] = '\0';
			}
			if (number_args > 1) {
				if (args[1][0] == '-') {
					if (!strcmp(args[1], "--help")) help_flag = 1;
					else if (!strcmp(args[1], "-n")) n_flag = 1;
				}
			}
			if (number_args == 1) {
				printf("\n");
			}
			else if (help_flag && number_args > 2) {
				for	(i = 1; i < number_args - 1; i++) {
					printf("%s ", args[i]);
				}
				printf("%s\n", args[number_args - 1]);
			}
			else if (help_flag) {
				FILE* echo_help = fopen("echo_help.txt", "r");
				char ch;
				while ((ch = fgetc(echo_help)) != EOF) {
					printf("%c", ch);
				}
				fclose(echo_help);
			}
			else if (n_flag) {
				for	(i = 2; i < number_args - 1; i++) {
					printf("%s ", args[i]);
				}
				printf("%s", args[number_args - 1]);
			}
			else {
				for	(i = 1; i < number_args - 1; i++) {
					printf("%s ", args[i]);
				}
				printf("%s\n", args[number_args - 1]);
			}
		}
		else if (!strcmp(command, "cd")) {
			int i, flag = 0, l_flag = 0, p_flag = 0, flag2 = 0;
			char* dirname;

			for	(i = 1; i < number_args; i++) {
				if (args[i][strlen(args[i]) - 1] == '\n') 
					args[i][strlen(args[i]) - 1] = '\0';
				if (args[i][0] == '-') {
					if (!strcmp(args[i], "-L")) l_flag = 1;
					else if (!strcmp(args[i], "-P")) p_flag = 1;
					else {
						printf("Invalid option '%s'\n", args[i]);
						flag2 = 1;
					}
				}
				else 
					dirname = args[i];
			}
			
			if ((l_flag||p_flag)&&number_args > 3 || 
				(l_flag&&p_flag)&&number_args > 4 || 
				(!l_flag && !p_flag)&&number_args > 2 && !flag2 ||
				(flag2 && number_args > 3)) {
				printf("Too many arguments!!\n");
				flag = 1;
			}
			if (flag) continue;
			char* dir_path;
			if (number_args == 1)
				dir_path = getenv("HOME");
			else {
				if (!strcmp(dirname, "~")) dir_path = getenv("HOME");
				// else if (!strcmp(dirname, "-")) path = "-";
				else if (dirname[0] == '~') {
					char temp1[1000];
					char* dup;
					printf("%lu\n", strlen(dirname));
					for (int k = 1; k < strlen(dirname); k++) {
						temp1[k - 1] = dirname[k];
					}
					temp1[strlen(dirname) - 1] = '\0';
					dup = strdup(temp1);
					char* home_path = strdup(getenv("HOME"));
					dir_path = strcat(home_path, dup);
					printf("%s\n", dir_path);
				}
				else if (!p_flag) {
					dir_path = dirname;
				}
				else {
					char temp[1000];
					dir_path = realpath(dirname, temp);
					if (dir_path == NULL) {
						printf("Directory '%s' doesn't exist!!\n", dirname);
						flag = 1;
					}
				}
			}
			if (!flag) {
				int result = chdir(dir_path);
				if (result == -1) printf("Directory '%s' doesn't exist!!\n", dirname);
			}
		}
		else if (!strcmp(command, "pwd")) {
			int error_flag = 0, l_flag = 0, p_flag = 0;
			if (number_args > 3) {
				printf("Too many arguments!!\n");
				error_flag = 1;
			}
			if (error_flag) continue;

			for (int k = 1; k < number_args; k++) {
				if (args[k][strlen(args[k]) - 1] == '\n') 
					args[k][strlen(args[k]) - 1] = '\0';
				if (args[k][0] == '-') {
					if (!strcmp(args[k], "-L")) l_flag = 1;
					else if (!strcmp(args[k], "-P")) p_flag = 1;
					else {
						printf("Invalid option '%s'\n", args[k]);
						error_flag = 1;
					}
				}
			}
			
			char working_directory[1000];
			getcwd(working_directory, sizeof(working_directory));
			if (p_flag) {
				char dir[1000];
				char* path = realpath(working_directory, dir);
				printf("%s\n", path);
			}
			else printf("%s\n", working_directory);	
		}
		else {
			//External Commands
			if (!thread_flag) {
				pid_t pid = fork();
				if (pid < 0) {
					printf("Fork failed!!");
				}
				else if (pid == 0) {
					args[number_args++] = NULL;

					if (!strcmp(command, "ls")) {
						args[0] = "/home/strayweeb/aditya/ls";					
						execvp(args[0], args);
					}
					else if (!strcmp(command, "date")) {
						args[0] = "/home/strayweeb/aditya/date";
						execvp(args[0], args);
					}
					else if (!strcmp(command, "mkdir")) {
						args[0] = "/home/strayweeb/aditya/mkdir";
						execvp(args[0], args);
					}
					else if (!strcmp(command, "rm")) {
						args[0] = "/home/strayweeb/aditya/rm";
						execvp(args[0], args);
					}
					else if (!strcmp(command, "cat")) {
						args[0] = "/home/strayweeb/aditya/cat";
						execvp(args[0], args);
					}
					else {
						printf("Invalid command!!\n");
					}
				}
				else {
					wait(NULL);
				}
			}
			else {
				pthread_t thread;
				if (!strcmp(command, "ls")) {
					args[0] = "/home/strayweeb/aditya/ls";
					char arguments[1000] = "";
					for (int j = 0; j < number_args; j++) {
						strcat(arguments, args[j]);
						strcat(arguments, " ");
					}
					char* new_arguments = strdup(arguments);
					pthread_create(&thread, NULL, exec_command, (void*)new_arguments);
					pthread_join(thread, NULL);
				}
				else if (!strcmp(command, "date")) {
					args[0] = "/home/strayweeb/aditya/date";
					char arguments[1000] = "";
					for (int j = 0; j < number_args; j++) {
						strcat(arguments, args[j]);
						strcat(arguments, " ");
					}
					char* new_arguments = strdup(arguments);
					pthread_create(&thread, NULL, exec_command, (void*)new_arguments);
					pthread_join(thread, NULL);
				}
				else if (!strcmp(command, "mkdir")) {
					args[0] = "/home/strayweeb/aditya/mkdir";
					char arguments[1000] = "";
					for (int j = 0; j < number_args; j++) {
						strcat(arguments, args[j]);
						strcat(arguments, " ");
					}
					char* new_arguments = strdup(arguments);
					pthread_create(&thread, NULL, exec_command, (void*)new_arguments);
					pthread_join(thread, NULL);
				}
				else if (!strcmp(command, "rm")) {
					args[0] = "/home/strayweeb/aditya/rm";
					char arguments[1000] = "";
					for (int j = 0; j < number_args; j++) {
						strcat(arguments, args[j]);
						strcat(arguments, " ");
					}
					char* new_arguments = strdup(arguments);
					pthread_create(&thread, NULL, exec_command, (void*)new_arguments);
					pthread_join(thread, NULL);
				}
				else if (!strcmp(command, "cat")) {
					args[0] = "/home/strayweeb/aditya/cat";
					char arguments[1000] = "";
					for (int j = 0; j < number_args; j++) {
						strcat(arguments, args[j]);
						strcat(arguments, " ");
					}
					char* new_arguments = strdup(arguments);
					pthread_create(&thread, NULL, exec_command, (void*)new_arguments);
					pthread_join(thread, NULL);
				}
				else {
					printf("Invalid command!!\n");
				}
			}	
		}
	}

	return 0;
}
