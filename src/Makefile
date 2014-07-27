#
#
#
#
#

include ../Makefile.env

TARGET := ../bin/protoc-gen-rpc ../bin/rpc
OBJS := objs/SimpleRpcGenerator.o objs/protoc-gen-rpc.o

all: $(TARGET)

clean:
	rm -rf ./objs/*
	rm -f $(TARGET)

../bin/protoc-gen-rpc: $(OBJS)
	$(CXX) $^ -o $@ $(LIBS)

../bin/rpc: rpc
	cp -f $^ $@






