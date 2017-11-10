#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <bitset>
#include <sys/stat.h>
#include <vector>

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
template <typename Iterator>
Iter compress(const std::string &uncompressed, Iter ret) {

  std::map<std::string, int> dictionary = compression_dictionary(); // initialize dictionary
  std::string w;
  for(auto it = uncompressed.begin(); it != uncompressed.end(); ++it){

    char c = *it;
    std::string wc = w + c;
    if (dictionary.find(wc) != dictionary.end())
      w = wc;
    else {
      *ret++ dictionary[w];
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
// "begin" and "end" are iterators to the beginning and ending of a range
// when this is called in main, begin and end point to begin and end in compressed string
template <typename Iter>
std::string decompress(Iter it_begin, Iter end) {

  std::map<int, std::string> dictionary = decompression_dictionary();

   // initialize a string to read the first 12 characters in the compressed string
   /*
   std::string bcode = "";
   for (std::vector<int>::iterator it = compressed.begin();
        it != compressed.end(); ++it) {
     if (*it < 256)
       bits = 8;
     else
       bits = 9;

     bits = 12;
     p = int2BinaryString(*it, bits);
     std::cout << "c=" << *it << " : binary string=" << p
               << "; back to code=" << binaryString2Int(p) << "\n";
     bcode += p;
   }
   */
  std::string w(1, *it_begin++); // std::string value(*it, 12); need to move 12 postitions over

  std::string result = w; // what is returned
  std::string entry;
  for (; begin != end; ++begin) {
    int k = *it_begin; // int b = binary_string_to_int(value);
    // ret.append(std::to_string(b));
    //std::map<int, std::string>::iterator i = dictionary.find(b);

    if (dictionary.count(k))
      entry = dictionary[k];
    else if (k == dictionary.size())
      entry = w.substr(w.begin(), 1); // entry = w + w.at(0)
    else
      throw "Bad compressed k";

    result.append(entry); // result += entry

    if (dictionary.size() < 4096)
      dictionary[dictionary.size() + 1] = w + entry.begin(); // w + entry[0]

    w = entry;
  }
  return result;
}

std::string int_to_binary_string(std::vector<int> &v) {
std::string s;
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
      // Compress input file
      // If program was run passing c and filename to compress
      case 'c': {
        // Pass input file contents string to get compressed
        // and pass empty vector t
        std::vector<int> v;
        compress(in, std::back_inserter(v));

        std::string output = int_to_binary_string(v);

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
        std::string d = decompress(in.begin(), in.end());
        filename.append("2")
        std::ofstream out(filename.c_str(), std::ios::binary);
        out << d;
        out .close();
        break;
      }
    }
  }
  catch (char const *err) {
    std::cout << "The library threw an exception:\n" << err << "\n";
  }

  return 0;
}
