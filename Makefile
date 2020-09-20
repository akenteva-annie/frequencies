# CC=gcc
# CFLAGS="-I -lstdc++"

make:
	gcc -o freq main.cpp -lstdc++ -g -O0 #-D_GLIBCXX_DEBUG

check: make
	./freq test

clean:
	rm -f freq
