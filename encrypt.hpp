#ifndef ENCRYPT_HPP
#define ENCRYPT_HPP

#include <filesystem>
#include <vector>
#include <cryptopp/secblock.h>

//namespace fs = std::filesystem;

void encryptSingleFile(const std::filesystem::path& filePath, const CryptoPP::SecByteBlock& generatedKey, const CryptoPP::SecByteBlock& generatedIv);
CryptoPP::SecByteBlock generateIv();
CryptoPP::SecByteBlock generateKey();

#endif