all:
	g++ -lpthread -pthread -O3 -mssse3 -malign-double -g -o affine affine.c `pkg-config opencv --cflags --libs`

clean:
	rm affine
