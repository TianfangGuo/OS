yash: yash.o fork.o jobs.o pipes.o redirection.o signals.o
	  gcc -o yash yash.o fork.o jobs.o pipes.o redirection.o signals.o -lreadline -g

yash.o: yash.c
	  gcc yash.c -c

fork.o: fork.c
	  gcc fork.c -c

jobs.o: jobs.c
	  gcc jobs.c -c

pipes.o: pipes.c
	  gcc pipes.c -c

redirection.o: redirection.c
	  gcc redirection.c -c

signals.o: signals.c
	  gcc signals.c -c

clean:
	  rm yash *.o