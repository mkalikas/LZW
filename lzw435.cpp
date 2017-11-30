#include "lzw435.hpp"
#include <bitset>
#include <cassert>
#include <iostream>
#include <iterator>
#include <sys/stat.h>

// Builds a dictionary of extended ASCII characters
std::map<std::string, int> compression_dictionary() {

  std::map<std::string, int> c_dictionary;

  // Build the dictionary.
  auto dictionary_size = 256;
  for (auto x = 0; x < dictionary_size; ++x)
    c_dictionary[std::string(1, x)] = x;

  return c_dictionary;
}

// Builds a dictionary of extended ASCII characters
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

  std::map<std::string, int> dictionary =
      compression_dictionary(); // initialize dictionary

  std::string lookahead;
  std::vector<int> v;
  for (auto it = uncompressed.begin(); it != uncompressed.end(); ++it) {
    char current = *it;
    std::string consume = lookahead + current;

    // The entry is already in the dictionary
    if (dictionary.count(consume)) {
      lookahead = consume;
    } else { // lookahead is not in the dictionary, add it
      v.push_back(dictionary[lookahead]);

      // TODO: modify so that the rest of the file gets put into dictionary
      if (dictionary.size() == 4096)
        return v;

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
  Reads a bit string the length of one byte.
  Replaces the 8 bit length string with the
  single character corresponding to the integer value
*/
std::string convert_to_bytes(std::string &s) {
  const int byte_size = 8;

  for (auto i = 0; i < s.size(); ++i) {
    std::string segments = s.substr(i, byte_size);
    int new_char = binary_string_to_int(segments);
    s.replace(i, byte_size, 1, static_cast<char>(new_char));
  }
  return s;
}

std::string make_string(std::string &s) {
  std::string str;
  for (auto i = 0; i < s.length(); ++i) {
    std::bitset<8> b(static_cast<int>(s.at(i)));
    str.append(b.to_string());
  }

  return str;
}

// Creates strings the length of bit_length, then calls
// binary_string_to_int to convert the string to an integer.
std::vector<int> separate(std::string &s, int bit_length) {
  std::vector<int> v;

  int zeros = bit_length - 8;

  assert((s.length() - zeros) % bit_length == 0);

  for (int i = 0; i < s.size() - zeros; i += bit_length) {
    std::string str = s.substr(i, bit_length);
    assert(str.length() == bit_length);
    v.push_back(binary_string_to_int(str));
  }
  return v;
}

// Adds values to the dictionary to get the original decompressed string
std::string decompress(std::vector<int> &v) {
  assert(!v.empty());
  std::map<int, std::string> dictionary = decompression_dictionary();

  std::string lookahead(1, static_cast<char>(v.at(0)));
  std::string result = lookahead;
  std::string entry;
  for (auto i = 1; i < v.size(); ++i) {
    int value = v.at(i); // value at i in vector

    // If the value is in the dictionary
    if (dictionary.count(value)) {
      entry = dictionary[value]; // set entry to the string pair of value
    } else if (value == dictionary.size()) {
      entry = lookahead + lookahead.at(0);
    } else
      throw "Cannot decompress!\n";

    result.append(entry);

    // Add the entry to the dictionary
    if (dictionary.size() < 4096)
      dictionary[dictionary.size()] = lookahead + entry.at(0);

    lookahead = entry;
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
  assert(v.empty()) if (v.empty()) return s;
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
