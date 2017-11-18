To compile my program,
    create a build directory named build that is inside of the directory where
        you unzipped my project.This directory should be named
            mkalikas_1.This folder should contain
                lzw435.cpp and CMakeLists.txt.Once inside of this directory,
    run the following sequence of commands
        .

    The sequence of commands are :

    mkdir build cd build cmake..make

        After running the entire sequence of commands,
    an executable named lzw435 will be produced
        .Any files that you plan to test shall be moved into the build
            directory.If you happen to get a clock skew error when you run make,
    either run make clean, or try
  rebuilding from the beginning by deleting the build directory and then
      creating it again
          .If this message comes up it is just a warning and happens
              sometimes if files have been saved in directories outside of the
                  build directory even after running make.If you try to run my program by listing a file name it will not work unless the files to be passed are in this
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
directory, then you must include ../ before the filename. So a file named text.txt that is in the same directory as the source code can be passed to the program by running ./lzw435 c ../text.txt to compress.

If you choose this method to run the program, the output file will be written to the directory where the input file is located.

If this is the case, the file passed when executing the program by
passing e will not include a ../ before the filename and will instead be executed as described above for running the program by passing e and
filename.lzw.

I included a .hpp file that includes the prototypes for all of
my functions that are used to compute the compression and
decompression for part one.

I included two functions to create the dictionary in the file for part one and part two. When a file is being compressed, the dictionary function creates a map of string, integer pairs. When a file is being expanded, the dictionary function creates a map of integer, string pairs.

I wrote a compress function that takes a string. This string represents the entire contents of the uncompressed file. This function adds new strings to the
map beginning at the 256th pair in the map. The preceding values are initialized when the dictionary is built using the above mentioned functions.
The pairs from 0 to 255 are pairs that represent single character strings that
correspond to the integer and symbol representations defined in the extended ASCII table.

For compression, I included a function called convert_to_bytes that converts the binary string to bytes that represent their corresponding character. This function takes a string and reads 8 characters at a time. It converts the segments read into a character by converting the binary number into an integer and then converting
that into a character. The substring segment then replaces the original input string byte by byte. After following
this process until the end of the input string is reached, the resulting string is a compressed string of bytes. I also included an int_to_binary_string function
that takes a vector of integers and converts the values in the vector to their
corresponding binary representation by using the bitset header file from the C++ standard library.

For decompression, I included a function that separates the string input into groups of 12.
The separated strings are then passed to the binary_string_to_int
function. Their result is returned as an integer and then put into a vector of integers.
This vector is then used for decompression. The decompression
function takes a vector of integers and returns a string
representing each integer as a string in the map by recursively
checking to see if the integer value is found in the dictionary.
If it is not, it is added and the corresponding string is matched as the integer pair in the map.
The resulting string is expected to be the file contents
of the original file before compression.
