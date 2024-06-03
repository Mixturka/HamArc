#pragma once
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void Traverse(const fs::path& directory, std::string& arc_name,
              std::string& root);

void ArchiveFolder(std::string& arc_name, std::string& file_names);

bool IsPower2(int x);

char* EncodeHam74(const char& data);

char DecodeHam74(const char* data);

void CreateArchive(std::string& arc_name, std::vector<std::string>& file_names);

bool CheckArcSignature(std::ifstream& arc);

bool CheckBlockStartSign(const char* data);

bool CheckBlockEndSign(const char* data);

void ListArchivedFileNames(std::string arc_name);

void DeleteFileFromArchive(std::string arc_name,
                           std::string file_name_to_delete);

void AddFileToArchive(std::string arc_name, std::string file_name_to_add,
                      std::string folder_name);

void ExtractArchive(std::string arc_name, std::string file_name_to_extract);

void ConcatenateArchives(std::string first_arc_name,
                         std::string second_arc_name,
                         std::string result_arc_name);
