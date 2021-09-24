test: main
	./main

main: main.c
	gcc -Wall $^ -o $@ $(shell pkg-config --cflags --libs gstreamer-1.0 gstreamer-audio-1.0)

clean:
	rm -rf test.ts main

.PHONY: test clean
