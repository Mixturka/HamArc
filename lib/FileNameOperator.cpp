#include "FileNameOperator.h"

std::string GetPathWithoutFileName(const std::string& file_path) {
  uint64_t pos = file_path.find_last_of("/\\");

  if (pos != std::string::npos) {
    return file_path.substr(0, pos + 1);
  }

  return std::string();
}

std::string ExtractFileName(const std::string& file_path) {
  uint64_t last_slash_pos = file_path.find_last_of("/\\");

  if (last_slash_pos == std::string::npos) {
    return file_path;
  }

  return file_path.substr(last_slash_pos + 1);
}

std::string RemoveExtension(const std::string& file_name) {
  uint64_t last_dot = file_name.find_last_of(".");
  if (last_dot == std::string::npos) return file_name;

  return file_name.substr(0, last_dot);
}

std::string GetPathFromFolder(const std::string& full_path,
                              const std::string& folder_name) {
  uint64_t pos = full_path.find("/" + folder_name);
  if (pos != std::string::npos) {
    return full_path.substr(pos);
  } else {
    return std::string("Folder found in the path");
  }
}

std::string GetLatestFolderName(const std::string& path) {
  if (path.empty()) {
    return "";
  }

  std::string normalized_path = path;
  std::replace(normalized_path.begin(), normalized_path.end(), '\\', '/');

  uint64_t last_pos = normalized_path.length() - 1;
  if (normalized_path[last_pos] == '/' && normalized_path.length() >= 1) {
    --last_pos;
  }

  uint64_t slash_pos = normalized_path.find_last_of('/', last_pos);

  if (slash_pos != std::string::npos && normalized_path.length() >= 2) {
    return normalized_path.substr(slash_pos + 1, last_pos - slash_pos);
  }
  return normalized_path;
}
