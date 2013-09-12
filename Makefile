executables = objput objget objlist
flags = -Wall
boost_static_libraries = lib/libboost_system.a lib/libboost_filesystem.a

build : $(executables)

objput : objput.c++ $(boost_static_libraries)
	c++ $(flags) -o $@ objput.c++ $(boost_static_libraries)
objget : objget.c++ $(boost_static_libraries)
	c++ $(flags) -o $@ objget.c++ $(boost_static_libraries)
objlist : objlist.c++ $(boost_static_libraries)
	c++ $(flags) -o $@ objlist.c++ $(boost_static_libraries)

.PHONY : test exec clean

test : build

exec : build

clean :
	rm -f $(executables) *.core *.o
