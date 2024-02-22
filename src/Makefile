LDFLAGS:= -lserialport -lubus -lubox -lblobmsg_json
CFILES:= $(wildcard *.c)

BIN:=esp-controler

all:$(BIN)

$(BIN):$(CFILES)
	$(CC) -o $@ $^ $(LDFLAGS)
clean:
	$(RM) $(BIN)

.PHONY: all clean
