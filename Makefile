SHELL=/bin/bash

TARGET = tmva

SRC_DIR = ./src
OBJ_DIR = ./bld

CC = g++

OBJS = $(OBJ_DIR)/tmva_main.o $(OBJ_DIR)/tmva_train.o $(OBJ_DIR)/tmva_apply.o

FLAGS = `root-config --cflags`
#INCS = -I`root-config --incdir` #already included with root-config --cflags
LIBS = `root-config --evelibs` -lTMVA -lTMVAGui -lRooFitCore -lRooFitMore -lRooFit

tmva : $(OBJS)
	$(CC) $(FLAGS) $(INCS) -o tmva $(OBJS) $(LIBS)

$(OBJ_DIR)/tmva_main.o : $(SRC_DIR)/tmva_main.h
	$(CC) $(FLAGS) $(INCS) -c $(SRC_DIR)/tmva_main.cc $(LIBS)
	mv tmva_main.o $(OBJ_DIR)/tmva_main.o

$(OBJ_DIR)/tmva_train.o : $(SRC_DIR)/tmva_train.cc $(SRC_DIR)/tmva_train.h
	$(CC) $(FLAGS) $(INCS) -c $(SRC_DIR)/tmva_train.cc $(LIBS)
	mv tmva_train.o $(OBJ_DIR)/tmva_train.o

$(OBJ_DIR)/tmva_apply.o : $(SRC_DIR)/tmva_apply.cc $(SRC_DIR)/tmva_train.h
	$(CC) $(FLAGS) $(INCS) -c $(SRC_DIR)/tmva_apply.cc $(LIBS)
	mv tmva_apply.o $(OBJ_DIR)/tmva_apply.o

.PHONY : clean
clean :
	-rm -rf tmva $(OBJS)
