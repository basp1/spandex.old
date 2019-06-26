#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <algorithm>
#include <spandex/misc/PriorityQueue.h>

namespace spandex::misc::test
{
	TEST_CLASS(PriorityQueue)
	{
	public:

		TEST_METHOD(Push_1)
		{
			misc::PriorityQueue<int> pq([](const int& x, const int& y) { return std::min(x, y); });

			pq.Push(3);
			Assert::AreEqual(3, pq.GetTop());
			Assert::AreEqual(1, pq.GetHeight());

			pq.Push(4);
			Assert::AreEqual(3, pq.GetTop());
			Assert::AreEqual(2, pq.GetHeight());

			pq.Push(5);
			Assert::AreEqual(3, pq.GetTop());
			Assert::AreEqual(2, pq.GetHeight());

			pq.Push(2);
			Assert::AreEqual(2, pq.GetTop());
			Assert::AreEqual(3, pq.GetHeight());

			pq.Push(1);
			Assert::AreEqual(1, pq.GetTop());
			Assert::AreEqual(3, pq.GetHeight());
		}

		TEST_METHOD(Push_2)
		{
			misc::PriorityQueue<int> pq([](const int& x, const int& y) { return std::min(x, y); });

			for (int i = 8; i >= 0; i--)
			{
				pq.Push(i);
			}

			Assert::AreEqual(0, pq.GetTop());
			Assert::AreEqual(4, pq.GetHeight());

			pq.Push(9);
			Assert::AreEqual(0, pq.GetTop());
			Assert::AreEqual(4, pq.GetHeight());
		}

		TEST_METHOD(Push_3)
		{
			misc::PriorityQueue<int> pq([](const int& x, const int& y) { return std::min(x, y); }, 5);
			for (int i = 8; i >= 0; i--)
			{
				pq.Push(i);
			}

			Assert::AreEqual(0, pq.GetTop());
			Assert::AreEqual(4, pq.GetHeight());

			pq.Push(9);
			Assert::AreEqual(0, pq.GetTop());
			Assert::AreEqual(4, pq.GetHeight());
		}

		TEST_METHOD(Pop_1)
		{
			misc::PriorityQueue<int> pq([](const int& x, const int& y) { return std::min(x, y); });

			pq.Push(18);
			pq.Push(19);
			pq.Push(20);
			Assert::AreEqual(18, pq.GetTop());

			pq.Pop();
			Assert::AreEqual(19, pq.GetTop());

			pq.Pop();
			Assert::AreEqual(20, pq.GetTop());

			pq.Pop();
			Assert::AreEqual(0, pq.size);
		}

		TEST_METHOD(Pop_2)
		{
			misc::PriorityQueue<int> pq([](const int& x, const int& y) { return std::min(x, y); });
			for (int i = 8; i >= 0; i--)
			{
				pq.Push(i);
			}

			Assert::AreEqual(0, pq.GetTop());
			Assert::AreEqual(4, pq.GetHeight());

			pq.Pop();
			Assert::AreEqual(1, pq.GetTop());
			Assert::AreEqual(4, pq.GetHeight());

			pq.Pop();
			Assert::AreEqual(2, pq.GetTop());
			Assert::AreEqual(3, pq.GetHeight());

			pq.Pop();
			Assert::AreEqual(3, pq.GetTop());
			Assert::AreEqual(3, pq.GetHeight());

			pq.Pop();
			Assert::AreEqual(4, pq.GetTop());
			Assert::AreEqual(3, pq.GetHeight());
		}

		TEST_METHOD(Pop_3)
		{
			const int N = 20;
			misc::PriorityQueue<int> pq([](const int& x, const int& y) { return std::min(x, y); });

			for (int i = N; i > 0; i--)
			{
				pq.Push(i);
			}

			Assert::AreEqual(1, pq.GetTop());

			for (int i = 0; i < (int)(N / 2); i++)
			{
				pq.Pop();
			}
			Assert::AreEqual(1 + N / 2, pq.GetTop());

			for (int i = 0; i < (int)((N / 2) - 1); i++)
			{
				pq.Pop();
			}

			Assert::AreEqual(N, pq.GetTop());
		}
	};
}
