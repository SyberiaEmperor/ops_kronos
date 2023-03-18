#include"SeqHashDeepWalker.h"

void SeqHashDeepWalker::visit(BlockStatement& bs)
{
	int curSize = getSize();
	HashDeepWalker::visit(bs);

	SeqNodeInfo sni(&bs);

	for (int i = curSize; i < nodes.size(); i++)
	{
		if (nodes[i].node->getParent() == &bs)
		{
			sni.statements.push_back(nodes[i]);
		}
	}

	sni.CalcHash();
	seqNodes.push_back(sni);
}

map<size_t, vector<SeqHashDeepWalker::SeqNodeInfo>> SeqHashDeepWalker::getSeqBuckets(int MassThreshold)
{
	map<size_t, vector<SeqHashDeepWalker::SeqNodeInfo>> buckets = map<size_t, vector<SeqHashDeepWalker::SeqNodeInfo>>();

	for (int i = 0; i<seqNodes.size(); i++)
	{
		if (seqNodes[i].statements.size() > MassThreshold)
		{
			if (buckets.find(seqNodes[i].hashCode) == buckets.end())
			{
				buckets[seqNodes[i].hashCode] = vector<SeqHashDeepWalker::SeqNodeInfo>();
			}
			buckets[seqNodes[i].hashCode].push_back(seqNodes[i]);
		}
	}

	return buckets;
}