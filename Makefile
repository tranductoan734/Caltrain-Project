CFLAGS?=-g -Wall -Wno-unused-value

all: caltrain 

caltrain: caltrain-runner.c caltrain.c pintos_thread.h
	$(CC) $(CFLAGS) -o caltrain caltrain-runner.c -lpthread 

reaction: reaction-runner.c reaction.c pintos_thread.h
	$(CC) $(CFLAGS) -o reaction reaction-runner.c -lpthread 

run: caltrain
	./caltrain
	./caltrain
	./caltrain
clean:
	$(RM) caltrain
	$(RM) -r *.dSYM
