#include <functional>
#include "FileParser.hpp"

// temporary
#include <iostream>

namespace vespertiine
{
  /* ==================== local prototypes ==================== */
  inline istream& parseline(istream&, string&);
  istream& parseline(istream&, string&, char*);
  string trim(string);
  /* ================== end local prototypes ================== */

  FileParser::FileParser(string filepath)
  {
    in.open(filepath);
    runner();
    in.close();
  }

  const svector FileParser::getEntities() const
  {
    return vec;
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

    auto header_pair = parseHeader(buffer);
    file_type = header_pair.first;
    file_version = header_pair.second;
    while (parseline(in, buffer))
    {
      unsigned long int index = buffer.find(' ');
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

        vec.push_back(parseEntity(in));
      }
    }
  }

  hpair FileParser::parseHeader(const string buffer)
  {
    string numeric = "0123456789";
    auto index = buffer.find("RLG327");
    if (index == string::npos) throw std::invalid_argument(
      (string) "Invalid File Syntax"
      + "\n\tCheck that file contains RLG327 on the first line."
    );

    auto ftype =
      buffer.substr(buffer.find(' ') + 1,
        buffer.find_last_not_of(numeric) - buffer.find(' ') - 1
      );
    auto fversion =
      stoi(buffer.substr(buffer.find_last_not_of(numeric), buffer.size()));
    return make_pair(ftype, fversion);
  }

  strmap FileParser::parseEntity(istream& is)
  {
    string buffer;
    strmap m;
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
        m[buffer] = parseMultilineKeyValue(is);
        continue;
      }
      key = buffer.substr(0, index);
      m[key] = buffer.substr(index + 1, buffer.size() - index - 1);
    }

    return m;
  }

  string FileParser::parseMultilineKeyValue(istream& is)
  {
    string buffer = "";
    string str;

    while (1)
    {
      if (!parseline(is, buffer))
        throw std::invalid_argument(
          (string) "Invalid File Syntax\n"
          + "\tReached EOF before finding multiline property terminator [.].");
      if (buffer == ".") break;
      str += buffer + "\n";
    }

    str.pop_back();
    return str;
  }

  /* ==================== local utilities ==================== */
  /*
    Returns: param str with all whitespace outside of the first and
      last characters removed.
  */
  string trim(string str)
  {
    while (!str.empty() && std::isspace(str.back())) str.pop_back();
    std::size_t pos = 0;
    while (pos < str.size() && std::isspace(str[pos])) pos++;
    return str.substr(pos);
  }

  // acts as a wrapper over std::getline() which also trims whitespace
  inline istream& parseline(istream& is, string& str)
  {
    return parseline(is, str, NULL);
  }

  // acts as a wrapper over std::getline() which also trims whitespace
  istream& parseline(istream& is, string& str, char* delim)
  {
    istream& stream = (delim == NULL)
      ? getline(is, str)
      : getline(is, str, *delim);
    if (stream) trim(str);
    return stream;
  }
  /* ================== end local utilities ================== */
}