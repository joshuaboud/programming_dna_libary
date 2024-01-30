#pragma once

#include <iomanip>
#include <openssl/evp.h>
#include <sstream>
#include <string>

class Password {
public:
  static Password fromHash(const std::string &hashed) {
    return Password(hashed);
  }

  static Password fromPlainText(const std::string &plainText) {
    std::string hashed = hashPassword(plainText);
    return Password(hashed);
  }

  const std::string &getHash() const {
    return mHashed;
  }

  void setHash(const std::string &hashed) {
    mHashed = hashed;
  }

  bool operator==(const Password &other) const {
    return getHash() == other.getHash();
  }

private:
  std::string mHashed;

  Password() = default;
  Password(const std::string &hashed) : mHashed(hashed) {}

  static std::string hashPassword(const std::string &plainText) {
    const EVP_MD *md = EVP_sha256(); // SHA-256 hash function

    EVP_MD_CTX *mdCtx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdCtx, md, nullptr);

    // Update hash with the password bytes
    EVP_DigestUpdate(mdCtx, plainText.c_str(), plainText.length());

    // Finalize the hash
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;
    EVP_DigestFinal_ex(mdCtx, hash, &hashLen);

    EVP_MD_CTX_free(mdCtx);

    // Convert the hash to a hexadecimal string
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (unsigned int i = 0; i < hashLen; ++i) {
      ss << std::setw(2) << static_cast<unsigned int>(hash[i]);
    }

    return ss.str();
  }
};
