To compile my program, create a build directory named build and run the command "cmake .." followed by make.

The sequence of commands are:

mkdir build

cd build

cmake ..

make



After running the make command, an executable named lzw will be produced.

You can run the program by executing:

./lzw c path_to_filename or



Any files that you plan to test shall be moved into the build directory. If you plan to leave the files in the source directory,  
