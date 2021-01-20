using namespace std;
using namespace chrono;

namespace cli::system_vars {
	/**
	 * returns the formatted date by given params
	 *
	 * @param param the parameters to format
	 * @return formatted date
	 */
	string base_command_show_date(const string &param) {
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
		else return "";
		
		strftime(time_buffer, sizeof(time_buffer), time_format, &t_struct);
		return time_buffer;
	}
	
	/**
	 * returns the formatted time by given params
	 *
	 * @param param the parameters to format
	 * @return formatted time
	 */
	string base_command_show_time(const string &param) {
		auto now = system_clock::now();
		time_t time = system_clock::to_time_t(now);
		
		struct tm t_struct;
		localtime_s(&t_struct, &time);
		
		if (param == "s" || param.empty()) {
			char time_buffer[50];
			strftime(time_buffer, sizeof(time_buffer), "%X", &t_struct);
			return time_buffer;
		} else if (param == "l") return "echo";
		else return "";
	}
}
