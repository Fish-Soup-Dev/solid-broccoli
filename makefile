exe = perceptron.exe
sources = $(wildcard src/*.c)
flags = -o

build: $(sources)
	gcc $(sources) $(flags) $(exe)