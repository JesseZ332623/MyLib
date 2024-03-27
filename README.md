# 自己编写的几个简易库

## `myLogDef` 一个简易的日志库

能完成基本的日志功能，原理就是将字符串拼凑成 `ANSI` 转义序列发送到标准输出流，
从而改变文本颜色，在 `Windwos` 和 `Linux` 操作系统的终端均可使用。

### 库核心函数如下所示

```C++
/**
 * @brief 用户可以通过该函数简便的使用该日志库
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
static inline void log(std::ostream &__os, const TerminalTextColor __logLevel, OutPutType... __out)
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
