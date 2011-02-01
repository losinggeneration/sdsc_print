CC=sdcc
Z80CFLAGS=-mz80 --oldralloc
AS=sdasz80
BIN=makebin
SMS=../tools/smscheckhead/smscheckhead
# We set the data location (for globals and statics) in the middle of the RAM
LDFLAGS=--no-std-crt0 --data-loc 0xD000

TARGET=sdsc_print
SRCS=main.c utils.c

DEPDIR = .deps
DEPS=$(SRCS:%.c=$(DEPDIR)/%.d)

# Remember, crt0.rel must always be first
OBJS=crt0.rel $(SRCS:%.c=%.rel)

all: $(DEPS) $(OBJS)
	$(CC) $(Z80CFLAGS) $(LDFLAGS) -o $(TARGET).ihx $(OBJS)
	$(BIN) $(TARGET).ihx $(TARGET).sms
	$(BIN) -p $(TARGET).ihx $(TARGET).bin
	$(SMS) -f $(TARGET).sms

%.rel: %.c
	$(CC) $(Z80CFLAGS) $(CFLAGS) -c $<

%.rel: %.s
	$(AS) -o $@ $<

$(DEPDIR)/%.d: %.c
	@mkdir -p  $(DEPDIR)
	@$(CC) -M $(Z80CFLAGS) $(CFLAGS) $< > $(DEPDIR)/$*.d

clean:
	rm -f $(TARGET).bin* $(TARGET).sms* *~ *.lk *.rel *.sym *.lst *.asm *.noi *.map *.lnk *.ihx $(DEPS)

-include $(DEPS)
