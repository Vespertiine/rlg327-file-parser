#pragma once
#include <fstream>
#include <vector>
#include <map>

namespace vespertiine
{
  using strmap = std::map<std::string,std::string>;
  using svector = std::vector<strmap>;
  using hpair = std::pair<std::string, long unsigned int>;

  class FileParser
  {
  private:
    std::ifstream in;
    std::string file_type;
    int file_version;
    svector vec;
    hpair parseHeader(const std::string);
    strmap parseEntity(std::istream&);
    std::string parseMultilineKeyValue(std::istream&);
    void runner();
  public:
    FileParser(std::string);
    const svector getEntities() const { return vec; }
    const unsigned int getFileVersion() const { return file_version; }
    const std::string getFileType() const { return file_type; }
    friend std::ostream& operator<<(std::ostream& output, const FileParser &F);
  };
}
