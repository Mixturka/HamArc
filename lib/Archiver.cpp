#include "Archiver.h"

#include <cstdlib>
#include <fstream>
#include <vector>

#include "FileNameOperator.h"
#include "Hamming.h"

void Traverse(const fs::path& directory, std::string& arc_name,
              std::string& root) {
  for (const fs::directory_entry& entry : fs::directory_iterator(directory)) {
    if (fs::is_symlink(entry.symlink_status())) {
      continue;
    }
    if (fs::is_directory(entry.status())) {
      Traverse(entry.path(), arc_name, root);
    } else if (fs::is_regular_file(entry.status())) {
      std::cout << "File: " << entry.path() << std::endl;
      AddFileToArchive(arc_name, entry.path(), root);
    }
  }
}

void CreateArchive(std::string& arc_name,
                   std::vector<std::string>& file_names) {
  if (arc_name.find(".haf") == std::string::npos) {
    arc_name += ".haf";
  }

  std::ofstream arc(arc_name, std::ios::binary);

  if (!arc.is_open()) {
    std::cerr << "Unable to create archive: " + arc_name + ". Stopping...\n";
    exit(EXIT_FAILURE);
  }

  char sign[2] = {'H', 'A'};

  for (int i = 0; i < 2; ++i) {
    char* encoded = EncodeHam74(sign[i]);
    arc.write(encoded, 2);

    delete[] encoded;
  }
  for (int i = 0; i < file_names.size(); ++i) {
    if (fs::is_directory(file_names[i])) {
      arc.close();
      std::string last_folder = GetLatestFolderName(file_names[i]);
      Traverse(file_names[i], arc_name, last_folder);
      continue;
    } else {
      arc.close();
      AddFileToArchive(arc_name, file_names[i], "");
    }
  }
}

bool CheckArcSignature(std::ifstream& arc) {
  arc.seekg(0);
  char* sign1 = new char[2];
  char* sign2 = new char[2];

  arc.read(sign1, 2);
  arc.read(sign2, 2);

  char decoded_H = DecodeHam74(sign1);
  char decoded_A = DecodeHam74(sign2);

  delete[] sign1;
  delete[] sign2;

  if (decoded_H == 'H' && decoded_A == 'A') {
    return true;
  }

  return false;
}

void ListArchivedFileNames(std::string arc_name) {
  std::ifstream arc(arc_name, std::ios::binary);

  if (!arc.is_open()) {
    std::cerr << "Unable to open archive " + arc_name + ". Stopping...\n";
    exit(EXIT_FAILURE);
  }
  if (CheckArcSignature(arc)) {
    char ch;
    while (arc.peek() != EOF) {
      char* name_size_cstr = new char[16];
      arc.read(name_size_cstr, 16);
      char* decoded_name_size = new char[8];
      int it = 0;

      for (int i = 0; i < 16; i += 2) {
        char decoded[2] = {name_size_cstr[i], name_size_cstr[i + 1]};
        decoded_name_size[it] = DecodeHam74(decoded);
        ++it;
      }

      int64_t name_length;
      std::memcpy(&name_length, decoded_name_size, sizeof(name_length));

      char* name = new char[name_length];
      arc.read(name, name_length);

      std::string name_str;
      for (int i = 0; i < name_length; i += 2) {
        char decoded[2] = {name[i], name[i + 1]};
        name_str += DecodeHam74(decoded);
      }
      std::cout << name_str << '\n';
      char* file_size_cstr = new char[16];
      arc.read(file_size_cstr, 16);
      char* decoded_file_size = new char[8];
      it = 0;

      for (int i = 0; i < 16; i += 2) {
        char decoded[2] = {file_size_cstr[i], file_size_cstr[i + 1]};
        decoded_file_size[it] = DecodeHam74(decoded);
        ++it;
      }

      int64_t file_length;
      std::memcpy(&file_length, decoded_file_size, sizeof(file_length));
      arc.ignore(file_length);

      delete[] name;
      delete[] file_size_cstr;
      delete[] decoded_file_size;
      delete[] decoded_name_size;
      delete[] name_size_cstr;
    }
  } else {
    std::cerr << "Unable to read archive. Archive is corrupted!";
    exit(EXIT_FAILURE);
  }
}

