#include <string>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <cassert>
#include <vector>

namespace np
{
	template <typename InputIterator, typename UnaryPredicate>
	bool none_of(InputIterator it, InputIterator itEnd, UnaryPredicate pred)
	{
		for (; it != itEnd; ++it)
		{
			if (pred(*it))
				return false;
		}
		return true;
	}

	template <typename InputIterator, typename UnaryFunction>
	UnaryFunction for_each(InputIterator it, InputIterator itEnd, UnaryFunction func)
	{
		for (; it != itEnd; ++it) 
			func(*it);
		return func;
	}
}
void Ex1()
{
	class GreateThan
	{
	public:
		GreateThan(int value)
			: m_value(value)
		{}
		bool operator()(int value) const
		{
			return (value > m_value);
		}
	private:
		int m_value;
	};

	std::list<int> list;
	list.push_front(3);
	list.push_front(6);
	list.push_front(9);
	list.push_front(4);

	assert(!np::none_of(list.cbegin(), list.cend(), GreateThan(0)));
	assert(!np::none_of(list.cbegin(), list.cend(), GreateThan(2)));
	assert(!np::none_of(list.cbegin(), list.cend(), GreateThan(5)));
}

void Ex2()
{
	class EqualTo
	{
	public:
		EqualTo(char ch)
			: m_ch(ch)
		{}
		bool operator()(char ch) const
		{
			return (ch == m_ch);
		}
	private:
		char m_ch;
	};

	typedef std::set<char> Set;
	Set set;
	set.insert('c');
	set.insert('w');
	set.insert('c');
	set.insert('i');
	set.insert('q');
	set.insert('a');
	assert(np::none_of(set.cbegin(), set.cend(), EqualTo('p')));
}

void Ex3()
{
	class EqualTo
	{
	public:
		EqualTo(const std::pair<int, char>& pair)
			: m_pair(pair)
		{}
		bool operator()(const std::pair<int, char>& pair) const
		{
			return (pair == m_pair);
		}
	private:
		std::pair<int, char> m_pair;
	};

	typedef std::map<int, char> Map;
	typedef Map::value_type ValueType;

	Map map;
	map.insert(ValueType(1, 'c'));
	map.insert(ValueType(3, 'x'));
	map.insert(ValueType(6, 'q'));

	assert(np::none_of(map.cbegin(), map.cend(), EqualTo(ValueType(2, 'c'))));
}

void Ex4()
{
	class Print
	{
	public:
		void operator()(int value) const 
		{ 
			std::cout << value << "\n";
		}
	};

	std::vector<int> v;
	v.push_back(3);
	v.push_back(6);
	v.push_back(9);
	v.push_back(4);

	np::for_each(v.cbegin(), v.cend(), Print());
}
void Ex5()
{
	class Sum
	{
	public:
		Sum(int sum = 0)
			: m_sum(sum)
		{}
		void operator()(int value) 
		{
			m_sum += value;
		}
		int m_sum;
	};

	std::set<int> set;
	set.insert(2);
	set.insert(3);
	set.insert(8);
	set.insert(9);
	set.insert(1);
	set.insert(7);

	Sum sum1 = np::for_each(set.cbegin(), set.cend(), Sum());
	assert(sum1.m_sum == 30);

	Sum sum2 = np::for_each(set.cend(), set.cend(), Sum());
	assert(sum2.m_sum == 0);
}
void Ex6()
{
	class GreaterThan
	{
	public:
		GreaterThan(int value = 6)
			: m_value(value)
			, m_result(0) 
		{}
		void operator()(int value)
		{
			if (value > m_value)
				m_result = value;
		}
		const int m_value;
		int m_result;
	};

	std::list<unsigned> list;
	list.push_front(2);
	list.push_front(4);
	list.push_front(6);
	list.push_front(9);
	list.push_front(4);
	list.push_front(3);
	list.push_front(5);

	GreaterThan result = np::for_each(list.crbegin(), list.crend(), GreaterThan());
	std::cout << result.m_result << "\n";
}

int main()
{
	Ex6();
	std::system("PAUSE");
	return 0;
}