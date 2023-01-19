shell: shell.c ls.c mkdir.c date.c rm.c cat.c
	gcc ls.c -o ls
	gcc mkdir.c -o mkdir
	gcc date.c -o date
	gcc rm.c -o rm
	gcc cat.c -o cat
	gcc shell.c -o shell