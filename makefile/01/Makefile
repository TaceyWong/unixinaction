#版本2： 引入变量   V0.0.2
SRCS := lib_a.c lib_b.c main.c
DEST := test
CFLAGS :=  -I./include 


$(DEST) : ${SRCS:%.c=%.o}
	$(CC) $(CFLAGS) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean

clean: 
	$(RM) *.o $(DEST)
