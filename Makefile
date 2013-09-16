homework_number = 1
executables = objput objget objlist objsetacl
flags = -Wall
includes = -I include -I .
boost_libraries = -l boost_system -l boost_filesystem -l boost_regex

build : $(executables)

objput : objput.c++ objstore.h
	c++ $(flags) $(includes) -o $@ $@.c++ $(boost_libraries)
objget : objget.c++ objstore.h
	c++ $(flags) $(includes) -o $@ $@.c++ $(boost_libraries)
objlist : objlist.c++ objstore.h
	c++ $(flags) $(includes) -o $@ $@.c++ $(boost_libraries)
objsetacl : objsetacl.c++ objstore.h
	c++ $(flags) $(includes) -o $@ $@.c++ $(boost_libraries)

.PHONY : test exec clean cleanstore archive

test : build

exec : build

clean :
	rm -vf $(executables) *.core *.o

cleanstore :
	rm -vrf data

archive :
	git archive -v -o ../dtagatac.$(homework_number).tar \
		    --prefix=dmt2150.$(homework_number)/ HEAD
