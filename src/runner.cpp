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
      try
      {
        std::cout << e.at("NAME") << std::endl;
      }
      catch (const std::out_of_range& ex)
      {
        std::cerr << ex.what() << std::endl << "One of the entities in the"
          << " parsed file did not contain a 'NAME' symbol.";
      }
    }
  );

  // you could, optionally, choose to add 'entity' as a using statement
  using vespertiine::entity;

  entity e = entity_vector[0];
  std::cout << std::endl << "SYMB: " << e["SYMB"] << std::endl;

  // While it's discouraged, you can also use namespaces.
  using namespace vespertiine;

  file_value val = e["DESC"];
  std::cout << std::endl << "DESC: " << val << std::endl;


  // you can create a parser with your own definitions
  std::vector< std::map< std::string, std::string> > new_vector;
  std::map<std::string, std::string> new_entity;
  new_entity["NAME"] = "Lara Croft";
  new_entity["Desc"] = "Keys will automatically save as uppercase.";
  new_vector.push_back(new_entity);

  FileParser p2(new_vector, "CHARACTER TRIMMED", "DESCRIPTION", 2);

  // you can also export to a file if you were so inclined
  std::ofstream fout;
  fout.open("./samples/export.txt");
  fout << p2;
  return 0;
}
