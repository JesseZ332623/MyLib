# 自己编写的几个简易库

|日期|新增或改进|
|---|---|
|2024.3.30|1. 增加了一个对元素为数字类型的 STL 容器进行测试的新库 `simpleContainerOperator`。<br> 2. 将所有的库都放入 `MyLib` 命名空间之中|
|2024.4.18|1. 修改函数名为 `loger` 避免和 `std::log` 混淆|
|2024.4.28|1. 在 `myLogerDef.h` 上增加了几个预设宏函数，可以完成简单的日志操作，避免频繁使用 `loger` 函数的繁琐 <br> 2. 增加一个新库：`cinCheck.h` 用于对 C++ 标准输入流 `std::cin` 进行检查|
|2024.08.06|新增 `cryption` 库，能对字符串进行 AES 加密解密|

## `myLogDef.h` 一个简易的日志库

能完成基本的日志功能，原理就是将字符串拼凑成 `ANSI` 转义序列发送到标准输出流，
从而改变文本颜色，在 `Windwos` 和 `Linux` 操作系统的终端均可使用。

### 库核心函数如下所示

```C++
/**
 * @brief 用户可以通过该函数简便的使用该日志库（函数名使用 loger 避免和 std::log 混淆）
 *
 * @tparam OutPutType 需要输出的类型
 *
 * @param __os          标准输出流的引用
 * @param __logLevel    日志等级
 * @param __out         要输出的数据，会被打包成形参包
 *
 * @return              non-return
 */
template <typename... OutPutType>
static inline void loger(std::ostream &__os, const TerminalTextColor __logLevel, OutPutType... __out)
{
    __os << __logLevel;

    /*
        在 WARNING 或 ERROR 类别的日志下需要带上时间信息
    */
    if (__logLevel == WARNING || __logLevel == ERROR)
    {
        __os << '[' << getCurrentTime() << ']';
    }

    /**
     * 通过 C++17 标准推出的折叠表达式功能，可以将形参包解包然后依次完美转发，
     * 逐个向标准输出流发送数据。
     */
    (void)(__os << ... << std::forward<OutPutType>(__out));

    __os.flush();
    __os << ORIGINAL;
}
```

### 几个预设的宏函数

它们可以往标准输出发送不同日志等级的字符流，避免频繁调用 `loger` 函数。

```C++
#define ORIGINAL_LOG(__message)     loger(std::cout, ORIGINAL, __message);
#define CORRECT_LOG(__message)      loger(std::cout, CORRECT, __message);
#define NOTIFY_LOG(__message)       loger(std::cout, NOTIFY, __message);
#define WARNING_LOG(__message)      loger(std::clog, WARNING, __message);
#define ERROR_LOG(__message)        loger(std::cerr, ERROR, __message);
#define DONE                        loger(std::cerr, CORRECT, "Done.\n");
```

## `myDelay` 简易的延时函数

通过 `clock()` 以及 `while` 循环消耗掉用户指定的时间以达到延时的效果，
在 `Windwos` 和 `Linux` 操作系统的终端均可使用。

```C++

/**
  * @brief 自制延时函数，用于代替 windows 库中的 Sleep()
  * 
  * @param __millisSeconds 延时时间（毫秒）
  * 
  * @return non-return
*/
static void delay(long int __millisSeconds)
{
    clock_t startTime = clock();
    
    while (clock() < (startTime + __millisSeconds)) {}
}

#endif // _MY_DELAY_H_
```

## `simpleContainerOperator.h` 一个对元素为数字类型的 STL 容器进行测试的库

主要是在一些针对 STL 容器或仿 STL 容器的编写、学习和测试中，不用重复的编写遍历，插入容器数据等操作，
目前就只有两个函数（配合前两个库使用）：

```C++
/**
 * @brief 往指定标准输出流发送 Container 容器的相关信息和数据（只适用于元素为数字类型的 STL 容器）。
 *
 * @tparam Container STL 容器类型
 *
 * @param __os              标准输出流的引用
 * @param __container       要发送给输出流的容器的引用
 * @param __eachLineCount   每一行输出 `__eachLineCount` 个元素后换行，默认为 5
 *
 * @return non-return
 */
template <typename Container>
void showContainerToStream(std::ostream &__os, const Container &__container, std::size_t __eachLineCount = 5)
{
    using namespace MyLoger;
    using MyDelay::delay;

    loger(__os, NOTIFY, "This Container size = ", __container.size(), '\n');

    std::size_t containerIndex = 0L;

    std::for_each(__container.cbegin(), __container.cend(),
                  [&__os, &containerIndex, &__eachLineCount](const decltype((*__container.cbegin())) &n)
                  {
                      __os << n << ' ';
                      delay(45);
                      if (containerIndex % __eachLineCount == __eachLineCount - 1)
                      {
                          __os << std::endl;
                      }
                      ++containerIndex;
                  });
    __os << std::endl;
}

/**
 * @brief 往一个容器里面插入随机的数据，只适用于元素为数字类型的 STL 容器。
 *
 * @tparam __Container      STL 容器类型
 *
 * @param __container       一个 STL 容器的引用，但这个容器的元素最好是数字类型
 * @param __limit           随机数生成的返回不超过这个值
 * @param __randomCount     生成的次数（默认为 10 次）
 * @param __deviationVal    误差值，用于随机生成小数，不填就为 0
 *
 * @return non-return
 */
template <typename __Container>
void pushRandomValue(__Container &__container, int __limit, std::size_t __randomCount = 10, double __deviationVal = 0.0)
{
    for (std::size_t randomIndex = 0; randomIndex < __randomCount; ++randomIndex)
    {
        __container.push_back(std::rand() % __limit + __deviationVal);
    }
}
```

