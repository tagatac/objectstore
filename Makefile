executables = objput objget objlist

all : $(executables)

objput : objput.c
	cc -o $@ objput.c
objget : objget.c
	cc -o $@ objget.c
objlist : objlist.c
	cc -o $@ objlist.c

.PHONY : clean
clean :
	rm -f $(executables) *.core *.o
