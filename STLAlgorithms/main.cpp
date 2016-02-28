#include <string>
#include <list>
#include <map>
#include <set>
#include <cassert>

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

int main()
{
	std::system("PAUSE");
	return 0;
}