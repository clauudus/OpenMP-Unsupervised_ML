# K-Means and Floyd Warshall

## Introduction:

Project that explores the optimization of two algorithms (K-means and Floyd-Warshall).
Main objective of the project is to understand deeply how memory is distribuited and the usage of the private caché for each core in a multicore machine.
To get to this goal, the codes make a use of OpenMP functionalities as well as code-based optimizations.

This is still a draft, it is expected to have all the documentation and coding done by the beginning of March (aprox).

## Compiling and Running the Programs

### How to compile/run code:
Feel free to use the Makefile to compile, run, and format the code with clang-format.

### How to measure algorithm performance:
Inside each directory, you will find "ejec.h", which runs the code and reports execution time using hyperfine.

### How to find your machine specifications:
In the main directory, you will find "config.sh". Running this script prints the key machine specifications that may significantly affect execution time.

### Extra Explanations:
To understand in a bigger depth the project and/or get a more guided step by setp code usage, please, check the Documentation pdf (included in the main directory).

## Future Works

Currently, most of the documentation is in Spanish, it is being translated to English, hopefuly it will be fully in English for the end of the project. :)

There's part of the obtained data that has not been produced on the same machine. This data is specified in the document. In the end of February the whole experiment will be repited on a single machine (hardware used and OS will be specified in the documentation) so all data and results can be consistend, sorry about this temp issue!!!!! :(
