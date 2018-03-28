#include <iostream>
#include "FileParser.hpp"
int main(int argc, char const *argv[])
{
  std::string file_path = "./samples/desc01.txt";

  if (argc > 2) std::cerr << "Error: Only one argument accepted." << std::endl;
  if (argc > 1) file_path = argv[1];
  vespertiine::FileParser p(file_path);

  auto entity_vector = p.getEntities();
  for (auto &e : entity_vector)
    // print the 'NAME' field for each entity.
    std::cout << e["NAME"] << std::endl;

  return 0;
}
