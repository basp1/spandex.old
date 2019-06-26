#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <algorithm>
#include <spandex/misc/DirectedGraph.h>

namespace spandex::misc::test
{
	TEST_CLASS(DirectedGraph)
	{
	public:

		TEST_METHOD(AreConnected)
		{
			misc::DirectedGraph g(10);
			g.Insert(0, 1);
			g.Insert(1, 2);
			g.Insert(1, 3);
			g.Insert(3, 5);
			g.Insert(4, 5);
			g.Insert(5, 6);
			g.Insert(6, 3);

			Assert::IsTrue(g.AreConnected(0, 1));
			Assert::IsTrue(g.AreConnected(0, 2));
			Assert::IsTrue(g.AreConnected(0, 3));
			Assert::IsTrue(g.AreConnected(0, 5));
			Assert::IsTrue(g.AreConnected(0, 6));

			Assert::IsTrue(g.AreConnected(6, 3));
			Assert::IsTrue(g.AreConnected(6, 5));
			Assert::IsTrue(g.AreConnected(4, 3));

			Assert::IsFalse(g.AreConnected(0, 4));
			Assert::IsFalse(g.AreConnected(2, 0));
			Assert::IsFalse(g.AreConnected(2, 1));

			Assert::IsFalse(g.AreConnected(5, 0));
			Assert::IsFalse(g.AreConnected(4, 2));
		}

		TEST_METHOD(FindAllPaths_1)
		{
			misc::DirectedGraph g(10);
			g.Insert(0, 1);
			g.Insert(0, 2);
			g.Insert(1, 3);
			g.Insert(2, 3);

			auto paths = g.FindAllPaths(0, 3);
			Assert::AreEqual(2, (int)paths.size());

			auto path = paths.front();
			Assert::AreEqual(3, (int)path.size());
			Assert::AreEqual(0, path[0]);
			Assert::AreEqual(1, path[1]);
			Assert::AreEqual(3, path[2]);
			paths.pop_front();

			path = paths.front();
			Assert::AreEqual(3, (int)path.size());
			Assert::AreEqual(0, path[0]);
			Assert::AreEqual(2, path[1]);
			Assert::AreEqual(3, path[2]);
		}

		TEST_METHOD(FindAllPaths_2)
		{
			misc::DirectedGraph g(10);
			g.Insert(0, 1);
			g.Insert(0, 2);
			g.Insert(1, 3);
			g.Insert(2, 3);
			g.Insert(2, 4);
			g.Insert(4, 3);

			auto paths = g.FindAllPaths(0, 3);
			Assert::AreEqual(3, (int)paths.size());

			auto path = paths.front();
			Assert::AreEqual(3, (int)path.size());
			Assert::AreEqual(0, path[0]);
			Assert::AreEqual(1, path[1]);
			Assert::AreEqual(3, path[2]);
			paths.pop_front();

			path = paths.front();
			Assert::AreEqual(3, (int)path.size());
			Assert::AreEqual(0, path[0]);
			Assert::AreEqual(2, path[1]);
			Assert::AreEqual(3, path[2]);
			paths.pop_front();

			path = paths.front();
			Assert::AreEqual(4, (int)path.size());
			Assert::AreEqual(0, path[0]);
			Assert::AreEqual(2, path[1]);
			Assert::AreEqual(4, path[2]);
			Assert::AreEqual(3, path[3]);
		}

		TEST_METHOD(FindAllPaths_3)
		{
			misc::DirectedGraph g(10);
			g.Insert(0, 1);
			g.Insert(1, 2);
			g.Insert(0, 2);

			auto paths = g.FindAllPaths(0, 2);
			Assert::AreEqual(2, (int)paths.size());

			auto path = paths.front();
			Assert::AreEqual(3, (int)path.size());
			Assert::AreEqual(0, path[0]);
			Assert::AreEqual(1, path[1]);
			Assert::AreEqual(2, path[2]);
			paths.pop_front();

			path = paths.front();
			Assert::AreEqual(2, (int)path.size());
			Assert::AreEqual(0, path[0]);
			Assert::AreEqual(2, path[1]);
		}

		TEST_METHOD(FindAllPaths_4)
		{
			misc::DirectedGraph g(10);
			g.Insert(0, 3);
			g.Insert(3, 0);
			g.Insert(3, 6);
			g.Insert(6, 3);
			g.Insert(0, 4);
			g.Insert(4, 0);
			g.Insert(4, 7);
			g.Insert(7, 4);
			g.Insert(4, 5);
			g.Insert(5, 4);
			g.Insert(5, 7);
			g.Insert(7, 5);
			g.Insert(0, 2);
			g.Insert(2, 0);
			g.Insert(0, 1);
			g.Insert(1, 0);
			g.Insert(1, 2);
			g.Insert(2, 1);
			g.Insert(1, 5);
			g.Insert(5, 1);
			g.Insert(2, 5);
			g.Insert(5, 2);

			auto paths = g.FindAllPaths(0, 7);

			Assert::AreEqual(10, (int)paths.size());
		}

		TEST_METHOD(FindAllPaths_5)
		{
			misc::DirectedGraph g(10);
			g.Insert(0, 1);
			g.Insert(1, 0);
			g.Insert(0, 2);
			g.Insert(2, 0);
			g.Insert(2, 3);
			g.Insert(3, 2);
			g.Insert(3, 4);
			g.Insert(4, 3);
			g.Insert(2, 5);
			g.Insert(5, 2);
			g.Insert(1, 5);
			g.Insert(5, 1);
			g.Insert(5, 4);
			g.Insert(4, 5);
			g.Insert(3, 6);
			g.Insert(6, 3);
			g.Insert(4, 6);
			g.Insert(6, 4);

			auto paths = g.FindAllPaths(1, 5);

			Assert::AreEqual(4, (int)paths.size());
		}

		TEST_METHOD(FindAllPaths_6)
		{
			misc::DirectedGraph g(10);
			g.Insert(0, 1);
			g.Insert(1, 0);
			g.Insert(0, 2);
			g.Insert(2, 0);
			g.Insert(2, 3);
			g.Insert(3, 2);
			g.Insert(3, 4);
			g.Insert(4, 3);
			g.Insert(2, 5);
			g.Insert(5, 2);
			g.Insert(1, 5);
			g.Insert(5, 1);
			g.Insert(5, 4);
			g.Insert(4, 5);
			g.Insert(3, 6);
			g.Insert(6, 3);
			g.Insert(4, 6);
			g.Insert(6, 4);

			auto paths = g.FindAllPaths(0, 6);

			Assert::AreEqual(8, (int)paths.size());
		}

		TEST_METHOD(FindAllPaths_7)
		{
			misc::DirectedGraph g(10);
			g.Insert(0, 1);
			g.Insert(1, 0);
			g.Insert(1, 2);
			g.Insert(2, 1);
			g.Insert(1, 3);
			g.Insert(3, 1);
			g.Insert(3, 4);
			g.Insert(4, 3);
			g.Insert(3, 6);
			g.Insert(6, 3);
			g.Insert(4, 8);
			g.Insert(8, 4);
			g.Insert(8, 5);
			g.Insert(5, 8);
			g.Insert(6, 7);
			g.Insert(7, 6);
			g.Insert(7, 5);
			g.Insert(5, 7);
			g.Insert(7, 3);
			g.Insert(3, 7);

			auto paths = g.FindAllPaths(0, 2);

			Assert::AreEqual(1, (int)paths.size());
		}

		TEST_METHOD(FindAllPaths_8)
		{
			misc::DirectedGraph g(10);
			g.Insert(0, 1);
			g.Insert(1, 0);
			g.Insert(0, 2);
			g.Insert(2, 0);
			g.Insert(0, 3);
			g.Insert(3, 0);
			g.Insert(0, 4);
			g.Insert(4, 0);
			g.Insert(1, 2);
			g.Insert(2, 1);
			g.Insert(1, 3);
			g.Insert(3, 1);
			g.Insert(1, 4);
			g.Insert(4, 1);
			g.Insert(2, 3);
			g.Insert(3, 2);
			g.Insert(2, 4);
			g.Insert(4, 2);
			g.Insert(3, 4);
			g.Insert(4, 3);

			auto paths = g.FindAllPaths(0, 4);

			Assert::AreEqual(16, (int)paths.size());
		}

		TEST_METHOD(FindAllPaths_9)
		{
			misc::DirectedGraph g(10);
			g.Insert(0, 1);
			g.Insert(1, 0);
			g.Insert(0, 2);
			g.Insert(2, 0);
			g.Insert(0, 3);
			g.Insert(3, 0);
			g.Insert(0, 4);
			g.Insert(4, 0);
			g.Insert(1, 2);
			g.Insert(2, 1);
			g.Insert(1, 3);
			g.Insert(3, 1);
			g.Insert(1, 4);
			g.Insert(4, 1);
			g.Insert(2, 3);
			g.Insert(3, 2);
			g.Insert(2, 4);
			g.Insert(4, 2);
			g.Insert(3, 4);
			g.Insert(4, 3);

			auto paths = g.FindAllPaths(0, 4);

			Assert::AreEqual(16, (int)paths.size());
		}
	};
}