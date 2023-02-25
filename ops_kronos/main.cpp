#include "Reprise/Reprise.h"
#include "Frontend/Frontend.h"
#include <OPS_Core/IO.h>


#include <iostream>
#include <map>

using namespace std;
using namespace OPS;
using namespace OPS::Reprise;


const size_t MassThreshold = 2;
const double SimilarityThreshold = 0.5;

struct SubTreeInfo {
	RepriseBase *node;
	size_t hashCode;
	size_t subTreeSize;

	SubTreeInfo(RepriseBase* n, int h, int s) : node(n), hashCode(h), subTreeSize(s) {}
};

map<size_t, vector<SubTreeInfo>> buckets = map<size_t, vector<SubTreeInfo>>();


pair<size_t,size_t> hashSubTree(RepriseBase* node, int depth = 0)
{
	size_t h = 0;
	size_t size = 1;

	if (node->is_a<SubroutineDeclaration>())
	{
		auto decl = (SubroutineDeclaration*)(node);
		cout << string(" ",depth) << "It is subroutinedecl" << endl;
		auto &type = decl->getType();
		//h += hash<string>{}(type.getReturnType()); implement hasher for TypeBase
		h += hash<string>{}(decl->getName());
		auto res = hashSubTree(&decl->getBodyBlock(),depth+1);
		h += res.first << 1;
		size += res.second;
	}
	if (node->is_a<BlockStatement>())
	{
		auto block = (BlockStatement*)(node);
		cout << string(" ", depth) << "It is block" << endl;
		for (int i = 0; i < block->getChildCount(); i++)
		{
			auto res = hashSubTree(&block->getChild(i), depth + 1);
			h += res.first << 1;
			size += res.second;
		}
	}
	if (node->is_a<IfStatement>())
	{
		auto ifNode = (IfStatement*)node;
		cout << string(" ", depth) << "It is if" << endl;
		h += hash<string>{}("if_node");
		for (int i = 0; i < ifNode->getChildCount(); i++)
		{
			auto res = hashSubTree(&ifNode->getChild(i), depth + 1);
			h += res.first << 1;
			size += res.second;
		}
	}
	if (node->is_a<VariableDeclaration>())
	{
		auto var = (VariableDeclaration*)(node);
		cout << string(" ", depth) << "It is var decl" << endl;
		for (int i = 0; i < var->getChildCount(); i++)
		{
			hashSubTree(&var->getChild(i), depth + 1);
		}
	}
	if (node->is_a<ExpressionStatement>())
	{
		auto exp = (ExpressionStatement*)(node);
		cout << string(" ", depth) << "It is expression" << endl;
		for (int i = 0; i < exp->getChildCount(); i++)
		{
			auto res = hashSubTree(&exp->getChild(i), depth + 1);
			h += res.first << 1;
			size += res.second;
		}
	}
	if (node->is_a<ExpressionBase>())
	{
		auto exp = (ExpressionBase*)(node);
		cout << string(" ", depth) << "It is expr base" << endl;
		auto r = exp->is_a<ExpressionBase>();

		if (node->is_a<BasicCallExpression>())
		{
			auto bce = (BasicCallExpression*)node;
			cout << bce->getKind() << endl;
			h += hash<int>{}(bce->getKind()) << 1;
		}
		if (node->is_a<BasicLiteralExpression>())
		{
			auto ble = (BasicLiteralExpression*)node;
			cout << ble->getLiteralType() << endl;
			h += hash<int>{}(ble->getLiteralType()) << 1;
		}
		if (node->is_a<StrictLiteralExpression>())
		{
			auto bse = (StrictLiteralExpression*)node;
		}

		for (int i = 0; i < exp->getChildCount(); i++)
		{
			hashSubTree(&exp->getChild(i), depth + 1);
		}
	}
	if (node->is_a<PlainCaseLabel>())
	{
		cout << string(" ", depth) << "It is label" << endl;
	}
	if (node->is_a<GotoStatement>())
	{
		cout << string(" ", depth) << "It is go to" << endl;
	}
	if (node->is_a<ReturnStatement>())
	{
		cout << string(" ", depth) << "It is return" << endl;
	}

	if (size > MassThreshold)
	{
		if (buckets.find(h) == buckets.end())
		{
			buckets[h] = vector<SubTreeInfo>();
		}

		buckets[h].push_back(SubTreeInfo(node, h, size));
	}

	return {h,size};
}

