#include <MyLib/myLogerDef.h>

#include <MyLib/cryption.h>

using namespace MyLib::MyCryption;

int main(int argc, char const *argv[])
{
    using namespace MyLib::MyLoger;

    Cryption testCryption;
    //std::string plainText = "这是一个危险的世界，隐藏自己，做好清理！！";

    std::string cipherText = testCryption.encryption("这是一个危险的世界，隐藏自己，做好清理！！");

    ERROR_LOG(cipherText + '\n');

    std::string resultText = testCryption.decryption(cipherText);

    CORRECT_LOG(resultText + '\n');    

    return EXIT_SUCCESS;
}
