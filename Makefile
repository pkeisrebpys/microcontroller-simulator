all:main.exe

main.exe:src/main.cpp src/common.cpp src/Parameter.hpp src/UserProcess.cpp
	g++ -o $@ $< -lm
