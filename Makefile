SHELL=/bin/bash

TARGET = tmva

SRC_DIR = ./src
OBJ_DIR = ./bld

CC = g++

OBJS = $(OBJ_DIR)/example.o $(OBJ_DIR)/TFileParser.o

FLAGS = `root-config --cflags`
#INCS = -I`root-config --incdir` #already included with root-config --cflags
LIBS = `root-config --evelibs` -lTMVA -lTMVAGui -lRooFitCore -lRooFitMore -lRooFit

example : $(OBJS)
	$(CC) $(FLAGS) $(INCS) -o example $(OBJS) $(LIBS)

$(OBJ_DIR)/example.o :
	$(CC) $(FLAGS) $(INCS) -c $(SRC_DIR)/example.cc $(LIBS)
	mv example.o $(OBJ_DIR)/example.o

$(OBJ_DIR)/TFileParser.o :# $(SRC_DIR)/TFileParser.cc $(SRC_DIR)/TFileParser.h
	$(CC) $(FLAGS) $(INCS) -c $(SRC_DIR)/TFileParser.cc $(LIBS)
	mv TFileParser.o $(OBJ_DIR)/TFileParser.o

.PHONY : clean
clean :
	-rm -rf tmva $(OBJS)
