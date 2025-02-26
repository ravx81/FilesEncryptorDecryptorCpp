#pragma once
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

std::vector<fs::path> findFiles(const fs::path& folder, const std::vector<std::string>& extensions);

bool matchesAnyExtension(const std::string& extension, const std::vector<std::string>& supportedExtensions);

bool isHiddenOrSystem(const fs::path& p);

std::string getUserProfilePath();


