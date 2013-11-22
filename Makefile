homework_number = 3
compile = c++
executables = objput objget objlist objsetacl objgetacl objtestacl
executable_dependencies = common.o Object.o RegexConstraint.o
flags = -Wall -std=c++0x
libraries = -l boost_system -l boost_filesystem -l boost_regex -l crypto
tclap_version = tclap-1.2.1
gtest_version = gtest-1.7.0
prefix = '$(shell pwd)'

build : $(executables)

objput : objput.cpp $(executable_dependencies)
	$(compile) $(flags) -o $@ $@.cpp $(executable_dependencies) $(libraries)
objget : objget.cpp $(executable_dependencies)
	$(compile) $(flags) -o $@ $@.cpp $(executable_dependencies) $(libraries)
objlist : objlist.cpp $(executable_dependencies)
	$(compile) $(flags) -I include -o $@ $@.cpp $(executable_dependencies) $(libraries)
objsetacl : objsetacl.cpp $(executable_dependencies)
	$(compile) $(flags) -o $@ $@.cpp $(executable_dependencies) $(libraries)
objgetacl : objgetacl.cpp $(executable_dependencies)
	$(compile) $(flags) -o $@ $@.cpp $(executable_dependencies) $(libraries)
objtestacl : objtestacl.cpp $(executable_dependencies)
	$(compile) $(flags) -I include -o $@ $@.cpp $(executable_dependencies) $(libraries)
tester : tester.o $(executable_dependencies)
	$(compile) $(flags) -I include -o $@ $@.o $(executable_dependencies) $(libraries) -l pthread lib/libgtest.a

common.o : common.cpp common.h Object.h RegexConstraint.h tclap
	$(compile) $(flags) -I include -c common.cpp
Object.o : Object.cpp Object.h common.h
	$(compile) $(flags) -c Object.cpp
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

.PHONY : build test install exec clean cleanstore cleanlibs archive
test : cleanstore tester
	./tester
install : build
	echo "Password"
	sudo rm -vrf /var/local/objectstore
	sudo mkdir -vp /var/local/objectstore/data
	sudo chmod -v g-s /var/local/objectstore/data
	sudo chmod -v 700 /var/local/objectstore/data
	sudo cp -v $(executables) /var/local/objectstore
	sudo chmod -v 711 /var/local/objectstore/obj*
	sudo chown -v tinyvm /var/local/objectstore/*
	sudo find /var/local/objectstore -type f -exec chmod -v u+s {} \;
exec : build
	@read -p "Please enter the command to run: " command; \
	./$$command
clean :
	rm -vf $(executables) tester *.core *.o
cleanstore :
	rm -vrf data
cleanlibs :
	rm -vrf include lib share tclap gtest
archive :
	git archive -v -o ../dtagatac.$(homework_number).tar \
		    --prefix=dmt2150.$(homework_number)/ HEAD
