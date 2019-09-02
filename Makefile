NAME = libbase64

SRCDIR = src
INCDIR = include

OBJDIR = obj
OUTDIR = bin
DIRS = $(OBJDIR) $(OUTDIR)

CFLAGS += -std=gnu99 -pedantic -Wall -I$(INCDIR)

ifeq (1,$(DEBUG))
CFLAGS += -g
else
CFLAGS += -O2
endif

# TODO -fPIC for static libraries?
# CFLAGS += -fPIC
CFLAGS += $(DEFINES)

SOURCES = $(wildcard $(SRCDIR)/*.c)

OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

BIN = $(OUTDIR)/$(NAME).a


.PHONY: all clean default

default: $(BIN)
all: default

$(BIN): % : $(OBJECTS) | $(OUTDIR)
	ar rcs $@ $^

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(DIRS):
	mkdir -p $@

clean::
	rm -rf $(DIRS)
