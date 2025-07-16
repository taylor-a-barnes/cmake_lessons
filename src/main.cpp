#include <iostream>
#include <yaml-cpp/yaml.h>
#include <brotli/encode.h>
#include <fstream>

bool read_file(const std::string& filename, std::vector<uint8_t>& buffer) {
    std::ifstream input(filename, std::ios::binary);
    if (!input) return false;

    input.seekg(0, std::ios::end);
    size_t size = input.tellg();
    buffer.resize(size);
    input.seekg(0, std::ios::beg);
    input.read(reinterpret_cast<char*>(buffer.data()), size);
    return true;
}

bool write_file(const std::string& filename, const std::vector<uint8_t>& buffer) {
    std::ofstream output(filename, std::ios::binary);
    if (!output) return false;

    output.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
    return true;
}

bool compress(const std::string& readname, const std::string& writename) {
  std::vector<uint8_t> uncompressed_data;
  std::vector<uint8_t> compressed_data;

  // Read the input data
  bool success = read_file(readname, uncompressed_data);
  if (!success) {
    std::cerr << "Reading file failed.\n";
    return false;
  }

  // Set the initial size of the buffer for the compressed data
  size_t max_compressed_size = BrotliEncoderMaxCompressedSize(uncompressed_data.size());
  compressed_data.resize(max_compressed_size);
  size_t encoded_size = max_compressed_size;
  std::cout << "Uncompressed size: " << uncompressed_data.size() << "\n";

  // Use brotli to compress the data
  int quality = 11;
  int lgwin = 22;
  success = BrotliEncoderCompress(
    quality,
    lgwin,
    BROTLI_MODE_GENERIC,
    uncompressed_data.size(), uncompressed_data.data(),
    &encoded_size, compressed_data.data()
  );
  if (!success) return false;

  // Resize the output buffer and write it
  compressed_data.resize(encoded_size);
  std::cout << "Compressed size: " << compressed_data.size() << "\n";
  success = write_file(writename, compressed_data);
  if (!success) {
    std::cerr << "Writing file failed.\n";
    return false;
  }

  return true;
}

int main() {
  YAML::Node books = YAML::LoadFile("data/books.yaml");

  for (const auto& pair : books["books"]) {
    const auto& book = pair.second;

    std::cout << book["name"] << "\n";
    std::cout << "    Pages: " << book["pages"].as<std::string>() << "\n";
    std::cout << "\n";
  }

  // Compress the file
  bool success = compress("data/books.yaml", "books.br");
  if (!success) {
    std::cerr << "Compression failed.\n";
    return 1;
  }


  return 0;
}
