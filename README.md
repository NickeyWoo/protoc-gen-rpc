# Protoc-gen-rpc
  Protoc-gen-rpc is third-party Add-ons for Protocol Buffers. 

## Service Dictionary ##
  you can dump proto dictionary via rpc script.
```shell
	rpc dump --proto=Service.proto
	--rpc_out: Service.proto: global dictionary {
		BI_NEWLINE: ><
		BI_SPACE: > <
	};
	dictionary 'PROTO' {
		FILE: >Service.proto<			# proto file name
		NAME: >Echo<					# first Service Name
		section DATA (dict 1 of 1) -->
			dictionary 'PROTO/DATA#1' {	# outside data, --data=KEY:VALUE
		}
		section DATE (dict 1 of 1) -->
			dictionary 'PROTO/DATE#1' {	# current date
				DAY: >30<
				MONTH: >7<
				YEAR: >2014<
		}
		section PROTOLIB (dict 1 of 1) -->
			dictionary 'PROTO/PROTOLIB#1' {
				PROTOLIB_NAME: >Service<
		}
		section SERVICES (dict 1 of 1) -->				# service dictionary
			dictionary 'PROTO/SERVICES#1' {
				SERVICE_NAME: >Echo<					# service name
				section METHODS (dict 1 of 1) -->		# service rpc method
					dictionary 'PROTO/SERVICES#1/METHODS#1' {
						METHOD_NAME: >Echo<				# method name
						section INPUT_TYPE (dict 1 of 1) -->
							dictionary 'PROTO/SERVICES#1/METHODS#1/INPUT_TYPE#1' {
								TYPE_NAME: >Request<	# method input type name
						}
						section OUTPUT_TYPE (dict 1 of 1) -->
							dictionary 'PROTO/SERVICES#1/METHODS#1/OUTPUT_TYPE#1' {
								TYPE_NAME: >Response<	# method output type name
						}
				}
		}
	}
```

## Template syntax ##
  * Template [Example][1]
  * Read about [google::ctemplate][2]


## Example ##
*1. write '.proto' file, and declare RPC service.*
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

*2. prepare template documents*
```c++
	template/main.cc
	template/service.cc
```

*2. exec rpc, auto generate rpc code.*
```shell
	rpc dump --proto=Service.proto								# dump proto dict
	rpc build --proto=Service.proto --template=./template/		# generate code
```

  [1]: https://github.com/NickeyWoo/protoc-gen-rpc/tree/master/example
  [2]: https://code.google.com/p/ctemplate/


