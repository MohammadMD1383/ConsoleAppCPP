using namespace std;

namespace util {
	/**
	 * trims both sides of a string and returns it
	 *
	 * @param str the string to be trimmed
	 * @return trimmed string
	 */
	// string trim(const string &str) {
	// 	size_t first = str.find_first_not_of(' ');
	// 	if (string::npos == first) return str;
	// 	size_t last = str.find_last_not_of(' ');
	// 	return str.substr(first, (last - first + 1));
	// }
	
	/**
	 * trims both sides of a string and applies it on source
	 *
	 * @param str the string to be modified
	 */
	void trim(string *str) {
		size_t first = str->find_first_not_of(' ');
		if (string::npos == first) return;
		size_t last = str->find_last_not_of(' ');
		*str = str->substr(first, (last - first + 1));
	}
	
	/**
	 * finds the second occurrence of a character
	 *
	 * @param str the string to search in
	 * @param ch the character
	 * @param first the position of first occurrence of the character found by string.first() or string.find_first_of()
	 * @return returns the position of second occurrence
	 */
	size_t find_second_of(const string &str, const char &ch, const size_t &first) {
		return str.find(ch, first + 1);
	}
	
	/**
	 * replaces variables mentioned inside the string given with its value or empty if not found
	 *
	 * @param str the string to be modified
	 * @param vars map of variables
	 */
	void replace_with_var(string &str, map<string, string> &vars) {
		again:
		
		size_t first = str.find_first_of('^');
		if (first == string::npos) return;
		
		size_t second = find_second_of(str, '^', first);
		if (second == string::npos) return;
		
		string var_key = str.substr(first + 1, (second - first) - 1);
		string replacement = vars.find(var_key) == vars.end() ? "" : vars[var_key];
		
		str.replace(first, (second - first) + 1, replacement);
		
		goto again;
	}
	
	/**
	 * splits the string given by white space
	 *
	 * @param str the string to be split
	 * @return list of split strings
	 */
	list<string> split_string_space(const string &str) {
		if (str.find_first_of(' ') == string::npos) return list{str};
		
		auto &temp = const_cast<string &>(str);
		list<string> str_list;
		
		again:
		size_t end_point = str.find_first_of(' ');
		if (end_point == string::npos) {
			str_list.push_back(temp);
			return str_list;
		}
		
		str_list.push_back(temp.substr(0, end_point));
		temp.erase(0, end_point);
		trim(&temp);
		goto again;
	}
	
	/**
	 * replaces the string with special character:
	 * 'n' new line
	 * 't' tab
	 * 's' space
	 *
	 * @param str the string to be modified
	 */
	void replace_with_special_chars(string *str) {
		size_t pos;
		while ((pos = str->find("\\s")) != string::npos) str->replace(pos, 2, " ");
		while ((pos = str->find("\\t")) != string::npos) str->replace(pos, 2, "\t");
		while ((pos = str->find("\\n")) != string::npos) str->replace(pos, 2, "\n");
	}
	
	/**
	 * replaces the string with special characters:
	 * 'q' ?
	 * 'c' :
	 *
	 * @param str the string to be modified
	 */
	void replace_with_if_command_special_chars(string *str) {
		size_t pos;
		while ((pos = str->find("\\q")) != string::npos) str->replace(pos, 2, "?");
		while ((pos = str->find("\\c")) != string::npos) str->replace(pos, 2, ":");
	}
	
	/**
	 * parses a condition based on compare operation
	 *
	 * @param cond the condition string
	 * @param is_n defines if the sides should compare in numeric way
	 * @return the result of condition
	 */
	bool parse_condition(const string &cond, const bool &is_n) {
		string operation;
		list<string>::iterator operation_pos;
		list<string> parts = split_string_space(cond);
		
		for (auto part = parts.begin(); part != parts.end(); part++) {
			if (*part == "==" || *part == "!=" ||
			    *part == ">" || *part == "<" ||
			    *part == ">=" || *part == "<=") {
				operation = *part;
				operation_pos = part;
				break;
			}
		}
		
		if (operation.empty()) return true;
		
		string left_part;
		string right_part;
		
		for (auto part = parts.begin(); part != operation_pos; part++) left_part += *part;
		for (auto part = next(operation_pos); part != parts.end(); part++) right_part += *part;
		
		if (is_n) {
			try {
				if (operation == "==") return stod(left_part) == stod(right_part);
				if (operation == "!=") return stod(left_part) != stod(right_part);
				if (operation == "<") return stod(left_part) < stod(right_part);
				if (operation == ">") return stod(left_part) > stod(right_part);
				if (operation == "<=") return stod(left_part) <= stod(right_part);
				if (operation == ">=") return stod(left_part) >= stod(right_part);
			} catch (...) {
				return false;
			}
		} else {
			if (operation == "==") return left_part == right_part;
			if (operation == "!=") return left_part != right_part;
			if (operation == "<") return left_part < right_part;
			if (operation == ">") return left_part > right_part;
			if (operation == "<=") return left_part <= right_part;
			if (operation == ">=") return left_part >= right_part;
		}
		
		return false;
	}
}
