#include <functional>
#include "FileParser.hpp"

/**
  \file FileParser.cpp

  \author Freya Gaynor
  \date 2018-04-13

  A modular file parser for COM S 327 wtih Dr. Shaeffer.
*/

/**
  \addtogroup vespertiine
  @{
*/
namespace vespertiine
{
  using string = std::string;
  using istream = std::istream;
  using entity_vector = std::vector<entity>;

  FileParser::FileParser(string filepath)
  {
    in.open(filepath);
    runner();
    in.close();
  }

  FileParser::FileParser(std::vector<entity> entities,
    std::string entity_type,
    std::string file_type, unsigned int version_number)
  {
    entity_type = trim(entity_type.substr(0, entity_type.find(' ')));
    file_type = trim(file_type.substr(0, file_type.find(' ')));
    this->file_type = "";
    for (auto &c : entity_type) this->file_type += (char) std::toupper(c);
    this->file_type += " ";
    for (auto &c : file_type) this->file_type += (char) std::toupper(c);
    file_version = version_number;

    for(auto &e : entities)
    {
      entity tmp;
      for (auto &p : e)
      {
        string key = "";
        for (auto &c : p.first) key += (char) std::toupper(c);
        tmp[key] = p.second;
      }

      this->entities.push_back(tmp);
    }
  }

  const entity_vector FileParser::getEntities() const { return entities; }
  const unsigned int FileParser::getFileVersion() const { return file_version; }
  const string FileParser::getFileType() const { return file_type; }

  std::ostream& operator<<(std::ostream& output, const vespertiine::FileParser &F)
  {
    string etype = F.getFileType();
    etype = etype.substr(0, etype.find(' '));
    output << "RLG327 " << F.getFileType() << " " << F.getFileVersion() << std::endl
    << std::endl;

    for (auto &map : F.getEntities())
    {
      output << "BEGIN " << etype << std::endl;
      for (auto &pair : map)
      {
        if (pair.second.find('\n') != string::npos)
        {
          output << pair.first << std::endl << pair.second << std::endl << "."
          << std::endl;
          continue;
        }
        output << pair.first << " " << pair.second << std::endl;
      }
      output << "END" << std::endl << std::endl;
    }
    return output;
  }

  void FileParser::runner()
  {
    string buffer;
    if (!in) throw std::invalid_argument(
      "Invalid File:\n\tCheck that it exists and is accessible."
    );


    if (!parseline(in, buffer)) throw std::invalid_argument(
      "Invalid File:\n\tReached EOF before reading any fields."
    );

    file_header header = parseHeader(buffer);
    file_type = header.first;
    file_version = header.second;
    while (parseline(in, buffer))
    {
      long unsigned int index = buffer.find(' ');
      if (index != string::npos
        && buffer.substr(0, index) == "BEGIN"
      )
      {
        string key = buffer.substr(index + 1, buffer.size() - index - 1);
        if (file_type.substr(0, file_type.find(' ')) != key)
          throw std::invalid_argument(
            (string) "Invalid File Syntax"
            + "\n\tCheck that the file keys match the file type."
          );

        entities.push_back(parseEntity(in));
      }
    }
  }

  file_header FileParser::parseHeader(const string buffer)
  {
    string numeric = "0123456789";
    auto index = buffer.find("RLG327");
    if (index == string::npos) throw std::invalid_argument(
      (string) "Invalid File Syntax"
      + "\n\tCheck that file contains RLG327 on the first line."
    );

    index = buffer.find('\r');
    if (index != string::npos) throw std::invalid_argument(
      (string) "Invalid File"
      + "\n\tFileParser can only read files with LF-style newlines."
    );

    auto ftype =
      buffer.substr(buffer.find(' ') + 1,
        buffer.find_last_not_of(numeric) - buffer.find(' ') - 1
      );

    auto fversion =
      stoi(buffer.substr(buffer.find_last_not_of(numeric), buffer.size()));
    return make_pair(ftype, fversion);
  }

  entity FileParser::parseEntity(istream& is)
  {
    string buffer;
    entity e;
    long unsigned int index;
    string key;
    while (1)
    {
      auto loc = in.tellg();
      parseline(in, buffer);
      if (buffer == "END") break;
      in.seekg(loc, std::ios_base::beg);
      parseline(is, buffer);
      index = buffer.find(' ');
      if (index == string::npos)
      {
        e[buffer] = parseMultilineKeyValue(is);
        continue;
      }
      key = buffer.substr(0, index);
      e[key] = buffer.substr(index + 1, buffer.size() - index - 1);
    }

    return e;
  }

  file_value FileParser::parseMultilineKeyValue(istream& is)
  {
    string buffer = "";
    file_value val;

    while (1)
    {
      if (!parseline(is, buffer))
        throw std::invalid_argument(
          (string) "Invalid File Syntax\n"
          + "\tReached EOF before finding multiline property terminator [.].");
      if (buffer == ".") break;
      val += buffer + "\n";
    }

    val.pop_back();
    return val;
  }

  string FileParser::trim(string str)
  {
    std::size_t pos = 0;
    while (!str.empty() && std::isspace(str.back())) str.pop_back();
    while (pos < str.size() && std::isspace(str[pos])) pos++;
    return str.substr(pos);
  }

  inline istream& FileParser::parseline(istream& is, string& str)
  {
    return parseline(is, str, NULL);
  }

  istream& FileParser::parseline(istream& is, string& str, char* delim)
  {
    istream& stream = (delim == NULL)
      ? getline(is, str)
      : getline(is, str, *delim);
    if (stream) trim(str);
    return stream;
  }
}
/** }@ */
