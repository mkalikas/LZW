#include <iostream>
#include "lzw.hpp"

int main(int argc, char const *argv[]) {
  try {
    lzw lzw;
    // Run program passing c and filename to compress
    switch (*argv[1]) {
      // Compress input file
    case 'c': {
      // Save v file as filename.lzw
      std::vector<char> v;
      if (FILE *fp = fopen("filename", "r")) {
        char buf[1024];
        while (size_t len = fread(buf, 1, sizeof(buf), fp))
          v.insert(v.end(), buf, buf + len);
        fclose(fp);
      }
      compress(v, v.end());
      copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, ", "));
      lzw.binaryIODemo(v);
      }
        // Expand input file
      case 'e': {
        // Save expanded file as filename2
        // filename2 should be identical to filename
        std::string dev = decompress(v.begin(), v.end());
        std::cout << "\n" << dev << "\n";
      }
    }
    catch (char const *err) {
      std::cout << "The library threw an exception:\n" << err << std::endl;
    }

    return 0;
  }
