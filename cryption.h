#ifndef _CRYPTION_H__
#define _CRYPTION_H__

/**
 * ʹ�� Crypto++ ���ܿ⣬
 * �ṩ�������ַ������� AES ���ܽ��ܲ�����
*/

#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/aes.h>

#include <MyLib/myLogerDef.h>
#include <fstream>
#include <cstring>

namespace MyLib
{
    namespace MyCryption
    {
        using namespace CryptoPP;

        struct keys
        {
            byte key[AES::DEFAULT_KEYLENGTH];
            byte randomVector[AES::BLOCKSIZE];

            /**
             * @brief ��ָ���ļ��ж�ȡ����䡣
             * 
             * @param __path    ָ�������ļ�
             */
            void getToFile(const std::string & __path)
            {
                using std::ios_base;

                std::ifstream ifs(__path, ios_base::binary);

                if (!ifs.is_open() || !ifs)
                {
                    throw std::runtime_error("Can't open file " + __path + '\n');
                }

                ifs.read(reinterpret_cast<char *>(this->key), sizeof(this->key));
                ifs.read(reinterpret_cast<char *>(this->randomVector), sizeof(this->randomVector));

                ifs.close();
            }

            void clear(void) { std::memset(this, 0, sizeof(this->key) + sizeof(this->randomVector)); }
        };
        class Cryption
        {
            private:
                byte key[AES::DEFAULT_KEYLENGTH];
                byte randomVector[AES::BLOCKSIZE];
                AutoSeededRandomPool randSeedPool;

                /**
                 * @brief ����Կ��ʸ��д��һ�������ļ���
                 */
                void writeDataToFile(const std::string & __path);
            
            public:
                /**
                 * @brief Ĭ�Ϲ��캯����
                 *        �����������Կ��ʸ����д��ָ�����ļ���ȥ��
                 * 
                 * @param ʸ������Կ�ļ�·��
                */
                Cryption(const std::string & __dataPath);

                /**
                 * @brief ���캯���������ⲿ���ݵ���Կ��ʸ����
                 *        ����ͳһ�ļӽ��ܹ������д��á�
                 * 
                 * @param __key         ��Կ
                 * @param __randVec     ʸ��
                */
                Cryption(const byte * __key, const byte * __randVec);
                
                /**
                 * @brief ʹ�� AES �㷨�����ַ���
                 * 
                 * @param __plainText   ����
                 * 
                 * @return std::string  ���ܺ������
                */
                std::string encryption(std::string & __plainText);

                /**
                 * @brief ʹ�� AES �㷨�����ַ���
                 * 
                 * @param __plainText   ���ģ����������ַ���������
                 * 
                 * @return std::string  ���ܺ������
                */
                std::string encryption(std::string && __plainText);

                /**
                 * @brief ʹ�� AES �㷨�����ַ�������
                 * 
                 * @param __cipherText ����
                 * 
                 * @return std::string ���ܺ������ 
                */
                std::string decryption(std::string & __cipherText);

                /**
                 * @brief ʹ�� AES �㷨�����ַ�������
                 * 
                 * @param __cipherText ���ģ����������ַ���������
                 * 
                 * @return std::string ���ܺ������ 
                */
                std::string decryption(std::string && __cipherText);
        };

        /**
         * @brief ������д���ļ�
         * 
         * @param cipherText        ���ܺ������
         * @param fileName          Ҫд����ļ�
         */
        void writeEncryptedDataToFile(const std::string &cipherText, const std::string &fileName);

        /**
         * @brief ��ָ�������ļ��ж�ȡ���ģ����������ַ���
         */
        std::string readEncryptedDataFromFile(const std::string &fileName);

    }; // END namespace MyCryption

};  // END namespace MyLib

void MyLib::MyCryption::Cryption::writeDataToFile(const std::string & __path)
{
    using std::ios_base;

    std::ofstream ofs(__path, ios_base::out | ios_base::binary);

    if (!ofs.is_open() || !ofs)
    {
        throw std::runtime_error("Can't open file " + __path + '\n');
    }

    ofs.write(reinterpret_cast<char *>(this->key), sizeof(this->key));
    ofs.write(reinterpret_cast<char *>(this->randomVector), sizeof(this->randomVector));

    ofs.close();
}

MyLib::MyCryption::Cryption::Cryption(const std::string & __dataPath)
{
    this->randSeedPool.GenerateBlock(this->key, sizeof(this->key));
    this->randSeedPool.GenerateBlock(this->randomVector, sizeof(randomVector));

    this->writeDataToFile(__dataPath);
}

MyLib::MyCryption::Cryption::Cryption(const byte * __key, const byte * __randVec)
{
    std::memcpy(this->key, __key, sizeof(this->key));
    std::memcpy(this->randomVector, __randVec, sizeof(this->randomVector));
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
    std::string plainText = std::move(__plainText);

    return this->encryption(plainText);
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
    std::string cipherText = std::move(__cipherText);

    return this->decryption(cipherText);
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

    // ����һ����ʱ�ַ�������ʱ�ÿ��ַ����
    std::string cipherText(length, '\0');

    file.read(&cipherText[0], length);
    file.close();

    return cipherText;
}

#endif // _ENCRYPTION_H__