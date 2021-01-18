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

int main(int argc, char *argv[]) {
	system("title C++ Command Line");
	
	/* check if a file is passed to execute */
	if (argc == 2) {
		ifstream file(argv[1]);
		string line;
		
		if (!file) {
			cout << '\'' << argv[1] << '\'' << " is not a valid file path" << endl;
			system("pause");
			return 0;
		}
		
		while (getline(file, line)) {
			command_line::read_command(line);
		}
	} else {
		/* standard command line functionality */
		cout << "Command Line created by C++20: V" << cmdline_version << endl;
		
		while (true) command_line::read_command();
	}
	
	return 0;
}
