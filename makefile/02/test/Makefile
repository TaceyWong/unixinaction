#版本2： 引入变量   V0.0.2
SRCS := main.c
DEST := builtin.o
CFLAGS := -I../include 


$(DEST) : ${SRCS:%.c=%.o}
	$(LD) -r $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean

clean: 
	$(RM) *.o $(DEST)
	@echo "dir:test clean done"
