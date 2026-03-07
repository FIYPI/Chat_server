all:test async main
main:main.cc
	g++ -std=c++17 $^ -o $@ -lspdlog -lfmt -pthread -lgflags
test:test.cc
	g++ -std=c++17 $^ -o $@ -lspdlog -lfmt -pthread
async:async.cc
	g++ -std=c++17 $^ -o $@ -lspdlog -lfmt -pthread
PHONY:clean
clean:
	rm -f test async