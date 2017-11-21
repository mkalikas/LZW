#pragma once

#include <map>
#include <string>
#include <vector>
#include <tuple>


// All functions used for lzw project part 2
std::map<std::string, int> compression_dictionary();
std::map<int, std::string> decompression_dictionary();
std::vector<int> compress(const std::string &);
std::string convert_to_bytes(std::string &);
std::vector<int> separate(std::string &, int);
std::string make_string(std::string &);
std::string decompress(std::vector<int> &);
std::tuple<int, int> get_code_and_length(std::vector<int>);
std::string int_to_binary_string(std::vector<int>, std::string);
int binary_string_to_int(std::string);
