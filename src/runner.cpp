#include <iostream>
#include <algorithm> 
#include "FileParser.hpp"
int main(int argc, char const *argv[])
{
  std::string file_path = "./samples/desc01.txt";

  if (argc > 2) std::cerr << "Error: Only one argument accepted." << std::endl;
  if (argc > 1) file_path = argv[1];

  // create the file parser using a path string like so
  vespertiine::FileParser p(file_path);

  // retrieve data from the file parser like so
  std::vector<std::map<std::string,std::string> > entity_vector = p.getEntities();

  // or with the namespace type
  std::vector<vespertiine::entity> entity_vector3 = p.getEntities();

  // or with auto : recommended
  auto entity_vector2 = p.getEntities();

  std::for_each(entity_vector.begin(), entity_vector.end(),
    [](vespertiine::entity const& e)
    {
      std::cout << e.at("NAME") << std::endl;
    }
  );

  // you can also export to a file if you were so inclined
  std::ofstream fout;
  fout.open("./samples/export.txt");
  fout << p;
  return 0;
}
