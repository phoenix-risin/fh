LIB_PATH = -L../include/
LIB = -lircclient -lsqlite3
CFLAGS = -O3 -s

Bot: main.o irc.o log.o
	gcc ${CFLAGS} -o $@ src/main.o src/irc.o src/log.o -Linclude/ $(LIB)
main.o: src/main.cpp
	gcc ${CFLAGS} -o src/$@ -c src/main.cpp $(LIB_PATH) $(LIB)
irc.o: src/irc.cpp
	gcc ${CFLAGS} -o src/$@ -c src/irc.cpp $(LIB_PATH) $(LIB)
log.o: src/log.cpp
	gcc ${CFLAGS} -o src/$@ -c src/log.cpp $(LIB_PATH) $(LIB)

