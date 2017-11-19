#include "lzw435.hpp"
#include <bitset> 
#include <cassert>
#include <iostream>
#include <iterator>
#include <sys/stat.h>

// Builds a dictionary of extended ASCII characters
// The pairs are (string, int) pairs
// These fill up the keys from 0 to 255
std::map<std::string, int> compression_dictionary() {

  std::map<std::string, int> c_dictionary;
 
  // Build the dictionary.
  auto dictionary_size = 256;
  for (auto x = 0; x < dictionary_size; ++x)
    c_dictionary[std::string(1, x)] = x;

  return c_dictionary;
}

// Builds a dictionary of extended ASCII characters
// The pairs are (int, string) pairs
// These fill up the keys from 0 to 255
std::map<int, std::string> decompression_dictionary() {
  std::map<int, std::string> d_dictionary;
 
  // Build the dictionary.
  auto dictionary_size = 256;
  for (auto x = 0; x < dictionary_size; ++x)
    d_dictionary[x] = std::string(1, x);

  return d_dictionary;
}

// Take a string and create a vector of integers to represent
// tokens in the string
std::vector<int> compress(const std::string &uncompressed) {

  std::map<std::string, int> dictionary = compression_dictionary(); // initialize dictionary

  std::string lookahead;
  std::vector<int> v;
  for (auto it = uncompressed.begin(); it != uncompressed.end(); ++it) {
    char current = *it;
    std::string consume = lookahead + current;

    // The entry is already in the dictionary
    if (dictionary.count(consume)) {
      lookahead = consume;
    } else { // lookahead is not in the dictionary, add it
      if (dictionary.size() == 4096)
        return v;

      v.push_back(dictionary[lookahead]);

      // Add consume to the dictionary
      dictionary[consume] = dictionary.size() - 1;
      lookahead = std::string(1, current);
    }
  }
  if (!lookahead.empty())
    v.push_back(dictionary[lookahead]);

  return v;
}

/*
  Takes a string and reads 8 characters at a time.
  Converts the segments read into a character.
  Replaces each segment in the string with a character.
  Returns the string once the input string has been
  read to the end.
*/
std::string convert_to_bytes(std::string &s) {
  int byte_size = 8;

  for (auto i = 0; i < s.size(); ++i) {
    std::string segments = s.substr(i, byte_size);
    int new_char = binary_string_to_int(segments);
    s.replace(i, byte_size, 1, static_cast<char>(new_char));

  }
  return s;
}

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
    v.push_back(binary_string_to_int(str));
  }
  /*
  //\DELETE: Output each element in vector
  for(int i = 0; i < v.size(); ++i)
    std::cout << v.at(i) << " ";
  std::cout << "\n";
  */

  return v;
}

/*
  Takes a vector of integers and returns a string
  representing each integer as a string in the map.
  It builds the dictionary and then recursively computes the
  value of the string as an integer, then checks
  if the value is in the dictionary. If it is not, it adds it.
  The resulting string is expected to be the file contents
  of the original file before compression.
*/
std::string decompress(std::vector<int> &v) {
  // assert(!v.empty());
  std::map<int, std::string> dictionary = decompression_dictionary();

  int next;
  std::string result;
  // need to have w at next char

  // std::string str // same as entry
  for (auto i = 0; i < v.size(); ++i) {
    // = (dictionary.find(value) != dictionary.end()) ? dictionary[value] : w +
    // w.at(0);
    std::string entry;

    int value = v.at(i); // value at i in vector

    // If the value is in the dictionary
    if (dictionary.find(value) != dictionary.end()) {
      entry = dictionary[value]; // set entry to the string pair of value
      std::cout << entry << " found ";
    }
    // The value is not in the dictionary

    result.append(entry);

    std::string s; // an empty string

    // Add the entry to the dictionary
    if (dictionary.size() < 4096)
      dictionary[dictionary.size() + 1] = s + entry.at(0); // s + entry[0]

    // std::cout << s << " this is w\n";
    s = entry;
  }
  return result;
}

std::string int_to_binary_string(std::vector<int> v, std::string s) {

  while (!v.empty()) {
    const int bits = 12;
    std::bitset<bits> b(v.front());

    s.append(b.to_string());
    v.erase(v.begin());
  }

  assert(v.empty());

  if (v.empty())
    return s;

  /* Using original code and vector 
  while (!v.empty()) {
    int code = v.front();
    s.append();
    v.erase(v.begin());

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
    } else {
      for (auto i = 0; i < zeros;
           ++i) // pad 0s to left of the binary code if needed
        str = "0" + str;
    }
    s.append(str);
    v.erase(v.begin());
  }

  if (v.empty())
    return s;
  */
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
