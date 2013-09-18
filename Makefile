homework_number = 1
executables = objput objget objlist objtestacl
executable_dependencies = common.o Object.o RegexConstraint.o
flags = -Wall
boost_libraries = -l boost_system -l boost_filesystem -l boost_regex

build : $(executables)

objput : objput.cpp $(executable_dependencies)
	c++ $(flags) -o $@ $@.cpp $(executable_dependencies) $(boost_libraries)
objget : objget.cpp $(executable_dependencies)
	c++ $(flags) -o $@ $@.cpp $(executable_dependencies) $(boost_libraries)
objlist : objlist.cpp RegexConstraint.o
	c++ $(flags) -I include -o $@ $@.cpp RegexConstraint.o $(boost_libraries)
objtestacl : objtestacl.cpp $(executable_dependencies)
	c++ $(flags) -I include -o $@ $@.cpp $(executable_dependencies) $(boost_libraries)

common.o : common.cpp common.h RegexConstraint.h
	c++ $(flags) -I include -c common.cpp
RegexConstraint.o : RegexConstraint.cpp RegexConstraint.h
	c++ $(flags) -I include -c RegexConstraint.cpp

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
