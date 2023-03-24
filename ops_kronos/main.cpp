#include"SeqHashDeepWalker.h"

#include <iostream>
#include <map>
#include <set>

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

bool isSeqSimilar(shared_ptr < HashDeepWalker::SubTreeInfo> t1, shared_ptr < HashDeepWalker::SubTreeInfo> t2)
{
	double sharedNodes = 0;

	for (int i = 0; i < t1->children.size(); i++)
	{
		for (int j = 0; j < t2->children.size(); j++)
		{
			if (t1->children[i]->hashCode == t2->children[j]->hashCode)
			{
				sharedNodes += basicTreeCompare(t1->children[i]->node, t2->children[j]->node);
				break;
			}

		}
	}

	double similarity = (2 * sharedNodes) / (2 * sharedNodes + (t1->children.size() - sharedNodes) + (t2->children.size() - sharedNodes));
	return similarity > SimilarityThreshold;
}

bool isSimilar(shared_ptr <HashDeepWalker::SubTreeInfo> t1, shared_ptr <HashDeepWalker::SubTreeInfo> t2)
{
	if (t1->node->is_a<BlockStatement>() && t2->node->is_a<BlockStatement>())
	{
		return isSeqSimilar(t1, t2);
	}
	double sharedNodes = (double)basicTreeCompare(t1->node, t2->node);
	double similarity = (2 * sharedNodes) / (2 * sharedNodes + (t1->subTreeSize - sharedNodes) + (t2->subTreeSize - sharedNodes));
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



struct Clone { // research best representation of clone class
	int size;
	set<shared_ptr<HashDeepWalker::SubTreeInfo>> refs;
	Clone() {}
	Clone(int s) :size(s), refs() {}
};

map<size_t, Clone> clones = map<size_t, Clone>();

void cloneGeneralizing()
{
	for (auto&clone : clones)
	{
		for (auto& i : clone.second.refs)
		{
			for (auto& j : clone.second.refs)
			{
				if (i != j)
				{
					if (i->parent && j->parent)
					{
						if (isSimilar(i->parent, j->parent))
						{
							if (clones.find(i->hashCode) == clones.end())
							{
								clones[i->hashCode] = Clone(i->subTreeSize);
							}
							clones[i->hashCode].refs.insert(i);
							clones[i->hashCode].refs.insert(j);
							clone.second.refs.erase(i);
							clone.second.refs.erase(j);
						}
					}
				}
			}
		}
	}
}

void deepTraverse(shared_ptr < HashDeepWalker::SubTreeInfo> n)
{
	if (clones.find(n->hashCode) != clones.end())
	{
		clones[n->hashCode].refs.erase(n);
		if (clones[n->hashCode].refs.size() == 1)
			clones[n->hashCode].refs.clear();
	}
}

void eraseSubClones(shared_ptr < HashDeepWalker::SubTreeInfo> root)
{
	for (int i = 0; i < root->children.size(); i++)
		deepTraverse(root->children[i]);
}

void addClonePair(shared_ptr < HashDeepWalker::SubTreeInfo> s1, shared_ptr < HashDeepWalker::SubTreeInfo> s2)
{
	if (clones.find(s1->hashCode) == clones.end())
	{
		clones[s1->hashCode] = Clone(s1->subTreeSize);
	}
	eraseSubClones(s1);
	eraseSubClones(s2);
	clones[s1->hashCode].refs.insert(s1);
	clones[s1->hashCode].refs.insert(s2);
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
					addClonePair(bucket.second[i], bucket.second[j]);
				}
			}
		}
	}

	system("pause");
	return 0;
}