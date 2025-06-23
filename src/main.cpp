#include <iostream>
#include <yaml-cpp/yaml.h>

int main() {
  YAML::Node books = YAML::LoadFile("data/books.yaml");

  for (const auto& pair : books["books"]) {
    const auto& book = pair.second;

    std::cout << book["name"] << "\n";
    std::cout << "    Pages: " << book["pages"].as<std::string>() << "\n";
    std::cout << "\n";
  }

  return 0;
}
