#include <cassert>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <bitset>
#include <sys/stat.h>
#include <vector>
#include "lzw435.hpp"

// Builds a dictionary of extended ASCII characters
// The pairs are (string, int) pairs
// These fill up the keys from 0 to 255
std::map<std::string, int> compression_dictionary() {

  std::map<std::string, int> c_dictionary;
  // Build the dictionary.
  std::size_t dictionary_size = 256;
  for (auto x = 0; x < dictionary_size; ++x)
    c_dictionary[std::string(1, x)] = x;

  return c_dictionary;
}


// Take a string and create a vector of integers to represent 
// tokens in the string
std::vector<int> compress(const std::string &uncompressed) {

  std::map<std::string, int> dictionary =
      compression_dictionary(); // initialize dictionary

  std::vector<int> v;

  std::string w;
  for (auto it = uncompressed.begin(); it != uncompressed.end(); ++it) {

    char c = *it;
    std::string wc = w + c;
    if (dictionary.count(wc))
      w = wc;
    else { // wc is not in the dictionary, add it
      v.push_back(dictionary[w]);
      // Add wc to the dictionary. Assuming the size is 4096!!!
      if (dictionary.size() < 4096)
        dictionary[wc] = dictionary.size() + 1;
      w = std::string(1, c);
    }
  
  }

  // Output the code for w.
  if (!w.empty())
    v.push_back(dictionary[w]);

  return v;
}

// Builds a dictionary of extended ASCII characters
// The pairs are (int, string) pairs
// These fill up the keys from 0 to 255
std::map<int, std::string> decompression_dictionary() {

  std::map<int, std::string> d_dictionary;
  // Build the dictionary.
  std::size_t dictionary_size = 256;
  for (auto x = 0; x < dictionary_size; ++x)
    d_dictionary[x] = std::string(1, x);

  return d_dictionary;
}

// Decompress a list of output ks to a string.
// "begin" and "end" must form a valid range of ints 

std::string decompress(std::string &compressed) {

  std::map<int, std::string> dictionary = decompression_dictionary();
  std::string ret;
   // initialize a string to read the first 12 characters in the compressed string
  for(auto it = compressed.begin(); it != compressed.end(); it + 12) {
    std::string value = compressed.substr(*it, 12);
    int b = binary_string_to_int(value);
    ret.append(std::to_string(b));
    
    std::map<int, std::string>::iterator i = dictionary.find(b);
    if (i != dictionary.end()) {
      dictionary[b] = std::string(1, b);
    }
    else { // not found
      std::string nx = compressed.substr(*(it + 12), 12);
      b += binary_string_to_int(nx);
      std::string entry = std::to_string(b);
      ret.append(entry);
      if(dictionary.size() < 4096)
        dictionary[dictionary.size() + 1] = entry + entry;

    }


  }  
  return ret;
}


std::string int_to_binary_string(std::vector<int> &v, std::string s) {

  while(!v.empty()) {
    std::bitset<12> b(v.front());
    s.append(b.to_string());
    v.erase(v.begin());
    //int_to_binary_string(v, s);

  }
  
  // Return binary string of values in vector
  if(v.empty())
    return s;

}


int binary_string_to_int(std::string p) {

  int code = 0;
  if (p.size() > 0) {
    if (p.at(0) == '1')
      code = 1;
    p = p.substr(1);
    while (p.size() > 0) {
      code = code << 1;
      if (p.at(0) == '1')
        code++;
      p = p.substr(1);
    }
  }
  return code;
}
