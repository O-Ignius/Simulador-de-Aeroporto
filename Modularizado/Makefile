CC = gcc
CFLAGS = -Wall

OBJS = main.o controle.o lista.o relatorio.o
TARGET = Aeroporto

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c controle.h lista.h relatorio.h
	$(CC) $(CFLAGS) -c main.c

lista.o: lista.c lista.h
	$(CC) $(CFLAGS) -c lista.c

controle.o: controle.c controle.h relatorio.h lista.h
	$(CC) $(CFLAGS) -c controle.c

relatorio.o: relatorio.c relatorio.h lista.h
	$(CC) $(CFLAGS) -c relatorio.c

clean:
	rm -f $(OBJS) $(TARGET)
