CFLAGS=-std=c11 -g -static

fcc: fcc.c

.PHONY: test
test: fcc
	@chmod a+x test.sh
	./test.sh

.PHONY: clean
clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: on-linux
on-linux:
	docker run --rm -it -v /Users/aoimoon/prog/c/fcc:/home/user/fcc -w /home/user/fcc compilerbook /bin/bash
