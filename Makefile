all:main RL SPRL

main:main.exe

RL:RL.exe
	
SPRL:SPRL.exe
	
main.exe:src/main.cpp src/UserProcess.cpp
	g++ -o $@ $< -lm

RL.exe:src/mainRL.cpp src/UserProcessRL.cpp
	g++ -o $@ $< -lm

SPRL.exe:src/mainSPRL.cpp src/UserProcessSPRL.cpp src/Parameter.hpp
	g++ -o $@ $< -lm

