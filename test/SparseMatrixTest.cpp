#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <rope/CommonGraph.h>
#include <spandex/SparseMatrix.h>

namespace spandex::test
{
	TEST_CLASS(SparseMatrix)
	{
	public:

		TEST_METHOD(FromCSR)
		{
			auto a = spandex::SparseMatrix<int>::FromCSR(3, 4,
				std::vector<int>{0, 4, 7, 11},
				std::vector<int>{0, 1, 2, 3, 0, 1, 3, 0, 1, 2, 3},
				std::vector<int>{10, 11, 12, 13, 20, 21, 23, 30, 31, 32, 33});

			Assert::AreEqual(3, a.rowCount);
			Assert::AreEqual(4, a.columnCount);
			Assert::AreEqual(11, a.nnz);

			Assert::AreEqual(10, a.GetRowwise(0, 0));
			Assert::AreEqual(11, a.GetRowwise(0, 1));
			Assert::AreEqual(12, a.GetRowwise(0, 2));
			Assert::AreEqual(13, a.GetRowwise(0, 3));
			Assert::AreEqual(23, a.GetRowwise(1, 3));
			Assert::AreEqual(20, a.GetRowwise(1, 0));
			Assert::AreEqual(21, a.GetRowwise(1, 1));
			Assert::AreEqual(33, a.GetRowwise(2, 3));
			Assert::AreEqual(32, a.GetRowwise(2, 2));
			Assert::AreEqual(31, a.GetRowwise(2, 1));
			Assert::AreEqual(30, a.GetRowwise(2, 0));
		}

		TEST_METHOD(FromGraph)
		{
			auto a = spandex::SparseMatrix<int>::FromCSR(3, 4,
				std::vector<int>{0, 4, 7, 11},
				std::vector<int>{0, 1, 2, 3, 0, 1, 3, 0, 1, 2, 3},
				std::vector<int>{10, 11, 12, 13, 20, 21, 23, 30, 31, 32, 33});

			rope::CommonGraph<int> graph(3);
			graph.Insert(0, 0, 10);
			graph.Insert(0, 1, 11);
			graph.Insert(0, 2, 12);
			graph.Insert(0, 3, 13);
			graph.Insert(1, 3, 23);
			graph.Insert(1, 0, 20);
			graph.Insert(1, 1, 21);
			graph.Insert(2, 3, 33);
			graph.Insert(2, 2, 32);
			graph.Insert(2, 1, 31);
			graph.Insert(2, 0, 30);

			auto b = spandex::SparseMatrix<int>::FromGraph(3, 4, graph);

			Assert::IsTrue(a.Equals(b));
			Assert::IsTrue(b.Equals(a));
		}

		TEST_METHOD(Transpose)
		{
			auto a = spandex::SparseMatrix<int>::FromCSR(3, 4,
				std::vector<int>{0, 4, 7, 11},
				std::vector<int>{0, 1, 2, 3, 0, 1, 3, 0, 1, 2, 3},
				std::vector<int>{10, 11, 12, 13, 20, 21, 23, 30, 31, 32, 33});

			auto at = a.Transpose();
			auto att = at.Transpose();
			auto attt = att.Transpose();

			Assert::IsTrue(a.Equals(att));
			Assert::IsTrue(at.Equals(attt));
			Assert::IsFalse(a.Equals(at));
		}

		TEST_METHOD(GetRow_1)
		{
			auto a = spandex::SparseMatrix<int>::FromCSR(4, 4,
				std::vector<int>{0, 4, 7, 11, 11},
				std::vector<int>{0, 1, 2, 3, 0, 1, 3, 0, 1, 2, 3},
				std::vector<int>{10, 11, 12, 13, 20, 21, 23, 30, 31, 32, 33});

			auto row = a.GetRow(0);
			Assert::AreEqual(4, row.size);
			Assert::AreEqual(4, row.nnz);
			Assert::AreEqual(10, row.At(0));
			Assert::AreEqual(11, row.At(1));
			Assert::AreEqual(12, row.At(2));
			Assert::AreEqual(13, row.At(3));

			row = a.GetRow(1);
			Assert::AreEqual(4, row.size);
			Assert::AreEqual(3, row.nnz);
			Assert::AreEqual(20, row.At(0));
			Assert::AreEqual(21, row.At(1));
			Assert::AreEqual(23, row.At(3));

			row = a.GetRow(2);
			Assert::AreEqual(4, row.size);
			Assert::AreEqual(4, row.nnz);
			Assert::AreEqual(30, row.At(0));
			Assert::AreEqual(31, row.At(1));
			Assert::AreEqual(32, row.At(2));
			Assert::AreEqual(33, row.At(3));

			row = a.GetRow(3);
			Assert::AreEqual(4, row.size);
			Assert::AreEqual(0, row.nnz);
		}

		TEST_METHOD(GetColumn_1)
		{
			auto a = spandex::SparseMatrix<int>::FromCSR(3, 5,
				std::vector<int>{0, 4, 7, 11},
				std::vector<int>{0, 1, 2, 3, 0, 1, 3, 0, 1, 2, 3},
				std::vector<int>{10, 11, 12, 13, 20, 21, 23, 30, 31, 32, 33});

			auto column = a.GetColumn(0);
			Assert::AreEqual(3, column.size);
			Assert::AreEqual(3, column.nnz);
			Assert::AreEqual(10, column.At(0));
			Assert::AreEqual(20, column.At(1));
			Assert::AreEqual(30, column.At(2));

			column = a.GetColumn(1);
			Assert::AreEqual(3, column.size);
			Assert::AreEqual(3, column.nnz);
			Assert::AreEqual(11, column.At(0));
			Assert::AreEqual(21, column.At(1));
			Assert::AreEqual(31, column.At(2));

			column = a.GetColumn(2);
			Assert::AreEqual(3, column.size);
			Assert::AreEqual(2, column.nnz);
			Assert::AreEqual(12, column.At(0));
			Assert::AreEqual(32, column.At(2));

			column = a.GetColumn(3);
			Assert::AreEqual(3, column.size);
			Assert::AreEqual(3, column.nnz);
			Assert::AreEqual(13, column.At(0));
			Assert::AreEqual(23, column.At(1));
			Assert::AreEqual(33, column.At(2));

			column = a.GetColumn(4);
			Assert::AreEqual(3, column.size);
			Assert::AreEqual(0, column.nnz);
		}

		TEST_METHOD(GetValue_1)
		{
			auto a = spandex::SparseMatrix<int>::FromCSR(3, 4,
				std::vector<int>{0, 4, 7, 11},
				std::vector<int>{0, 1, 2, 3, 0, 1, 3, 0, 1, 2, 3},
				std::vector<int>{10, 11, 12, 13, 20, 21, 23, 30, 31, 32, 33});

			Assert::AreEqual(10, a.GetRowwise(0, 0));
			Assert::AreEqual(11, a.GetRowwise(0, 1));
			Assert::AreEqual(12, a.GetRowwise(0, 2));
			Assert::AreEqual(13, a.GetRowwise(0, 3));
			Assert::AreEqual(23, a.GetRowwise(1, 3));
			Assert::AreEqual(20, a.GetRowwise(1, 0));
			Assert::AreEqual(21, a.GetRowwise(1, 1));
			Assert::AreEqual(33, a.GetRowwise(2, 3));
			Assert::AreEqual(32, a.GetRowwise(2, 2));
			Assert::AreEqual(31, a.GetRowwise(2, 1));
			Assert::AreEqual(30, a.GetRowwise(2, 0));

			Assert::AreEqual(a.GetColumnwise(0, 0), a.GetRowwise(0, 0));
			Assert::AreEqual(a.GetColumnwise(0, 1), a.GetRowwise(0, 1));
			Assert::AreEqual(a.GetColumnwise(0, 2), a.GetRowwise(0, 2));
			Assert::AreEqual(a.GetColumnwise(0, 3), a.GetRowwise(0, 3));
			Assert::AreEqual(a.GetColumnwise(1, 3), a.GetRowwise(1, 3));
			Assert::AreEqual(a.GetColumnwise(1, 0), a.GetRowwise(1, 0));
			Assert::AreEqual(a.GetColumnwise(1, 1), a.GetRowwise(1, 1));
			Assert::AreEqual(a.GetColumnwise(2, 3), a.GetRowwise(2, 3));
			Assert::AreEqual(a.GetColumnwise(2, 2), a.GetRowwise(2, 2));
			Assert::AreEqual(a.GetColumnwise(2, 1), a.GetRowwise(2, 1));
			Assert::AreEqual(a.GetColumnwise(2, 0), a.GetRowwise(2, 0));
		}

		TEST_METHOD(SetValue_1)
		{
			auto a = spandex::SparseMatrix<int>::FromCSR(3, 4,
				std::vector<int>{0, 4, 7, 11},
				std::vector<int>{0, 1, 2, 3, 0, 1, 3, 0, 1, 2, 3},
				std::vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});

			a.SetRowwise(0, 0, 10);
			a.SetRowwise(0, 1, 11);
			a.SetRowwise(0, 2, 12);
			a.SetRowwise(0, 3, 13);
			a.SetRowwise(1, 3, 23);
			a.SetRowwise(1, 0, 20);
			a.SetRowwise(1, 1, 21);
			a.SetRowwise(2, 3, 33);
			a.SetRowwise(2, 2, 32);
			a.SetRowwise(2, 1, 31);
			a.SetRowwise(2, 0, 30);

			Assert::AreEqual(10, a.GetRowwise(0, 0));
			Assert::AreEqual(11, a.GetRowwise(0, 1));
			Assert::AreEqual(12, a.GetRowwise(0, 2));
			Assert::AreEqual(13, a.GetRowwise(0, 3));
			Assert::AreEqual(23, a.GetRowwise(1, 3));
			Assert::AreEqual(20, a.GetRowwise(1, 0));
			Assert::AreEqual(21, a.GetRowwise(1, 1));
			Assert::AreEqual(33, a.GetRowwise(2, 3));
			Assert::AreEqual(32, a.GetRowwise(2, 2));
			Assert::AreEqual(31, a.GetRowwise(2, 1));
			Assert::AreEqual(30, a.GetRowwise(2, 0));

			a.SetColumnwise(0, 0, 100);
			a.SetColumnwise(0, 1, 110);
			a.SetColumnwise(0, 2, 120);
			a.SetColumnwise(0, 3, 130);
			a.SetColumnwise(1, 3, 230);
			a.SetColumnwise(1, 0, 200);
			a.SetColumnwise(1, 1, 210);
			a.SetColumnwise(2, 3, 330);
			a.SetColumnwise(2, 2, 320);
			a.SetColumnwise(2, 1, 310);
			a.SetColumnwise(2, 0, 300);

			Assert::AreEqual(100, a.GetRowwise(0, 0));
			Assert::AreEqual(110, a.GetRowwise(0, 1));
			Assert::AreEqual(120, a.GetRowwise(0, 2));
			Assert::AreEqual(130, a.GetRowwise(0, 3));
			Assert::AreEqual(230, a.GetRowwise(1, 3));
			Assert::AreEqual(200, a.GetRowwise(1, 0));
			Assert::AreEqual(210, a.GetRowwise(1, 1));
			Assert::AreEqual(330, a.GetRowwise(2, 3));
			Assert::AreEqual(320, a.GetRowwise(2, 2));
			Assert::AreEqual(310, a.GetRowwise(2, 1));
			Assert::AreEqual(300, a.GetRowwise(2, 0));
		}

