#include "utils.hpp"
#include <iostream>
#include <windows.h> 
#include "shout.h"

bool isHiddenOrSystem(const fs::path& p)
{
    DWORD attrs = GetFileAttributesW(p.wstring().c_str());
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return true;
    }
    if ((attrs & FILE_ATTRIBUTE_HIDDEN) || (attrs & FILE_ATTRIBUTE_SYSTEM)) {
        return true;
    }
    return false;
}

std::vector<fs::path> findFiles(const fs::path& folder, const std::vector<std::string>& extensions)
{
    std::vector<fs::path> files;

    if (!fs::exists(folder) || !fs::is_directory(folder))
    {
        warn("Folder does not exist or is not a directory: ") << folder << std::endl;
        return files;
    }

    try
    {
        for (const auto& entry : fs::recursive_directory_iterator(
            folder, fs::directory_options::skip_permission_denied))
        {
            bool entryIsDir = false;
            try
            {
                entryIsDir = entry.is_directory();
            }
            catch (const fs::filesystem_error& e)
            {
                warn("Skipping (error in is_directory()): ") << e.what()
                    << " -> " << entry.path() << std::endl;
                continue;
            }
            info("Checking: ") << entry.path() << std::endl;
            if (entryIsDir)
            {
                DWORD attrs = GetFileAttributesW(entry.path().wstring().c_str());
                if (attrs != INVALID_FILE_ATTRIBUTES &&
                    (attrs & FILE_ATTRIBUTE_HIDDEN || attrs & FILE_ATTRIBUTE_SYSTEM))
                {
                    info("Skipping hidden/system folder: ")
                        << entry.path() << std::endl;
                    continue;
                }
            }
            bool entryIsRegFile = false;
            try
            {
                entryIsRegFile = entry.is_regular_file();
            }
            catch (const fs::filesystem_error& e)
            {
                warn("Skipping (error in is_regular_file()): ") << e.what()
                    << " -> " << entry.path() << std::endl;
                continue;
            }

            if (!entryIsRegFile)
                continue;
            fs::path filePath = entry.path();

            if (matchesAnyExtension(filePath.extension().string(), extensions))
            {
                files.push_back(filePath);
            }
        }
    }
    catch (const fs::filesystem_error& e)
    {
        warn("filesystem_error in loop: ") << e.what() << std::endl;
    }

    return files;
}
bool matchesAnyExtension(const std::string& extension, const std::vector<std::string>& supportedExtensions) {
    for (const auto& ext : supportedExtensions) {
        if (extension == ext) {
            return true;
        }
    }
    return false;
}
std::string getUserProfilePath() {
    char userProfile[MAX_PATH];
    if (GetEnvironmentVariable("USERPROFILE", userProfile, MAX_PATH)) {
        return std::string(userProfile) + "\\dir";
    }
    else {
        warn("Can't get the path");
        return "";
    }
}
