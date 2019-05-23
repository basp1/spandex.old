#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <rope/CommonGraph.h>
#include <spandex/SparseMatrix.h>
#include <spandex/Permutation.h>

#include <algorithm>
#include <set>
#include <vector>

namespace spandex::test
{
	TEST_CLASS(Permutation)
	{
	public:

		TEST_METHOD(AMD_1)
		{
			rope::CommonGraph<double> g(3);
			g.Insert(0, 0, 6);
			g.Insert(0, 1, -4);
			g.Insert(0, 2, 7);
			g.Insert(1, 0, 4);
			g.Insert(1, 1, -3);
			g.Insert(1, 2, 0);
			g.Insert(2, 0, 1);
			g.Insert(2, 1, 2);
			g.Insert(2, 2, 5);
			auto a = spandex::SparseMatrix<double>::FromGraph(3, 3, g);

			auto pt = spandex::Permutation::Build(a, spandex::Permutation::Type::AMD);

			std::set<int> primary;
			std::set<int> permuted;
			for (int i = 0; i < 3; i++)
			{
				primary.insert(pt.GetPrimary(i));
				permuted.insert(pt.GetPermuted(i));
			}

			Assert::AreEqual(3, (int)primary.size());
			Assert::AreEqual(3, (int)permuted.size());
		}

		TEST_METHOD(AMD_2)
		{
			rope::CommonGraph<double> g(5);
			g.Insert(0, 0, 0.44561);
			g.Insert(0, 1, 0.92962);
			g.Insert(3, 1, 0.87043);
			g.Insert(4, 1, 0.47918);
			g.Insert(0, 2, 0.10582);
			g.Insert(3, 2, 0.060281);
			g.Insert(0, 3, 0.87256);
			g.Insert(1, 3, 0.41421);
			g.Insert(4, 3, 0.42161);
			g.Insert(2, 4, 0.58921);
			g.Insert(4, 4, 0.15305);
			g.Insert(3, 5, 0.63730);
			auto a = spandex::SparseMatrix<double>::FromGraph(5, 6, g);

			auto pt = spandex::Permutation::Build(a, spandex::Permutation::Type::AMD);

			std::vector<int> primary;
			std::vector<int> permuted;
			for (int i = 0; i < 6; i++)
			{
				primary.push_back(pt.GetPrimary(i));
				permuted.push_back(pt.GetPermuted(i));
			}

			Assert::IsTrue(4 == permuted[0] || 5 == permuted[0]);
			Assert::IsTrue(4 == permuted[1] || 5 == permuted[1]);

			std::sort(permuted.begin(), permuted.end());
			int uniques = (int)(std::unique(permuted.begin(), permuted.end()) - permuted.begin());
			Assert::AreEqual(6, uniques);

			std::sort(primary.begin(), primary.end());
			uniques = (int)(std::unique(primary.begin(), primary.end()) - primary.begin());
			Assert::AreEqual(6, uniques);
		}
	};
}