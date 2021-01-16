#include <iostream>
#include <fstream>
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
	 * gets or sets the bool 'cmdline_output_errors'
	 */
	void command_errors(const string &);
	
	/**
	 * gets or sets the bool 'cmdline_start_showing'
	 */
	void command_echo(const string &);
	
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
	 * prints a text with or without variables
	 */
	void command_print(const string &);
	
	/**
	 * prints a text with or without variables without new line
	 */
	void command_print_i(const string &);
	
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
	
	/**
	 * prints some predefined variables with 0 or more args without new line
	 */
	void command_show_i(const string &);
	
	/* commands map */
	map<string, void (*)(const string &)> commands = {
		{"commands",  command_commands},
		{"error",     command_errors},
		{"echo",      command_echo},
		{"exit",      command_exit},
		{"pause",     command_pause},
		{"clear",     command_clear},
		{"print",     command_print},
		{"print_i",   command_print_i},
		{"var",       command_var},
		{"var_unset", command_var_unset},
		{"show",      command_show}
	};
	
	/* the description of commands */
	map<string, string> definitions = {
		{"exit",      "closes the console"},
		{"error",     "gets or sets the error level"},
		{"echo",      "gets or sets the echo level"},
		{"pause",     "pauses the console"},
		{"clear",     "clears the console"},
		{"print",     "prints a text"},
		{"print_i",   "prints a text without new line"},
		{"var",       "defines or changes a variable"},
		{"var_unset", "destroys a variable"},
		{"show",      "shows the specified system variable"}
	};
	
	/* command line variables */
	map<string, string> variables;
	
	/* command line properties */
	static string cmdline_version = "1.3.1";
	static string cmdline_start = "CLI>";
	static bool cmdline_start_showing = true;
	static bool cmdline_output_errors = true;
	
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
				error_out("unknown command");
			}
		}
		
		/**
		 * interpret command from given parameter
		 *
		 * @param _command the command to be interpreted
		 */
		static void read_command(const string &_command) {
			util::trim(const_cast<string *>(&_command));
			string cmd = _command.substr(0, _command.find(' '));
			string args = _command.substr(_command.find_first_of(" \t") + 1);
			util::trim(&args);
			args = args == cmd ? "" : args;
			
			if (commands.find(cmd) != commands.end()) {
				commands[cmd](args);
			} else if (!cmd.empty()) {
				error_out("unknown command");
			}
		}
		
		/**
		 * outputs errors
		 *
		 * @param err the error text
		 */
		static void error_out(const string &err) {
			if (cmdline_output_errors) cout << err << endl;
		}
	};
	
	/* implementing commands */
	void command_commands(const string &) {
		for (auto &definition : definitions) {
			cout << definition.first << "\t" << definition.second << endl;
		}
	}
	
	void command_errors(const string &args) {
		if (args.empty()) cout << (int) cmdline_output_errors << endl;
		else if (args == "0") cmdline_output_errors = false;
		else if (args == "1") cmdline_output_errors = true;
		else command_line::error_out("wrong error level");
	}
	
	void command_echo(const string &args) {
		if (args.empty()) cout << (int) cmdline_start_showing << endl;
		else if (args == "0") cmdline_start_showing = false;
		else if (args == "1") cmdline_start_showing = true;
		else command_line::error_out("wrong echo level");
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
	
	void command_print_i(const string &args) {
		string temp = args;
		util::replace_with_var(temp, variables);
		cout << temp;
	}
	
	void command_var(const string &args) {
		string key = args.substr(0, args.find(' '));
		if (key.empty()) {
			command_line::error_out("variable name and value missing");
			return;
		}
		
		string value = args.substr(args.find_first_of(" \t") + 1);
		util::trim(&value);
		if (value == key) {
			command_line::error_out("variable value missing");
			return;
		}
		
		if (value == "+input") getline(cin, value);
		
		variables[key] = value;
	}
	
	void command_var_unset(const string &args) {
		if (args.empty()) {
			command_line::error_out("no variable mentioned");
			return;
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
				else {
					command_line::error_out("wrong date format");
					return;
				}
				
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
				else {
					command_line::error_out("wrong time format");
					return;
				}
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
			command_line::error_out('\'' + subcommand + '\'' + " is not defined in command 'show'");
		}
	}
}
