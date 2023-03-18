#pragma once
#include"HashDeepWalker.h"

class SeqHashDeepWalker : public HashDeepWalker
{

public:

	struct SeqNodeInfo {
		BlockStatement* bs;
		size_t hashCode;
		vector<HashDeepWalker::SubTreeInfo> statements;

		SeqNodeInfo(BlockStatement* b) :bs(b), hashCode(0),statements() {}
		void CalcHash() {
			hashCode = 0;
			for (auto& st : statements)
			{
				hashCode += st.hashCode;
			}
		}
		int CalcTotalSize() {
			int size = 0;
			for (auto& st : statements) {
				size += st.subTreeSize;
			}
			return size + 1;
		}
	};


protected:
	vector<SeqNodeInfo> seqNodes;

public:
	SeqHashDeepWalker() : seqNodes() {}

	using HashDeepWalker::visit;
	void visit(BlockStatement&);

	map<size_t, vector<SeqHashDeepWalker::SeqNodeInfo>> getSeqBuckets(int MassThreshold);
};