# microcontroller-simulator
Microcontroller simulator for online experiment class.

## Usage
You can compile the first example by the following command.

```
make main
```

This command generates program named `main.exe`.
This program simulates circuit which has an RL load drived by a chopper (voltage drop converter).
You can control the chopper via the microcontroller which is also simulated in the program.
The program outputs simulated voltage and current data to stdout.
The program also outputs calculation time to stderr.
Here, the calculation time is the time used by the user process in the microcontroller during the simulation.

This example consists of the following files.
The file `src/main.cpp` defines the circuit and the microcontroller simulation model.
This file also defines the connection between the circuit and the controller.
The file `src/UserProcess.cpp` defines the program ran by the microcontroller.
The file `src/common.cpp` contains commonly used simulation environment model.
The file `src/Parameter.hpp` contains commonly used circuit and control parameters.

Class owners can generate new practice by editing `src/main.cpp`.
Editing the following points is required.
Circuit simulation model.
Circuit-microcontroller connection.
Output voltage and current definition.

Students edit only `src/UserProcess.cpp` and send it as answer.

## Examples
`src/mainSPRL.cpp` and `src/UserProcessSPRL.cpp` are the other simulation example.
This simulation has single phase voltage source, RL line and single phase inverter.
The microcontroller controls the inverter.
The inverter send power from DC side to single phase voltage source.
(This simulates PV inverter connected to the grid.)

You can compile it by `make SPRL.txt` and run it by `./SPRL.txt`


