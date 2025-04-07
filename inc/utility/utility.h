#ifndef UTILITY_H_
#define UTILITY_H_

#include <string>
#include <vector>

class Utility {
public:
  static std::string getCurrentPwdPath();
  static void getDirFiles(const std::string& target_path, std::vector<std::string>& file_vec);
  static bool createDir(const std::string& path);
protected:
  Utility() = delete;
  ~Utility() = delete;
};

#endif