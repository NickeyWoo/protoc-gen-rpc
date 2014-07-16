#
#
#
#
#

include ../Makefile.env

TARGET := ../bin/protoc-gen-rpc
OBJS := objs/SimpleRpcGenerator.o objs/protoc-gen-rpc.o objs/Log.o objs/RpcService.o

all: $(TARGET)

clean:
	rm -rf ./objs/*
	rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LIBS)



