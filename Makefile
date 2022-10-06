SHELL=/bin/bash

TRGT = tf_parse

SRC_DIR = ./src
OBJ_DIR = ./bld

CC = g++

OBJS = $(OBJ_DIR)/tf_parse.o $(OBJ_DIR)/TFPWrapper.o $(OBJ_DIR)/TFileParser.o 

FLAGS = `root-config --cflags`
#INCS = -I`root-config --incdir` #already included with root-config --cflags
LIBS = `root-config --evelibs` -lTMVA -lTMVAGui -lRooFitCore -lRooFitMore -lRooFit

$(TRGT) : $(OBJS)
	$(CC) $(FLAGS) $(INCS) -o $(TRGT) $(OBJS) $(LIBS)

$(OBJ_DIR)/tf_parse.o :
	$(CC) $(FLAGS) $(INCS) -c $(SRC_DIR)/tf_parse.cc $(LIBS)
	mv tf_parse.o $(OBJ_DIR)/tf_parse.o

$(OBJ_DIR)/TFPWrapper.o :
	$(CC) $(FLAGS) $(INCS) -c $(SRC_DIR)/TFPWrapper.cc $(LIBS)
	mv TFPWrapper.o $(OBJ_DIR)/TFPWrapper.o

$(OBJ_DIR)/TFileParser.o :
	$(CC) $(FLAGS) $(INCS) -c $(SRC_DIR)/TFileParser.cc $(LIBS)
	mv TFileParser.o $(OBJ_DIR)/TFileParser.o

.PHONY : clean
clean :
	-rm -rf $(TRGT) $(OBJS)
