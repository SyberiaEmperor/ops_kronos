#include "Reprise/Reprise.h"
#include "Frontend/Frontend.h"
#include <OPS_Core/IO.h>
#include <Reprise/Service/DeepWalker.h>


#include <iostream>
#include <map>

using namespace std;
using namespace OPS;
using namespace OPS::Reprise;


const size_t MassThreshold = 2;
const double SimilarityThreshold = 0.5;


class HashDeepWalker : public Service::DeepWalker
{
public:
	struct SubTreeInfo {
		RepriseBase* node;
		size_t hashCode;
		size_t subTreeSize;

		SubTreeInfo(RepriseBase* n, int h, int s) : node(n), hashCode(h), subTreeSize(s) {}
	};

private:
	size_t h=0;
	int size=0;

	vector<SubTreeInfo> nodes;

	template<class Func>
	void processNode(Func f, RepriseBase* n)
	{
		size++;
		f();
		nodes.push_back(SubTreeInfo(n, h, size));
	}

public:

	void visit(ProgramUnit& pu)
	{
		processNode([&]() {
			cout << string(" ") << "It is programUnit" << endl;
			DeepWalker::visit(pu);
			}, &pu);
	}
	void visit(TranslationUnit& tu)
	{
		processNode([&]() {
			cout << string(" ") << "It is translation unit" << endl;
			DeepWalker::visit(tu);
			}, &tu);
	}

	//void visit(ProgramFragment& pf) 
	//{
	//	//processNode([&]() {
	//	//	cout << string(" ") << "It is subroutinedecl" << endl;
	//	//	DeepWalker::visit(pf);
	//	//	}, &pf);
	//}

	void visit(Declarations& dec) 
	{
		processNode([&]() {
			cout << string(" ") << "It is declarations" << endl;
			h += hash<int>{}(dec.getChildCount()) << 1;
			DeepWalker::visit(dec);
			}, &dec);
	}

	void visit(VariableDeclaration& vd) 
	{
		processNode([&]() {
			cout << string(" ") << "It is variable declaration" << endl;
			h += hash<int>{}(vd.getKind()) << 1;
			// vd.getType(); implement hasher for TypeBase
			DeepWalker::visit(vd);
			}, &vd);
	}

	void visit(TypeDeclaration& td) 
	{
		processNode([&]() {
			cout << string(" ") << "It is type declaration" << endl;
			h += hash<int>{}(td.getKind()) << 1;
			td.getKind();
			DeepWalker::visit(td);
			}, &td);
	}

	void visit(SubroutineDeclaration& sd)
	{
		processNode([&]() {
			cout << string(" ") << "It is subroutine decl" << endl;
			auto& type = sd.getType();
			//h += hash<string>{}(type.getReturnType()); implement hasher for TypeBase
			h += hash<string>{}(sd.getName()) << 1;
			DeepWalker::visit(sd);
			}, &sd);
	}

	//Statements
	void visit(BlockStatement& bs) {
		processNode([&]() {
			cout << string(" ") << "It is block statement" << endl;
			h += hash<int>{}(bs.getChildCount()) << 1;
			DeepWalker::visit(bs);
			}, &bs);
	}
	void visit(ForStatement& fs) 
	{
		processNode([&]() {
			cout << string(" ") << "It is FOR statement" << endl;
			h += hash<string>{}(typeid(fs).name()) << 1;
			DeepWalker::visit(fs);
			}, &fs);
	}
	void visit(WhileStatement& ws) 
	{
		processNode([&]() {
			cout << string(" ") << "It is WHILE statement" << endl;
			h += hash<string>{}(typeid(ws).name()) << 1;
			DeepWalker::visit(ws);
			}, &ws);
	}
	void visit(IfStatement& ifs) 
	{
		processNode([&]() {
			cout << string(" ") << "It is IF statement" << endl;
			h += hash<string>{}(typeid(ifs).name()) << 1;
			DeepWalker::visit(ifs);
			}, &ifs);
	}
	void visit(PlainCaseLabel& pcl) 
	{
		processNode([&]() {
			cout << string(" ") << "It is plaint case label" << endl;
			h += hash<sqword>{}(pcl.getValue()) << 1;
			DeepWalker::visit(pcl);
			}, &pcl);
	}
	void visit(PlainSwitchStatement& pss) 
	{
		processNode([&]() {
			cout << string(" ") << "It is plain switch statement" << endl;
			h += hash<string>{}(typeid(pss).name()) << 1;
			DeepWalker::visit(pss);
			}, &pss);
	}
	void visit(GotoStatement& gs) 
	{
		processNode([&]() {
			cout << string(" ") << "It is GOTO statement" << endl;
			h += hash<string>{}(typeid(gs).name()) << 1;
			DeepWalker::visit(gs);
			}, &gs);
	}
	void visit(ReturnStatement& rs) 
	{
		processNode([&]() {
			cout << string(" ") << "It is RETURN statement" << endl;
			h += hash<string>{}(typeid(rs).name()) << 1;
			DeepWalker::visit(rs);
			}, &rs);
	}
	void visit(ExpressionStatement& es) 
	{
		processNode([&]() {
			cout << string(" ") << "It is expr statement" << endl;
			h += hash<int>{}(es.getChildCount()) << 1;
			DeepWalker::visit(es);
			}, &es);
	}
	void visit(ASMStatement& as) 
	{
		processNode([&]() {
			cout << string(" ") << "It is ASM statement" << endl;
			h += hash<string>{}(as.getASMString()) << 1;
			DeepWalker::visit(as);
			}, &as);
	}
	void visit(EmptyStatement& es) 
	{
		processNode([&]() {
			cout << string(" ") << "It is EMPTY statement" << endl;
			h += hash<string>{}(typeid(es).name()) << 1;
			DeepWalker::visit(es);
			}, &es);
	}

