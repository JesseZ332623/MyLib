#include <MyLib/myLogerDef.h>

#include <MyLib/cryption.h>

using namespace MyLib::MyCryption;

int main(int argc, char const *argv[])
{
    using namespace MyLib::MyLoger;

    Cryption testCryption;
    //std::string plainText = "����һ��Σ�յ����磬�����Լ�������������";

    std::string cipherText = testCryption.encryption("����һ��Σ�յ����磬�����Լ�������������");

    ERROR_LOG(cipherText + '\n');

    std::string resultText = testCryption.decryption(cipherText);

    CORRECT_LOG(resultText + '\n');    

    return EXIT_SUCCESS;
}
