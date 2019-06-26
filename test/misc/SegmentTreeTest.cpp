#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <algorithm>
#include <limits>
#include <vector>

#include <spandex/misc/SegmentTree.h>

namespace spandex::misc::test
{
	TEST_CLASS(SegmentTree)
	{
	public:

		TEST_METHOD(Range_1)
		{
			misc::SegmentTree<int> u(std::numeric_limits<int>::min(),
				[](const int& x, const int& y) { return std::max(x, y); },
				std::vector<int> { 1 });

			Assert::AreEqual(1, u.GetTop());
			Assert::AreEqual(1, u.Range(0, 0));
			Assert::AreEqual(1, u.Get(0));
		}

		TEST_METHOD(Range_2)
		{
			misc::SegmentTree<int> u(std::numeric_limits<int>::min(),
				[](const int& x, const int& y) { return std::max(x, y); },
				{ 3, 8, 6, 4, 2, 5, 9, 0, 7, 1 });

			Assert::AreEqual(9, u.GetTop());
			Assert::AreEqual(9, u.Range(0, 9));
			Assert::AreEqual(9, u.Range(5, 9));
			Assert::AreEqual(8, u.Range(0, 4));
			Assert::AreEqual(8, u.Range(1, 1));
			Assert::AreEqual(8, u.Range(1, 4));
			Assert::AreEqual(7, u.Range(7, 8));
			Assert::AreEqual(5, u.Range(4, 5));
			Assert::AreEqual(6, u.Range(2, 5));
		}

		TEST_METHOD(Range_3)
		{
			misc::SegmentTree<int> u(std::numeric_limits<int>::min(),
				[](const int& x, const int& y) { return std::max(x, y); },
				{ 5, 4, 3, 2, 1 });

			Assert::AreEqual(5, u.GetTop());
			Assert::AreEqual(3, u.Range(2, 4));
			Assert::AreEqual(5, u.Get(0));
			Assert::AreEqual(4, u.Get(1));
			Assert::AreEqual(3, u.Get(2));
		}

		TEST_METHOD(Set_1)
		{
			std::vector<int> values = { 3, 8, 6, 4, 2, 5, 9, 0, 7, 1 };
			misc::SegmentTree<int> u(std::numeric_limits<int>::min(),
				[](const int& x, const int& y) { return std::max(x, y); },
				values);

			misc::SegmentTree<int> v(std::numeric_limits<int>::min(),
				[](const int& x, const int& y) { return std::max(x, y); },
				10);

			Assert::IsFalse(u.Equals(v));

			for (int i = 0; i < 10; i++)
			{
				v.Set(i, values[i]);
			}

			Assert::IsTrue(u.Equals(v));
		}

		TEST_METHOD(Set_2)
		{
			std::vector<int> values = { 3, 8, 6, 4, 2, 5, 9, 0, 7, 1 };
			misc::SegmentTree<int> u(std::numeric_limits<int>::min(),
				[](const int& x, const int& y) { return std::max(x, y); },
				values);

			u.Set(6, 0);

			Assert::AreEqual(8, u.GetTop());
			Assert::AreEqual(8, u.Range(0, 9));
			Assert::AreEqual(7, u.Range(5, 9));
			Assert::AreEqual(8, u.Range(0, 4));
			Assert::AreEqual(8, u.Get(1));
			Assert::AreEqual(8, u.Range(1, 4));
			Assert::AreEqual(7, u.Range(7, 8));
			Assert::AreEqual(5, u.Range(4, 5));
			Assert::AreEqual(6, u.Range(2, 5));
		}
	};
}
