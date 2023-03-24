#include"HashDeepWalker.h"

void HashDeepWalker::visit(ProgramUnit& pu)
{
	processNode([&]() {
		cout << string(" ") << "It is programUnit" << endl;
		DeepWalker::visit(pu);
		}, &pu);
}
void HashDeepWalker::visit(TranslationUnit& tu)
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

void HashDeepWalker::visit(Declarations& dec)
{
	processNode([&]() {
		cout << string(" ") << "It is declarations" << endl;
		h += hash<int>{}(dec.getChildCount()) << 1;
		DeepWalker::visit(dec);
		}, &dec);
}

void HashDeepWalker::visit(VariableDeclaration& vd)
{
	processNode([&]() {
		cout << string(" ") << "It is variable declaration" << endl;
		h += hash<int>{}(vd.getKind()) << 1;
		//vd.getName()
		// vd.getType(); implement hasher for TypeBase
		DeepWalker::visit(vd);
		}, &vd);
}

void HashDeepWalker::visit(TypeDeclaration& td)
{
	processNode([&]() {
		cout << string(" ") << "It is type declaration" << endl;
		h += hash<int>{}(td.getKind()) << 1;
		td.getKind();
		DeepWalker::visit(td);
		}, &td);
}

void HashDeepWalker::visit(SubroutineDeclaration& sd)
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
void HashDeepWalker::visit(BlockStatement& bs) {
	processNode([&]() {
		cout << string(" ") << "It is block statement" << endl;
		h += hash<int>{}(bs.getChildCount()) << 1;
		DeepWalker::visit(bs);
		}, &bs);
}
void HashDeepWalker::visit(ForStatement& fs)
{
	processNode([&]() {
		cout << string(" ") << "It is FOR statement" << endl;
		h += hash<string>{}(typeid(fs).name()) << 1;
		DeepWalker::visit(fs);
		}, &fs);
}
void HashDeepWalker::visit(WhileStatement& ws)
{
	processNode([&]() {
		cout << string(" ") << "It is WHILE statement" << endl;
		h += hash<string>{}(typeid(ws).name()) << 1;
		DeepWalker::visit(ws);
		}, &ws);
}
void HashDeepWalker::visit(IfStatement& ifs)
{
	processNode([&]() {
		cout << string(" ") << "It is IF statement" << endl;
		h += hash<string>{}(typeid(ifs).name()) << 1;
		DeepWalker::visit(ifs);
		}, &ifs);
}
void HashDeepWalker::visit(PlainCaseLabel& pcl)
{
	processNode([&]() {
		cout << string(" ") << "It is plaint case label" << endl;
		h += hash<sqword>{}(pcl.getValue()) << 1;
		DeepWalker::visit(pcl);
		}, &pcl);
}
void HashDeepWalker::visit(PlainSwitchStatement& pss)
{
	processNode([&]() {
		cout << string(" ") << "It is plain switch statement" << endl;
		h += hash<string>{}(typeid(pss).name()) << 1;
		DeepWalker::visit(pss);
		}, &pss);
}
void HashDeepWalker::visit(GotoStatement& gs)
{
	processNode([&]() {
		cout << string(" ") << "It is GOTO statement" << endl;
		h += hash<string>{}(typeid(gs).name()) << 1;
		DeepWalker::visit(gs);
		}, &gs);
}
void HashDeepWalker::visit(ReturnStatement& rs)
{
	processNode([&]() {
		cout << string(" ") << "It is RETURN statement" << endl;
		h += hash<string>{}(typeid(rs).name()) << 1;
		DeepWalker::visit(rs);
		}, &rs);
}
void HashDeepWalker::visit(ExpressionStatement& es)
{
	processNode([&]() {
		cout << string(" ") << "It is expr statement" << endl;
		h += hash<int>{}(es.getChildCount()) << 1;
		DeepWalker::visit(es);
		}, &es);
}
void HashDeepWalker::visit(ASMStatement& as)
{
	processNode([&]() {
		cout << string(" ") << "It is ASM statement" << endl;
		h += hash<string>{}(as.getASMString()) << 1;
		DeepWalker::visit(as);
		}, &as);
}
void HashDeepWalker::visit(EmptyStatement& es)
{
	processNode([&]() {
		cout << string(" ") << "It is EMPTY statement" << endl;
		h += hash<string>{}(typeid(es).name()) << 1;
		DeepWalker::visit(es);
		}, &es);
}

