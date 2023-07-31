#include <fstream>
#include<vector>

#include "file_reader.h"

#ifndef YADRO_TEST_CONFIG_H
#define YADRO_TEST_CONFIG_H

class config
{
public:
	explicit config(const std::string& config_path, const std::string& input, const std::string& output);

	std::vector<file_reader> create_io_readers();

	std::vector<file_reader> create_tmp_readers();

	int get_available_space();

	bool is_fail();

private:
	int n, m;
	int get_time, set_time;
	int move_time, shift_time;
	bool fail_flag;
	std::string input_path;
	std::string output_path;
};

#endif //YADRO_TEST_CONFIG_H
