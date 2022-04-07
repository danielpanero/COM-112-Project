CXX = g++
PROGRAM = projet
CXXFILES = projet.cc simulation.cc squarecell.cc error_squarecell.cc anthill.cc \
ants.cc food.cc message.cc gui.cc
OBJS = $(CXXFILES:.cc=.o)
DEPDIR = .deps
CXXFLAGS = `pkg-config --cflags gtkmm-3.0` -g -Wall -std=c++11
LIBS = `pkg-config --libs gtkmm-3.0`

all: $(PROGRAM)

-include $(OBJS:%.o=$(DEPDIR)/%.Po)

%.o: %.cc
	@mkdir -p $(DEPDIR)
	$(CXX) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $(CXXFLAGS) $<
	@mv -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po

$(PROGRAM): $(OBJS)
	$(CXX) -o $(PROGRAM) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS)
	rm -f $(PROGRAM)