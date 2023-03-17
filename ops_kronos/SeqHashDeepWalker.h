#pragma once
#include"HashDeepWalker.h"

class SeqHashDeepWalker : public HashDeepWalker
{

public:

	struct SeqNodeInfo {
		BlockStatement* bs;
		size_t h;
		vector<HashDeepWalker::SubTreeInfo> statements;

		SeqNodeInfo(BlockStatement* b) :bs(b), h(0),statements() {}
		void CalcHash() {
			h = 0;
			for (auto& st : statements)
			{
				h += st.hashCode;
			}
		}
	};


protected:
	vector<SeqNodeInfo> seqNodes;

public:
	SeqHashDeepWalker() : seqNodes() {}

	using HashDeepWalker::visit;
	void visit(BlockStatement&);
};