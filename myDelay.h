#ifndef _MY_DELAY_H_
#define _MY_DELAY_H_

#include <time.h>

namespace MyDelay
{
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
}

#endif // _MY_DELAY_H_