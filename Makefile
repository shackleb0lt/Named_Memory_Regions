CFLAGS = -Wall -Wextra
CC = gcc 
PROG = regions
HEADER =  linked_list.h regions.h  
SOURCE = linked_list.c regions.c  main.c
OBJS = regions.o linked_list.o main.o

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROG)
%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -f $(PROG) $(OBJS)