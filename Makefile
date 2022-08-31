objects = tmva_train.o
libs = -lRooFitCore -lRooFitMore -lRooFit
flags = $(CFLAGS) $(CPPFLAGS)
root_args = `root-config --cflags --evelibs`

tmva : $(objects)
	cc -o tmva $(objects)

$(objects) : tmva_train.cc tmva_train.h
	cc -c $(libs) $(flags) $(root_args)

clean :
	rm $(tmva_objects)
