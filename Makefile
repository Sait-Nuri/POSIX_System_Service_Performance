SRCDIR := src
subdirs := $(wildcard $(SRCDIR)/*/)

all:
	for dir in $(subdirs); do \
		(cd $$dir; ${MAKE} all); \
	done