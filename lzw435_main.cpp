#include "lzw435.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>

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
  std::string in{std::istreambuf_iterator<char>(infile),
                 std::istreambuf_iterator<char>()};

  infile.close();

  try {

    switch (*argv[1]) {
    // Compress input file
    // If program was run passing c and filename to compress
    case 'c': {
      // Pass input file contents string to get vector of integers
      // representing integer value that corresponds to string in dictionary
      std::vector<int> v = compress(in);
      // Convert all integers to corresponding binary values
      // each appended to a string representing the file as a
      // string of binary numbers
      std::string output = int_to_binary_string(v, "");

      // Make the output string divisible by 8
      auto bit_length = output.length() % 8;
      output.append(bit_length, '0');

      assert(output.length() == (output.length() % 8 + output.length()));
      output = convert_to_bytes(output);

      filename.append(".lzw"); // Add .lzw extension to input file name

      std::ofstream out(filename.c_str(), std::ios::binary);
      out << output; // Output the compressed file contents to a new file

      out.close();
      break;
    }
    // Expand input file
    case 'e': {
      // filename2 should be identical to filename
      // separate the input into strings of length 12,
      // put these strings into a vector of integers
      in = make_string(in);
      std::vector<int> v = separate(in, 12);
      std::string d = decompress(v);
      filename.append("2"); // Save expanded file as filename2

      std::ofstream out(filename.c_str(), std::ios::binary);
      out << d;
      out.close();
      break;
    }
    }
  } catch (char const *err) {
    std::cout << "The library threw an exception:\n" << err << "\n";
  }

  return 0;
}
