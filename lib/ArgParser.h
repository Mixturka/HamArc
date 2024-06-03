#pragma once
#include <iostream>
#include <vector>

class Arguments {
 public:
  bool CheckOperCount() {
    if (list + create + extract + append + remove + concatenate > 1) {
      return false;
    }
    return true;
  }

  bool IsCreate() { return (create) ? true : false; }

  bool IsList() { return (list) ? true : false; }

  bool IsExtract() { return (extract) ? true : false; }

  bool IsAppend() { return (append) ? true : false; }

  bool IsDelete() { return (remove) ? true : false; }

  bool IsConcatenate() { return (concatenate) ? true : false; }

  std::string GetFileName() { return file; }

  std::vector<std::string> GetFilesToArchive() { return files_to_archive; }

  std::vector<std::string> GetFilesToExtract() { return files_to_extract; }

  std::vector<std::string> GetFilesToAppend() { return files_to_append; }

  std::vector<std::string> GetFilesToDelete() { return files_to_delete; }

  std::vector<std::string> GetFilesToConcatenate() {
    return files_to_concatenate;
  }

 private:
  bool create = false;
  bool list = false;
  bool extract = false;
  bool append = false;
  bool remove = false;
  bool concatenate = false;

  std::string file;
  std::vector<std::string> files_to_archive;
  std::vector<std::string> files_to_extract;
  std::vector<std::string> files_to_append;
  std::vector<std::string> files_to_delete;
  std::vector<std::string> files_to_concatenate;

  friend void ParseArgs(uint64_t argc, char* argv[], Arguments& args);
};
