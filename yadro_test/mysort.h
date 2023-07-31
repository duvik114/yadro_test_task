#include <algorithm>
#include <vector>
#include <cmath>

#include "config.h"

#ifndef YADRO_TEST_SORT_H
#define YADRO_TEST_SORT_H

class mysort
{
public:
	mysort(config& conf, file_reader& in, file_reader& out);

	int make_sort();

private:
	config& conf;
	file_reader& tape_in;
	file_reader& tape_out;
};

#endif //YADRO_TEST_SORT_H
