#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#define SHOW_RESULTS false

struct Test
{
	std::string name;
	std::string in;
	std::string expected;
	Test(std::string _name, std::string _in, std::string _expected)
	{
		name = _name;
		in = _in;
		expected = _expected;
	}
};

static void test_word_counts();
static std::string get_word_counts(std::string &in);
static bool compareWordFreq(std::pair<int, std::string> a,
							std::pair<int, std::string> b);
static void check(int num, Test &test);

int main(int argc, char *argv[])
{
	std::string str;

	// Run tests if requested
	if (argc >= 2 && (std::string(argv[1]).compare("test") == 0))
	{
		test_word_counts();
		return 0;
	}

	while (getline(std::cin, str))
	{
		if (str.compare("\\q") == 0)
			// Exit the program on \q
			break;
		else
			// Otherwise, keep processing lines
			std::cout << get_word_counts(str);
	}

	return 0;
}

/*
 * get_word_counts() - count word frequencies in a string.
 *
 * in: string
 * out: string describing word frequencies
 *
 * Words can contain symbols 'a'-'z' and 'A'-'Z',
 * everything else is treated as a space.
 */
std::string
get_word_counts(std::string &in)
{
	std::vector<std::pair<int, std::string>> result;
	std::unordered_map<std::string, int> count;
	std::string word;
	std::stringstream ss;

	// Preprocess: make letters small and change other symbols to spaces
	for(std::string::size_type i = 0; i < in.size(); ++i)
	{
		if (in[i] >= 'a' && in[i] <= 'z')
			continue;
		else if (in[i] >= 'A' && in[i] <= 'Z')
			in[i] = 'a' + (in[i] - 'A');
		else
			in[i] = ' ';
	}

	// Iterate through words and count their frequencies
	ss = std::stringstream(in);
	while(getline(ss, word, ' ')) {
		if (word != "")
		{
			if (!count[word])
				count[word] = 1;
			else
				count[word] += 1;
		}
	}

	// Put words with their frequencies into a vector for sorting
	for (auto i = count.begin(); i != count.end(); i++)
		result.push_back(std::make_pair(i->second, i->first));

	// Sort by word frequencies (desc) and by alphabet (asc)
	sort(result.begin(), result.end(), compareWordFreq);

	// Print out the sorted results
	ss = std::stringstream("");
	for (auto i = result.begin(); i != result.end(); i++)
		ss << i->first << " " << i->second << std::endl;

	return ss.str();
}

/*
 * test_word_counts() - run tests for function get_word_counts()
 */
void
test_word_counts()
{
	int num = 1;

	std::vector<Test> tests =
	{
		Test("Empty input works",
			 "",
			 ""
			),
		Test("Input with only spaces and special symbols works",
			 " 1234567890-= ",
			 ""
			),
		Test("Trivial case works",
			 "hello world",
			 "1 hello\n"
			 "1 world\n"
			),
		Test("Spaces in beginning and end",
			 "  hello  world  ",
			 "1 hello\n"
			 "1 world\n"
			),
		Test("Non-letter symbols are treated as spaces",
			 "_+=hello=,.\n/1234567890=world=+_",
			 "1 hello\n"
			 "1 world\n"
			),
		Test("Correct output ordering",
			 "a a b b bb aa",
			 "2 a\n"
			 "2 b\n"
			 "1 aa\n"
			 "1 bb\n"
			),
		Test("No big letters in output",
			 "HeLLO hello",
			 "2 hello\n"
			)
	};

	for (auto i = 0; i < tests.size(); i++, num++)
		check(num, tests[i]);
}

/*
 * compareWordFreq() - function for sorting word frequencies
 *
 * First, sort by frequencies in desc order.
 * Then, sort by alphabet in asc order.
 */
bool
compareWordFreq(std::pair<int, std::string> a, std::pair<int, std::string> b)
{
	if (a.first == b.first)
		return (a.second < b.second);
	else
		return (a.first > b.first);
}

/*
 * check() - run one test
 */
void
check(int num, Test &test)
{
	bool ok;
	std::string got;
	std::cout << "======= test " << num << ": "<< test.name << " ...... ";

	got = get_word_counts(test.in);
	ok = (got.compare(test.expected) == 0);
	if (ok)
		std::cout << "ok" << std::endl;
	else
		std::cout << " failed:" << std::endl;

	if (!ok || SHOW_RESULTS)
	{
		std::cout << "=> expected:" << std::endl << test.expected << std::endl;
		std::cout << "=> got:" << std::endl << got << std::endl;
	}
}
