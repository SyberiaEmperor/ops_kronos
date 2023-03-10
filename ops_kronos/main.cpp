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
const double SimilarityThreshold = 0.1;


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
		size_t th = h;
		h = 0;
		f();
		h = th;
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
			//vd.getName()
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
	void visit(BasicType& bt) 
	{
		processNode([&]() {
			cout << string(" ") << "It is basic type" << endl;
			h += hash<int>{}(bt.getKind()) << 1;
			h += hash<int>{}(bt.getSizeOf()) << 1;
			DeepWalker::visit(bt);
			}, &bt);
	}
	void visit(PtrType& pt) 
	{
		processNode([&]() {
			cout << string(" ") << "It is pointer" << endl;
			h += hash<string>{}(typeid(pt).name()) << 1;
			DeepWalker::visit(pt);
			}, &pt);
	}
	void visit(TypedefType& tdt) 
	{
		processNode([&]() {
			cout << string(" ") << "It is typedef" << endl;
			h += hash<string>{}(typeid(tdt).name()) << 1;
			DeepWalker::visit(tdt);
			}, &tdt);
	}
	void visit(ArrayType& at) 
	{
		processNode([&]() {
			cout << string(" ") << "It is array" << endl;
			h += hash<string>{}(typeid(at).name()) << 1;
			h += hash<int>{}(at.getElementCount()) << 1;
			DeepWalker::visit(at);
			}, &at);
	}
	void visit(StructMemberDescriptor& structMember) 
	{
		processNode([&]() {
			cout << string(" ") << "It is struct member" << endl;
			h += hash<string>{}(typeid(structMember).name()) << 1;
			h += hash<int>{}(structMember.getBitsLimit()) << 1;
			//h += hash<string>{}(structMember.getName()) << 1;
			DeepWalker::visit(structMember);
			}, &structMember);
	}
	void visit(StructType& st) 
	{
		processNode([&]() {
			cout << string(" ") << "It is struct type" << endl;
			h += hash<string>{}(typeid(st).name()) << 1;
			h += hash<int>{}(st.getMemberCount()) << 1;
			DeepWalker::visit(st);
			}, &st);
	}
	void visit(EnumMemberDescriptor& emdt) 
	{
		processNode([&]() {
			cout << string(" ") << "It is enum member descriptor" << endl;
			h += hash<string>{}(typeid(emdt).name()) << 1;
			h += hash<int>{}(emdt.getValue()) << 1;
			DeepWalker::visit(emdt);
			}, &emdt);
	}
	void visit(EnumType& et) 
	{
		processNode([&]() {
			cout << string(" ") << "It is enum type" << endl;
			h += hash<string>{}(typeid(et).name()) << 1;
			h += hash<int>{}(et.getMemberCount()) << 1;
			DeepWalker::visit(et);
			}, &et);
	}
	void visit(ParameterDescriptor& pd) 
	{
		processNode([&]() {
			cout << string(" ") << "It is parameter desctiptor" << endl;
			h += hash<string>{}(typeid(pd).name()) << 1;
			h += hash<int>{}(pd.getTransitKind()) << 1;
			DeepWalker::visit(pd);
			}, &pd);
	}
	void visit(SubroutineType& st) 
	{
		processNode([&]() {
			cout << string(" ") << "It is subr type" << endl;
			h += hash<string>{}(typeid(st).name()) << 1;
			h += hash<int>{}(st.getCallingKind()) << 1;
			h += hash<int>{}(st.getParameterCount()) << 1;
			DeepWalker::visit(st);
			}, &st);
	}
	void visit(DeclaredType& dt) 
	{
		processNode([&]() {
			cout << string(" ") << "It is decl type" << endl;
			h += hash<string>{}(typeid(dt).name()) << 1;
			DeepWalker::visit(dt);
			}, &dt);
	}
	void visit(VectorType& vt) 
	{
		processNode([&]() {
			cout << string(" ") << "It is decl type" << endl;
			h += hash<string>{}(typeid(vt).name()) << 1;
			h += hash<int>{}(vt.getElementCount()) << 1;
			DeepWalker::visit(vt);
			}, &vt);
	}

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
	void visit(CompoundLiteralExpression& cle) 
	{
		processNode([&]() {
			cout << string(" ") << "It is compound literal expression" << endl;
			h += hash<int>{}(cle.getChildCount()) << 1;
			DeepWalker::visit(cle);
			}, &cle);
	}
	void visit(ReferenceExpression& re) 
	{
		processNode([&]() {
			cout << string(" ") << "It is ref expression" << endl;
			h += hash<string>{}(typeid(re).name()) << 1;
			h += hash<int>{}(re.getChildCount()) << 1;
			DeepWalker::visit(re);
			}, &re);
	}
	void visit(SubroutineReferenceExpression& sre) 
	{
		processNode([&]() {
			cout << string(" ") << "It is subr ref expression" << endl;
			h += hash<string>{}(typeid(sre).name()) << 1;
			h += hash<int>{}(sre.getChildCount()) << 1;
			DeepWalker::visit(sre);
			}, &sre);
	}
	void visit(StructAccessExpression& sae) 
	{
		processNode([&]() {
			cout << string(" ") << "It is struct access expression" << endl;
			h += hash<string>{}(typeid(sae).name()) << 1;
			h += hash<int>{}(sae.getChildCount()) << 1;
			DeepWalker::visit(sae);
			}, &sae);
	}
	void visit(EnumAccessExpression& eae) 
	{
		processNode([&]() {
			cout << string(" ") << "It is enum expression" << endl;
			h += hash<string>{}(typeid(eae).name()) << 1;
			h += hash<int>{}(eae.getChildCount()) << 1;
			DeepWalker::visit(eae);
			}, &eae);
	}
	void visit(TypeCastExpression& tce)
	{
		processNode([&]() {
			cout << string(" ") << "It is type cast expression" << endl;
			h += hash<string>{}(typeid(tce).name()) << 1;
			h += hash<int>{}(tce.getChildCount()) << 1;
			DeepWalker::visit(tce);
			}, &tce);
	}
	void visit(BasicCallExpression& bce) 
	{
		processNode([&]() {
			cout << string(" ") << "It is basic call expression" << endl;
			h += hash<string>{}(typeid(bce).name()) << 1;
			h += hash<int>{}(bce.getKind()) << 1;
			h += hash<int>{}(bce.getChildCount()) << 1;
			DeepWalker::visit(bce);
			}, &bce);
	}
	void visit(SubroutineCallExpression& sce) 
	{
		processNode([&]() {
			cout << string(" ") << "It is sbur call expression" << endl;
			h += hash<string>{}(typeid(sce).name()) << 1;
			h += hash<int>{}(sce.getArgumentCount()) << 1;
			DeepWalker::visit(sce);
	}, &sce);
	}
	void visit(EmptyExpression& ee)
	{
		processNode([&]() {
			cout << string(" ") << "It is empty expr" << endl;
			DeepWalker::visit(ee);
			}, &ee);
	}

	friend map<size_t, vector<HashDeepWalker::SubTreeInfo>> getBuckets(const HashDeepWalker& hdw)
	{
		map<size_t, vector<HashDeepWalker::SubTreeInfo>> buckets = map<size_t, vector<HashDeepWalker::SubTreeInfo>>();

		for (int i = 0; i < hdw.nodes.size(); i++)
		{
			if (hdw.nodes[i].subTreeSize > MassThreshold)
			{
				if (buckets.find(hdw.nodes[i].hashCode) == buckets.end())
				{
					buckets[hdw.nodes[i].hashCode] = vector<HashDeepWalker::SubTreeInfo>();
				}
				buckets[hdw.nodes[i].hashCode].push_back(hdw.nodes[i]);
			}
		}

		return buckets;
	}
};

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

bool isSimilar(HashDeepWalker::SubTreeInfo& t1, HashDeepWalker::SubTreeInfo& t2)
{
	double sharedNodes = (double)compareTree(t1.node, t2.node);
	double similarity = (2 * sharedNodes) / (2 * sharedNodes + (t1.subTreeSize - sharedNodes) + (t2.subTreeSize - sharedNodes));
	return similarity > SimilarityThreshold;
}


int main()
{
	Frontend::Frontend frontend;
	const OPS::Reprise::CompileResult& result = frontend.compileSingleFile("./tests/if_test.c");
	if (result.errorCount() > 0) { std::cout << result.errorText(); std::cout.flush(); }

	TranslationUnit& unit = frontend.getProgramUnit().getUnit(0);
	HashDeepWalker hdw;
	Service::DeepWalker dw;
	hdw.visit(unit);
	auto buckets = getBuckets(hdw);

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