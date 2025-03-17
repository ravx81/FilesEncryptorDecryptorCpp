#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <string>
#include "utils.hpp"
#include "shout.h"

#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>
#include <cryptopp/secblock.h>

namespace fs = std::filesystem;

void decryptSingleFile(const fs::path& filePath) {
    CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decrypter;

    std::string userProfilePath = getUserProfilePath();
    if (userProfilePath.empty()) {
        warn("Failed to get user profile path!") << '\n';
        return;
    }

    std::string userProfile = userProfilePath + "\\keys.txt";
    std::string keyBase64, ivBase64;

    std::ifstream keys(userProfile);

    std::getline(keys, keyBase64);
    std::getline(keys, ivBase64);

    if (keyBase64.empty() || ivBase64.empty()) {
        warn("File keys.txt does not contain key or IV") << '\n';
        return;
    }
    info("keyBase64: ") << keyBase64 << '\n';
    info("ivBase64: ") << ivBase64 << '\n';

    CryptoPP::SecByteBlock key_decrypted(CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::SecByteBlock iv_decrypted(CryptoPP::AES::BLOCKSIZE);

    CryptoPP::StringSource ssKeyDecrypt(keyBase64, true, new CryptoPP::Base64Decoder(
        new CryptoPP::ArraySink(key_decrypted, sizeof(key_decrypted))
    ));
    info("Decrypted first key \n");

    CryptoPP::StringSource ssIVDecrypt(ivBase64, true, new CryptoPP::Base64Decoder(
        new CryptoPP::ArraySink(iv_decrypted, sizeof(iv_decrypted))
    ));
    info("Decrypted second key \n");

    std::ifstream inputFile(filePath, std::ios::binary);
    if (!inputFile) {
        warn("Can't open file ;c \n");
        return;
    }
    std::vector<unsigned char> fileData{ std::istreambuf_iterator<char>(inputFile), std::istreambuf_iterator<char>() };
    inputFile.close();
    std::string decrypted;

    info("IV size: ") << iv_decrypted.size() << '\n';
    info("Key size: ") << key_decrypted.size() << '\n';

    decrypter.SetKeyWithIV(key_decrypted, key_decrypted.size(), iv_decrypted);
    okay("Keys are set well \n");

    try {
        CryptoPP::StringSource(
            reinterpret_cast<const CryptoPP::byte*>(fileData.data()),
            fileData.size(),
            true,
            new CryptoPP::StreamTransformationFilter(
                decrypter,
                new CryptoPP::StringSink(decrypted),
                CryptoPP::StreamTransformationFilter::PKCS_PADDING
            )
        );
    }
    catch (const CryptoPP::Exception& e) {
        warn("Can't decrypt file: ") << filePath << ": " << e.what() << '\n';
        return;
    }
    info("After decryption data \n");

    std::ofstream outFile(filePath, std::ios::binary | std::ios::trunc);
    if (!outFile) {
        warn("Can't open file ;c \n");
    }
    outFile.write(decrypted.data(), decrypted.size());
    outFile.close();

    std::string str = filePath.string();

    fs::path newFilePath = str.substr(0, str.find("."));
    newFilePath += ".txt";
    try
    {
        fs::rename(filePath, newFilePath);
        okay("Decrypted and renamed file: ") << newFilePath << '\n';
    }
    catch (const fs::filesystem_error& e)
    {
        warn("Error while renaming file: ") << e.what() << '\n';
    }

    okay("File has been successfully decrypted! \n");
}
