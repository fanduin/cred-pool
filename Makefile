CC := clang
OFLAGS := -fPIC
SOFLAGS :=
LFLAGS := -pthread

LIB := credpool
OBJS := credpool.c

$(LIB).o: $(OBJS)
	$(CC) $(CFLAGS) -o $@ -c $^ $(LFLAGS)

lib: $(LIB).o
	$(CC) -shared -o lib$(LIB).so $^

install: lib
	cp $(LIB).h /usr/include
	cp lib$(LIB).so /usr/lib

uninstall:
	rm /usr/include/$(LIB).h
	rm /usr/lib/lib$(LIB).so

.PHONY: clean
clean:
	rm -rf *.o
