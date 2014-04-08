CC 	:= gcc

SRCDIR	:= src
INCDIR	:= include
TSTDIR	:= t
VPATH	:= $(SRCDIR) $(INCDIR) $(TSTDIR)

SOFLAGS		:= -shared
SHAREDFLAGS	:= -fPIC
KGILIB		:= -L. -lkgi -Wl,-rpath=$(PWD)

CPPFLAGS	:= -I$(INCDIR)
CFLAGS		:= -Wall -Wextra -pedantic

TEST.cgi := ok.cgi

all: libkgi.so test

libkgi.so: kgi.o kgi-cookie.o kgi-data.o kgi-header.o arraylist.o
	$(CC) $(CFLAGS) $(SOFLAGS) -o $@ $^
kgi.o: $(CFLAGS) += $(SHAREDFLAGS)
kgi.o: kgi.c kgi.h arraylist.h
kgi-cookie.o: kgi-cookie.c kgi.h arraylist.h
kgi-data.o: kgi-data.c kgi.h arraylist.h
kgi-header.o: kgi-header.c kgi.h arraylist.h
arraylist.o: arraylist.c arraylist.h

test: $(TEST.cgi)
%.cgi: %.c kgi.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $(KGILIB) -o $(TSTDIR)/$@ $^

clean: cleantest
	$(RM) *.o
	$(RM) *.so

cleantest:
	$(RM) $(TSTDIR)/*.cgi
