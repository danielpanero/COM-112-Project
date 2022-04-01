# Definitions de macros
CXX     = g++
CXXFLAGS = -g -Wall -std=c++11 -I ./
CXXFILES = project.cc simulation.cc squarecell.cc error_squarecell.cc entities/anthill.cc entities/ants.cc entities/food.cc message.cc
OFILES = project.o simulation.o squarecell.o error_squarecell.o entities/anthill.o entities/ants.o entities/food.o message.o

all: $(OFILES)
	$(CXX) ${CXXFLAGS} $(OFILES) -o project

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' makefile && \
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
project.o: project.cc simulation.h entities/anthill.h entities/ants.h \
 constantes.h squarecell.h entities/food.h
simulation.o: simulation.cc entities/anthill.h entities/ants.h \
 constantes.h squarecell.h entities/ants.h entities/food.h message.h \
 simulation.h
squarecell.o: squarecell.cc error_squarecell.h squarecell.h
error_squarecell.o: error_squarecell.cc error_squarecell.h
anthill.o: entities/anthill.cc message.h squarecell.h entities/anthill.h \
 entities/ants.h constantes.h
ants.o: entities/ants.cc constantes.h message.h squarecell.h \
 entities/ants.h
food.o: entities/food.cc message.h squarecell.h entities/food.h
message.o: message.cc message.h
