# Definitions de macros
CXX     = g++
CXXFLAGS = -g -Wall -std=c++11 -I ./
CXXFILES = project.cc simulation.cc squarecell.cc error_squarecell.cc anthill.cc ants.cc food.cc message.cc
OFILES = project.o simulation.o squarecell.o error_squarecell.o anthill.o ants.o food.o message.o

all: $(OFILES)
	$(CXX) ${CXXFLAGS} $(OFILES) -o project

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -rf *.o *.x *.cc~ *.h~ project
	@/bin/find . -name '*.o' -delete

#
# -- Regles de dependances generees automatiquement
#
# DO NOT DELETE THIS LINE
project.o: project.cc simulation.h anthill.h ants.h constantes.h \
 squarecell.h food.h
simulation.o: simulation.cc anthill.h ants.h constantes.h squarecell.h \
 food.h message.h simulation.h
squarecell.o: squarecell.cc error_squarecell.h squarecell.h
error_squarecell.o: error_squarecell.cc error_squarecell.h
anthill.o: anthill.cc message.h squarecell.h anthill.h ants.h \
 constantes.h
ants.o: ants.cc constantes.h message.h squarecell.h ants.h
food.o: food.cc message.h squarecell.h food.h
message.o: message.cc message.h
