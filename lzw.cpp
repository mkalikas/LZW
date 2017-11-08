#include "lzw.hpp"
#include <sys/stat.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>

std::map<std::string, int> &lzw::build_dictionary() {
  // Build the dictionary.
  int dictionary_size = 256;
  for (int x = 0; x < dictionary_size; ++x) dictionary[std::string(1, x)] = x;

  for (auto x : dictionary) {
    std::cout << x << "\n";
  }

  return dictionary;
}
std::string lzw::int_to_binary(int c, int cl) {
  std::string p;  // a binary code string with code length = cl
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
         ++i)  // pad 0s to left of the binary code if needed
      p = "0" + p;
  }
  return p;
}

int lzw::binary_to_int(std::string p) {
  /*if(p.size() < 0)
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
  */
  int code = 0;
  if (p.size() > 0) {
    if (p.at(0) == '1') code = 1;
    p = p.substr(1);
    while (p.size() > 0) {
      code = code << 1;
      if (p.at(0) == '1') code++;
      p = p.substr(1);
    }
  }
  return code;
}

void lzw::binaryIODemo(std::vector<int> compressed) {
  std::string bcode;
  for (std::vector<int>::iterator it = compressed.begin();
       it != compressed.end(); ++it) {
    int bits = *it < 256 ? 8 : 9;
    // bits = 12;
    std::string p = int_to_binary(*it, bits);
    std::cout << "c=" << *it << " : binary string=" << p
              << "; back to code=" << binary_to_int(p) << "\n";
    bcode += p;
  }

  // Writing to file
  std::cout << "string 2 save : " << bcode << "\n";
  std::string fileName = "example435.lzw";
  std::ofstream outfile(fileName.c_str(), std::ios::binary);

  std::string zeros = "00000000";
  if (bcode.size() %
      8)  // Make sure the length of the binary string is a multiple of 8
    bcode += zeros.substr(0, 8 - bcode.size() % 8);

  for (int i = 0; i < bcode.size(); i += 8) {
    int b = 1;
    for (int j = 0; j < 8; j++) {
      b = b << 1;
      if (bcode.at(i + j) == '1') b += 1;
    }
    char c = (char)(b & 255);  // Save the string byte by byte
    outfile.write(&c, 1);
  }
  outfile.close();

  // Reading from a file
  std::ifstream infile(fileName.c_str(), std::ios::binary);

  struct stat filestatus;
  stat(fileName.c_str(), &filestatus);
  auto fsize = filestatus.st_size;  // Get the size of the file in bytes

  char c2[fsize];
  infile.read(c2, fsize);

  std::string s;
  for (auto i = 0; i < fsize; ++i) {
    signed char uc = (unsigned char)c2[i];

    std::string p;  // a binary string
    for (int j = 0; j < 8; ++j) {
      assert(uc != 0);
      p = (uc % 2 == 0) ? ("0" + p) : ("1" + p);
      uc = uc >> 1;
    }
    p = zeros.substr(0, 8 - p.size()) + p;  // Pad 0s to left if needed
    s += p;
    ++i;
  }
  infile.close();
  std::cout << " saved string : " << s << "\n";
}
