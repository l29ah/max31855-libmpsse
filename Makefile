CFLAGS += -D_DEFAULT_SOURCE -Wall -Wextra -std=c11
LDLIBS += -lmpsse

PROJECT=max31855-libmpsse

all: ${PROJECT}

${PROJECT}: ${PROJECT}.o

.PHONY:	clean

clean:
	rm -rf *.o
	rm -rf ${PROJECT}

