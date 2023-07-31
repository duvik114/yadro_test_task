#include "config.h"

config::config(const std::string& config_path, const std::string& input, const std::string& output)
{
	input_path = input;
	output_path = output;

	std::ifstream file;
	file.open(config_path);
	if (file.fail())
	{
		std::cout << "Cannot open config file: " << config_path << std::endl;
		fail_flag = true;
		return;
	}
	std::string description;
	std::string value;

	try
	{
		int count = 0;
		while ((file >> description >> value) && count < 6)
		{
			if (description == "tape_size")
			{
				n = stoi(value);
			}
			else if (description == "available_space")
			{
				m = stoi(value);
			}
			else if (description == "read_time")
			{
				get_time = stoi(value);
			}
			else if (description == "write_time")
			{
				set_time = stoi(value);
			}
			else if (description == "move_time")
			{
				move_time = stoi(value);
			}
			else if (description == "shift_time")
			{
				shift_time = stoi(value);
			}
			count++;
		}
		if (count != 6) {
			std::cout << "Not enough config properties: need 6, got " << count << std::endl;
		}
		fail_flag = count != 6;
	} catch (std::exception& e) {
		std::cout << "Conversion failure: " << e.what() << std::endl;
		fail_flag = true;
	}
}

std::vector<file_reader> config::create_io_readers()
{
	std::vector<file_reader> res;
	res.emplace_back(input_path, n, get_time, set_time, move_time, shift_time);
	res.emplace_back(output_path, n, get_time, set_time, move_time, shift_time);
	if (res[0].is_fail())
	{
		std::cout << "Cannot open input file: " << input_path << std::endl;
	}
	if (res[1].is_fail())
	{
		std::cout << "Cannot open output file: " << output_path << std::endl;
	}
	return res;
}

std::vector<file_reader> config::create_tmp_readers()
{
	std::vector<file_reader> res;
	std::ofstream tape_tmp1, tape_tmp2, tape_tmp3;
	tape_tmp1.open("tmp/tape_tmp1.txt", std::fstream::trunc);
	tape_tmp2.open("tmp/tape_tmp2.txt", std::fstream::trunc);
	tape_tmp3.open("tmp/tape_tmp3.txt", std::fstream::trunc);
	if (tape_tmp1.fail())
	{
		std::cout << "Cannot open tmp1 file: tmp/tape_tmp1.txt" << std::endl;
	}
	if (tape_tmp2.fail())
	{
		std::cout << "Cannot open tmp2 file: tmp/tape_tmp2.txt" << std::endl;
	}
	if (tape_tmp3.fail())
	{
		std::cout << "Cannot open tmp3 file: tmp/tape_tmp3.txt" << std::endl;
	}
	for (int i = 0; i < n; i++)
	{
		tape_tmp1 << "+0000000000";
		tape_tmp2 << "+0000000000";
		tape_tmp3 << "+0000000000";
	}
	tape_tmp1.close();
	tape_tmp2.close();
	tape_tmp3.close();

	res.emplace_back("tmp/tape_tmp1.txt", n, get_time, set_time, move_time, shift_time);
	res.emplace_back("tmp/tape_tmp2.txt", n, get_time, set_time, move_time, shift_time);
	res.emplace_back("tmp/tape_tmp3.txt", n, get_time, set_time, move_time, shift_time);
	return res;
}

int config::get_available_space()
{
	return m;
}

bool config::is_fail()
{
	return fail_flag;
}