void DeleteFileFromArchive(std::string arc_name,
                           std::string file_name_to_delete) {
  std::ifstream arc(arc_name, std::ios::binary);
  std::string temp_arc_name = arc_name + ".temp";
  std::ofstream temp_arc(temp_arc_name, std::ios::binary);

  if (!arc.is_open()) {
    std::cerr << "Unable to open archive " + arc_name + ". Stopping...\n";
    exit(EXIT_FAILURE);
  }

  if (!temp_arc.is_open()) {
    std::cerr << "Unable to create temporary archive. Stopping\n";
    exit(EXIT_FAILURE);
  }

  if (CheckArcSignature(arc)) {
    char sign[2] = {'H', 'A'};

    for (int i = 0; i < 2; ++i) {
      char* encoded = EncodeHam74(sign[i]);
      temp_arc.write(encoded, 2);

      delete[] encoded;
    }

    char ch;
    while (arc.peek() != EOF) {
      char* name_size_cstr = new char[16];
      arc.read(name_size_cstr, 16);
      char* decoded_name_size = new char[8];
      int it = 0;

      for (int i = 0; i < 16; i += 2) {
        char decoded[2] = {name_size_cstr[i], name_size_cstr[i + 1]};
        decoded_name_size[it] = DecodeHam74(decoded);
        ++it;
      }

      int64_t name_length;
      std::memcpy(&name_length, decoded_name_size, sizeof(name_length));

      char* name = new char[name_length];
      arc.read(name, name_length);
      std::string name_str;
      for (int i = 0; i < name_length; i += 2) {
        char decoded[2] = {name[i], name[i + 1]};
        name_str += DecodeHam74(decoded);
      }

      char* file_size_cstr = new char[16];
      arc.read(file_size_cstr, 16);
      char* decoded_file_size = new char[8];
      it = 0;

      for (int i = 0; i < 16; i += 2) {
        char decoded[2] = {file_size_cstr[i], file_size_cstr[i + 1]};
        decoded_file_size[it] = DecodeHam74(decoded);
        ++it;
      }

      int64_t file_length;
      std::memcpy(&file_length, decoded_file_size, sizeof(file_length));

      if (name_str != file_name_to_delete) {
        temp_arc.write(name_size_cstr, 2);
        temp_arc.write(name, name_length);
        temp_arc.write(file_size_cstr, 16);
        for (int i = 0; i < file_length; ++i) {
          arc.read(&ch, 1);
          temp_arc.write(&ch, 1);
        }
      } else {
        arc.ignore(file_length);
      }
      delete[] name;
      delete[] file_size_cstr;
      delete[] decoded_file_size;
      delete[] name_size_cstr;
      delete[] decoded_name_size;
    }
    arc.close();
    temp_arc.close();

    std::remove(arc_name.c_str());
    std::rename(temp_arc_name.c_str(), arc_name.c_str());
  } else {
    std::cerr << "Unable to read archive. Archive is corrupted!";
    exit(EXIT_FAILURE);
  }
}

void AddFileToArchive(std::string arc_name, std::string file_name_to_add,
                      std::string folder_name) {
  std::ifstream arc_in(arc_name, std::ios::binary);
  arc_in.seekg(0);
  std::ofstream arc_of(arc_name, std::ios::app | std::ios::binary);
  if (CheckArcSignature(arc_in)) {
    if (folder_name == "") {
      std::ifstream file_to_add(file_name_to_add, std::ios::binary);
      file_name_to_add = ExtractFileName(file_name_to_add);
      uint64_t name_length = file_name_to_add.length() * 2;

      char* name_size_cstr = reinterpret_cast<char*>(&name_length);
      for (int j = 0; j < sizeof(name_size_cstr); ++j) {
        char* encoded = EncodeHam74(name_size_cstr[j]);
        arc_of.write(encoded, 2);

        delete[] encoded;
      }
      for (int j = 0; j < file_name_to_add.length(); ++j) {
        char* encoded = EncodeHam74(file_name_to_add[j]);
        arc_of.write(encoded, 2);

        delete[] encoded;
      }

      int64_t file_size;
      file_to_add.seekg(0, file_to_add.end);
      file_size = file_to_add.tellg();
      file_size *= 2;
      file_to_add.seekg(0, file_to_add.beg);

      char* file_size_cstr = reinterpret_cast<char*>(&file_size);

      for (int j = 0; j < sizeof(file_size_cstr); ++j) {
        char* encoded = EncodeHam74(file_size_cstr[j]);
        arc_of.write(encoded, 2);

        delete[] encoded;
      }
      char ch;
      while (file_to_add.get(ch)) {
        char* encoded = EncodeHam74(ch);
        arc_of.write(encoded, 2);

        delete[] encoded;
      }
    } else {
      std::string temp_name = file_name_to_add;
      if (folder_name != "/") {
        file_name_to_add = GetPathFromFolder(file_name_to_add, folder_name);
      }

      std::ifstream file_to_add(temp_name, std::ios::binary);
      if (!file_to_add.is_open()) {
        std::cerr << "Unable to open file you want to add.";
        exit(1);
      }
      uint64_t name_length = file_name_to_add.length() * 2;

      char* name_size_cstr = reinterpret_cast<char*>(&name_length);
      for (int j = 0; j < sizeof(name_size_cstr); ++j) {
        char* encoded = EncodeHam74(name_size_cstr[j]);
        arc_of.write(encoded, 2);

        delete[] encoded;
      }

      for (int j = 0; j < file_name_to_add.size(); ++j) {
        char* encoded = EncodeHam74(file_name_to_add[j]);
        arc_of.write(encoded, 2);

        delete[] encoded;
      }

      int64_t file_size;
      file_to_add.seekg(0, file_to_add.end);
      file_size = file_to_add.tellg();
      file_size *= 2;
      file_to_add.seekg(0, file_to_add.beg);

      char* file_size_cstr = reinterpret_cast<char*>(&file_size);

      for (int j = 0; j < sizeof(file_size_cstr); ++j) {
        char* encoded = EncodeHam74(file_size_cstr[j]);
        arc_of.write(encoded, 2);

        delete[] encoded;
      }

      char ch;
      while (file_to_add.get(ch)) {
        char* encoded = EncodeHam74(ch);
        arc_of.write(encoded, 2);

        delete[] encoded;
      }
    }
  } else {
    std::cerr << "Unable to read archive. Archive is corrupted!";
    exit(EXIT_FAILURE);
  }
}

