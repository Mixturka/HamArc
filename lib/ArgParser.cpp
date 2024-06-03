#include "ArgParser.h"

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>

#include "Archiver.h"

void ParseArgs(uint64_t argc, char* argv[], Arguments& args) {
  int i = 0;
  while (i < argc) {
    if (std::strncmp(argv[i], "--create", 8) == 0 ||
        std::strcmp(argv[i], "-c") == 0) {
      args.create = true;
    } else if (std::strncmp(argv[i], "--file=", 7) == 0) {
      std::string file_name = std::string(argv[i]).substr(7);
      args.file = file_name;
    } else if (std::strcmp(argv[i], "--list") == 0 ||
               std::strcmp(argv[i], "-l") == 0) {
      args.list = true;
    } else if (std::strcmp(argv[i], "--extract") == 0 ||
               std::strcmp(argv[i], "-x") == 0) {
      args.extract = true;
    } else if (std::strcmp(argv[i], "--append") == 0 ||
               std::strcmp(argv[i], "-a") == 0) {
      args.append = true;
    } else if (std::strcmp(argv[i], "--delete") == 0 ||
               std::strcmp(argv[i], "-d") == 0) {
      args.remove = true;
    } else if (std::strcmp(argv[i], "--concatenate") == 0 ||
               std::strcmp(argv[i], "-A") == 0) {
      args.concatenate = true;
    } else if (std::strcmp(argv[i], "-f") == 0) {
      if (i + 1 < argc) {
        args.file = argv[i + 1];
        ++i;
      }
    } else {
      if (args.extract) {
        args.files_to_extract.push_back(argv[i]);
      } else if (args.concatenate) {
        args.files_to_concatenate.push_back(argv[i]);
      } else if (args.remove) {
        args.files_to_delete.push_back(argv[i]);
      } else if (args.append) {
        args.files_to_append.push_back(argv[i]);
      } else if (args.create) {
        args.files_to_archive.push_back(argv[i]);
      }
    }
    ++i;
  }
}

int main(int argc, char* argv[]) {
  Arguments args;
  ParseArgs(argc, argv, args);

  if (args.CheckOperCount()) {
    if (args.IsCreate()) {
      std::string arc_name = args.GetFileName();
      std::vector<std::string> files_to_archive = args.GetFilesToArchive();
      CreateArchive(arc_name, files_to_archive);
    } else if (args.IsList()) {
      std::string arc_name = args.GetFileName();
      ListArchivedFileNames(arc_name);
    } else if (args.IsExtract()) {
      std::string arc_name = args.GetFileName();
      std::vector<std::string> files_to_extract = args.GetFilesToExtract();

      if (files_to_extract.size() != 0) {
        for (int i = 0; i < files_to_extract.size(); ++i) {
          ExtractArchive(arc_name, files_to_extract[i]);
        }
      } else {
        ExtractArchive(arc_name, "");
      }
    } else if (args.IsAppend()) {
      std::string arc_name = args.GetFileName();
      std::vector<std::string> files_to_append = args.GetFilesToAppend();

      for (int i = 0; i < files_to_append.size(); ++i) {
        AddFileToArchive(arc_name, files_to_append[i], "");
      }
    } else if (args.IsDelete()) {
      std::string arc_name = args.GetFileName();
      std::vector<std::string> files_to_delete = args.GetFilesToDelete();

      for (int i = 0; i < files_to_delete.size(); ++i) {
        DeleteFileFromArchive(arc_name, files_to_delete[i]);
      }
    } else if (args.IsConcatenate()) {
      std::string arc_name = args.GetFileName();
      std::vector<std::string> files_to_delete = args.GetFilesToConcatenate();

      if (files_to_delete.size() != 2) {
        std::cerr << "Too many or too few files to merge!\n";
        exit(EXIT_FAILURE);
      } else {
        ConcatenateArchives(files_to_delete[0], files_to_delete[1], arc_name);
      }
    } else if (args.IsList()) {
    }
  } else {
    std::cerr << "Please make sure you operating 1 command per execution!";
    exit(EXIT_FAILURE);
  }
}
