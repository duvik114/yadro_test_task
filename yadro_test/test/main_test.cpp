#include "../file_reader.h"
#include "../config.h"
#include "../mysort.h"

int run_config_tests()
{
	for (int i = 1; i < 2; i++)
	{
		config conf("test/config_test/test_conf" + std::to_string(i) + ".conf", "test/config_test/in.txt",
				"test/config_test/out.txt");
		if (conf.is_fail())
		{
			std::cout << "Error in config " + std::to_string(i) + " test" << std::endl;
			return -1;
		}
		std::vector<file_reader> io_readers = conf.create_io_readers();
		if (io_readers[0].is_fail() || io_readers[1].is_fail())
		{
			std::cout << "Error in config " + std::to_string(i) + " test" << std::endl;
			return -1;
		}
	}
	for (int i = 2; i < 4; i++)
	{
		config conf("test/config_test/test_conf" + std::to_string(i) + ".conf", "test/config_test/in.txt",
				"test/config_test/out.txt");
		if (!conf.is_fail())
		{
			std::cout << "Error in config " + std::to_string(i) + " test" << std::endl;
			return -1;
		}
	}
	return 0;
}

int run_io_tests()
{
	int res = 0;
	config conf("test/io_test/test_conf.conf", "test/io_test/in.txt", "test/io_test/out.txt");
	if (conf.is_fail())
	{
		std::cout << "Some error occurred during program execution: code: " << res << std::endl;
		return -2;
	}
	std::vector<file_reader> io_readers = conf.create_io_readers();
	if (io_readers[0].is_fail() || io_readers[1].is_fail())
	{
		std::cout << "Some error occurred during program execution: code: " << -2 << std::endl;
		return -2;
	}
	file_reader& tape_in = io_readers[0];
	file_reader& tape_out = io_readers[1];

	while (true)
	{
//		std::cout << tape_in.get_element() << " ";
		if (!tape_in.next())
			break;
	}
	for (int i = 0; i < tape_in.get_size(); i++)
	{
		bool res_prev = tape_in.prev();
		if (i < tape_in.get_size() - 1 && !res_prev)
		{
			std::cout << "Not enough elements (" << (i + 1) << ") inside input tape in io tests" << std::endl;
			return -2;
		}
	}
	while (true)
	{
		tape_out.set_element(tape_in.get_element());
		if (!tape_in.next())
			break;
		if (!tape_out.next())
			break;
	}
	tape_in.move_pointer(0);
	tape_out.move_pointer(0);
	for (int i = 0; i < 10; i++)
	{
		if (tape_in.get_element() != tape_out.get_element())
		{
			std::cout << "Wrong values inside output tape" << std::endl;
			return -2;
		}
		if (i != 9 && !tape_in.next())
		{
			std::cout << "Not enough values inside input tape" << std::endl;
			return -2;
		}
		if (i != 9 && !tape_out.next())
		{
			std::cout << "Not enough values inside input tape" << std::endl;
			return -2;
		}
	}
	return 0;
}

int run_sort_tests()
{
	for (int i = 1; i < 5; i++)
	{
		int res;
		config conf("test/sort_test/test_conf" + std::to_string(i) + ".conf",
				"test/sort_test/in" + std::to_string(i) + ".txt", "test/sort_test/out" + std::to_string(i) + ".txt");
		if (conf.is_fail())
		{
			std::cout << "Error in sort " + std::to_string(i) + " test" << std::endl;
			return -1;
		}
		std::vector<file_reader> io_readers = conf.create_io_readers();
		if (io_readers[0].is_fail() || io_readers[1].is_fail())
		{
			std::cout << "Error in sort " + std::to_string(i) + " test" << std::endl;
			return -1;
		}
		file_reader& tape_in = io_readers[0];
		file_reader& tape_out = io_readers[1];

		mysort sort(conf, tape_in, tape_out);
		res = sort.make_sort();
		if (res != 0)
		{
			std::cout << "Error in sort test, res: " << res << std::endl;
			return res;
		}

		tape_out.move_pointer(0);
		std::ifstream correct("test/sort_test/out_correct" + std::to_string(i) + ".txt");
		for (int j = 0; j < tape_in.get_size(); j++)
		{
			char char_res[11];
			correct.read(char_res, 11);
			std::string str_res(char_res);
			int int_res = std::stoi(str_res);
			if (int_res != tape_out.get_element())
			{
				std::cout << "Wrong result in sort test " << i << std::endl;
			}
			if (j != tape_in.get_size() - 1 && !tape_out.next())
			{
				std::cout << "Not enough integers in result of in sort test " << i << std::endl;
			}
		}
	}
	return 0;
}

//int main()
//{
//	int res;
//	res = run_config_tests();
//	if (res != 0)
//	{
//		return res;
//	}
//	res = run_io_tests();
//	if (res != 0)
//	{
//		return res;
//	}
//	res = run_sort_tests();
//	if (res != 0)
//	{
//		return res;
//	}
//	std::cout << "ALL CLEAR!" << std::endl;
//	return res;
//}
