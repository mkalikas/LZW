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

/*
// Take a string and create a vector of integers to represent
// tokens in the string
template <typename Iter>
Iter compress(const std::string &uncompressed, Iter ret) {

  std::map<std::string, int> dictionary =compression_dictionary(); // initialize
dictionary std::string w; for(auto i = uncompressed.at(0); i !=
uncompressed.size(); ++i) {

    char c = *i;
    std::string wc = w + c;
    if (dictionary.find(wc) != dictionary.end())
      w = wc;
    else {
      *ret++ = dictionary[w];
      // Add wc to the dictionary. Assuming the size is 4096!!!
      if (dictionary.size() < 4096)
        dictionary[wc] = dictionary.size() + 1;
      w = std::string(1, c);
    }
  }

  // Output the code for w.
  if (!w.empty())
    *ret++ = dictionary[w];

  return ret;
}
*/

// Take a string and create a vector of integers to represent
// tokens in the string
std::vector<int> compress(const std::string &uncompressed) {

  std::map<std::string, int> dictionary =compression_dictionary(); // initialize dictionary

  std::vector<int> v;

  std::string w;
  for (auto it = uncompressed.begin(); it != uncompressed.end(); ++it) {

   // char c = *it;
    std::string wc = w + *it;
    if (dictionary.count(wc))
      w = wc;
    else { // wc is not in the dictionary, add it
      v.push_back(dictionary[w]);

      // Add wc to the dictionary
      if (dictionary.size() < 4096)
        dictionary[wc] = dictionary.size() + 1;
      w = std::string(1, *it);
    }
  }

  // Output the code for w.
  if (!w.empty())
    v.push_back(dictionary[w]);

  return v;
}

/*
  Takes a string and bit_length
  Creates strings the length of bit_length and adds
  them to a vector. Returns the vector after the entire
  input string has been separated into substrings.
*/
std::vector<std::string> separate(std::string &s, int bit_length) {
  std::vector<std::string> v;
  for (int i = 0; i < s.size(); i += bit_length) {
    std::string str = s.substr(s.at(i), s.at(i + bit_length));
    std::cout << str;
    // assert(str.length() == bit_length);
    v.push_back(str);
  }
  return v;
}

// Decompress a list of output ks to a string.
// "begin" and "end" are iterators to the beginning and ending of a range
// when this is called in main, begin and end point to begin and end in
// compressed string

// Recursively computes the value of the string as an integer, then checks to
// see if the value is in the dictionary. If it is not, it adds it.
std::string decompress(std::vector<std::string> &v) {
  assert(!v.empty());
  std::map<int, std::string> dictionary = decompression_dictionary();

  std::string s = "decompress\n";
  for(auto it = v.begin(); it != v.end(); ++it) {
    std::cout << v.front() << " ";

    //int b = binary_string_to_int(str);

  }
  /*
  std::string w(1, v.front()); // std::string value(*it, 12); need to move 12 positions over

  std::string result = w; // what is returned
  std::string entry;
  for (; std::begin != std::end; std::next) {
    int k = *it_begin; // int b = binary_string_to_int(value);
    // ret.append(std::to_string(b));
    // std::map<int, std::string>::iterator i = dictionary.find(b);

    if (dictionary.count(k))
      entry = dictionary[k];
    else if (k == dictionary.size())
      entry = w.substr(w.begin(), 1); // entry = w + w.at(0)
    else
      throw "Bad compressed k";

    result.append(entry); // result += entry

    if (dictionary.size() < 4096)
      dictionary[dictionary.size() + 1] = w + entry.at(0); // w + entry[0]

    w = entry;
  }
  */
  return s;

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
      std::cout << "\nWarning: Overflow. code " << code << " is too big to be coded by 12 bits!\n";
      str = str.substr(str.size() - 12);
    } else {
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
        code++;
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
    // Compress input file
    // If program was run passing c and filename to compress
    case 'c': {
      // Pass input file contents string to get compressed
      // and pass empty vector t
      std::vector<int> v = compress(in);

      std::string output = int_to_binary_string(v, "");

      // Add .lzw extension to input file name
      filename.append(".lzw");

      std::ofstream out(filename.c_str(), std::ios::binary);
      out << output;
      out.close();
      break;
    }
    // binaryIODemo(v);
    // Expand input file
    case 'e': {
      // Save expanded file as filename2
      // filename2 should be identical to filename
      // separate the input into strings of length 12,
      // put these strings into a vector
      std::vector<std::string> v = separate(in, 12);
      std::string d = decompress(v);
      filename.append("2");
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
