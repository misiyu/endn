INC_DIR= -I.
SRC_DIR = ./deamon ./face ./common ./table 
OBJ_DIR = ./objs

#SRC=$(wildcard $(SRC_DIR)/*.cpp)   

SRC=  main.cpp ./deamon/endn_deamon.cpp ./face/face.cpp  \
	  ./deamon/tcp_server.cpp ./deamon/flist.cpp ./face/tcp_channel.cpp \
	  ./table/pit.cpp ./table/pit_map.cpp ./table/fib.cpp ./table/fib_map.cpp

OBJS := $(patsubst %.cpp, %.o,$(SRC))

CC := g++ 
CFLAGS := -g -std=c++14 ${INC_DIR}
LFLAGS := -lpthread -ljsoncpp
TARGET := main # name of the file where the main function in . 

all: ${TARGET} endndc

endndc : ./endndc/endndc.cpp
	g++ ./endndc/endndc.cpp -o ./endndc/endndc

$(TARGET): ${OBJS}
	#$(CC) ${OBJS} -o ${TARGET}  $(LFLAGS)

${OBJS} : ./%.cpp
	@#${CC} ${CFLAGS} -c $< -o $@
	@#echo $(patsubst %.o, %.cpp, $@ )
	@echo $<


clean:  
	rm -rf $(TARGET) *.o ${OBJS} endndc/endndc

test:
	echo $(SRC)
	echo $(OBJS)
	echo $(CFLAGS)
