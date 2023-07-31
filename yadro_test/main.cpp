#include "file_reader.h"
#include "config.h"
#include "mysort.h"

int main(int argc, char* argv[])
{
	if (argc != 3) {
		std::cout << "Please, enter input and output tapes in arguments" << std::endl;
		return -3;
	}

	int res;
	config conf("conf/config.conf", argv[1], argv[2]);
	if (conf.is_fail()) {
		std::cout << "Some error occurred during program execution: code: " << -2 << std::endl;
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

	mysort sort(conf, tape_in, tape_out);
	res = sort.make_sort();
	if (res != 0)
	{
		std::cout << "Some error occurred during program execution: code: " << res << std::endl;
	}
	return res;
}
