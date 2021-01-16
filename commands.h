#include <iostream>
#include <string>
#include <map>
#include <list>
#include <chrono>
#include <iomanip>
#include "util.h"

using namespace std;
using namespace chrono;

namespace cli {
	/* commands interface */
	/**
	 * shows the commands and definitions of the available commands
	 */
	void command_commands(const string &);
	
	/**
	 * exits the program
	 */
	void command_exit(const string &);
	
	/**
	 * pauses the program until key press
	 */
	void command_pause(const string &);
	
	/**
	 * clears the console window
	 */
	void command_clear(const string &);
	
	/**
	 * print a text with or without variables
	 */
	void command_print(const string &);
	
	/**
	 * defines or changes a variable
	 */
	void command_var(const string &);
	
	/**
	 * unsets a variable
	 */
	void command_var_unset(const string &);
	
	/**
	 * prints some predefined variables with 0 or more args
	 */
	void command_show(const string &);
	
	/* commands map */
	map<string, void (*)(const string &)> commands = {
		{"commands",  command_commands},
		{"exit",      command_exit},
		{"pause",     command_pause},
		{"clear",     command_clear},
		{"print",     command_print},
		{"var",       command_var},
		{"var_unset", command_var_unset},
		{"show",      command_show}
	};
	
	/* the description of commands */
	map<string, string> definitions = {
		{"exit",      "closes the console"},
		{"pause",     "pauses the console"},
		{"clear",     "clears the console"},
		{"print",     "prints a text"},
		{"var",       "defines or changes a variable"},
		{"var_unset", "destroys a variable"},
		{"show",      "shows the specified system variable"}
	};
	
	/* command line variables */
	map<string, string> variables;
	
	/* command line properties */
	static string cmdline_version = "1.2.0";
	static string cmdline_start = "CLI>";
	static bool cmdline_start_showing = true;
	
	/* command line class */
	class command_line {
	public:
		/**
		 * reads input and interpret it
		 */
		static void read_command() {
			if (cmdline_start_showing) cout << cmdline_start << ' ';
			string _command;
			getline(cin, _command);
			util::trim(&_command);
			
			string cmd = _command.substr(0, _command.find(' '));
			string args = _command.substr(_command.find_first_of(" \t") + 1);
			util::trim(&args);
			args = args == cmd ? "" : args;
			
			if (commands.find(cmd) != commands.end()) {
				commands[cmd](args);
			} else if (!cmd.empty()) {
				cout << "unknown command" << endl;
			}
		}
	};
	
	/* implementing commands */
	void command_commands(const string &) {
		for (auto &definition : definitions) {
			cout << definition.first << "\t" << definition.second << endl;
		}
	}
	
	void command_exit(const string &args) {
		exit(0);
	}
	
	void command_pause(const string &) {
		system("pause");
	}
	
	void command_clear(const string &) {
		system("cls");
	}
	
	void command_print(const string &args) {
		string temp = args;
		util::replace_with_var(temp, variables);
		cout << temp << endl;
	}
	
	void command_var(const string &args) {
		string key = args.substr(0, args.find(' '));
		if (key.empty()) {
			cout << "variable name and value missing" << endl;
			return;
		}
		
		string value = args.substr(args.find_first_of(" \t") + 1);
		util::trim(&value);
		if (value == key) {
			cout << "variable value missing" << endl;
			return;
		}
		
		variables[key] = value;
	}
	
	void command_var_unset(const string &args) {
		if (args.empty()) {
			cout << "no variable mentioned" << endl;
		}
		
		list arg_list = util::split_string_space(args);
		for (const auto &arg : arg_list) variables.erase(arg);
	}
	
	void command_show(const string &args) {
		/* the map of show's subcommands */
		static map<string, void (*)(const string &)> subcommands = {
			/**
			 * prints the date in 4 different formats. args: s/m/l/f
			 */
			{"date",     [](const string &param) {
				auto now = system_clock::now();
				time_t time = system_clock::to_time_t(now);
				
				struct tm t_struct;
				localtime_s(&t_struct, &time);
				
				char time_format[50];
				char time_buffer[100];
				
				if (param == "s" || param.empty()) strcpy_s(time_format, sizeof(time_format), "%F");
				else if (param == "m") strcpy_s(time_format, sizeof(time_format), "%a - %F");
				else if (param == "l") strcpy_s(time_format, sizeof(time_format), "%a - %b %d, %Y");
				else if (param == "f") strcpy_s(time_format, sizeof(time_format), "%A - %d of %B, %Y");
				else strcpy_s(time_format, sizeof(time_format), "wrong date format");
				
				strftime(time_buffer, sizeof(time_buffer), time_format, &t_struct);
				cout << time_buffer << endl;
			}},
			
			/**
			 * prints the time in 2 different formats. args: s/l
			 */
			{"time",     [](const string &param) {
				auto now = system_clock::now();
				time_t time = system_clock::to_time_t(now);
				
				struct tm t_struct;
				localtime_s(&t_struct, &time);
				
				if (param == "s" || param.empty()) {
					char time_buffer[50];
					strftime(time_buffer, sizeof(time_buffer), "%X", &t_struct);
					cout << time_buffer << endl;
				} else if (param == "l") system("echo %time%");
				else cout << "wrong time format" << endl;
			}},
			
			/**
			 * prints both date and time with no args.
			 */
			{"datetime", [](const string &param) {
				system("echo %date% %time%");
			}},
			
			/**
			 * prints the CLI version
			 */
			{"version",  [](const string &) {
				cout << cmdline_version << endl;
			}},
		};
		
		/* functionality of show command here */
		size_t first = args.find_first_of(' ');
		string subcommand = args;
		string param;
		if (first != string::npos) {
			subcommand = args.substr(0, first);
			param = args.substr(first);
			util::trim(&param);
		}
		
		if (subcommands.find(subcommand) != subcommands.end()) {
			subcommands[subcommand](param);
		} else {
			cout << '\'' << subcommand << '\'' << " is not defined in command 'show'" << endl;
		}
	}
}