//Types
void HashDeepWalker::visit(BasicType& bt)
{
	processNode([&]() {
		cout << string(" ") << "It is basic type" << endl;
		h += hash<int>{}(bt.getKind()) << 1;
		h += hash<int>{}(bt.getSizeOf()) << 1;
		DeepWalker::visit(bt);
		}, &bt);
}
void HashDeepWalker::visit(PtrType& pt)
{
	processNode([&]() {
		cout << string(" ") << "It is pointer" << endl;
		h += hash<string>{}(typeid(pt).name()) << 1;
		DeepWalker::visit(pt);
		}, &pt);
}
void HashDeepWalker::visit(TypedefType& tdt)
{
	processNode([&]() {
		cout << string(" ") << "It is typedef" << endl;
		h += hash<string>{}(typeid(tdt).name()) << 1;
		DeepWalker::visit(tdt);
		}, &tdt);
}
void HashDeepWalker::visit(ArrayType& at)
{
	processNode([&]() {
		cout << string(" ") << "It is array" << endl;
		h += hash<string>{}(typeid(at).name()) << 1;
		h += hash<int>{}(at.getElementCount()) << 1;
		DeepWalker::visit(at);
		}, &at);
}
void HashDeepWalker::visit(StructMemberDescriptor& structMember)
{
	processNode([&]() {
		cout << string(" ") << "It is struct member" << endl;
		h += hash<string>{}(typeid(structMember).name()) << 1;
		h += hash<int>{}(structMember.getBitsLimit()) << 1;
		//h += hash<string>{}(structMember.getName()) << 1;
		DeepWalker::visit(structMember);
		}, &structMember);
}
void HashDeepWalker::visit(StructType& st)
{
	processNode([&]() {
		cout << string(" ") << "It is struct type" << endl;
		h += hash<string>{}(typeid(st).name()) << 1;
		h += hash<int>{}(st.getMemberCount()) << 1;
		DeepWalker::visit(st);
		}, &st);
}
void HashDeepWalker::visit(EnumMemberDescriptor& emdt)
{
	processNode([&]() {
		cout << string(" ") << "It is enum member descriptor" << endl;
		h += hash<string>{}(typeid(emdt).name()) << 1;
		h += hash<int>{}(emdt.getValue()) << 1;
		DeepWalker::visit(emdt);
		}, &emdt);
}
void HashDeepWalker::visit(EnumType& et)
{
	processNode([&]() {
		cout << string(" ") << "It is enum type" << endl;
		h += hash<string>{}(typeid(et).name()) << 1;
		h += hash<int>{}(et.getMemberCount()) << 1;
		DeepWalker::visit(et);
		}, &et);
}
void HashDeepWalker::visit(ParameterDescriptor& pd)
{
	processNode([&]() {
		cout << string(" ") << "It is parameter desctiptor" << endl;
		h += hash<string>{}(typeid(pd).name()) << 1;
		h += hash<int>{}(pd.getTransitKind()) << 1;
		DeepWalker::visit(pd);
		}, &pd);
}
void HashDeepWalker::visit(SubroutineType& st)
{
	processNode([&]() {
		cout << string(" ") << "It is subr type" << endl;
		h += hash<string>{}(typeid(st).name()) << 1;
		h += hash<int>{}(st.getCallingKind()) << 1;
		h += hash<int>{}(st.getParameterCount()) << 1;
		DeepWalker::visit(st);
		}, &st);
}
void HashDeepWalker::visit(DeclaredType& dt)
{
	processNode([&]() {
		cout << string(" ") << "It is decl type" << endl;
		h += hash<string>{}(typeid(dt).name()) << 1;
		DeepWalker::visit(dt);
		}, &dt);
}
void HashDeepWalker::visit(VectorType& vt)
{
	processNode([&]() {
		cout << string(" ") << "It is decl type" << endl;
		h += hash<string>{}(typeid(vt).name()) << 1;
		h += hash<int>{}(vt.getElementCount()) << 1;
		DeepWalker::visit(vt);
		}, &vt);
}

