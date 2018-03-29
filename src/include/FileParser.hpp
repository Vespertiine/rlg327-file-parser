#pragma once
#include <fstream>
#include <vector>
#include <map>

namespace vespertiine
{
  using strmap = std::map<std::string,std::string>;
  using svector = std::vector<strmap>;
  using hpair = std::pair<std::string, unsigned int>;

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
    const svector getEntities() const;
    const unsigned int getFileVersion() const;
    const std::string getFileType() const;
    friend std::ostream& operator<<(std::ostream& output, const FileParser &F);
  };
}
