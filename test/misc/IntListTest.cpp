#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <algorithm>
#include <spandex/misc/IntList.h>

namespace spandex::misc::test
{
	TEST_CLASS(IntList)
	{
	public:

		TEST_METHOD(Push_1)
		{
			misc::IntList ii(10);
			ii.Push(1);
			ii.Push(7);
			ii.Push(3);
			ii.Push(2);

			Assert::AreEqual(10, ii.capacity);
			Assert::AreEqual(4, ii.size);
		}

		TEST_METHOD(Contains_1)
		{
			misc::IntList ii(10);
			ii.Push(1);
			ii.Push(7);
			ii.Push(3);
			ii.Push(2);

			Assert::IsTrue(ii.Contains(1));
			Assert::IsTrue(ii.Contains(2));
			Assert::IsTrue(ii.Contains(3));
			Assert::IsTrue(ii.Contains(7));

			Assert::IsFalse(ii.Contains(0));
			Assert::IsFalse(ii.Contains(4));
		}

		TEST_METHOD(Pop_1)
		{
			misc::IntList ii(10);
			ii.Push(1);
			ii.Push(7);
			ii.Push(3);
			ii.Push(2);

			Assert::AreEqual(2, ii.Pop());
			Assert::AreEqual(3, ii.Pop());
			Assert::AreEqual(7, ii.Pop());
			Assert::AreEqual(1, ii.Pop());
		}

		TEST_METHOD(PopAll_1)
		{
			misc::IntList ii(10);
			ii.Push(1);
			ii.Push(7);
			ii.Push(3);
			ii.Push(2);

			auto values = ii.PopAll();

			Assert::AreEqual(0, ii.size);

			Assert::AreEqual(2, values[0]);
			Assert::AreEqual(3, values[1]);
			Assert::AreEqual(7, values[2]);
			Assert::AreEqual(1, values[3]);
		}
	};
}