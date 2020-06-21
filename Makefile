all:main RL

main:main.exe

RL:RL.exe
	
main.exe:src/main.cpp src/UserProcess.cpp
	g++ -o $@ $< -lm

RL.exe:src/mainRL.cpp src/UserProcessRL.cpp
	g++ -o $@ $< -lm

