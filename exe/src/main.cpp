#include <iostream>
#include <cmpwrapper.h>

int main() {

  // Compress the file
  bool success = compress("data/books.yaml", "books.br");
  if (!success) {
    std::cerr << "Compression failed.\n";
    return 1;
  }

  return 0;
}
