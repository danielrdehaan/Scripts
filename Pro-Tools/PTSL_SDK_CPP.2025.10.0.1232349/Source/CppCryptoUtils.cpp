// Copyright 2021, 2023-2025 by Avid Technology, Inc.
// CONFIDENTIAL: this document contains confidential information of Avid. Do not disclose to any third party. Use of the information contained in this document is subject to an Avid SDK license.

/**
 * @file
 * @brief Implementation file for the CppCryptoUtils.h
 */

#include "CppCryptoUtils.h"

#include <memory>
#include <stdexcept>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

namespace PTSLC_CPP
{
    std::vector<unsigned char> encrypt(const std::string& text, const std::string& publicEncryptionKey)
    {
        std::unique_ptr<BIO, int(*)(BIO*)> bio(BIO_new_mem_buf(publicEncryptionKey.data(), static_cast<int>(publicEncryptionKey.size())), BIO_free);
        if (!bio)
        {
            throw std::runtime_error("Encryption failed: could not create bio.");
        }

        std::unique_ptr<RSA, void(*)(RSA*)> rsa(PEM_read_bio_RSAPublicKey(bio.get(), nullptr, nullptr, nullptr), RSA_free);
        if (!rsa)
        {
            throw std::runtime_error("Encryption failed: could not read RSA.");
        }

        std::vector<unsigned char> encryptedData(RSA_size(rsa.get()));
        int outlen = RSA_public_encrypt(text.size(), reinterpret_cast<const unsigned char*>(text.data()), encryptedData.data(), rsa.get(), RSA_PKCS1_OAEP_PADDING);
        if (outlen <= 0)
        {
            throw std::runtime_error("Encryption failed: could not encrypt.");
        }
        encryptedData.resize(outlen);
        
        return encryptedData;
    }

    std::string encodeBase64(const std::vector<unsigned char>& data)
    {
        BIO* bio = BIO_new(BIO_s_mem());
        BIO* b64 = BIO_new(BIO_f_base64());

        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

        std::unique_ptr<BIO, void(*)(BIO*)> bioChain(BIO_push(b64, bio), BIO_free_all);

        BIO_write(bioChain.get(), data.data(), static_cast<int>(data.size()));
        BIO_flush(bioChain.get());

        BUF_MEM* bufferPtr {};
        BIO_get_mem_ptr(bioChain.get(), &bufferPtr);

        return std::string { bufferPtr->data, bufferPtr->length };
    }
} // namespace PTSLC_CPP
