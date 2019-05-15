#include <vector>
#include <numeric>

#include "utils/tests_helper.h"
#include "utils/ElapsedTimer.h"

#include "SuffixTree.h"

#include "gtest/gtest.h"

TEST(TestSuffixTree, TempTestGetMismatchIndex)
{
	ASSERT_EQ(ubl::SuffixTree::getMismatchIndex("ANANA", "N"), 0);
	ASSERT_EQ(ubl::SuffixTree::getMismatchIndex("M", "N"), 0);
	ASSERT_EQ(ubl::SuffixTree::getMismatchIndex("", "ABCD"), 0);
	ASSERT_EQ(ubl::SuffixTree::getMismatchIndex("ABCD", ""), 0);
	ASSERT_EQ(ubl::SuffixTree::getMismatchIndex("ANANA", "AB"), 1);
	ASSERT_EQ(ubl::SuffixTree::getMismatchIndex("ANANA", "ANAN"), 4);
	ASSERT_EQ(ubl::SuffixTree::getMismatchIndex("ABCDEFG", "ABCDEFG"), 7);
	ASSERT_EQ(ubl::SuffixTree::getMismatchIndex("BANANA", "BANANAX"), 6);
}

TEST(TestSuffixTree, TempTestBuildSuffixTree1)
{
	ubl::SuffixTree tree("BANANA");

	auto suffixes = tree.getSuffixes();
	std::set<std::string_view> idealSuffixes =
	{
		"$", "A$", "NA$", "ANA$", "NANA$", "ANANA$", "BANANA$"
	};

	ASSERT_EQ(idealSuffixes.size(), suffixes.size());

	for (auto suffix : suffixes) {
		auto iter = idealSuffixes.find(suffix);
		ASSERT_TRUE(iter != idealSuffixes.end());

		idealSuffixes.erase(iter);
	}

	ASSERT_TRUE(idealSuffixes.empty());
}

TEST(TestSuffixTree, DISABLED_TestBananaSuffixes)
{
	ubl::SuffixTree tree("BANANA");

	auto suffixes = tree.getSuffixes();
	const std::vector<std::string_view> idealSuffixes =
	{
		"A", "NA", "ANA", "NANA", "ANANA", "BANANA"
	};

	ASSERT_EQ(idealSuffixes, suffixes);
}
