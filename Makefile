homework_number = 0
executables = objput objget objlist
flags = -Wall
boost_libraries = -l boost_system -l boost_filesystem

build : $(executables)

objput : objput.c++
	c++ $(flags) -o $@ objput.c++ $(boost_libraries)
objget : objget.c++
	c++ $(flags) -o $@ objget.c++ $(boost_libraries)
objlist : objlist.c++
	c++ $(flags) -o $@ objlist.c++ $(boost_libraries)

.PHONY : test exec clean archive

test : build

exec : build

clean :
	rm -vf $(executables) *.core *.o

archive :
	rm -vf 
	git archive -o ../dtagatac.$(homework_number).tar \
		    --prefix=dmt2150.$(homework_number)/ HEAD
