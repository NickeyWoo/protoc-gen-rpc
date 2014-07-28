
all: client {{NAME}}

client: {{#PROTOLIB}}{{PROTOLIB_NAME}}.pb.o{{/PROTOLIB}} client.o
	$(CXX) -o $@ $^ -L../../SimpleRPC/thirdparty/protobuf/lib -lprotobuf

{{NAME}}: {{#PROTOLIB}}{{PROTOLIB_NAME}}.pb.o{{/PROTOLIB}} main.o {{NAME}}.o
	$(CXX) -o $@ $^ -L../../SimpleRPC/thirdparty/protobuf/lib -lprotobuf

{{#PROTOLIB}}{{PROTOLIB_NAME}}.pb.cc{{/PROTOLIB}}: Service.proto
	protoc Service.proto --cpp_out=./

{{#PROTOLIB}}{{PROTOLIB_NAME}}.pb.o{{/PROTOLIB}}: {{#PROTOLIB}}{{PROTOLIB_NAME}}.pb.cc{{/PROTOLIB}}
	$(CXX) -c $^ -o $@ -I../../SimpleRPC/thirdparty/protobuf/include

%.o: %.cc
	$(CXX) -c $^ -o $@ -I../../SimpleRPC/thirdparty/protobuf/include

