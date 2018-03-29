#pragma once
#include <fstream>
#include <vector>
#include <map>

namespace vespertiine
{
  using file_key = std::string;
  using file_value = std::string;
  using entity = std::map<file_key,file_value>;
  using file_header = std::pair<std::string, unsigned int>;

  class FileParser
  {
  private:
    std::ifstream in;
    std::string file_type;
    int file_version;
    std::vector<entity> vec;
    file_header parseHeader(const std::string);
    entity parseEntity(std::istream&);
    file_value parseMultilineKeyValue(std::istream&);
    void runner();
  public:
    FileParser(std::string);
    const std::vector<entity> getEntities() const;
    const unsigned int getFileVersion() const;
    const std::string getFileType() const;
    friend std::ostream& operator<<(std::ostream& output, const FileParser &F);
  };
}