void ExtractArchive(std::string arc_name, std::string file_name_to_extract) {
  std::cout << "EXTRACT\n";
  std::ifstream arc(arc_name, std::ios::binary);
  if (CheckArcSignature(arc)) {
    char ch;
    while (arc.peek() != EOF) {
      char* name_size_cstr = new char[16];
      arc.read(name_size_cstr, 16);
      char* decoded_name_size = new char[8];
      int it = 0;

      for (int i = 0; i < 16; i += 2) {
        char decoded[2] = {name_size_cstr[i], name_size_cstr[i + 1]};
        decoded_name_size[it] = DecodeHam74(decoded);
        ++it;
      }

      int64_t name_length;
      std::memcpy(&name_length, decoded_name_size, sizeof(name_length));

      char* name = new char[name_length];
      arc.read(name, name_length);
      std::string name_str;
      for (int i = 0; i < name_length; i += 2) {
        char decoded[2] = {name[i], name[i + 1]};
        name_str += DecodeHam74(decoded);
      }

      char* file_size_cstr = new char[16];
      arc.read(file_size_cstr, 16);
      char* decoded_file_size = new char[8];
      it = 0;

      for (int i = 0; i < 16; i += 2) {
        char decoded[2] = {file_size_cstr[i], file_size_cstr[i + 1]};
        decoded_file_size[it] = DecodeHam74(decoded);
        ++it;
      }

      int64_t file_length;
      std::memcpy(&file_length, decoded_file_size, sizeof(file_length));

      if (file_name_to_extract == name_str || file_name_to_extract == "") {
        std::cout << name_str << '\n';
        if (name_str.find("/") == std::string::npos) {
          name_str = "./" + RemoveExtension(arc_name) + "/" + name_str;
        } else {
          name_str = "./" + RemoveExtension(arc_name) + name_str;
        }

        if (!fs::exists("./" + RemoveExtension(arc_name))) {
          fs::create_directory("./" + RemoveExtension(arc_name));
        }
        if (!fs::exists(GetPathWithoutFileName(name_str))) {
          fs::create_directories(GetPathWithoutFileName(name_str));
        }
        std::ofstream output(name_str, std::ios::binary);

        if (!output.is_open()) {
          std::cout << "Not Opened: " << name_str << '\n';
        }

        int byte = arc.tellg();
        int64_t num_chunks = file_length / 2;
        for (int64_t i = 0; i < num_chunks; ++i) {
          char* ch2 = new char[2];
          arc.read(ch2, 2);
          char decoded = DecodeHam74(ch2);
          output.put(decoded);

          delete[] ch2;
        }

      } else {
        arc.ignore(file_length);
      }

      delete[] name;
      delete[] file_size_cstr;
      delete[] decoded_file_size;
      delete[] name_size_cstr;
      delete[] decoded_name_size;
    }
  } else {
    std::cerr << "Unable to read archive. Archive is corrupted!";
    exit(EXIT_FAILURE);
  }
}

void ConcatenateArchives(std::string first_arc_name,
                         std::string second_arc_name,
                         std::string result_arc_name) {
  if (result_arc_name == "") {
    std::cerr << "No result archive name given. Stopping...";
    exit(EXIT_FAILURE);
  }

  std::ifstream first_arc(first_arc_name, std::ios::binary);
  std::ifstream second_arc(second_arc_name, std::ios::binary);
  std::ofstream result_arc(result_arc_name, std::ios::binary);

  if (CheckArcSignature(first_arc) && CheckArcSignature(second_arc)) {
    char sign[2] = {'H', 'A'};

    for (int i = 0; i < 2; ++i) {
      char* encoded = EncodeHam74(sign[i]);
      result_arc.write(encoded, 2);

      delete[] encoded;
    }
    char ch;
    while (first_arc.get(ch)) {
      result_arc.put(ch);
    }
    while (second_arc.get(ch)) {
      result_arc.put(ch);
    }
  }
}
