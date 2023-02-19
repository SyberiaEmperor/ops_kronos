#include "Reprise/Reprise.h"
#include "Frontend/Frontend.h"
#include <OPS_Core/IO.h>


#include <iostream>
#include <map>

using namespace std;
using namespace OPS;
using namespace OPS::Reprise;


const size_t MassThreshold = 1;

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


int main()
{
	Frontend::Frontend frontend;
	const OPS::Reprise::CompileResult& result = frontend.compileSingleFile("./tests/if_test.c");
	if (result.errorCount() > 0) { std::cout << result.errorText(); std::cout.flush(); }

	TranslationUnit& unit = frontend.getProgramUnit().getUnit(0);
	hashSubTree(&(*unit.getGlobals().getFirstSubr()));

	system("pause");
	return 0;
}