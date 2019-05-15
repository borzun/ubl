#pragma once

#include <memory>
#include <vector>
#include <string>
#include <string_view>

#include "ubl.h"

UBL_NAMESPACE_BEGIN

class SuffixTree
{
private:
	struct Node;
	using NodePtr = std::shared_ptr<Node>;

public:
	explicit SuffixTree(std::string&& str);

	std::vector<std::string_view> getSuffixes() const;

	// Temp
	static int getMismatchIndex(const std::string_view& str1, const std::string_view& str2);

private:
	void buildTree();
	void processNodeToGetSuffixes(NodePtr node, std::vector<std::string_view>& suffixes) const;
	
	
private:
	static const char DELIMETER = '$';

	

	struct Node
	{
		static constexpr int INVALID_ID = -1;

		std::string_view suffix;
		std::vector<NodePtr> nodes;
		int substrIndex = INVALID_ID;
	};

	std::string m_str;
	std::vector<NodePtr> m_rootNodes;
};

UBL_NAMESPACE_END
