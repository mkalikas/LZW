#include <bitset>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <sys/stat.h>
#include <tuple>
#include <vector>
#include "lzw435.hpp"

/*
  Takes a string and bit_length
  Creates strings the length of bit_length, then calls 
  binary_string_to_int to convert the string to an integer.
  This integer is then added to a vector. 
  Returns the vector after the entire input string has been 
  separated into substrings and the integer result of each string
  has been computed.
*/
std::vector<int> separate(std::string &s, int bit_length) {
  std::vector<int> v;
  assert(s.length() % bit_length == 0);

  for (int i = 0; i < s.size(); i += bit_length) {
    std::string str = s.substr(i, bit_length);
    assert(str.length() == bit_length);
  //\DELETE: set integer value to string //val = binary_string_to_int(str);
    v.push_back(binary_string_to_int(str));
  }
  //\DELETE: Output each element in vector
  for(int i = 0; i < v.size(); ++i)
    std::cout << v.at(i) << " ";
  std::cout << "\n";
  
  return v;
}

// Decompress a list of output ks to a string.
// "begin" and "end" are iterators to the beginning and ending of a range
// when this is called in main, begin and end point to begin and end in
// compressed string

// Recursively computes the value of the string as an integer, then checks to
// see if the value is in the dictionary. If it is not, it adds it.
std::string decompress(std::vector<int> &v) {
//  assert(!v.empty());
  std::map<int, std::string> dictionary = decompression_dictionary();

  int next = v.at(1); // get the next value

  std::string s = dictionary[next]; // init string to first val in vector 
  std::cout << s << " next val";
  std::string result = s;
  std::string str;
  for(auto i = 0; i < v.size(); ++i) {
    // = (dictionary.find(value) != dictionary.end()) ? dictionary[value] : w + w.at(0);
    
    int value = v.at(i); // value at i in vector 

    // If the value is in the dictionary
    if(dictionary.find(value) != dictionary.end()) {
      str = dictionary[value]; // set str to the string pair of value
      //std::cout << str << " found ";

    }
    // The value is not in the dictionary
    // If the value is the value of the dictionary size
    else if(value == dictionary.size()) {
      //std::cout << w << " w\n" << w.at(0) << " w.at(0)\n";
     // str = std::to_string(value); // entry = w + w.at(0) 
      str = s + s.at(0);
      //std::cout << str << " not in dictionary ";
    }
    else {
      throw "Bad compressed.";
    }

    result.append(str);
    //std::cout << s << " this is s ";

    if (dictionary.size() < 4096)
      dictionary[dictionary.size() + 1] = s + str.at(0); // w + entry[0]

    s = str;
    //std::cout << s << " this is w\n";

  }
  return result;

}

std::string int_to_binary_string(std::vector<int> v, std::string s) {
  while(!v.empty()) {
    std::string str = "";
    int code = v.front();
    std::cout << code << "\n";
    while (code > 0) {
      if (code % 2 == 0)
        str = "0" + str;
      else
        str = "1" + str;
      code = code >> 1;
    }
    int zeros = 12 - str.size();
    if (zeros < 0) {
      throw "Warning: Overflow. Code is too big to be coded by 12 bits!\n";
      //str = str.substr(str.size() - 12);
    }
    else {
      for (auto i = 0; i < zeros; ++i) // pad 0s to left of the binary code if needed
        str = "0" + str;
    }
    s.append(str);
    v.erase(v.begin());
  }

  if(v.empty())
    return s;
}

int binary_string_to_int(std::string s) {

  int code = 0;
  if (s.size() > 0) {
    if (s.at(0) == '1')
      code = 1;
    s = s.substr(1);
    while (s.size() > 0) {
      code = code << 1;
      if (s.at(0) == '1')
        ++code;
      s = s.substr(1);
    }
  }
  return code;
}

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

    // Expand input file
    case 'e': {
      // filename2 should be identical to filename
      // separate the input into strings of length 12,
      // put these strings into a vector of integers
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
