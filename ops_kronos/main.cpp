#include"SeqHashDeepWalker.h"

#include <iostream>
#include <map>

using namespace std;
using namespace OPS;
using namespace OPS::Reprise;


const size_t MassThreshold = 2;
const size_t MassSeqThreshold = 2;
const double SimilarityThreshold = 0.9;


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

//int basicSeqCompare(BlockStatement* b1, BlockStatement* b2)
//{
//	int shared = 0;
//
//	auto iter1 = b1->getFirst();
//	auto iter2 = b2->getFirst();
//
//	while (iter1.isValid() && iter2.isValid())
//	{
//		if (typeid(*iter1) == typeid(*iter2))
//		{
//			shared++;
//		}
//
//		iter1.goNext();
//		iter2.goNext();
//	}
//
//	return shared;
//}

bool isSeqSimilar(SeqHashDeepWalker::SeqNodeInfo& t1, SeqHashDeepWalker::SeqNodeInfo& t2)
{
	double sharedNodes = 0;

	for (int i = 0; i < t1.statements.size(); i++)
	{
		for (int j = 0; j < t2.statements.size(); j++)
		{
			if (t1.statements[i].hashCode == t2.statements[j].hashCode)
			{
				sharedNodes += basicTreeCompare(t1.statements[i].node, t2.statements[j].node);
				break;
			}

		}
	}

	double similarity = (2 * sharedNodes) / (2 * sharedNodes + (t1.statements.size() - sharedNodes) + (t2.statements.size() - sharedNodes));
	return similarity > SimilarityThreshold;
}

struct Clone { // research best representation of clone class
	vector<RepriseBase*> refs;
};

void cloneGeneralizing(vector<Clone>& clones)
{
	for (auto&clone : clones)
	{
		for (int i = 0; i < clone.refs.size(); i++)
		{
			for (int j = i; j < clone.refs.size(); j++)
			{
				auto parent1 = clone.refs[i]->getParent();
				auto parent2 = clone.refs[j]->getParent();

				//TODO
			}
		}
	}
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
	auto seqBuckets = shdw.getSeqBuckets(MassSeqThreshold);

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

	for (auto& bucket : seqBuckets)
	{
		for (int i = 0; i < bucket.second.size(); i++)
		{
			for (int j = i + 1; j < bucket.second.size(); j++)
			{
				if (isSeqSimilar(bucket.second[i], bucket.second[j]))
				{
					cout << "Seems like seq clone found" << endl;
					cout << bucket.first << endl;
				}
			}
		}
	}

	system("pause");
	return 0;
}