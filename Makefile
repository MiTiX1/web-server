CC = gcc
EXEC = web-server
RM = rm -rf
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) -o $@ $^
	
%.o: %.c 
	$(CC) -o $(EXEC) $@ $<

clean:
	$(RM) $(OBJ)

mrproper:
	$(RM) $(EXEC)

run:
	./$(EXEC)