	//Types
	void visit(BasicType&) {}
	void visit(PtrType&) {}
	void visit(TypedefType&) {}
	void visit(ArrayType&) {}
	void visit(StructMemberDescriptor& structMember) {}
	void visit(StructType&) {}
	void visit(EnumMemberDescriptor&) {}
	void visit(EnumType&) {}
	void visit(ParameterDescriptor&) {}
	void visit(SubroutineType&) {}
	void visit(DeclaredType&) {}
	void visit(VectorType&) {}

	//Expressions
	void visit(BasicLiteralExpression& ble) 
	{
		processNode([&]() {
			cout << string(" ") << "It is basic literal exlpression" << endl;
			h += hash<int>{}(ble.getLiteralType()) << 1;
			h += hash<int>{}(ble.getChildCount()) << 1;
			DeepWalker::visit(ble);
			}, &ble);
	}
	void visit(StrictLiteralExpression& sle
	) 
	{
		processNode([&]() {
			cout << string(" ") << "It is strict literal expression" << endl;
			h += hash<int>{}(sle.getLiteralType()) << 1;
			h += hash<int>{}(sle.getChildCount()) << 1;
			DeepWalker::visit(sle);
			}, &sle);
	}
	void visit(CompoundLiteralExpression&) {}
	void visit(ReferenceExpression&) {}
	void visit(SubroutineReferenceExpression&) {}
	void visit(StructAccessExpression&) {}
	void visit(EnumAccessExpression&) {}
	void visit(TypeCastExpression&){}
	void visit(BasicCallExpression&) {}
	void visit(SubroutineCallExpression&) {}
	void visit(EmptyExpression&) {}
};

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

	//if (size > MassThreshold)
	//{
	//	if (buckets.find(h) == buckets.end())
	//	{
	//		buckets[h] = vector<HashDeepWalker::SubTreeInfo>();
	//	}

	//	buckets[h].push_back(HashDeepWalker::SubTreeInfo(node, h, size));
	//}

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

//bool isSimilar(SubTreeInfo& t1, SubTreeInfo& t2)
//{
//	int sharedNodes = compareTree(t1.node, t2.node);
//	double similarity = (2 * sharedNodes) / (2 * sharedNodes + (t1.subTreeSize - sharedNodes) + (t2.subTreeSize - sharedNodes));
//	return similarity > SimilarityThreshold;
//}

map<size_t, vector<HashDeepWalker::SubTreeInfo>> buckets = map<size_t, vector<HashDeepWalker::SubTreeInfo>>();


int main()
{
	Frontend::Frontend frontend;
	const OPS::Reprise::CompileResult& result = frontend.compileSingleFile("./tests/if_test.c");
	if (result.errorCount() > 0) { std::cout << result.errorText(); std::cout.flush(); }

	TranslationUnit& unit = frontend.getProgramUnit().getUnit(0);
	HashDeepWalker hdw;
	hdw.visit(unit);
	//hashSubTree(&(*unit.getGlobals().getFirstSubr()));

	//for (auto &bucket : buckets)
	//{
	//	for (int i = 0; i < bucket.second.size(); i++)
	//	{
	//		for (int j = i + 1; j < bucket.second.size(); j++)
	//		{
	//			if (isSimilar(bucket.second[i], bucket.second[j]))
	//			{
	//				cout << "Seems like clone found" << endl;
	//				cout << bucket.first << endl;
	//			}
	//		}
	//	}
	//}

	system("pause");
	return 0;
}