all:discovery registry
discovery : discovery.cc main.pb.cc
	clang++ -std=c++17 $^ -o $@ -lspdlog -lfmt -lgflags -letcd-cpp-api -lcpprest -lbrpc -lleveldb -lssl -lcrypto -lprotobuf -lpthread
registry : registry.cc main.pb.cc
	clang++ -std=c++17 $^ -o $@ -lspdlog -lfmt -lgflags -letcd-cpp-api -lcpprest -lbrpc -lleveldb -lssl -lcrypto -lprotobuf -lpthread

PHONY:clean
clean:
	rm -f discovery registry
