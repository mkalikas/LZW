#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

  struct lzw {
    lzw(std::map<std::string, int> d) : d(d) {}

    std::map<std::string, int> d;



  };

  std::map<std::string, int> &build_dictionary();
  std::string int_to_binary(int c, int cl);
  int binary_to_int(std::string p);
  void binaryIODemo(std::vector<char> compressed);

// Compress a string to a list of output symbols.
// The result will be written to the output iterator
// starting at "result"; the final iterator is returned.
template <typename Iterator>
Iterator compress(const std::vector<char> &uncompressed, Iterator result) {
  // Build the dictionary.binaryIODemo(v);
  int dictSize = 256;
  std::map<std::string, int> dictionary;
  for (int i = 0; i < 256; i++) dictionary[std::string(1, i)] = i;

  std::string w;
  for (std::string::const_iterator it = uncompressed.begin();
       it != uncompressed.end(); ++it) {
    char c = *it;
    std::string wc = w + c;
    if (dictionary.count(wc))
      w = wc;
    else {
      *result++ = dictionary[w];
      // Add wc to the dictionary. Assuming the size is 4096!!!
      if (dictionary.size() < 4096) dictionary[wc] = dictSize++;
      w = std::string(1, c);
    }
  }

  // Output the code for w.
  if (!w.empty()) *result++ = dictionary[w];
  return result;
}

// Decompress a list of output ks to a string.
// "begin" and "end" must form a valid range of ints
template <typename Iterator>
std::string decompress(Iterator begin, Iterator end) {
  // Build the dictionary.
  int dictSize = 256;
  std::map<int, std::string> dictionary;
  for (int i = 0; i < 256; i++) dictionary[i] = std::string(1, i);

  std::string w(1, *begin++);
  std::string result = w;
  std::cout << result << "???:::\n";
  std::string entry;
  for (; begin != end; begin++) {
    int k = *begin;
    if (dictionary.count(k))
      entry = dictionary[k];
    else if (k == dictSize)
      entry = w + w[0];
    else
      throw "Bad compressed k";

    result += entry;

    // Add w+entry[0] to the dictionary.
    if (dictionary.size() < 4096) dictionary[dictSize++] = w + entry[0];

    w = entry;
  }
  return result;
}
