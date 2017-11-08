#include "lzw.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <sys/stat.h>
#include <vector>

std::string int_to_binary(int c, int cl) {
  std::string p = ""; // a binary code string with code length = cl
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
  }

  else {
    for (int i = 0; i < zeros;
         i++) // pad 0s to left of the binary code if needed
      p = "0" + p;
  }

  return p;
}

int binary_to_int(std::string p) {
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

void binaryIODemo(std::vector<int> compressed) {
  int c = 69;
  int bits = 9;
  std::string p = int_to_binary(c, bits);
  std::cout << "c=" << c << " : binary string=" << p
            << "; back to code=" << binary_to_int(p) << "\n";

  std::string bcode = "";
  for (std::vector<char>::iterator it = compressed.begin();
       it != compressed.end(); ++it) {
    if (*it < 256)
      bits = 8;
    else
      bits = 9;

    bits = 12;
    p = int_to_binary(*it, bits);
    std::cout << "c=" << *it << " : binary string=" << p
              << "; back to code=" << binary_to_int(p) << "\n";
    bcode += p;
  }

  // writing to file
  std::cout << "string 2 save : " << bcode << "\n";
  std::string fileName = "example435.lzw";
  std::ofstream myfile;
  myfile.open(fileName.c_str(), std::ios::binary);

  std::string zeros = "00000000";
  if (bcode.size() % 8 !=
      0) // make sure the length of the binary string is a multiple of 8
    bcode += zeros.substr(0, 8 - bcode.size() % 8);

  int b;
  for (int i = 0; i < bcode.size(); i += 8) {
    b = 1;
    for (int j = 0; j < 8; j++) {
      b = b << 1;
      if (bcode.at(i + j) == '1')
        b += 1;
    }
    char c = (char)(b & 255); // save the string byte by byte
    myfile.write(&c, 1);
  }
  myfile.close();

  // reading from a file
  std::ifstream myfile2;
  myfile2.open(fileName.c_str(), std::ios::binary);

  struct stat filestatus;
  stat(fileName.c_str(), &filestatus);
  long fsize = filestatus.st_size; // get the size of the file in bytes

  char c2[fsize];
  myfile2.read(c2, fsize);

  std::string s = "";

  long count = 0;
  while (count < fsize) {
    unsigned char uc = (unsigned char)c2[count];

    std::string p = ""; // a binary string

    for (int j = 0; j < 8 && uc > 0; j++) {
      if (uc % 2 == 0)
        p = "0" + p;
      else
        p = "1" + p;
      uc = uc >> 1;
    }
    p = zeros.substr(0, 8 - p.size()) + p; // pad 0s to left if needed
    s += p;
    count++;
  }
  myfile2.close();
  std::cout << " saved string : " << s << "\n";
}
