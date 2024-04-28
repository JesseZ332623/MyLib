
#include <cstdlib>
#include <ctime>
#include <csignal>
#include <climits>
#include <fstream>

#include "../../myLogerDef.h"
#include "../../myDelay.h"

using namespace MyLib;
static std::size_t TIMES = 0L;

const char FILE_PATH[] = {"./exitedTimes.dat"};

/**
 * @brief 一个信号函数，用于控制按下 CTRL + C 之后，程序的行为。
 * 
 * @param	__signalIndex 信号向量表索引
 * 
 * @return non-return
*/
void endSignalHandle(int __signalIndex);

void endSignalHandle(int __signalIndex)
{
	/* 使用读和二进制模式打开数据文件 */
	std::ofstream writeStream(FILE_PATH, std::ios_base::out | std::ios_base::binary);
	
	if (!writeStream.is_open()) 
	{ 
		MyLoger::loger(std::cerr, MyLoger::ERROR, "Can't Open File: ", FILE_PATH, '\n');  
		std::exit(EXIT_FAILURE);
	}

	/* 写入数据后关闭文件。*/
	writeStream.write(reinterpret_cast<char *>(&TIMES), sizeof(std::size_t));
	writeStream.close();
	
	/* 报告写入完毕的消息后正式退出程序 */
	MyLoger::printSplitLine(35, '-');
	MyLoger::loger(std::cout, MyLoger::NOTIFY, "The loger write to the file: [", FILE_PATH, "] Current times = ", TIMES, '\n');
	MyLoger::loger(std::cout, MyLoger::CORRECT, "Done.\n");
	std::exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[])
{
	system("cls");

	std::signal(SIGINT, endSignalHandle);
	std::srand(std::time(nullptr));
	using namespace MyLib::MyLoger;

	/*
		在程序目录下创建一个名为 exitedTimes.dat 的数据文件，
		然后用只读和二进制模式打开。
	*/
	std::fstream readStream(FILE_PATH, std::ios_base::in | std::ios_base::binary | std::ios_base::app);
	if (!readStream.is_open()) 
	{ 
		MyLoger::loger(std::cerr, MyLoger::ERROR, "Can't Open File: ", FILE_PATH, '\n'); 
		std::exit(EXIT_FAILURE); 
	}

	/*
		将文件内的数据读取后存入一个全局变量 TIMES 中，
		然后关闭文件。
	*/
	readStream.read(reinterpret_cast<char *>(&TIMES), sizeof(std::size_t));
	readStream.close();
	

	/*
		通过一个死循环，随机的从预设日志等级数组中挑选一个等级输出日志，
		每输出 200 个清空一次屏幕。
	*/
	while (true) 
	{ 
		MyLoger::loger(std::cout, defaultLogLevel[std::rand() % 5], "Show message ", ++TIMES, " Times!\n");  
		MyDelay::delay(45);

		if (TIMES % 200 == 0) { system("cls"); }

		/*
			当输出次数到达 std::size_t 类型的最大值时，跳出循环。
		*/
		else if (TIMES == ULLONG_MAX) { break; }
	}

	MyLoger::loger(std::clog, WARNING, "The times was fulled. Final Times = ", TIMES, '\n');
	MyLoger::loger(std::clog, NOTIFY, "PRESS ANY KEY TO QUIT: ");
	std::getchar();

	MyLoger::loger(std::clog, CORRECT, "Done.\n");

	return EXIT_SUCCESS;
}