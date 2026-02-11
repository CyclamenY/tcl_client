#ifndef UTILITY_H_
#define UTILITY_H_

#include <string>
#include <vector>

namespace Utility {
class File {
public:
  static std::string getCurrentPwdPath();
  static void getDirFiles(const std::string& target_path, std::vector<std::string>& file_vec);
  static bool createDir(const std::string& path);
protected:
  File() = delete;
  ~File() = delete;
};

class String {
public:
  static bool split(const std::string& str, std::vector<std::string>& split_str, const char& delim = ' ');
protected:
  String() = delete;
  ~String() = delete;
};

} // end of namepace utilty

#endif