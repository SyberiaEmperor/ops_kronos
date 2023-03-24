#pragma once
#include"HashDeepWalker.h"

class SeqHashDeepWalker : public HashDeepWalker
{
public:
	using HashDeepWalker::visit;
	void visit(BlockStatement&);
};