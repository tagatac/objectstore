homework_number = 1
compile = c++
executables = objput objget objlist objtestacl
executable_dependencies = common.o Object.o RegexConstraint.o
flags = -Wall
boost_libraries = -l boost_system -l boost_filesystem -l boost_regex
tclap_version = tclap-1.2.1
gtest_version = gtest-1.7.0
prefix = $(shell pwd)

build : $(executables)

objput : objput.cpp $(executable_dependencies)
	$(compile) $(flags) -o $@ $@.cpp $(executable_dependencies) $(boost_libraries)
objget : objget.cpp $(executable_dependencies)
	$(compile) $(flags) -o $@ $@.cpp $(executable_dependencies) $(boost_libraries)
objlist : objlist.cpp RegexConstraint.o
	$(compile) $(flags) -I include -o $@ $@.cpp RegexConstraint.o $(boost_libraries)
objtestacl : objtestacl.cpp $(executable_dependencies)
	$(compile) $(flags) -I include -o $@ $@.cpp $(executable_dependencies) $(boost_libraries)
tester : tester.o $(executable_dependencies)
	$(compile) $(flags) -I include -o $@ $@.o common.o RegexConstraint.o $(boost_libraries) lib/libgtest.a

common.o : common.cpp common.h RegexConstraint.h tclap
	$(compile) $(flags) -I include -c common.cpp
RegexConstraint.o : RegexConstraint.cpp RegexConstraint.h tclap
	$(compile) $(flags) -I include -c RegexConstraint.cpp
tester.o : tester.cpp common.h tclap gtest
	$(compile) $(flags) -I include -c tester.cpp

tclap : $(tclap_version).tar.gz
	rm -vrf $@
	tar -xzvf $(tclap_version).tar.gz
	mv -v $(tclap_version) $@
	cd $@; ./configure --prefix=$(prefix); make -i; make -i install

gtest : $(gtest_version).zip
	rm -vrf $@
	unzip $(gtest_version).zip
	mv -v $(gtest_version) $@
	cd $@; ./configure --prefix=$(prefix); make -i; make -i install

.PHONY : build test exec clean cleanstore cleanlibs archive
test : tester
	./tester
exec : build
clean :
	rm -vf $(executables) tester *.core *.o
cleanstore :
	rm -vrf data
cleanlibs :
	rm -vrf include lib share tclap gtest
archive :
	git archive -v -o ../dtagatac.$(homework_number).tar \
		    --prefix=dmt2150.$(homework_number)/ HEAD
