#configuration file
include config.mk

#-------------------------------------------------------------------------------
#local directories

#source code directory
dirs=src
#built object directory
diro=obj
#built executable directory
dirb=bin
#flags to include libode
odesrc=-I$(odepath)/src
odelib=-L$(odepath)/bin -lode

#-------------------------------------------------------------------------------
#stuff to compile

#independent objects to compile
obj=$(diro)/io.o $(diro)/util.o $(diro)/settings.o

#model object
mod=$(diro)/grid.o $(diro)/heat.o

#default targets
all: libodemake $(dirb)/libcrustalheat.a $(dirb)/crustal_heat.exe $(dirb)/crustal_heat_test.exe

#-------------------------------------------------------------------------------
#compilation rules

libodemake:
	$(MAKE) -C $(odepath)


$(obj): $(diro)/%.o: $(dirs)/%.cc $(dirs)/%.h
	$(cxx) $(flags) -o $@ -c $< -I$(dirs)

$(diro)/grid.o: $(dirs)/grid.cc $(dirs)/grid.h $(diro)/io.o
	$(cxx) $(flags) -o $@ -c $< -I$(dirs)

$(diro)/heat.o: $(dirs)/heat.cc $(dirs)/heat.h $(obj) $(diro)/grid.o
	$(cxx) $(flags) -o $@ -c $< -I$(dirs) $(odesrc) $(odelib)


$(dirb)/libcrustalheat.a: $(obj) $(mod)
	ar r $(dirb)/libcrustalheat.a $(obj) $(mod)


$(dirb)/crustal_heat.exe: $(dirs)/main.cc $(obj) $(mod)
	$(cxx) $(flags) -o $@ $< $(obj) $(mod) -I$(dirs) $(odesrc) $(odelib)

$(dirb)/crustal_heat_test.exe: $(dirs)/main_test.cc $(obj) $(mod)
	$(cxx) $(flags) -o $@ $< $(obj) $(mod) -I$(dirs) $(odesrc) $(odelib)



.PHONY : clean
clean:
	rm obj/* bin/*
