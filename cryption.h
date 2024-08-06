#ifndef _CRYPTION_H__
#define _CRYPTION_H__

/**
 * 使用 Crypto++ 加密库，
 * 提供对密码字符串进行 AES 加密解密操作。
*/

#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/aes.h>

#include <MyLib/myLogerDef.h>
#include <fstream>

namespace MyLib
{
    namespace MyCryption
    {
        using namespace CryptoPP;
        class Cryption
        {
            private:
                byte key[AES::DEFAULT_KEYLENGTH];
                byte randomVector[AES::BLOCKSIZE];
                AutoSeededRandomPool randSeedPool;
            
            public:
                Cryption();
                
                /**
                 * @brief 使用 AES 算法加密字符串
                 * 
                 * @param __plainText   明文
                 * 
                 * @return std::string  加密后的密文
                */
                std::string encryption(std::string & __plainText);

                /**
                 * @brief 使用 AES 算法加密字符串
                 * 
                 * @param __plainText   明文（可以填入字符串常量）
                 * 
                 * @return std::string  加密后的密文
                */
                std::string encryption(std::string && __plainText);

                /**
                 * @brief 使用 AES 算法解密字符串密文
                 * 
                 * @param __cipherText 密文
                 * 
                 * @return std::string 解密后的明文 
                */
                std::string decryption(std::string & __cipherText);

                /**
                 * @brief 使用 AES 算法解密字符串密文
                 * 
                 * @param __cipherText 密文（可以填入字符串常量）
                 * 
                 * @return std::string 解密后的明文 
                */
                std::string decryption(std::string && __cipherText);
        };

        /**
         * @brief 将密文写入文件
         * 
         * @param cipherText        加密后的密文
         * @param fileName          要写入的文件
         */
        void writeEncryptedDataToFile(const std::string &cipherText, const std::string &fileName);

        /**
         * @brief 从指定数据文件中读取密文，返回密文字符串
         */
        std::string readEncryptedDataFromFile(const std::string &fileName);
    }; // END namespace MyCryption

};  // END namespace MyLib



MyLib::MyCryption::Cryption::Cryption() : key(), randomVector()
{
    using namespace MyLib::MyLoger;

    this->randSeedPool.GenerateBlock(this->key, sizeof(this->key));
    this->randSeedPool.GenerateBlock(this->randomVector, sizeof(randomVector));
}

std::string MyLib::MyCryption::Cryption::encryption(std::string & __plainText)
{
    using namespace MyLib::MyLoger;

    std::string tempCipherText;

    try
    {
        CBC_Mode<AES>::Encryption encryptor(
            this->key, sizeof(this->key), this->randomVector
        );

        StringSource encryptionSource(
            __plainText, true,
            new StreamTransformationFilter(
                encryptor, 
                new HexEncoder(new StringSink(tempCipherText))
            )
        );
    }
    catch (const CryptoPP::Exception & __except)
    {
        ERROR_LOG(__except.what() + '\n');
        tempCipherText.clear();
    }

    return tempCipherText;
}

std::string MyLib::MyCryption::Cryption::encryption(std::string && __plainText)
{
    using namespace MyLib::MyLoger;

    std::string tempCipherText;

    try
    {
        CBC_Mode<AES>::Encryption encryptor(
            this->key, sizeof(this->key), this->randomVector
        );

        StringSource encryptionSource(
            std::move(__plainText), true,
            new StreamTransformationFilter(
                encryptor, 
                new HexEncoder(new StringSink(tempCipherText))
            )
        );
    }
    catch (const CryptoPP::Exception & __except)
    {
        ERROR_LOG(__except.what() + '\n');
        tempCipherText.clear();
    }

    return tempCipherText;
}

std::string MyLib::MyCryption::Cryption::decryption(std::string & __cipherText)
{
    using namespace MyLib::MyLoger;

    std::string tempPlainText;

    try
    {
        CBC_Mode<AES>::Decryption decryptor(
            this->key, sizeof(this->key), this->randomVector
        );
        
        StringSource decryptionSource(
            __cipherText, true,
            new HexDecoder(
                new StreamTransformationFilter(decryptor,
                    new StringSink(tempPlainText)
                )
            )
        );
    }
    catch (const CryptoPP::Exception & __except)
    {
        ERROR_LOG(__except.what() + '\n');
        tempPlainText.clear();
    }

    return tempPlainText;
}

std::string MyLib::MyCryption::Cryption::decryption(std::string && __cipherText)
{
    using namespace MyLib::MyLoger;

    std::string tempPlainText;

    try
    {
        CBC_Mode<AES>::Decryption decryptor(
            this->key, sizeof(this->key), this->randomVector
        );
        
        StringSource decryptionSource(
            std::move(__cipherText), true,
            new HexDecoder(
                new StreamTransformationFilter(decryptor,
                    new StringSink(tempPlainText)
                )
            )
        );
    }
    catch (const CryptoPP::Exception & __except)
    {
        ERROR_LOG(__except.what() + '\n');
        tempPlainText.clear();
    }

    return tempPlainText;
}

void MyLib::MyCryption::writeEncryptedDataToFile(const std::string &cipherText, const std::string &fileName)
{
    std::ofstream file(fileName, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }
    file.write(cipherText.c_str(), cipherText.size());
    file.close();
}

std::string MyLib::MyCryption::readEncryptedDataFromFile(const std::string &fileName)
{
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file for reading." << std::endl;
        return "";
    }
    
    file.seekg(0, std::ios::end);
    size_t length = file.tellg();
    file.seekg(0, std::ios::beg);

    // 创建一个临时字符串，暂时用空字符填充
    std::string cipherText(length, '\0');

    file.read(&cipherText[0], length);
    file.close();

    return cipherText;
}

#endif // _ENCRYPTION_H__