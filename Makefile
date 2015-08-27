# catpoint

LDLIBS = -lncursesw

BIN = catpoint
all: $(BIN)

clean:
	rm -f $(BIN)
