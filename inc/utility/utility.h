#ifndef UTILITY_H_
#define UTILITY_H_

#include <string>
#include <vector>

class Utility {
public:
  static std::string getCurrentPwdPath();
  static void getDirFiles(std::string target_path, std::vector<std::string>& file_vec);
protected:
  Utility() = delete;
  ~Utility() = delete;
};

#endif