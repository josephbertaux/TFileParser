SHELL=/bin/bash

TARGET = tmva

SRC_DIR = ./src
OBJ_DIR = ./bld

CC = g++

OBJS = $(OBJ_DIR)/example.o $(OBJ_DIR)/TDatasetParser.o

FLAGS = `root-config --cflags`
#INCS = -I`root-config --incdir` #already included with root-config --cflags
LIBS = `root-config --evelibs` -lTMVA -lTMVAGui -lRooFitCore -lRooFitMore -lRooFit

example : $(OBJS)
	$(CC) $(FLAGS) $(INCS) -o example $(OBJS) $(LIBS)

$(OBJ_DIR)/example.o :
	$(CC) $(FLAGS) $(INCS) -c $(SRC_DIR)/example.cc $(LIBS)
	mv example.o $(OBJ_DIR)/example.o

$(OBJ_DIR)/TDatasetParser.o :# $(SRC_DIR)/TDatasetParser.cc $(SRC_DIR)/TDatasetParser.h
	$(CC) $(FLAGS) $(INCS) -c $(SRC_DIR)/TDatasetParser.cc $(LIBS)
	mv TDatasetParser.o $(OBJ_DIR)/TDatasetParser.o

.PHONY : clean
clean :
	-rm -rf tmva $(OBJS)
