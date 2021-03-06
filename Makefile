src	:= $(shell find ./ -name "*.c")
exec := $(src:%.c=%.out)

.PHONY: clean all

all: $(exec)

%.out: %.c
	gcc -O3 $< -o $@ -pthread -I./include -lm

clean:
	rm $(exec)
