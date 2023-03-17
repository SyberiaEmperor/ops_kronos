#include"SeqHashDeepWalker.h"

#include <iostream>
#include <map>

using namespace std;
using namespace OPS;
using namespace OPS::Reprise;


const size_t MassThreshold = 2;
const double SimilarityThreshold = 0.4;


int basicTreeCompare(RepriseBase* t1, RepriseBase* t2)
{
	int shared = 0;

	if (typeid(*t1) == typeid(*t2))
	{
		if (t1->is_a<ExpressionBase>() && t2->is_a <ExpressionBase>())
		{
			auto exp1 = t1->cast_ptr<ExpressionBase>();
			auto exp2 = t2->cast_ptr<ExpressionBase>();

			if (exp1->isEqual(*exp2))
				shared++;
		}
		else
			shared++;
	}

	int minChildren = min(t1->getChildCount(), t2->getChildCount());
	for (int i = 0; i < minChildren; i++)
		shared += basicTreeCompare(&t1->getChild(i), &t2->getChild(i));

	return shared;
}

bool isSimilar(HashDeepWalker::SubTreeInfo& t1, HashDeepWalker::SubTreeInfo& t2)
{
	double sharedNodes = (double)basicTreeCompare(t1.node, t2.node);
	double similarity = (2 * sharedNodes) / (2 * sharedNodes + (t1.subTreeSize - sharedNodes) + (t2.subTreeSize - sharedNodes));
	return similarity > SimilarityThreshold;
}


int main()
{
	Frontend::Frontend frontend;
	const OPS::Reprise::CompileResult& result = frontend.compileSingleFile("./tests/if_test2.c");
	if (result.errorCount() > 0) { std::cout << result.errorText(); std::cout.flush(); }

	TranslationUnit& unit = frontend.getProgramUnit().getUnit(0);
	SeqHashDeepWalker shdw;
	Service::DeepWalker dw;
	shdw.visit(unit);
	auto buckets = shdw.getBuckets(MassThreshold);

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