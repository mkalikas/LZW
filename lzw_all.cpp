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
std::map<std::string, std::size_t> compression_dictionary() {

  std::map<std::string, std::size_t> c_dictionary;
  // Build the dictionary.
  std::size_t dictionary_size = 256;
  for (auto x = 0; x < dictionary_size; ++x)
    c_dictionary[std::string(1, x)] = x;

  return c_dictionary;
}

// Builds a dictionary of extended ASCII characters
// The pairs are (int, string) pairs
// These fill up the keys from 0 to 255
std::map<std::size_t, std::string> decompression_dictionary() {

  std::map<std::size_t, std::string> d_dictionary;
  // Build the dictionary.
  std::size_t dictionary_size = 256;
  for (auto x = 0; x < dictionary_size; ++x)
    d_dictionary[x] = std::string(1, x);

  return d_dictionary;
}

/*
// Take a string and create a vector of integers to represent
// tokens in the string
template <typename Iter>
Iter compress(const std::string &uncompressed, Iter ret) {

  std::map<std::string, int> dictionary =compression_dictionary(); // initialize dictionary
  std::string w;
  for(auto i = uncompressed.at(0); i != uncompressed.size(); ++i) {

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
std::vector<std::size_t> compress(const std::string &uncompressed) {

  std::map<std::string, int> dictionary =
      compression_dictionary(); // initialize dictionary

  std::vector<std::size_t> v;

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


/*
  Takes a string and bit_length
  Creates strings the length of bit_length and adds
  them to a vector. Returns the vector after the entire
  input string has been separated into substrings.
*/
std::vector<std::string> separate(std::string &s) {
  std::vector<std::string> v;
  for(auto i = 0; i < s.size(); i + bit_length) {
    std::string str = s.substr(s.at(i), s.at(i + bit_length));
    std::cout << str;
    assert(str.length() == bit_length);
    v.push_back(str);

  }
  return v;
}
// Decompress a list of output ks to a string.
// "begin" and "end" are iterators to the beginning and ending of a range
// when this is called in main, begin and end point to begin and end in compressed string

// Recursively computes the value of the string as an integer, then checks to see if the value is in the dictionary. If it is not, it adds it.
std::string decompress(std::vector<std::string> &v) {

  std::map<std::size_t, std::string> dictionary = decompression_dictionary();

  while(!v.empty()) {

  }
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
  for (; std::begin != std::end; std::next) {
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
      dictionary[dictionary.size() + 1] = w + entry.at(0); // w + entry[0]

    w = entry;
  }
  return result;
}

std::tuple<std::size_t, std::size_t> get_code_and_length(std::vector<int> v) {
  // Return binary string of values in vector
  if(v.empty())
    throw "ERROR: Cannot compute an empty vector.";

  while(!v.empty()) {
    std::size_t bits;
    std::size_t f = v.front();

    if(f < 256) {
      bits = 8;
      v.erase(v.begin());
      return {f, bits};
    }
    if (f < 512) {
      bits = 9;
      v.erase(v.begin());
      return {f, bits};
    }
    else if (f < 1'024) {
      bits = 10;
      v.erase(v.begin());

      return {f, bits};
    }
    else if (f < 2'048) {
      bits = 11;
      v.erase(v.begin());

      return {f, bits};
    }
    else if (f < 4'096) {
      bits = 12;
      v.erase(v.begin());

      return {f, bits};
    }
    else if (f < 8'192) {
      bits = 13;
      v.erase(v.begin());

      return {f, bits};
    }
    else if (f < 16'384) {
      bits = 14;
      v.erase(v.begin());

      return {f, bits};
    }    
    else if (f < 32'768) {
      bits = 15;
      v.erase(v.begin());

      return {f, bits};
    }
    else if (f < 65'536) {
      bits = 16;
      v.erase(v.begin());

      return {f, bits};
    }
    else
      throw "ERROR: Cannot create a bit length longer than 16 bits."; 
  }
}

std::string int_to_binary_string(std::size_t value, std::size_t bits) {
  std::string s;
  std::bitset<bits> b(value);
  s.append(std::to_string(b));
  
  int code = c;
  while (c > 0) {
    if (c % 2 == 0)
      p = "0" + p;
    else
      p = "1" + p;
    c = c >> 1;
  }
  int zeros = cl - p.size();
  if (zeros < 0) {
    std::cout << "\nWarning: Overflow. code " << code
              << " is too big to be coded by " << cl << " bits!\n";
    p = p.substr(p.size() - cl);
  } else {
    for (int i = 0; i < zeros;
         i++) // pad 0s to left of the binary code if needed
      p = "0" + p;
  }
  return p;

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
        std::vector<std::size_t> v = compress(in);

        std::string output = int_to_binary_string(get_code_and_length(v));

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
        std::vector<std::string> v = separate(in);
        std::string d = decompress(v);
        filename.append("2");
        std::ofstream out(filename.c_str(), std::ios::binary);
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