//Expressions
void HashDeepWalker::visit(BasicLiteralExpression& ble)
{
	processNode([&]() {
		cout << string(" ") << "It is basic literal exlpression" << endl;
		h += hash<int>{}(ble.getLiteralType()) << 1;
		h += hash<int>{}(ble.getChildCount()) << 1;
		DeepWalker::visit(ble);
		}, &ble);
}
void HashDeepWalker::visit(StrictLiteralExpression& sle
)
{
	processNode([&]() {
		cout << string(" ") << "It is strict literal expression" << endl;
		h += hash<int>{}(sle.getLiteralType()) << 1;
		h += hash<int>{}(sle.getChildCount()) << 1;
		DeepWalker::visit(sle);
		}, &sle);
}
void HashDeepWalker::visit(CompoundLiteralExpression& cle)
{
	processNode([&]() {
		cout << string(" ") << "It is compound literal expression" << endl;
		h += hash<int>{}(cle.getChildCount()) << 1;
		DeepWalker::visit(cle);
		}, &cle);
}
void HashDeepWalker::visit(ReferenceExpression& re)
{
	processNode([&]() {
		cout << string(" ") << "It is ref expression" << endl;
		h += hash<string>{}(typeid(re).name()) << 1;
		h += hash<int>{}(re.getChildCount()) << 1;
		DeepWalker::visit(re);
		}, &re);
}
void HashDeepWalker::visit(SubroutineReferenceExpression& sre)
{
	processNode([&]() {
		cout << string(" ") << "It is subr ref expression" << endl;
		h += hash<string>{}(typeid(sre).name()) << 1;
		h += hash<int>{}(sre.getChildCount()) << 1;
		DeepWalker::visit(sre);
		}, &sre);
}
void HashDeepWalker::visit(StructAccessExpression& sae)
{
	processNode([&]() {
		cout << string(" ") << "It is struct access expression" << endl;
		h += hash<string>{}(typeid(sae).name()) << 1;
		h += hash<int>{}(sae.getChildCount()) << 1;
		DeepWalker::visit(sae);
		}, &sae);
}
void HashDeepWalker::visit(EnumAccessExpression& eae)
{
	processNode([&]() {
		cout << string(" ") << "It is enum expression" << endl;
		h += hash<string>{}(typeid(eae).name()) << 1;
		h += hash<int>{}(eae.getChildCount()) << 1;
		DeepWalker::visit(eae);
		}, &eae);
}
void HashDeepWalker::visit(TypeCastExpression& tce)
{
	processNode([&]() {
		cout << string(" ") << "It is type cast expression" << endl;
		h += hash<string>{}(typeid(tce).name()) << 1;
		h += hash<int>{}(tce.getChildCount()) << 1;
		DeepWalker::visit(tce);
		}, &tce);
}
void HashDeepWalker::visit(BasicCallExpression& bce)
{
	processNode([&]() {
		cout << string(" ") << "It is basic call expression" << endl;
		h += hash<string>{}(typeid(bce).name()) << 1;
		h += hash<int>{}(bce.getKind()) << 1;
		h += hash<int>{}(bce.getChildCount()) << 1;
		DeepWalker::visit(bce);
		}, &bce);
}
void HashDeepWalker::visit(SubroutineCallExpression& sce)
{
	processNode([&]() {
		cout << string(" ") << "It is sbur call expression" << endl;
		h += hash<string>{}(typeid(sce).name()) << 1;
		h += hash<int>{}(sce.getArgumentCount()) << 1;
		DeepWalker::visit(sce);
		}, &sce);
}
void HashDeepWalker::visit(EmptyExpression& ee)
{
	processNode([&]() {
		cout << string(" ") << "It is empty expr" << endl;
		DeepWalker::visit(ee);
		}, &ee);
}

map<size_t, vector<shared_ptr<HashDeepWalker::SubTreeInfo>>> HashDeepWalker::getBuckets(int MassThreshold)
{
	map<size_t, vector<shared_ptr<HashDeepWalker::SubTreeInfo>>> buckets = map<size_t, vector<shared_ptr<HashDeepWalker::SubTreeInfo>>>();

	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->subTreeSize > MassThreshold)
		{
			if (buckets.find(nodes[i]->hashCode) == buckets.end())
			{
				buckets[nodes[i]->hashCode] = vector< shared_ptr<HashDeepWalker::SubTreeInfo>>();
			}
			buckets[nodes[i]->hashCode].push_back(nodes[i]);
		}
	}

	return buckets;
}