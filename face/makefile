COM_DIR=../common
INC_DIR= -I.. -I.

od=./objs/

SRCS := $(wildcard *.cpp) \
		$(wildcard ${COM_DIR}/*.cpp)

#OBJS := ${patsubst %.cpp, %.o, ${SRCS}}
OBJS := ${od}main.o ${od}tcp_channel.o ${od}face.o

CC := g++ 
CFLAGS := -g -std=c++14 ${INC_DIR}
LFLAGS := -lpthread -ljsoncpp
TARGET := main # name of the file where the main function in . 

define GCC_C
	${CC} ${CFLAGS} -c $(1) -o $(1)
endef

all: pre ${TARGET} 

pre :
	mkdir -p ${od}

$(TARGET): ${OBJS}
	$(CC) ${OBJS} -o ${TARGET}  $(LFLAGS)

${od}main.o : 
	${CC} $(CFLAGS) -c main.cpp -o ${od}main.o

${od}tcp_channel.o: 
	${CC} $(CFLAGS) -c tcp_channel.cpp -o ${od}tcp_channel.o

${od}face.o: 
	${CC} $(CFLAGS) -c face.cpp -o ${od}face.o

clean:  
	rm -rf $(TARGET) *.o ${OBJS}

test:
	echo $(SRCS)
	echo $(OBJS)
	echo $<
	echo $(CFLAGS)
