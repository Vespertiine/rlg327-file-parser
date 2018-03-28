#pragma once
#include <fstream>
#include <vector>
#include <map>

namespace vespertiine
{
  using string = std::string;
  using istream = std::istream;
  using strmap = std::map<string,string>;
  using svector = std::vector< std::map<string,string> >;
  using hpair = std::pair<string, long unsigned int>;

  class FileParser
  {
  private:
    std::ifstream in;
    string file_type;
    int file_version;
    svector vec;
    void runner();
  public:
    FileParser(string);
    hpair parseHeader(const string);
    strmap parseEntity(istream&);
    string parseMultilineKeyValue(istream&);
    const svector getEntities() const { return vec; }
    const unsigned int getFileVersion() const { return file_version; }
    const string getFileType() const { return file_type; }
  };
}
