#ifndef DECRYPT_HPP
#define DECRYPT_HPP

#include <filesystem>

namespace fs = std::filesystem;

void decryptSingleFile(const std::filesystem::path& filePath);

#endif
