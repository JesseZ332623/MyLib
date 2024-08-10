#ifndef _SIMPLE_CONTAINER_OPERATOR_H_
#define _SIMPLE_CONTAINER_OPERATOR_H_

#include "./myLogerDef.h"

#include <algorithm>
#include <cstdlib>
#include <random>

namespace MyLib
{
    namespace SimpleContainerOperator
    {
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
        void showContainerToStream(std::ostream & __os, const Container & __container, std::size_t __eachLineCount = 5)
        {
            using namespace MyLoger;

            //loger(__os, NOTIFY, "This Container size = ", __container.size(), '\n');

            std::size_t containerIndex = 0ULL;

            std::for_each(__container.cbegin(), __container.cend(), 
                        [&](const decltype((*__container.begin())) & n) 
                        { 
                            __os << n << ' '; 
                            if (containerIndex % __eachLineCount == __eachLineCount - 1) { __os << std::endl; }
                            ++containerIndex;
                        }
                    );
            //__os << std::endl;
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
        void pushRandomValue(__Container & __container, int __limit, std::size_t __randomCount = 10, double __deviationVal = 0.0)
        {
            typedef typename __Container::value_type generate_type;

            std::random_device randDevice;
            std::mt19937_64    randEngine(randDevice());

            /**
             * 判断 generate_type 是整数类型还是小数类型（编译期）
             */
            if constexpr (std::is_integral_v<generate_type>)
            { 
                std::uniform_int_distribution<generate_type> randDistribute(0, __limit);

                for (std::size_t randomIndex = 0; randomIndex < __randomCount; ++randomIndex)
                {
                    __container.push_back(randDistribute(randEngine) + __deviationVal);
                } 
            }
            else 
            {
                std::uniform_real_distribution<generate_type> randDistribute(0, __limit);

                for (std::size_t randomIndex = 0; randomIndex < __randomCount; ++randomIndex)
                {
                    __container.push_back(randDistribute(randEngine) + __deviationVal);
                } 
            }
        }
    } // END namespace SimpleContainerOperator
} // END namespace MyLib

#endif // _SIMPLE_CONTAINER_OPERATOR_H_