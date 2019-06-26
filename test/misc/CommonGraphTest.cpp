#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <algorithm>
#include <spandex/misc/CommonGraph.h>

namespace spandex::misc::test
{
	TEST_CLASS(CommonGraph)
	{
	public:

		TEST_METHOD(AreConnected_1)
		{
			misc::CommonGraph<char> a(10);
			a.Insert(0, 0, '-');
			a.Insert(1, 0, '-');

			Assert::IsTrue(a.AreConnected(0, 0));
			Assert::IsTrue(a.AreConnected(1, 0));

			Assert::IsFalse(a.AreConnected(0, 1));
			Assert::IsFalse(a.AreConnected(1, 1));
			Assert::IsFalse(a.AreConnected(2, 0));
			Assert::IsFalse(a.AreConnected(2, 1));
		}

		TEST_METHOD(AreConnected_2)
		{
			misc::CommonGraph<char> a(10);

			a.Insert(0, 0, '-');
			a.Insert(0, 1, '-');
			a.Insert(1, 0, '-');

			Assert::AreEqual(3, a.size);

			a.Insert(1, 1, '-');
			Assert::AreEqual(4, a.size);

			Assert::IsTrue(a.AreConnected(0, 1));
			a.RemoveFrom(0, 1);

			Assert::IsFalse(a.AreConnected(0, 1));

			a.Insert(2, 0, '-');
			a.Insert(2, 1, '-');

			Assert::IsTrue(a.AreConnected(0, 0));
			Assert::IsTrue(a.AreConnected(1, 0));
			Assert::IsTrue(a.AreConnected(1, 1));
			Assert::IsTrue(a.AreConnected(2, 0));
			Assert::IsTrue(a.AreConnected(2, 1));

			Assert::IsFalse(a.AreConnected(0, 1));
		}

		TEST_METHOD(RemoveFrom_1)
		{
			misc::CommonGraph<char> a(10);

			a.Insert(0, 0, '-');
			a.Insert(0, 1, '-');
			a.Insert(1, 0, '-');
			a.Insert(1, 1, '-');
			a.Insert(2, 0, '-');
			a.Insert(2, 1, '-');

			a.RemoveFrom(0, 1);
			a.RemoveFrom(1, 1);
			a.RemoveFrom(2, 0);
			a.RemoveFrom(2, 1);

			misc::CommonGraph<char> e(10);
			e.Insert(0, 0, '-');
			e.Insert(1, 0, '-');

			a.Sort();
			e.Sort();

			Assert::IsTrue(a.Equals(e));
		}

		TEST_METHOD(RemoveFrom_2)
		{
			misc::CommonGraph<char> a(10);
			a.Insert(0, 0, '-');
			a.Insert(0, 1, '-');
			a.Insert(1, 0, '-');
			a.Insert(1, 1, '-');
			a.Insert(2, 0, '-');
			a.Insert(2, 1, '-');

			a.RemoveFrom(0, 1);
			a.RemoveFrom(1, 1);
			a.RemoveFrom(2, 0);
			a.RemoveFrom(2, 1);

			a.RemoveFrom(0, 0);
			a.Insert(0, 0, '-');

			misc::CommonGraph<char> e(10);
			e.Insert(0, 0, '-');
			e.Insert(1, 0, '-');

			a.Sort();
			e.Sort();

			Assert::IsTrue(a.Equals(e));
		}

		TEST_METHOD(RemoveFrom_3)
		{
			misc::CommonGraph<char> a(10);
			a.Insert(0, 0, '-');
			a.Insert(0, 1, '-');
			a.Insert(1, 0, '-');
			a.Insert(1, 1, '-');
			a.Insert(2, 0, '-');
			a.Insert(2, 1, '-');

			Assert::AreEqual(6, a.size);

			misc::CommonGraph<char> e(a);

			a.RemoveFrom(0, 0);
			a.RemoveFrom(0, 1);
			a.RemoveFrom(1, 0);
			a.RemoveFrom(1, 1);
			a.RemoveFrom(2, 0);
			a.RemoveFrom(2, 1);

			Assert::AreEqual(0, a.size);

			a.Insert(0, 0, '-');
			a.Insert(0, 1, '-');
			a.Insert(1, 0, '-');
			a.Insert(1, 1, '-');
			a.Insert(2, 0, '-');
			a.Insert(2, 1, '-');

			a.Sort();
			e.Sort();

			Assert::IsTrue(a.Equals(e));
		}

		TEST_METHOD(RemoveFrom_4)
		{
			misc::CommonGraph<char> a(10);
			a.Insert(0, 0, '-');
			a.Insert(0, 1, '-');
			a.Insert(1, 0, '-');
			a.Insert(1, 1, '-');
			a.Insert(2, 0, '-');
			a.Insert(2, 1, '-');

			Assert::AreEqual(6, a.size);

			misc::CommonGraph<char> e(a);

			a.RemoveFrom(0);
			a.RemoveFrom(1);
			a.RemoveFrom(2);

			Assert::AreEqual(0, a.size);

			a.Insert(0, 0, '-');
			a.Insert(0, 1, '-');
			a.Insert(1, 0, '-');
			a.Insert(1, 1, '-');
			a.Insert(2, 0, '-');
			a.Insert(2, 1, '-');

			a.Sort();
			e.Sort();

			Assert::IsTrue(a.Equals(e));
		}

		TEST_METHOD(Sort_1)
		{
			misc::CommonGraph<char> a(10);
			a.Insert(0, 0, '-');
			a.Insert(0, 1, '-');
			a.Insert(1, 0, '-');
			a.Insert(1, 1, '-');
			a.Insert(2, 0, '-');
			a.Insert(2, 1, '-');

			misc::CommonGraph<char> b = a;
			Assert::IsTrue(a.Equals(b));

			a.Sort();
			b.Sort();
			Assert::IsTrue(a.Equals(b));

			a.Sort();
			Assert::IsTrue(a.Equals(b));
		}

		TEST_METHOD(Sort_2)
		{
			misc::CommonGraph<char> a(10);
			a.Insert(0, 0, '-');
			a.Insert(0, 1, '-');
			a.Insert(0, 2, '-');

			misc::CommonGraph<char> b = a;
			Assert::IsTrue(a.Equals(b));

			a.RemoveFrom(0, 0);
			a.RemoveFrom(0, 1);
			a.RemoveFrom(0, 2);

			a.Insert(0, 2, '-');
			a.Insert(0, 1, '-');
			a.Insert(0, 0, '-');

			a.Sort();
			b.Sort();
			Assert::IsTrue(a.Equals(b));

			a.RemoveFrom(0, 0);
			a.RemoveFrom(0, 1);
			a.RemoveFrom(0, 2);

			a.Insert(0, 0, '-');
			a.Insert(0, 1, '-');
			a.Insert(0, 2, '-');

			a.Sort();
			b.Sort();
			Assert::IsTrue(a.Equals(b));
		}

		TEST_METHOD(GetIterator_1)
		{
			misc::CommonGraph<char> a(10);
			a.Insert(0, 0, '-');
			a.Insert(0, 1, '-');
			a.Insert(0, 3, '-');
			a.Insert(2, 1, '-');
			a.Insert(2, 2, '-');
			a.Insert(2, 4, '-');

			a.Sort();

			std::vector<int> items;
			auto it = a.GetIterator();

			it.Setup(0);
			items.resize(0);
			while (it.HasNext())
			{
				items.push_back(it.Next().first);
			}
			Assert::AreEqual(3, (int)items.size());
			Assert::IsTrue(std::find(items.begin(), items.end(), 0) != items.end());
			Assert::IsTrue(std::find(items.begin(), items.end(), 1) != items.end());
			Assert::IsTrue(std::find(items.begin(), items.end(), 3) != items.end());

			it.Setup(1);
			items.resize(0);
			while (it.HasNext())
			{
				items.push_back(it.Next().first);
			}
			Assert::AreEqual(0, (int)items.size());

			it.Setup(2);
			items.resize(0);
			while (it.HasNext())
			{
				items.push_back(it.Next().first);
			}
			Assert::AreEqual(3, (int)items.size());
			Assert::IsTrue(std::find(items.begin(), items.end(), 1) != items.end());
			Assert::IsTrue(std::find(items.begin(), items.end(), 2) != items.end());
			Assert::IsTrue(std::find(items.begin(), items.end(), 4) != items.end());
		}
	};
}