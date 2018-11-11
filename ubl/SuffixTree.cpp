#include "SuffixTree.h"
#include <cassert>
#include <algorithm>

UBL_NAMESPACE_BEGIN

SuffixTree::SuffixTree(std::string&& str)
	: m_str(std::forward<std::string>(str))
{
	m_str += DELIMETER;

	buildTree();
}

void SuffixTree::buildTree()
{
	std::string_view original_str = m_str;
	for (int i = m_str.size() - 1; i>=0; --i) {
		std::string_view str = original_str.substr(i);

		// Find nodes:
		int mismatchIndex = 0;
		NodePtr matchingNode = nullptr;
		for (auto node : m_rootNodes) {
			int index = getMismatchIndex(str, node->suffix);
			if (index > 0) {
				mismatchIndex = index;
				matchingNode = node;
				break;
			}
		}

		if (mismatchIndex == 0) {
			auto node = m_rootNodes.emplace_back(std::make_shared<Node>());
			node->suffix = str;
			node->substrIndex = i;
		}
		else {
			auto suffix = matchingNode->suffix;
			auto newSuffix = suffix.substr(0, mismatchIndex);
			auto remainingSuffix = suffix.substr(mismatchIndex);
			auto matchingNodeIndex = matchingNode->substrIndex;
			
			// Replace matching node with new suffix data:
			matchingNode->suffix = newSuffix;
			matchingNode->substrIndex = Node::INVALID_ID;

			// Move previous matching node into new suffix node:
			auto newMatchingNode = matchingNode->nodes.emplace_back(std::make_shared<Node>());
			newMatchingNode->suffix = remainingSuffix;
			newMatchingNode->substrIndex = matchingNodeIndex;

			// Create new node for current suffix:
			auto newSuffixNode = matchingNode->nodes.emplace_back(std::make_shared<Node>());
			newSuffixNode->suffix = str.substr(mismatchIndex);
			newSuffixNode->substrIndex = i;
		}
	}
}

void SuffixTree::processNodeToGetSuffixes(NodePtr node, std::vector<std::string_view>& suffixes) const
{
	if (!node) {
		return;
	}

	if (node->substrIndex != Node::INVALID_ID) {
		suffixes.push_back(node->suffix);

		assert(node->nodes.empty());
	}

	for (auto childNode : node->nodes) {
		processNodeToGetSuffixes(childNode, suffixes);
	}
}

std::vector<std::string_view> SuffixTree::getSuffixes() const
{
	std::vector<std::string_view> suffixes;
	suffixes.reserve(m_str.size());

	for (auto node : m_rootNodes) {
		processNodeToGetSuffixes(node, suffixes);
	}

	return suffixes;
}

int SuffixTree::getMismatchIndex(const std::string_view& str1, const std::string_view& str2)
{
	size_t size = std::min(str1.size(), str2.size());
	for (size_t i = 0; i < size; ++i) {
		if (str1[i] != str2[i]) {
			return i;
		}
	}

	return size;
}

UBL_NAMESPACE_END