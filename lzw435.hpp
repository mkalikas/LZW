#pragma once

#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <bitset>
#include <sys/stat.h>
#include <vector>


// All files used for lzw project part 1
std::map<std::string, int> compression_dictionary();
std::vector<int> compress(const std::string &);
std::map<int, std::string> decompression_dictionary();
std::string decompress(std::string &);
std::string int_to_binary_string(std::vector<int> &, std::string);
int binary_string_to_int(std::string p);
