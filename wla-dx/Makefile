AS=wla-z80
LD=wlalink

AFLAGS=
LDFLAGS=-d -S -v

SRCS=main.asm
OBJS=$(SRCS:%.asm=%.o)
TARGET=sdsc_print.sms

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) linkfile $@

%.o: %.asm
	$(AS) $(AFLAGS) -o $@ $<

# main dependencies
main.o: $(main_o_DEPS)

clean:
	rm -f $(TARGET) $(OBJS) *.sym

