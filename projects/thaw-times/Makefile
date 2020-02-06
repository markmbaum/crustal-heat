#top crustal heat directory
dcru=../..

#get configuration from the top directory config file
include $(dcru)/config.mk

#linking to libode
Iode=-I$(dcru)/$(odepath)/src -L$(dcru)/$(odepath)/bin -lode

#linking to libcrustalheat
Icru=-I$(dcru)/src -L$(dcru)/bin -lcrustalheat

all: crustalheatmake thaw_times.exe

#rule for jumping to the libode makefile
crustalheatmake:
	$(MAKE) -C $(dcru)

#link order might matter!
thaw_times.exe: main.cc crustalheatmake
	$(cxx) $(flags) $(omp) -o $@ $< $(Icru) $(Iode)