int compareTree(RepriseBase* t1, RepriseBase* t2)
{
	int shared = 0;
	if (t1->is_a<SubroutineDeclaration>() && t2->is_a<SubroutineDeclaration>())
	{
		shared++;
	}
	if (t1->is_a<BlockStatement>() && t2->is_a<BlockStatement>())
	{
		shared++;
	}
	if (t1->is_a<IfStatement>() && t2->is_a<IfStatement>())
	{
		shared++;
	}
	if (t1->is_a<VariableDeclaration>() && t2->is_a<VariableDeclaration>())
	{
		shared++;
	}
	if (t1->is_a<ExpressionStatement>() && t2->is_a<ExpressionStatement>())
	{
		shared++;
	}
	if (t1->is_a<ExpressionBase>() && t2->is_a<ExpressionBase>())
	{

		if (t1->is_a<BasicCallExpression>() && t2->is_a<BasicCallExpression>())
		{
			auto b1 = (BasicCallExpression*)t1;
			auto b2 = (BasicCallExpression*)t2;

			if (b1->getKind() == b2->getKind())
				shared++;
		}
		if (t1->is_a<BasicLiteralExpression>() && t2->is_a<BasicLiteralExpression>())
		{
			auto b1 = (BasicLiteralExpression*)t1;
			auto b2 = (BasicLiteralExpression*)t2;

			if (b1->getLiteralType() == b2->getLiteralType())
				shared++;
		}
		if (t1->is_a<StrictLiteralExpression>() && t2->is_a<StrictLiteralExpression>())
		{
			shared++;
		}
	}
	if (t1->is_a<PlainCaseLabel>() && t2->is_a<PlainCaseLabel>())
	{
		shared++;
	}
	if (t1->is_a<GotoStatement>() && t2->is_a<GotoStatement>())
	{
		shared++;
	}
	if (t1->is_a<ReturnStatement>() && t2->is_a<ReturnStatement>())
	{
		shared++;
	}

	int minChildren = min(t1->getChildCount(), t2->getChildCount());
	for (int i = 0; i < minChildren; i++)
		shared += compareTree(&t1->getChild(i), &t2->getChild(i));

	return shared;
}

bool isSimilar(SubTreeInfo& t1, SubTreeInfo& t2)
{
	int sharedNodes = compareTree(t1.node, t2.node);
	double similarity = (2 * sharedNodes) / (2 * sharedNodes + (t1.subTreeSize - sharedNodes) + (t2.subTreeSize - sharedNodes));
	return similarity > SimilarityThreshold;
}


int main()
{
	Frontend::Frontend frontend;
	const OPS::Reprise::CompileResult& result = frontend.compileSingleFile("./tests/if_test.c");
	if (result.errorCount() > 0) { std::cout << result.errorText(); std::cout.flush(); }

	TranslationUnit& unit = frontend.getProgramUnit().getUnit(0);
	hashSubTree(&(*unit.getGlobals().getFirstSubr()));

	for (auto &bucket : buckets)
	{
		for (int i = 0; i < bucket.second.size(); i++)
		{
			for (int j = i + 1; j < bucket.second.size(); j++)
			{
				if (isSimilar(bucket.second[i], bucket.second[j]))
				{
					cout << "Seems like clone found" << endl;
					cout << bucket.first << endl;
				}
			}
		}
	}

	system("pause");
	return 0;
}