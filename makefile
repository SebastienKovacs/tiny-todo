SRCDAY = src/select_day.c src/select_day.h
SRCCDAY = src/select_day.c

SRCSQL = src/postgres.c src/postgres.h
SRCCSQL = src/postgres.c

SRCALLOC = src/special_allocs.c src/special_allocs.h
SRCCALLOCS = src/special_allocs.c

SRCGUI = src/gui.c
SRCCGUI = src/gui.c

SRC = $(SRCGUI) $(SRCDAY) $(SRCSQL) $(SRCALLOCS)  
SRCC = $(SRCCGUI) $(SRCCDAY) $(SRCCSQL) $(SRCCALLOCS)


CC = gcc

DEBUG = -g
ERR = -Wall -Wextra -pedantic
CFLAGS = $(DEBUG) $(ERR)

GTK = `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -rdynamic
PSQL = -I/usr/include/postgresql -L/usr/lib/postgresql/9.6/lib -lpq
LIBS = $(GTK) $(PSQL)

tiny-todo: $(SRC)
	$(CC) $(CFLAGS) $(LIBS) $(SRCC) -o tiny-todo
	@echo ""
	@echo "================"
	@echo "BUILD SUCCESSFUL"
	@echo "================"
	@echo ""

install:
	cp src/tiny-todo.desktop /usr/share/applications/tiny-todo.desktop
	cp tiny-todo /bin/tiny-todo
	cp src/glade/tiny-todo.glade /etc/tiny-todo/tiny-todo.glade
	cp src/icon.png /etc/tiny-todo/icon.png
	
clean:
	rm tiny-todo

fresh:
	make clean; make