		TEST_METHOD(Contains)
		{
			auto a = spandex::SparseMatrix<int>::FromCSR(4, 3,
				std::vector<int>{0, 1, 2, 4, 4},
				std::vector<int>{1, 0, 1, 2},
				std::vector<int>{1, 1, 1, 1});

			Assert::IsTrue(a.Contains(0, 1));
			Assert::IsTrue(a.Contains(1, 0));
			Assert::IsTrue(a.Contains(2, 1));
			Assert::IsTrue(a.Contains(2, 2));

			Assert::IsFalse(a.Contains(0, 0));
			Assert::IsFalse(a.Contains(0, 2));
			Assert::IsFalse(a.Contains(1, 1));
			Assert::IsFalse(a.Contains(1, 2));
			Assert::IsFalse(a.Contains(2, 0));
			Assert::IsFalse(a.Contains(3, 0));
			Assert::IsFalse(a.Contains(3, 1));
			Assert::IsFalse(a.Contains(3, 2));
		}

		TEST_METHOD(Add_1)
		{
			rope::CommonGraph<int> g(3);
			g.Insert(0, 1, 5);
			g.Insert(0, 2, 6);
			g.Insert(1, 0, 1);
			g.Insert(1, 1, 4);
			g.Insert(1, 2, 7);
			g.Insert(2, 0, 2);
			g.Insert(2, 1, 3);
			g.Insert(2, 2, 8);
			auto a = spandex::SparseMatrix<int>::FromGraph(3, 3, g);

			g.Clear();
			g.Insert(0, 0, -1);
			g.Insert(0, 2, 1);
			g.Insert(1, 0, -1);
			g.Insert(1, 2, 1);
			g.Insert(2, 0, -1);
			g.Insert(2, 2, 1);
			auto b = spandex::SparseMatrix<int>::FromGraph(3, 3, g);

			g.Clear();
			g.Insert(0, 0, -1);
			g.Insert(0, 1, 5);
			g.Insert(0, 2, 7);
			g.Insert(1, 0, 0);
			g.Insert(1, 1, 4);
			g.Insert(1, 2, 8);
			g.Insert(2, 0, 1);
			g.Insert(2, 1, 3);
			g.Insert(2, 2, 9);
			auto e = spandex::SparseMatrix<int>::FromGraph(3, 3, g);

			auto c = a.Add(b);

			Assert::IsFalse(a.Equals(c));
			Assert::IsFalse(b.Equals(c));
			Assert::IsTrue(e.Equals(c));
		}

