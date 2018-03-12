CC = gcc
SRC = menu.c
OBJ = menu.o
LIBMENU = menu.a
EST = ar rv
RAN = ranlib

all: static 

$(OBJ): $(SRC) 
		$(CC) -c $(SRC)

static: $(OBJ) 
		$(EST) $(LIBMENU) $(OBJ)
		$(RAN) $(LIBMENU)






