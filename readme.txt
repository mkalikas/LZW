To compile my program, create a build directory named build that is
inside of the directory where you unzipped my project. This directory
should be named mkalikas_1. This folder should contain lzw435.cpp and
CMakeLists.txt. Once inside of this directory, run the following
sequence of commands.

The sequence of commands are:

        mkdir build
        cd build
        cmake ..
        make

After running the entire sequence of commands, an executable named
lzw435 will be produced. Any files that you plan to test shall be moved
into the build directory. If you try to run my program by listing a file
name it will not work unless the files to be passed are in this
directory.

You can run the program by executing either:    
    ./lzw435 c filename
            or     
    ./lzw435 e filename.lzw

Note that filename depends on whatever file you pass when running the
program. This can be any file. For example, text.txt, file.txt,
monkey.jpg, etc.

If you run my program by passing the argument c, then the file you
passed will be compressed and a new file will be written named
filename.lzw, where filename will be the name of the file you passed
including its original extension with the .lzw extension added. So,
file.txt would turn into file.txt.lzw if you passed a file named
file.txt.

If you run my program by passing the argument e, then the file you
passed will be expanded and the result will be a new file that is
written named filename2, where again filename depends on what file name
you passed to begin with. This file will be the decompressed version of
the file you passed. So, when you passed c as an argument when running
my program, the file written was the compressed version of the original
file passed when running the program. When you run my program by passing
the argument e and filename.lzw, the file written from this execution
should be the same as filename. This execution of the program generates
a file named filename2, where filename will be the name of the file you
passed when executing the program including its original extension with
the number 2 added at the end. So, file.txt.lzw would turn into
file.txt.lzw2 if you passed a file named file.txt.lzw. The contents of
this generated file should be the same as the contents of filename
before being compressed.

Also, note that these generated files will be inside of the build
directory and not with the rest of the source files included in the
project.

If you want to keep the files to be passed when executing the program in
the same directory as the source files, that is, outside of the build
directory, then you must include ../ before the filename. So a file
named text.txt that is in the same directory as the source code can be
passed to the program by running ./lzw435 c ../text.txt to compress.

If this is the case, the file passed when executing the program by
passing e will not include a ../ before the filename and will instead be
executed as described above for running the program by passing e and
filename.lzw.
