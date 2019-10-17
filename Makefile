LDFLAGS = -lncurses -lpthread

BouncingBalls: BouncingBalls.c
	$(CC) -o $@ $^ $(LDFLAGS)

