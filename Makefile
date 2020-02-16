src	:= $(shell find ./ -name "*.c")
exec := $(src:%.c=%.out)

.PHONY: clean all

all: $(exec)

%.out: %.c
	gcc -O0 $< -o $@ -pthread

clean:
	rm *.out
