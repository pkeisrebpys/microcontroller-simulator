all:main SPRL

main:main.exe

SPRL:SPRL.exe
	
main.exe:src/main.cpp src/UserProcess.cpp src/UserProcess.hpp src/Parameter.hpp src/common.cpp
	g++ -o $@ $< -lm

SPRL.exe:src/mainSPRL.cpp src/UserProcessSPRL.cpp src/UserProcess.hpp src/Parameter.hpp src/common.cpp
	g++ -o $@ $< -lm

