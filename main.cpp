/***********************************************************
 * this project is developed by Mohammad Mostafa Dastjerdi *
 * this is an OPEN SOURCE project and can be modified by   *
 * others who want to extend it.                           *
 * ------------------------------------------------------- *
 * this project is an exercise to work with C++ language   *
 * and challenge your programming skills.                  *
 * ------------------------------------------------------- *
 * after modifying please change the 'cmdline_version' and *
 * change it to something like 'unofficial X.X.X'.         *
 * ------------------------------------------------------- *
 * the command line published at my own repository         *
 * https://github.com/MohammadMD1383/ConsoleAppCPP/        *
 * won't contain any harmful or dangerous commands.        *
 ***********************************************************/

#include "commands.h"

using namespace std;
using namespace cli;

int main() {
	system("title C++ Command Line");
	cout << "Command Line created by C++20: "<< cmdline_version << endl;
	
	while (true) command_line::read_command();
	
	return 0;
}