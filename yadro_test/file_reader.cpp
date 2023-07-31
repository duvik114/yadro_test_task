#include "file_reader.h"

file_reader::file_reader(const std::string& path, int size, int get_t, int set_t, int move_t, int shift_t)
{
	file = std::fstream(path, std::ios::in | std::ios::out);
	fail_flag = file.fail();
	line_num = 0;
	n = size;
	get_time = get_t;
	set_time = set_t;
	move_time = move_t;
	shift_time = shift_t;
}

int file_reader::get_element()
{
	char char_res[11];
	auto before_pos = file.tellp();
	file.read(char_res, 11);
	std::string str_res(char_res);
	file.seekp(before_pos);
	file.seekg(before_pos);
	using namespace std::this_thread;
	using namespace std::chrono;
	sleep_for(nanoseconds(get_time));
	return stoi(str_res);
}

void file_reader::set_element(int val)
{
	bool is_negative = val < 0;
	val = abs(val);
	std::string str_val = std::to_string(val);
	std::string lead_zeros(VALUE_SIZE - str_val.length(), '0');
	auto before_pos = file.tellg();
	str_val = (is_negative ? "-" : "+") + lead_zeros + str_val;
	file.write(str_val.c_str(), 11);
	file.seekg(before_pos);
	file.seekp(before_pos);
	using namespace std::this_thread;
	using namespace std::chrono;
	sleep_for(nanoseconds(set_time));
}

bool file_reader::next()
{
	using namespace std::this_thread;
	using namespace std::chrono;
	sleep_for(nanoseconds(shift_time));
	return next_fast();
}

bool file_reader::prev()
{
	using namespace std::this_thread;
	using namespace std::chrono;
	sleep_for(nanoseconds(shift_time));
	return prev_fast();
}

bool file_reader::next_fast()
{
	if (line_num >= n - 1)
	{
		return false;
	}
	line_num++;
	file.seekg(file.tellg().operator+(VALUE_SIZE + 1));
	file.seekp(file.tellg());
	return true;
}

bool file_reader::prev_fast()
{
	if (line_num <= 0)
	{
		return false;
	}
	line_num--;
	file.seekg(file.tellg().operator-((VALUE_SIZE + 1)));
	file.seekp(file.tellg());
	return true;
}

void file_reader::move_pointer(int pos)
{
	using namespace std::this_thread;
	using namespace std::chrono;
	sleep_for(nanoseconds(move_time));
	while (line_num != pos)
	{
		line_num < pos ? next_fast() : prev_fast();
	}
}

int file_reader::get_cur_pos()
{
	return line_num;
}

int file_reader::get_size()
{
	return n;
}

bool file_reader::is_fail()
{
	return fail_flag;
}
