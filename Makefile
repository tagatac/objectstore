homework_number = 1
executables = objput objget objlist objsetacl
flags = -Wall
includes = -I include -I .
boost_libraries = -l boost_system -l boost_filesystem

build : $(executables)

objput : objput.c++
	c++ $(flags) $(includes) -o $@ objput.c++ $(boost_libraries)
objget : objget.c++
	c++ $(flags) $(includes) -o $@ objget.c++ $(boost_libraries)
objlist : objlist.c++
	c++ $(flags) $(includes) -o $@ objlist.c++ $(boost_libraries)
objsetacl : objsetacl.c++
	c++ $(flags) $(includes) -o $@ $@.c++

.PHONY : test exec clean archive

test : build

exec : build

clean :
	rm -vf $(executables) *.core *.o

cleanstore :
	rm -vrf data

archive :
	git archive -v -o ../dtagatac.$(homework_number).tar \
		    --prefix=dmt2150.$(homework_number)/ HEAD
