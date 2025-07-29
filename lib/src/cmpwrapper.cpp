#include <iostream>
#if defined(__use_brotli)
#include <brotli/encode.h>
#elif defined(__use_zstd)
#include <zstd.h>
#endif
#include <fstream>
#include <vector>

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
#if defined(__use_brotli)
  size_t max_compressed_size = BrotliEncoderMaxCompressedSize(uncompressed_data.size());
#elif defined(__use_zstd)
  size_t max_compressed_size = ZSTD_compressBound(uncompressed_data.size());
#endif
  compressed_data.resize(max_compressed_size);
  size_t encoded_size = max_compressed_size;
  std::cout << "Uncompressed size: " << uncompressed_data.size() << "\n";

#if defined(__use_brotli)
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
#elif defined(__use_zstd)
  int compression_level = 3;
  encoded_size = ZSTD_compress(
    compressed_data.data(),
    max_compressed_size,
    uncompressed_data.data(),
    uncompressed_data.size(),
    compression_level
  );
  if (ZSTD_isError(encoded_size)) {
    return false;
  }
#endif

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
