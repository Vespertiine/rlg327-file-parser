#pragma once
#include <fstream>
#include <vector>
#include <map>

/**
  \addtogroup vespertiine
  @{
*/
namespace vespertiine
{
  using file_key = std::string;
  using file_value = std::string;
  using entity = std::map<file_key,file_value>;
  using file_header = std::pair<std::string, unsigned int>;

  /**
    \class FileParser

    \brief Parses COM S 327 DIF files to a vector of maps of string pairs.

    The constructor expects a string of a filepath.
    The file can be parsed under the following assumptions:
    1. Possess a header on the first line of the format "RLG327 [entity_type] [file_type] [version_number]"
    2. Possess entities wrapped in BEGIN [ENTITY_NAME] ... END tags on their own lines.
    3. Possess keys of at least two capital letters followed by a newline or a value.
      3a. If followed by a newline, the multiline values must end with a [.] on its own line.

    \author Freya Gaynor
    \date 2018-03-28
  */
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
    std::string trim(std::string);
    inline std::istream& parseline(std::istream&, std::string&);
    std::istream& parseline(std::istream&, std::string&, char*);
    void runner();
  public:
    /**
      \brief Construct and populate the FileParser.

      The constructor runs the entire program using a private runner()
      function. It will populate the private entity vector which can then be
      accessed using getEntities().

      \param[in] file_path A file path string.
      \warning Will throw std::invalid_argument exception if any of the filetype assumptions are violated.
    */
    FileParser(std::string file_path);

    /**
      \brief Return the contents of the file.

      \return A vector of maps of string pairs, e.g. vector<map<string,string>>>.
      Each vector will be an individual entity in the file, and each map will
      contain all of the values in that entity with a key of their symbol.
    */
    const std::vector<entity> getEntities() const;

    /**
      \brief Return the version of the file as a single unsigned integer.
      \return Version number of the file.
    */
    const unsigned int getFileVersion() const;

    /**
      \brief Return the type of the file as a string.

      The string will be of the format, "[ENTITY_TYPE] [FILE_TYPE]".

      \return The file type string.
    */
    const std::string getFileType() const;

    /**
      \brief Overload the output operator to match the style of the DIF type.

      This will output a matching file similar to whatever is provided, however
      it will be alphabetically ordered by keys within each individual entity.
    */
    friend std::ostream& operator<<(std::ostream& output, const FileParser &F);
  };
}
/**}@*/
