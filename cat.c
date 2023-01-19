#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char** argv) {
	int i, option_flag = 0, b_flag = 0, e_flag = 0, number_files = 0, line_count = 1;
	char* file_names[100];
	char* output_filename = NULL;
	FILE* output_file;
	
	if (!strcmp(argv[argc - 1], ">\n")) {
		printf("Error in output redirection!!\n");
		return 0;
	}

	for	(i = 1; i < argc; i++) {
		if (argv[i][strlen(argv[i]) - 1] == '\n') 
			argv[i][strlen(argv[i]) - 1] = '\0';
		if (argv[i][0] == '-') {
			option_flag = 1;
			if (!strcmp(argv[i], "-b")) b_flag = 1;
			else if (!strcmp(argv[i], "-e")) e_flag = 1;
			else printf("Invalid option '%s'\n", argv[i]);
		}
		else if (!strcmp(argv[i], ">")) {
			output_filename = argv[i + 1];
		}
		else if (strcmp(argv[i - 1], ">")) {
			file_names[number_files++] = argv[i];
		}
	}
	if (output_filename == NULL)
		output_file = stdout;
	else {
		output_file = fopen(output_filename, "w");
		if (output_file == NULL) {
			output_file = stdout;
			printf("File '%s' doesn't exist!!\n", output_filename);
		}
	}

	if (number_files == 0) {
		char ch, prev = '\n';
		while ((ch = getchar()) != EOF) {
			if (ch == '\n' && prev != '\n')
				line_count++;
			if (ch == '\n' && e_flag) {
				fprintf(output_file, "$\n");
				prev = ch;
				continue;
			}
			if (prev == '\n' && b_flag) {
				fprintf(output_file, "\t%d  %c", line_count, ch);
				prev = ch;
				continue;
			}
			fprintf(output_file, "%c", ch);
			prev = ch;
		}
	}
	else {
		for (i = 0; i < number_files; i++) {
		char* file = file_names[i];
		FILE* read_file = fopen(file, "r");
		if (read_file == NULL) {
			printf("File '%s' doesn't exist!!\n", file);
			continue;
		}

		char ch, prev = '\n';
		while ((ch = fgetc(read_file)) != EOF) {
			if (ch == '\n' && prev != '\n')
				line_count++;
			if (ch == '\n' && e_flag) {
				fprintf(output_file, "$\n");
				prev = ch;
				continue;
			}
			if (prev == '\n' && b_flag) {
				fprintf(output_file, "\t%d  %c", line_count, ch);
				prev = ch;
				continue;
			}
			fprintf(output_file, "%c", ch);
			prev = ch;
		}
		fclose(read_file);
	}
	if (output_filename != NULL)
		fclose(output_file);
	}

	return 0;
}