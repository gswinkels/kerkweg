PROJ    =LSORT2
DEBUG   = 1
CC	=cl
CFLAGS_G        = /AC /W4 /DMSC /Lp /Fb
CFLAGS_D        = /Zi /qc
CFLAGS_R	= /O /Ot /DNDEBUG
# CFLAGS  =$(CFLAGS_G) $(CFLAGS_D)
CFLAGS =$(CFLAGS_G) $(CFLAGS_R)
LFLAGS_G        = /CP:0xffff /SE:0x80 /ST:0x2000
LFLAGS_D        = /CO /FAR /PACKC
LFLAGS_R	=
# LFLAGS  =$(LFLAGS_G) $(LFLAGS_D)
LFLAGS =$(LFLAGS_G)
RUNFLAGS	=
OBJS_EXT =
LIBS_EXT = /NOD:CLIBCE CLIBCEP

all:	$(PROJ).exe

lsort.obj:      lsort.c lsort.h

lsortcmp.obj:   lsortcmp.c  lsort.h

lsortdbf.obj:   lsortdbf.c  lsort.h

lsortio.obj:    lsortio.c   lsort.h

lsortprm.obj:   lsortprm.c  lsort.h

lsortqk.obj:    lsortqk.c   lsort.h

lsortsub.obj:   lsortsub.c  lsort.h

lssort.obj:     lssort.c    lsort.h

lsuser.obj:     lsuser.c    lsort.h

lsmerge.obj:    lsmerge.c   lsort.h

$(PROJ).exe:	lsort.obj lsortcmp.obj lsortdbf.obj lsortio.obj lsortprm.obj lsortqk.obj \
	lsortsub.obj lssort.obj lsuser.obj lsmerge.obj $(OBJS_EXT)
	echo >NUL @<<$(PROJ).crf
lsort.obj +
lsortcmp.obj +
lsortdbf.obj +
lsortio.obj +
lsortprm.obj +
lsortqk.obj +
lsortsub.obj +
lssort.obj +
lsuser.obj +
lsmerge.obj +
$(OBJS_EXT)
$(PROJ).exe

$(LIBS_EXT);
<<
	link $(LFLAGS) @$(PROJ).crf
        bind $(PROJ)

run: $(PROJ).exe
	$(PROJ) $(RUNFLAGS)