		TEST_METHOD(Mul_1)
		{
			rope::CommonGraph<int> g(3);
			g.Insert(0, 0, 6);
			g.Insert(0, 1, -4);
			g.Insert(0, 2, 7);
			g.Insert(1, 0, 4);
			g.Insert(1, 1, -3);
			g.Insert(1, 2, 0);
			g.Insert(2, 0, 1);
			g.Insert(2, 1, 2);
			g.Insert(2, 2, 5);
			auto a = spandex::SparseMatrix<int>::FromGraph(3, 3, g);

			g.Clear();
			g.Insert(0, 0, 6);
			g.Insert(0, 1, 3);
			g.Insert(1, 0, -1);
			g.Insert(1, 1, 4);
			g.Insert(2, 0, 8);
			g.Insert(2, 1, -3);
			auto b = spandex::SparseMatrix<int>::FromGraph(3, 2, g);

			g.Clear();
			g.Insert(0, 0, 96);
			g.Insert(0, 1, -19);
			g.Insert(1, 0, 27);
			g.Insert(1, 1, 0);
			g.Insert(2, 0, 44);
			g.Insert(2, 1, -4);
			auto e = spandex::SparseMatrix<int>::FromGraph(3, 2, g);

			auto c = a.Mul(b);

			Assert::IsFalse(a.Equals(c));
			Assert::IsFalse(b.Equals(c));
			Assert::IsTrue(e.Equals(c));
		}

