# 自己编写的几个简易库

|日期|新增或改进|
|---|---|
|2024.3.30|1. 增加了一个对元素为数字类型的 STL 容器进行测试的新库 `simpleContainerOperator`。<br> 2. 将所有的库都放入 `MyLib` 命名空间之中|
|2024.4.18|1. 修改函数名为 loger 避免和 std::log 混淆|

## `myLogDef` 一个简易的日志库

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

## `simpleContainerOperator` 一个对元素为数字类型的 STL 容器进行测试的库

主要是在一些针对 STL 容器或仿 STL 容器的编写、学习和测试中，不用重复的编写遍历，插入容器数据等操作，
目前就只有两个函数（配合前两个库使用）：

```C++
/**
 * @brief 往指定标准输出流发送 Container 容器的相关信息和数据（只适用于元素为数字类型的 STL 容器）。
 *
 * @tparam Container STL 容器类型
 *
 * @param __os          标准输出流的引用
 * @param __container   要发送给输出流的容器的引用
 *
 * @return non-return
 */
template <typename Container>
void showContainerToStream(std::ostream &__os, const Container &__container)
{

    log(__os, NOTIFY, "This Container size = ", __container.size(), '\n');

    std::size_t containerIndex = 0L;

    std::for_each(__container.cbegin(), __container.cend(),
                  [&__os, &containerIndex](const decltype((*__container.cbegin())) &n)
                  {
                      __os << n << ' ';
                      delay(45);
                      if (containerIndex % 5 == 4)
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
