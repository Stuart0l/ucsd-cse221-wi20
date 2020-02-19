src	:= $(shell find ./ -name "*.c")
exec := $(src:%.c=%.out)

.PHONY: clean all

all: $(exec)

%.out: %.c
	gcc $< -o $@ -pthread -I./include

clean:
	rm $(exec)
