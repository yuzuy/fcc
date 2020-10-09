CFLAGS=-std=c11 -g -static

fcc: fcc.c

.PHONY: test
test: fcc
	@chmod a+x test.sh
	./test.sh

.PHONY: test.on-linux
test.on-linux:
	docker run --rm -it -v /Users/aoimoon/prog/2020/c/fcc:/fcc -w /fcc compilerbook make test

.PHONY: clean
clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: on-linux
on-linux:
	docker run --rm -it -v /Users/aoimoon/prog/2020/c/fcc:/fcc -w /fcc compilerbook /bin/bash
