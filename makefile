# create by wgh
INC_DIR= -I.
SRC_DIR = ./deamon ./face ./common ./table 
OBJ_DIR = ./objs

#SRC=$(wildcard $(SRC_DIR)/*.cpp)   

SRC=  main.cpp \
	  ./deamon/endn_deamon.cpp \
	  ./deamon/tcp_server.cpp \
	  ./deamon/flist.cpp \
	  ./deamon/usocket_server.cpp \
	  ./deamon/ether_recvs.cpp \
	  ./face/face.cpp  \
	  ./face/tcp_channel.cpp \
	  ./face/channel.cpp \
	  ./face/ether_channel.cpp \
	  ./table/pit.cpp \
	  ./table/pit_map.cpp \
	  ./table/fib.cpp \
	  ./table/fib_map.cpp \
	  ./common/globle_var.cpp \
	  ./common/r_queue.cpp \

OBJS := $(patsubst %.cpp, %.o,$(SRC))

CC := g++ 
CFLAGS := -g -std=c++14 ${INC_DIR}
LFLAGS := -lpthread -ljsoncpp
TARGET := main # name of the file where the main function in . 

all: ${TARGET} endndc

endndc : ./endndc/endndc.cpp
	g++ ./endndc/endndc.cpp -o ./endndc/endndc

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
