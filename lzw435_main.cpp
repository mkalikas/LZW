#include "lzw435.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  // Throw an error if 3 parameters were not passed
  if (argc < 2) {
    std::cerr << "ERROR: no input files given\n";
    return 1;
  }

  std::string filename = argv[2];
  std::ifstream infile(filename.c_str(), std::ios::binary);

  // Throw an error if the file cannot be opened
  if (!infile.is_open()) {
    std::cerr << "ERROR: file cannot be opened. Check that the file exists.\n";
    return 2;
  }

  // Create a string of the file contents
    std::string in{std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>()};

    infile.close();

  try {
    switch (*argv[1]) {
     
      // If program was run passing c and filename to compress
      case 'c': { 
        // Pass input file contents to get compressed
        // Set vector v to compressed string
        std::vector<int> v = compress(in);

        std::string output = int_to_binary_string(v, "");

        // Add .lzw extension to input file name
        filename.append(".lzw");

        std::ofstream out(filename.c_str(), std::ios::binary);
        out << output;

        out.close();
        break;

      }
      // Expand input file
      case 'e': {
        // Save expanded file as filename2
        // filename2 should be identical to filename
        //\std::map<int, std::string> dictionary = decompression_dictionary();
        std::string d = decompress(in);
        //std::cout << d << " ";
        std::ofstream out(filename.append("2").c_str(), std::ios::binary);
        out << d;
        out.close();
        break;
      }
    }
  } 
  catch (char const *err) {
    std::cout << "The library threw an exception:\n" << err << "\n";
  }

  return 0;
}