		TEST_METHOD(Mul_2)
		{
			rope::CommonGraph<int> g(3);
			g.Insert(0, 0, 1);
			g.Insert(0, 2, 3);
			g.Insert(1, 1, 5);
			g.Insert(2, 1, 8);
			auto a = spandex::SparseMatrix<int>::FromGraph(3, 3, g);

			g.Clear();
			g.Insert(0, 1, 2);
			g.Insert(0, 2, 3);
			g.Insert(1, 0, 4);
			g.Insert(1, 2, 6);
			g.Insert(2, 0, 7);
			g.Insert(2, 1, 8);
			auto b = spandex::SparseMatrix<int>::FromGraph(3, 3, g);

			g.Clear();
			g.Insert(0, 0, 21);
			g.Insert(0, 1, 26);
			g.Insert(0, 2, 3);
			g.Insert(1, 0, 20);
			g.Insert(1, 2, 30);
			g.Insert(2, 0, 32);
			g.Insert(2, 2, 48);
			auto e = spandex::SparseMatrix<int>::FromGraph(3, 3, g);

			auto c = a.Mul(b);

			Assert::IsFalse(a.Equals(c));
			Assert::IsFalse(b.Equals(c));
			Assert::IsTrue(e.Equals(c));
		}

		TEST_METHOD(Mul_3)
		{
			rope::CommonGraph<int> g(3);
			g.Insert(0, 0, 1);
			g.Insert(0, 2, 3);
			g.Insert(1, 1, 5);
			g.Insert(2, 1, 8);
			auto a = spandex::SparseMatrix<int>::FromGraph(3, 3, g);

			g.Clear();
			g.Insert(0, 0, 6);
			g.Insert(1, 0, -1);
			g.Insert(2, 0, 8);
			auto b = spandex::SparseMatrix<int>::FromGraph(3, 1, g);

			g.Clear();
			g.Insert(0, 0, 30);
			g.Insert(1, 0, -5);
			g.Insert(2, 0, -8);
			auto e = spandex::SparseMatrix<int>::FromGraph(3, 1, g);

			auto c = a.Mul(b);

			Assert::IsFalse(a.Equals(c));
			Assert::IsFalse(b.Equals(c));
			Assert::IsTrue(e.Equals(c));
		}

