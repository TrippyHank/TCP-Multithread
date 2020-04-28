CHECK = \r\033[32m\xE2\x9C\x94\e[0m

PROGS = bin client server thread multithread

all: $(PROGS)

bin:
	mkdir -p bin

client:
	printf "+ $@.c .. "
	$(CC) $(CFLAGS) src/$@.c -o bin/$@
	printf "$(CHECK) $@.c : $@\n"

server:
	printf "+ $@.c .. "
	$(CC) -pthread $(CFLAGS) src/$@.c -o bin/$@
	printf "$(CHECK) $@.c : $@\n"

clean:
	printf "Cleaning.."
	rm -rf bin
	printf "$(CHECK) Everything has been duly cleaned up.\n"

.PHONY: $(PROGS) clean
.SILENT: $(PROGS) clean
