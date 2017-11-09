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

  /*
  //\ DELETE THIS COMMENTED OUT SECTION
  std::ofstream out("test.txt"); // used to write to file test.txt which
  includes only the pairs in the original dictionary

  //Loop to write each pair in dictionary to a file, each on its own line

  for (auto x : dictionary) {

    out << "( " << x.first << " , " << x.second << " ) "
        << "\n";
  }
  out.close();
  */

  return c_dictionary;
}


// Take a string and create a vector of integers to represent 
// tokens in the string 

std::vector<int> compress(const std::string &uncompressed) {

  std::map<std::string, int> dictionary = compression_dictionary(); // initialize dictionary
  std::vector<int> v;

  std::string w;
  for(auto it = uncompressed.begin(); it != uncompressed.end(); ++it){

    char c = *it;
    std::string wc = w + c;
    if (dictionary.find(wc) != dictionary.end())
      w = wc;
    else {
      v.push_back(dictionary[w]);
      //*result++ = dictionary[w];
      // Add wc to the dictionary. Assuming the size is 4096!!!
      if (dictionary.size() < 4096)
        dictionary[wc] = dictionary.size() + 1;
      w = std::string(1, c);
    }
  }

    // If the value found is in the range of the string, lookahead to the next char 
    // in the string and create a string of these characters to add to the dictionary 

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

  /*
  //\ DELETE THIS COMMENTED OUT SECTION
  std::ofstream out("test.txt"); // used to write to file test.txt which
  //includes only the pairs in the original dictionary

  //Loop to write each pair in dictionary to a file, each on its own line

  for (auto x : d_dictionary) {

    out << "( " << x.first << " , " << x.second << " ) "
        << "\n";
  }
  out.close();
  */

  return d_dictionary;
}



/*
// Decompress a list of output ks to a string.
// "begin" and "end" must form a valid range of ints
template <typename Iter> 
std::string decompress(Iter begin, Iter end) {


  std::string w(1, *begin++);
  std::string result = w;
  // std::cout << result<<"???:::\n";
  std::string entry;
  for (; begin != end; begin++) {
    int k = *begin;
    if (dictionary.count(k))
      entry = dictionary[k];
    else if (k == dictionary_size)
      entry = w + w[0];
    else
      throw "Bad compressed k";

    result += entry;

    if (dictionary.size() < 4096)
      dictionary[++dictionary_size] = w + entry[0];

    w = entry;
  }
  return result;
}
*/


std::string int_to_binary_string(std::vector<int> v) {

  /*
  std::string p; // a binary code string with code length = cl
  while (c > 0) {
    p = (c % 2 == 0) ? "0" + p : "1" + p;
    c = c >> 1;
  }
  int zeros = cl - p.size();
  if (zeros < 0) {
    std::cout << "\nWarning: Overflow. code " << c
              << " is too big to be coded by " << cl << " bits!\n";
    p = p.substr(p.size() - cl);
  } else {
    for (int i = 0; i < zeros;
         ++i) // pad 0s to left of the binary code if needed
      p = "0" + p;
  }
  return p;
  */

  std::bitset<12> b(v.front());
  std::string s = b.to_string();
  return s;
}


/*
int binary_string_to_int(std::string p) {
  if(p.size() < 0)
    return 0;

  p = p.at(0) == '1' ? ++code :
      ++code;
      break;

    p = p.substr(1);
    while (p.size() > 0) {
       code = code << 1;
      if (p.at(0) == '1')
        ++code;
      p = p.substr(1);
    }

   return code;

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
*/
/*

void binaryIODemo(std::vector<int> compressed) {
  std::string bcode;
  for (std::vector<int>::iterator it = compressed.begin();
       it != compressed.end(); ++it) {
    int bits = *it < 256 ? 8 : 9;
    // bits = 12;
    std::string p = int_to_binary_string(*it, bits);
    std::cout << "c=" << *it << " : binary string=" << p
              << "; back to code=" << binary_string_to_int(p) << "\n";
    bcode += p;
  }

  // Writing to file
  std::cout << "string 2 save : " << bcode << "\n";
  std::string fileName = "example435.lzw";
  std::ofstream outfile(fileName.c_str(), std::ios::binary);

  std::string zeros = "00000000";
  if (bcode.size() %
      8) // Make sure the length of the binary string is a multiple of 8
    bcode += zeros.substr(0, 8 - bcode.size() % 8);

  for (int i = 0; i < bcode.size(); i += 8) {
    int b = 1;
    for (int j = 0; j < 8; j++) {
      b = b << 1;
      if (bcode.at(i + j) == '1')
        b += 1;
    }
    char c = (char)(b & 255); // Save the string byte by byte
    outfile.write(&c, 1);
  }
  outfile.close();


  struct stat filestatus;
  stat(fileName.c_str(), &filestatus);
  auto fsize = filestatus.st_size; // Get the size of the file in bytes

  char c2[fsize];
  infile.read(c2, fsize);

  std::string s;
  for (auto i = 0; i < fsize; ++i) {
    signed char uc = (unsigned char)c2[i];

    std::string p; // a binary string
    for (int j = 0; j < 8; ++j) {
      assert(uc != 0);
      p = (uc % 2 == 0) ? ("0" + p) : ("1" + p);
      uc = uc >> 1;
    }
    p = zeros.substr(0, 8 - p.size()) + p; // Pad 0s to left if needed
    s += p;
    ++i;
  }
  infile.close();
  std::cout << " saved string : " << s << "\n";
}
*/


int main(int argc, char *argv[]) {
  try {

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

    //\DELETE std::cout << in << "\n"; // output string of file contents

    switch (*argv[1]) {
      // Compress input file
      // If program was run passing c and filename to compress
      case 'c': {
        // Pass input file contents string to get compressed
        // and pass empty vector t
        std::vector<int> v = compress(in);

        // Add .lzw extension to input file name
        filename.append(".lzw");

        for(auto x : v)
          std::cout << x << " ";

      }
      // binaryIODemo(v);
      // Expand input file
      case 'e': {
        // Save expanded file as filename2
        // filename2 should be identical to filename
        std::map<int, std::string> dictionary = decompression_dictionary();
        // std::string dev = decompress(v.begin(), v.end());
        // std::cout << "\n" << dev << "\n";
      }
    }
  } 
  catch (char const *err) {
    std::cout << "The library threw an exception:\n" << err << "\n";
  }

  return 0;
}
