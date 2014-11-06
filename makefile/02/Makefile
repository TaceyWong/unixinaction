#版本2： 引入变量   V0.0.2
SUB_DIRS := test lib
BUILTIN := builtin.o
DEST := main
#-g: enable gdb
#-O2: 开启优化器
CFLAGS :=  -I./include -g -O2

$(DEST) : ${SUB_DIRS:=/$(BUILTIN)}
	$(CC) $(CFLAGS) $^ -o $@

${SUB_DIRS:=/$(BUILTIN)} :
	$(MAKE)	-C $(@:/$(BUILTIN)=)

.PHONY: clean

clean:
	for dir in $(SUB_DIRS);do \
	$(MAKE) -C $$dir clean;	done 
	$(RM) $(DEST)
	@echo "clean all done"
