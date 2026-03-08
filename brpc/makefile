all: server client 
server: server.cc main.pb.cc 
	g++ -std=c++17 $^ -o $@ -lbrpc -lleveldb -lgflags -lssl -lcrypto -lprotobuf -lpthread
client: client.cc main.pb.cc 
	g++ -std=c++17 $^ -o $@ -lbrpc -lleveldb -lgflags -lssl -lcrypto -lprotobuf -lpthread
%.pb.cc : %.proto 
	protoc --cpp_out ./ $<