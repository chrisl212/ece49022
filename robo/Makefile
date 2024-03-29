TARGET=robo

CC=arm-none-eabi-gcc
CP=arm-none-eabi-objcopy
SF=st-flash

INCLUDE=-Isrc/ -Iinc/
CFLAGS = -std=gnu99 -g3 -Wall -Wno-unused-variable -pedantic
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -mcpu=cortex-m0
CFLAGS += -fsingle-precision-constant --specs=nosys.specs -Wno-builtin-declaration-mismatch
LDFLAGS = -lc -static -mcpu=cortex-m0 -mthumb

SRCDIR = src
BUILDDIR = build
SSRC = $(shell find src/ -iname *.S)
SOBJ = $(patsubst $(SRCDIR)/%.S, $(BUILDDIR)/%.o, $(SSRC))
CSRC = $(shell find src/ -iname *.c)
COBJ = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(CSRC))

.PHONY: all

all: $(BUILDDIR) $(TARGET).bin
$(BUILDDIR):
	mkdir -p $(BUILDDIR)
$(COBJ): $(BUILDDIR)/%.o : $(SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@
$(SOBJ): $(BUILDDIR)/%.o : $(SRCDIR)/%.S
	mkdir -p $(@D)
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@
$(TARGET).bin: $(COBJ) $(SOBJ)
	$(CC) -o $(TARGET).elf -T scripts/link.ld $(COBJ) $(SOBJ) $(LDFLAGS) 
	$(CP) -O binary $(TARGET).elf $(TARGET).bin
debug: flash
	st-util > /dev/null 2>&1 &
	arm-none-eabi-gdb -ex "tar extended-remote :4242" $(TARGET).elf
	pkill st-util || true
clean:
	rm -rf build/ *.elf *.bin
flash: $(TARGET).bin
	pkill st-util || true
	$(SF) write $(TARGET).bin 0x8000000
