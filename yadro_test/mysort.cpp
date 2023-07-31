#include "mysort.h"

mysort::mysort(config& c, file_reader& in, file_reader& out)
		:conf(c), tape_in(in), tape_out(out)
{
}

int mysort::make_sort()
{
	int n = tape_in.get_size();
	int m = conf.get_available_space();
	if (m < 1024008)
	{
		std::cout << "Not enough available space, min is 1024000 bytes" << std::endl;
		return -1;
	}
	m = (m - 1024000) / 4;
	if (m >= n)
	{
		std::vector<int> vec;
		for (int i = 0; i < n; i++)
		{
			vec.push_back(tape_in.get_element());
			if (i < n - 1 && !tape_in.next())
			{
				std::cout << "Input tape actual size is: " << (i + 1) << ", but in config size is: " << n << std::endl;
				return -1;
			}
		}
		std::sort(vec.begin(), vec.end());
		for (int i = 0; i < n; i++)
		{
			tape_out.set_element(vec[i]);
			if (i < n - 1 && !tape_out.next())
			{
				std::cout << "Output tape actual size is: " << (i + 1) << ", but in config size is: " << n << std::endl;
				return -1;
			}
		}
	}
	else
	{
		int cur_count = 0;
		std::vector<file_reader> tmp_readers = conf.create_tmp_readers();
		if (tmp_readers[0].is_fail() || tmp_readers[1].is_fail() || tmp_readers[2].is_fail())
		{
			return -2;
		}
		file_reader& tape_tmp1 = tmp_readers[0];
		file_reader& tape_tmp2 = tmp_readers[1];
		file_reader& tape_tmp3 = tmp_readers[2];
		int min_size = (int)pow(2, (int)log2(m));
		std::vector<int> tmp_vec(min_size, 0);
		for (int i = 0; i < n; i++)
		{
			if (cur_count < min_size)
			{
				tmp_vec[cur_count++] = tape_in.get_element();
			}
			else
			{
				std::sort(tmp_vec.begin(), tmp_vec.end());
				for (int j = 0; j < min_size; j++)
				{
					tape_tmp1.set_element(tmp_vec[j]);
					tape_tmp1.next();
				}
				cur_count = 0;
				tmp_vec[cur_count++] = tape_in.get_element();
			}
			if (i < n - 1 && !tape_in.next())
			{
				std::cout << "Input tape actual size is: " << (i + 1) << ", but in config size is: " << n << std::endl;
				return -1;
			}
		}
		if (cur_count != 0)
		{
			std::sort(tmp_vec.begin(), tmp_vec.begin() + cur_count);
			for (int i = 0; i < cur_count; i++)
			{
				tape_tmp1.set_element(tmp_vec[i]);
				tape_tmp1.next();
			}
		}

		file_reader* l_tmp1 = &tape_tmp1;
		file_reader* l_tmp2 = &tape_tmp2;
		file_reader* l_tmp3 = &tape_tmp3;
		while (min_size < n)
		{
			int count = 0;
			l_tmp1->move_pointer(0);
			l_tmp2->move_pointer(0);
			l_tmp3->move_pointer(0);
			while (count < n)
			{
				int count1 = count, count2 = count;
				for (int i = count; i < count + min_size && i < n; i++)
				{
					l_tmp2->set_element(l_tmp1->get_element());
					l_tmp1->next();
					l_tmp2->next();
					count1++;
				}
				for (int i = count; i < count1; i++)
				{
					l_tmp2->prev();
				}
				int count1_size = count1;
				int count2_size = count2 + (count1 - count);
				while ((count1 < count1_size + min_size && count1 < n) && count2 < count2_size)
				{
					if ((l_tmp1->get_element()) < (l_tmp2->get_element()))
					{
						l_tmp3->set_element(l_tmp1->get_element());
						l_tmp3->next();
						l_tmp1->next();
						count1++;
					}
					else
					{
						l_tmp3->set_element(l_tmp2->get_element());
						l_tmp3->next();
						l_tmp2->next();
						count2++;
					}
					count++;
				}
				while ((count1 < count1_size + min_size && count1 < n))
				{
					l_tmp3->set_element(l_tmp1->get_element());
					l_tmp3->next();
					l_tmp1->next();
					count1++;
					count++;
				}
				while (count2 < count2_size)
				{
					l_tmp3->set_element(l_tmp2->get_element());
					l_tmp3->next();
					l_tmp2->next();
					count2++;
					count++;
				}
			}
			file_reader* l_tmp = l_tmp1;
			l_tmp1 = l_tmp3;
			l_tmp3 = l_tmp;
			min_size = std::min(min_size * 2, n);
		}
		l_tmp1->move_pointer(0);
		for (int i = 0; i < n; i++)
		{
			tape_out.set_element(l_tmp1->get_element());
			l_tmp1->next();
			if (i < n - 1 && !tape_out.next())
			{
				std::cout << "Output tape actual size is: " << (i + 1) << ", but in config size is: " << n << std::endl;
				return -1;
			}
		}
	}
	return 0;
}
