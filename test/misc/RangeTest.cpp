#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <algorithm>
#include <vector>

#include <spandex/misc/Range.h>

namespace spandex::misc::test
{
	TEST_CLASS(Range)
	{
	public:

		TEST_METHOD(Equals_1)
		{
			misc::Range f(0, 5);
			misc::Range g(5);

			Assert::IsTrue(f == g);
		}

		TEST_METHOD(Equals_2)
		{
			misc::Range f(0, 5);
			misc::Range g(2, 5);

			Assert::IsFalse(f == g);

			auto i = f.begin();
			auto j = g.begin();

			Assert::IsFalse(i == j);

			++i; ++i;

			Assert::IsFalse(f == g);
			Assert::IsTrue(i == j);
		}

		TEST_METHOD(Loop_1)
		{
			misc::Range g(0, 4);

			std::vector<int> vals;

			for (auto i = g.begin(); i != g.end(); ++i)
			{
				vals.push_back(*i);
			}

			Assert::AreEqual(4, (int)vals.size());
			Assert::AreEqual(0, vals[0]);
			Assert::AreEqual(1, vals[1]);
			Assert::AreEqual(2, vals[2]);
			Assert::AreEqual(3, vals[3]);
		}

		TEST_METHOD(Loop_2)
		{
			misc::Range g(2, 5);

			std::vector<int> vals;

			for (auto i = g.begin(); i != g.end(); ++i)
			{
				vals.push_back(*i);
			}

			Assert::AreEqual(3, (int)vals.size());
			Assert::AreEqual(2, vals[0]);
			Assert::AreEqual(3, vals[1]);
			Assert::AreEqual(4, vals[2]);
		}
	};
}