# Definitions de macros
CXX     = g++
CXXFLAGS = -g -Wall -std=c++11
CXXFILES = project.cc simulation.cc squarecell.cc squarecell/error_squarecell.cc simulation/anthill.cc simulation/ants.cc simulation/food.cc shared/message.cc
OFILES = project.o simulation.o squarecell.o squarecell/error_squarecell.o simulation/anthill.cc simulation/ants.o simulation/food.o shared/message.o

all: $(OFILES)
	$(CXX) $(OFILES) -o project

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "/usr/include" \
	 ) >makefile.new
	@mv makefile.new makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -rf *.o *.x *.cc~ *.h~ project

#
# -- Regles de dependances generees automatiquement
#
# DO NOT DELETE THIS LINE
project.o: project.cc simulation.h
simulation.o: simulation.cc simulation/food.h simulation/../squarecell.h \
 simulation/anthill.h simulation/ants.h shared/message.h simulation.h
squarecell.o: squarecell.cc squarecell/error_squarecell.h squarecell.h
error_squarecell.o: squarecell/error_squarecell.cc \
 squarecell/error_squarecell.h
anthill.o: simulation/anthill.cc simulation/../squarecell.h \
 simulation/anthill.h
ants.o: simulation/ants.cc simulation/../squarecell.h \
 simulation/../shared/message.h simulation/ants.h
food.o: simulation/food.cc simulation/../squarecell.h \
 simulation/../shared/message.h simulation/food.h
message.o: shared/message.cc shared/message.h
