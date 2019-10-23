INC_DIR= -I. -I../userapi
#SRC_DIR = ./deamon ./face ./common ./table 
OBJ_DIR = ./objs

#SRC=$(wildcard $(SRC_DIR)/*.cpp)   

SRC= ./client.cpp ../userapi/edata.cpp ../userapi/eface.cpp \
	 ../userapi/einterest.cpp 
SRC2 = ./server.cpp ../userapi/edata.cpp ../userapi/eface.cpp \
	 ../userapi/einterest.cpp 
OBJS := $(patsubst %.cpp, %.o,$(SRC))
OBJ2 := $(patsubst %.cpp, %.o,$(SRC2))


CC := g++ 
CFLAGS := -g -std=c++14 ${INC_DIR}
LFLAGS := -lpthread -ljsoncpp
TARGET := client 

all: ${TARGET} server

$(TARGET): ${OBJS}
	$(CC) ${OBJS} -o ${TARGET}  $(LFLAGS)

server : ${OBJ2}
	$(CC) ${OBJ2} -o server $(LFLAGS)

${OBJS} : %.o: %.cpp %.h
	${CC} ${CFLAGS} -c $< -o $@
	@#echo $^

server.o : %.o: %.cpp %.h
	${CC} ${CFLAGS} -c $< -o $@
	@#echo $^

#$@  表示目标文件
#$^  表示所有依赖文件
#$<  表示第一个依赖文件
#$?  表示比目标新的依赖文件列表

clean:  
	rm -rf $(TARGET) *.o ${OBJS} server

test:
	echo $(SRC)
	echo $(OBJS)
	echo $(CFLAGS)
