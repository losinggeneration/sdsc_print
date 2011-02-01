CC=gcc

TARGET=smscheckhead
OBJS=smscheckhead.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

%.o: %.c
	$(CC) -c $<

clean:
	rm -f $(TARGET) *.o *~

