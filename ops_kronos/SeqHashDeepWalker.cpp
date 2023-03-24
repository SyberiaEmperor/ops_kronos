#include"SeqHashDeepWalker.h"

void SeqHashDeepWalker::visit(BlockStatement& bs)
{
	processNode([&] {
		int curSize = getSize();
		DeepWalker::visit(bs);
		//h = 0;
		//for (int i = curSize; i < nodes.size(); i++)
		//{
		//	if (nodes[i]->node->getParent() == &bs)
		//	{
		//		h += 1;
		//	}
		//}
		cout << "Block stmt" << endl;
		}, &bs);
}
