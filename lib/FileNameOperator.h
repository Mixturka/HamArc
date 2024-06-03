#pragma once
#include <iostream>

std::string GetPathWithoutFileName(const std::string& file_path);

std::string ExtractFileName(const std::string& file_path);

std::string RemoveExtension(const std::string& file_name);

std::string GetPathFromFolder(const std::string& full_path,
                              const std::string& folder_name);

std::string GetLatestFolderName(const std::string& path);