## `cinCheck.h` 一个对 `std::cin` 进行输入格式检查的库

主要是为了防止使用 `>>` 运算符往 `std::cin` 发送数据时，因输入不符合数据类型导致的程序错误（比如意外终止或进入不一致状态），核心函数如下所示：

```C++
/**
 * @brief 在对数字类型的数据使用 >> 运算符进行输入时，对标准输入流进行检查，
 *        若不符合数据格式的输入（比如用户输入非数字字符）就清理标志位并忽略此次输入。
 *
 * @param __is              标准输入流的引用
 * @param __errMessage      当出现意外输入时所报的错误消息
 *
 * @return 输入数据是否合规
 */
static inline bool istreamStateCheck(std::istream &__is, const std::string &__errMessage)
{
    using namespace MyLib::MyLoger;

    if (__is.fail())
    {
        ERROR_LOG(__errMessage);

        __is.clear();
        __is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        return false;
    }

    return true;
}

/**
 * @brief 封装对数字类型输入格式和范围的检查，
 *        用来过滤输入垃圾数据避免程序意外终止或进入不一致状态。
 *
 * @tparam DataType     输入数据的类型
 * @tparam InputRule    用户为输入的数字范围所指定的规则，通常是 `Lamba` 表达式
 *
 * @param __is          标准输入流的引用
 * @param __member      要输入的数据类型
 * @param __messages    针对不同的情况输出的消息结构体，需要在外部实例化
 * @param __rangeRule   为输入的数字范围所指定的规则，默认情况下永远返回 true
 *
 * @return non-return
 */
template <typename DataType, typename InputRule = std::function<bool(DataType)>>
static inline void istreamInputAndCheck(
                    std::istream &__is, DataType &__member,
                    const MessageStrings &__messages, InputRule __rangeRule = [](DataType) { return true; }
                    )
{
    using namespace MyLib::MyLoger;
    using MyDelay::delay;

    while (true)
    {
        NOTIFY_LOG(__messages.promptMessage);
        __is >> __member;

        if (!istreamStateCheck(__is, __messages.errorMessage))
        {
            delay(800); continue;
        }
        if (!__rangeRule(__member))
        {
            ERROR_LOG(__messages.outRangeMessage);
            eatLine();
            continue;
        }

        break;
    }
}
```

## `cryption.h`: 封装了 `Crypto++` 库，对字符串进行 `AES` 加密解密

库 API 如下：

```C++
class Cryption
{
private:
    byte key[AES::DEFAULT_KEYLENGTH];
    byte randomVector[AES::BLOCKSIZE];
    AutoSeededRandomPool randSeedPool;

    /**
     * @brief 将密钥和矢量写入一个数据文件。
     */
    void writeDataToFile(const std::string &__path);

public:
    /**
     * @brief 默认构造函数，
     *        生成随机的密钥和矢量，写入指定的文件中去。
     *
     * @param 矢量和密钥文件路径
     */
    Cryption(const std::string &__dataPath);

    /**
     * @brief 构造函数，接受外部传递的密钥和矢量，
     *        这在统一的加解密过程中有大用。
     *
     * @param __key         密钥
     * @param __randVec     矢量
     */
    Cryption(const byte *__key, const byte *__randVec);

    /**
     * @brief 使用 AES 算法加密字符串
     *
     * @param __plainText   明文
     *
     * @return std::string  加密后的密文
     */
    std::string encryption(std::string &__plainText);

    /**
     * @brief 使用 AES 算法加密字符串
     *
     * @param __plainText   明文（可以填入字符串常量）
     *
     * @return std::string  加密后的密文
     */
    std::string encryption(std::string &&__plainText);

    /**
     * @brief 使用 AES 算法解密字符串密文
     *
     * @param __cipherText 密文
     *
     * @return std::string 解密后的明文
     */
    std::string decryption(std::string &__cipherText);

    /**
     * @brief 使用 AES 算法解密字符串密文
     *
     * @param __cipherText 密文（可以填入字符串常量）
     *
     * @return std::string 解密后的明文
     */
    std::string decryption(std::string &&__cipherText);
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
```

详见：[cryption.h](https://github.com/JesseZ332623/MyLib/blob/master/cryption.h)