		TEST_METHOD(Sqr_1)
		{
			rope::CommonGraph<int> g(3);
			g.Insert(0, 1, 1);
			g.Insert(1, 0, 2);
			g.Insert(2, 1, 3);
			auto a = spandex::SparseMatrix<int>::FromGraph(3, 2, g);

			auto ata = a.SqrSym();

			Assert::AreEqual(2, ata.nnz);
			Assert::IsTrue(ata.Contains(0, 0));
			Assert::IsTrue(ata.Contains(1, 1));
		}

		TEST_METHOD(Sqr_2)
		{
			rope::CommonGraph<double> g(4);
			g.Insert(1, 4, 0.02675);
			g.Insert(2, 0, 0.78664);
			g.Insert(3, 0, 0.26856);
			g.Insert(3, 3, 0.51423);
			g.Insert(3, 5, 0.46234);
			auto a = spandex::SparseMatrix<double>::FromGraph(4, 6, g);

			g.Resize(6);
			g.Insert(0, 0, 0.69094);
			g.Insert(3, 0, 0.1381);
			g.Insert(3, 3, 0.26444);
			g.Insert(4, 4, 0.00072);
			g.Insert(5, 0, 0.12417);
			g.Insert(5, 3, 0.23775);
			g.Insert(5, 5, 0.21376);
			auto e = spandex::SparseMatrix<double>::FromGraph(6, 6, g);

			auto ata = a.SqrSym();

			Assert::AreEqual(12, ata.nnz);
			Assert::IsTrue(ata.Contains(0, 0));
			Assert::IsTrue(ata.Contains(1, 1));
			Assert::IsTrue(ata.Contains(2, 2));
			Assert::IsTrue(ata.Contains(3, 0));
			Assert::IsTrue(ata.Contains(3, 3));
			Assert::IsTrue(ata.Contains(4, 4));
			Assert::IsTrue(ata.Contains(5, 0));
			Assert::IsTrue(ata.Contains(5, 3));
			Assert::IsTrue(ata.Contains(5, 5));
			Assert::IsTrue(ata.Contains(0, 3));
			Assert::IsTrue(ata.Contains(0, 5));
			Assert::IsTrue(ata.Contains(3, 5));
		}

		TEST_METHOD(Sqr_3)
		{
			rope::CommonGraph<int> g(3);
			g.Insert(0, 1, 1);
			g.Insert(1, 0, 2);
			g.Insert(2, 1, 3);
			auto a = spandex::SparseMatrix<int>::FromGraph(3, 2, g);

			g.Clear();
			g.Insert(0, 0, 4);
			g.Insert(1, 1, 10);
			auto e = spandex::SparseMatrix<int>::FromGraph(2, 2, g);

			auto ata = a.Sqr();

			Assert::IsTrue(ata.Equals(e));

			auto e2 = a.Transpose().Mul(a);

			Assert::IsTrue(ata.Equals(e2));
		}

		TEST_METHOD(Sqr_4)
		{
			rope::CommonGraph<double> g(4);
			g.Insert(1, 4, 0.02674817948);
			g.Insert(2, 0, 0.7866442604);
			g.Insert(3, 0, 0.2685635172);
			g.Insert(3, 3, 0.5142332896);
			g.Insert(3, 5, 0.462339404);
			auto a = spandex::SparseMatrix<double>::FromGraph(4, 6, g);

			g.Resize(6);
			g.Insert(0, 0, 0.6909355552);
			g.Insert(1, 1, 0);
			g.Insert(2, 2, 0);
			g.Insert(3, 0, 0.13810430091);
			g.Insert(3, 3, 0.2644358761);
			g.Insert(4, 4, 0.0007154651053);
			g.Insert(5, 0, 0.1241674965);
			g.Insert(5, 3, 0.2377503126);
			g.Insert(5, 5, 0.2137577245);
			g.Insert(0, 3, 0.13810430091);
			g.Insert(0, 5, 0.1241674965);
			g.Insert(3, 5, 0.2377503126);
			auto e = spandex::SparseMatrix<double>::FromGraph(6, 6, g);

			auto ata = a.Sqr();

			Assert::IsTrue(ata.Equals(e,
				[](double x, double y) { return std::abs(x - y) < 1e-8; }));
		}

	};
}
