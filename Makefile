#SDCC=sdcc
#SDLD=sdld
SDCC=/var/projects/stm8/sdcc_compiled/bin/sdcc
SDLD=/var/projects/stm8/sdcc_compiled/bin/sdld
OBJECTS=main.ihx

.PHONY: all clean flash

all: $(OBJECTS)

clean:
	rm -f $(OBJECTS)

flash: $(OBJECT).ihx
	stm8flash -cstlinkv2 -pstm8s003 -w $(OBJECT).ihx 

%.ihx: %.c
	$(SDCC) -lstm8 -mstm8 --out-fmt-ihx --std-c11 $(CFLAGS) $(LDFLAGS) $<
