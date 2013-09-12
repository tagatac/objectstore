#executables = objput objget objlist
executables = objput

all : $(executables)

objput : objput.c++
	c++ -o $@ objput.c++
objget : objget.c
	c++ -o $@ objget.c
objlist : objlist.c
	c++ -o $@ objlist.c

.PHONY : clean
clean :
	rm -f $(executables) *.core *.o
