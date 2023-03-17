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