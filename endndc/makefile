# create by wgh
INC_DIR= -I.
SRC_DIR = ./
OBJ_DIR = ./objs

#SRC=$(wildcard $(SRC_DIR)/*.cpp)   

SRC=  ./endndc.cpp

OBJS := $(patsubst %.cpp, %.o,$(SRC))

CC := g++ 
CFLAGS := -g -std=c++14 ${INC_DIR}
LFLAGS := -lpthread -ljsoncpp
TARGET := endndc # name of the file where the main function in . 

all: ${TARGET}


$(TARGET): ${OBJS}
	$(CC) ${OBJS} -o ${TARGET}  $(LFLAGS)

${OBJS} : %.o: %.cpp %.h
	${CC} ${CFLAGS} -c $< -o $@
	@#echo $^


#$@  表示目标文件
#$^  表示所有依赖文件
#$<  表示第一个依赖文件
#$?  表示比目标新的依赖文件列表

clean:  
	rm -rf $(TARGET) *.o ${OBJS} endndc/endndc

test:
	echo $(SRC)
	echo $(OBJS)
	echo $(CFLAGS)
