#top crustal heat directory
dcru=../..

#get configuration from the top directory config file
include $(dcru)/config.mk

#linking to libode
Iode=-I$(dcru)/$(odepath)/src -L$(dcru)/$(odepath)/bin -lode

#linking to libcrustalheat
Icru=-I$(dcru)/src -L$(dcru)/bin -lcrustalheat

all: crustalheatmake impact_layer.exe

#rule for jumping to the libode makefile
crustalheatmake:
	$(MAKE) -C $(dcru)

impact_layer.o: impact_layer.cc impact_layer.h crustalheatmake
	$(cxx) $(flags) -o $@ -c $< $(Icru) $(Iode)

impact_layer.exe: main.cc impact_layer.o
	$(cxx) $(flags) $(omp) -o $@ $< impact_layer.o $(Icru) $(Iode)
