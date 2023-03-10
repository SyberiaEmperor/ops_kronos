#pragma once

#include "Reprise/Reprise.h"
#include "Frontend/Frontend.h"
#include <OPS_Core/IO.h>
#include <Reprise/Service/DeepWalker.h>

#include<iostream>

using namespace std;
using namespace OPS;
using namespace OPS::Reprise;

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
	size_t h = 0;
	int size = 0;

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
	void visit(ProgramUnit&);
	void visit(TranslationUnit&);

	//void visit(ProgramFragment&);

	void visit(Declarations&);
	void visit(VariableDeclaration&);
	void visit(TypeDeclaration&);
	void visit(SubroutineDeclaration&);

	void visit(BlockStatement&);
	void visit(ForStatement&);
	void visit(WhileStatement&);
	void visit(IfStatement&);
	void visit(PlainCaseLabel&);
	void visit(PlainSwitchStatement&);
	void visit(GotoStatement&);
	void visit(ReturnStatement&);
	void visit(ExpressionStatement&);
	void visit(ASMStatement&);
	void visit(EmptyStatement&);

	void visit(BasicType&);
	void visit(PtrType&);
	void visit(TypedefType&);
	void visit(ArrayType&);
	void visit(StructMemberDescriptor& structMember);
	void visit(StructType&);
	void visit(EnumMemberDescriptor&);
	void visit(EnumType&);
	void visit(ParameterDescriptor&);
	void visit(SubroutineType&);
	void visit(DeclaredType&);
	void visit(VectorType&);

	void visit(BasicLiteralExpression&);
	void visit(StrictLiteralExpression&);
	void visit(CompoundLiteralExpression&);
	void visit(ReferenceExpression&);
	void visit(SubroutineReferenceExpression&);
	void visit(StructAccessExpression&);
	void visit(EnumAccessExpression&);
	void visit(TypeCastExpression&);
	void visit(BasicCallExpression&);
	void visit(SubroutineCallExpression&);
	void visit(EmptyExpression&);

	map<size_t, vector<HashDeepWalker::SubTreeInfo>> getBuckets(int MassThreshold);
};