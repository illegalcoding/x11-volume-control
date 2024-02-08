CFLAGS = -O2 -pipe
all:
	$(CC) $(CFLAGS) -o x11-volume-control x11-volume-control.c `pkg-config --cflags --libs x11`
.PHONY.: clean
clean:
	rm x11-volume-control
