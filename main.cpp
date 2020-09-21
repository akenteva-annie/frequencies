#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <vector>

#define DEBUG false

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
static void get_word_counts(std::string in_filename,
							std::string out_filename);
static bool compareWordFreq(std::pair<int, std::string> a,
							std::pair<int, std::string> b);
static void check(int num, Test &test);
static void update_word_dictionary(std::unordered_map<std::string, int> &count,
								   std::string &line);

int main(int argc, char *argv[])
{
	std::string str;

	// Run tests if requested
	if (argc == 2 && (std::string(argv[1]).compare("test") == 0))
	{
		test_word_counts();
		return 0;
	}

	if (argc == 3)
		get_word_counts(argv[1], argv[2]);

	return 0;
}

/*
 * update_word_dictionary() - add words from specified line to frequency map
 */
void
update_word_dictionary(std::unordered_map<std::string, int> &count,
					   std::string &in)
{
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
}

std::vector<std::pair<int, std::string>>
get_sorted_word_frequencies(std::unordered_map<std::string, int> dict)
{
	std::vector<std::pair<int, std::string>> result;

	// Put words with their frequencies into a vector for sorting
	for (auto i = dict.begin(); i != dict.end(); i++)
		result.push_back(std::make_pair(i->second, i->first));

	// Sort by word frequencies (desc) and by alphabet (asc)
	sort(result.begin(), result.end(), compareWordFreq);

	return result;
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
void
get_word_counts(std::string in_filename, std::string out_filename)
{
	std::vector<std::pair<int, std::string>> result;
	std::unordered_map<std::string, int> dict;
	std::ifstream infile(in_filename);
	std::ofstream outfile(out_filename);
	std::string line;

	if (!infile)
		return;

	if (!outfile)
	{
		infile.close();
		return;
	}

	while (getline(infile, line))
		update_word_dictionary(dict, line);

	result = get_sorted_word_frequencies(dict);

	// Print out the sorted results
	for (auto i = result.begin(); i != result.end(); i++)
		outfile << i->first << " " << i->second << std::endl;

	infile.close();
	outfile.close();
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
		Test("Newlines don't mess anything up",
			 "\na\na\n b\nb\n bb\n\naa\n\n",
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
	const char *filename_in = "in.txt";
	const char *filename_out = "out.txt";
	std::ofstream infile(filename_in);
	std::stringstream got;

	// Write info into the input file
	if (infile.is_open())
	{
		infile << test.in;
		infile.close();
	}

	// Call the tested function
	get_word_counts(filename_in, filename_out);

	// Read results from the output file
	std::ifstream outfile(filename_out);
	if (outfile.is_open())
	{
		while (outfile >> got.rdbuf());
		outfile.close();
	}

	// Print test results
	std::cout << "======= test " << num << ": "<< test.name << " ...... ";
	ok = (test.expected.compare(got.str()) == 0);
	if (ok)
		std::cout << "ok" << std::endl;
	else
		std::cout << " failed:" << std::endl;

	if (!ok || DEBUG)
	{
		std::cout << "=> expected:" << std::endl << test.expected << std::endl;
		std::cout << "=> got:" << std::endl << got.str() << std::endl;
	}

	// Delete files that aren't needed anymore
	if (remove(filename_in) != 0)
		perror("Error deleting input file");

	if (remove(filename_out) != 0)
		perror("Error deleting output file");
}
