CC 	?= gcc
AR	:= ar -cvq
MKDIR	:= mkdir -p
RM	:= rm -rf
QUIET	?= @

SRCDIR	:= src
INCDIR	:= include
OBJDIR	:= obj
LIBDIR	:= lib
TSTDIR	:= t
KSTRDIR := kstring
KSTRSRC := $(KSTRDIR)/src
KSTRINC := $(KSTRDIR)/include
VPATH	:= $(SRCDIR) $(INCDIR) $(KSTRSRC)

SOFLAGS		?= -shared
SHAREDFLAGS	?= -fPIC
KGILIB		:= -L$(LIBDIR) -lkgi -Wl,-rpath=$(PWD)/$(LIBDIR)

CPPFLAGS	?= -I$(INCDIR) -I$(KSTRDIR)
CFLAGS		?= -Wall -Wextra -pedantic

TEST.c		:= $(shell find $(TSTDIR) -name '*.c')
TEST.cgi 	:= $(TEST.c:%.c=%.cgi)
KGIM		:= kgi kgi-cookie kgi-data kgi-header kgi_html arraylist \
		   kstring
KGIO		:= $(addprefix $(OBJDIR)/,$(addsuffix .o,$(KGIM)))

all: libkgi test
debug: CC += -g -DDEBUG
debug: all

libkgi: $(LIBDIR)/libkgi.so $(LIBDIR)/libkgi.a

$(LIBDIR)/lib%.so: CPPFLAGS += $(SHAREDFLAGS)
$(LIBDIR)/lib%.so:
	$(QUIET)$(MKDIR) $(LIBDIR)
	$(CC) $(CFLAGS) $(SOFLAGS) -o $@ $^
$(LIBDIR)/lib%.a:
	$(QUIET)$(MKDIR) $(LIBDIR)
	$(AR) $@ $^
$(LIBDIR)/libkgi.so: $(KGIO)
$(LIBDIR)/libkgi.a: $(KGIO)

$(OBJDIR)/%.o: %.c
	$(QUIET)$(MKDIR) $(OBJDIR)
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(OBJDIR)/kgi.o: kgi.c kgi.h arraylist.h
$(OBJDIR)/kgi-cookie.o: kgi-cookie.c kgi.h arraylist.h
$(OBJDIR)/kgi-data.o: kgi-data.c kgi.h arraylist.h
$(OBJDIR)/kgi-header.o: kgi-header.c kgi.h arraylist.h
$(OBJDIR)/kgi_html.o: kgi_html.c kgi/html.h arraylist.h
$(OBJDIR)/arraylist.o: arraylist.c arraylist.h
$(OBJDIR)/kstring.o: kstring.c $(KSTRINC)/kstring.h

test: $(TEST.cgi)
%.cgi: %.c kgi.h
	$(QUIET)$(MKDIR $(OBJDIR))
	$(CC) $(CFLAGS) $(CPPFLAGS) $(KGILIB) -o $@ $<

clean: cleantest
	$(RM) $(OBJDIR)
	$(RM) $(LIBDIR)

cleantest:
	$(RM) $(TSTDIR)/*.cgi
