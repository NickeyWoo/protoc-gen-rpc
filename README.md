# Protoc-gen-rpc
  Protoc-gen-rpc is third-party Add-ons for Protocol Buffers.

## Example ##
**write '.proto' file, and declare RPC service.**
```proto
message Request
{
	required int32 id = 1;
}

message Response
{
	required int32 id = 1;
	required string Name = 2;
}

service EchoService {
	rpc Echo(Request) returns(Response);
	rpc InvertEcho(Response) returns(Request);
}
```

**exec protoc, auto generate rpc code.**
```shell
	protoc --plugin=../bin/protoc-gen-rpc ./example.proto --rpc_out=./
```



