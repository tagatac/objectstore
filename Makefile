#executables = objput objget objlist
executables = objput

all : $(executables)

objput : objput.c++
	c++ -o $@ objput.c++ -L lib -l boost_system -l boost_filesystem -static
objget : objget.c
	c++ -o $@ objget.c
objlist : objlist.c
	c++ -o $@ objlist.c

.PHONY : clean
clean :
	rm -f $(executables) *.core *.o
