CXX = g++
CXXFLAGS = -std=c++14 -g -Wall -MMD
OBJECTS = main.o board.o player.o textdisplay.o acedemic.o property.o nonproperty.o
game.o residence.o square.o gym.o gameexception.o timscup
DEPENDS = ${OBJECTS:.o=.d}
EXEC = watopoly


${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

clean :
	rm ${DEPENDS} ${OBJECTS} ${EXEC}

-include ${DEPENDS} # reads the .d files and reruns dependencies
