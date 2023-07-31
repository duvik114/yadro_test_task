#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <string>

#ifndef YADRO_TEST_FILE_READER_H
#define YADRO_TEST_FILE_READER_H

#define VALUE_SIZE 10

class file_reader
{
public:
	file_reader(const std::string& path, int size, int get_time, int set_time, int move_time, int shift_time);

	int get_element();

	void set_element(int val);

	int get_cur_pos();

	bool next();

	bool prev();

	void move_pointer(int pos);

	int get_size();

	bool is_fail();

private:
	int n;
	int line_num;
	int get_time;
	int set_time;
	int move_time;
	int shift_time;
	std::fstream file;
	bool fail_flag;

	bool next_fast();

	bool prev_fast();
};

#endif //YADRO_TEST_FILE_READER_H
