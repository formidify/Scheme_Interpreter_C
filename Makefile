CC = clang
CFLAGS = -g

#modified make by Chae. May want to check

SRCS = linkedlist.c talloc.c vector.c main_tokenize.c tokenizer.c main_parser.c parser.c
HDRS = vector.h linkedlist.h value.h talloc.h tokenizer.h parser.h
OBJS = $(SRCS:.c=.o)

parser: $(OBJS)
	$(CC)  $(CFLAGS) $^  -o $@

tokenizer: $(OBJS)
	$(CC)  $(CFLAGS) $^  -o $@

linkedlist: $(OBJS)
	$(CC)  $(CFLAGS) $^  -o $@

memtest: linkedlist
	valgrind --leak-check=full --show-leak-kinds=all ./$<

%.o : %.c $(HDRS)
	$(CC)  $(CFLAGS) -c $<  -o $@

clean:
	rm *.o
	rm linkedlist
	rm tokenizer
	rm parser
