#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <rope/CommonGraph.h>
#include <rope/Range.h>
#include <spandex/SparseMatrix.h>
#include <spandex/CholeskySolver.h>

namespace spandex::test
{
	TEST_CLASS(CholeskySolver)
	{
	public:

		TEST_METHOD(Chol_1)
		{
			rope::CommonGraph<double> g(3);
			g.Insert(0, 0, 6);
			g.Insert(1, 0, 8);
			g.Insert(1, 1, 27);
			g.Insert(2, 0, 14);
			g.Insert(2, 1, 27);
			g.Insert(2, 2, 41);
			auto ata = spandex::SparseMatrix<double>::FromGraph(3, 3, g);
			ata.layout = spandex::Layout::LowerSymmetric;

			spandex::CholeskySolver<double> solver(3, 3);
			auto b = solver.CholSym(ata);

			Assert::AreEqual(6, (int)b.nnz);

			Assert::IsTrue(b.Contains(0, 0));
			Assert::IsTrue(b.Contains(1, 0));
			Assert::IsTrue(b.Contains(1, 1));
			Assert::IsTrue(b.Contains(2, 0));
			Assert::IsTrue(b.Contains(2, 1));
			Assert::IsTrue(b.Contains(2, 2));
		}

		TEST_METHOD(Chol_2)
		{
			rope::CommonGraph<double> g(11);
			g.Insert(0, 0, 1);
			g.Insert(1, 1, 2);
			g.Insert(2, 1, 3);
			g.Insert(2, 2, 3);
			g.Insert(3, 3, 4);
			g.Insert(4, 4, 5);
			g.Insert(5, 0, 6);
			g.Insert(5, 3, 6);
			g.Insert(5, 5, 6);
			g.Insert(6, 0, 7);
			g.Insert(6, 6, 7);
			g.Insert(7, 1, 8);
			g.Insert(7, 4, 8);
			g.Insert(7, 7, 8);
			g.Insert(8, 5, 9);
			g.Insert(8, 8, 9);
			g.Insert(9, 2, 10);
			g.Insert(9, 3, 10);
			g.Insert(9, 5, 10);
			g.Insert(9, 7, 10);
			g.Insert(9, 9, 10);
			g.Insert(10, 2, 11);
			g.Insert(10, 4, 11);
			g.Insert(10, 6, 11);
			g.Insert(10, 7, 11);
			g.Insert(10, 9, 11);
			g.Insert(10, 10, 11);
			auto ata = spandex::SparseMatrix<double>::FromGraph(11, 11, g);
			ata.layout = spandex::Layout::LowerSymmetric;

			spandex::CholeskySolver<double> solver(11, 11);
			auto b = solver.CholSym(ata);

			Assert::AreEqual(33, b.nnz);

			Assert::IsTrue(b.Contains(6, 5));
			Assert::IsTrue(b.Contains(7, 2));
			Assert::IsTrue(b.Contains(8, 6));
			Assert::IsTrue(b.Contains(9, 6));
			Assert::IsTrue(b.Contains(9, 8));
			Assert::IsTrue(b.Contains(10, 8));
		}

		TEST_METHOD(Chol_3)
		{
			rope::CommonGraph<double> g(3);
			g.Insert(0, 0, 6);
			g.Insert(1, 0, 8);
			g.Insert(1, 1, 27);
			g.Insert(2, 0, 14);
			g.Insert(2, 1, 27);
			g.Insert(2, 2, 41);
			auto ata = spandex::SparseMatrix<double>::FromGraph(3, 3, g);
			ata.layout = spandex::Layout::LowerSymmetric;

			g.Clear();
			g.Insert(0, 0, 6);
			g.Insert(1, 0, 1.0 + 1.0 / 3.0);
			g.Insert(1, 1, 16.0 + 1.0 / 3.0);
			g.Insert(2, 0, 2.0 + 1.0 / 3.0);
			g.Insert(2, 1, 0.510204081632653);
			g.Insert(2, 2, 4.08163265306122);
			auto e = spandex::SparseMatrix<double>::FromGraph(3, 3, g);
			e.layout = spandex::Layout::LowerSymmetric;

			spandex::CholeskySolver<double> solver(5, 5);
			auto ld = solver.CholSym(ata);
			solver.CholTo(ata, ld);

			Assert::IsTrue(ld.Equals(e,
				[](double x, double y) { return std::abs(x - y) < 1e-8; }));
		}

		TEST_METHOD(Chol_4)
		{
			rope::CommonGraph<double> g(5);
			g.Insert(0, 0, 0.454154210872255);
			g.Insert(1, 0, 0.493313382040145);
			g.Insert(1, 1, 0.673117517240105);
			g.Insert(2, 0, 0);
			g.Insert(2, 1, 0.267770063806461);
			g.Insert(2, 2, 0.753202118586013);
			g.Insert(3, 0, 0.597779567627811);
			g.Insert(3, 1, 0.650049443659237);
			g.Insert(3, 2, 0.337426886372401);
			g.Insert(3, 3, 1.72955430012221);
			g.Insert(4, 0, 0.0939734363675742);
			g.Insert(4, 1, 0.0200830025404996);
			g.Insert(4, 2, 0);
			g.Insert(4, 3, 0.25140727798654);
			g.Insert(4, 4, 0.527264476708613);
			auto ata = spandex::SparseMatrix<double>::FromGraph(5, 5, g);
			ata.layout = spandex::Layout::LowerSymmetric;

			g.Clear();
			g.Insert(0, 0, 0.454154210872255);
			g.Insert(1, 0, 1.08622439301549);
			g.Insert(1, 1, 0.137268488267129);
			g.Insert(2, 0, 0);
			g.Insert(2, 1, 1.95070308697049);
			g.Insert(2, 2, 0.230862228520464);
			g.Insert(3, 0, 1.31624799091856);
			g.Insert(3, 1, 0.00529397289085283);
			g.Insert(3, 2, 1.45545384824111);
			g.Insert(3, 3, 0.453678241855123);
			g.Insert(4, 0, 0.206919663228681);
			g.Insert(4, 1, -0.597320167013766);
			g.Insert(4, 2, 0.692813459608376);
			g.Insert(4, 3, -0.2306541683174);
			g.Insert(4, 4, 0.323895344304452);
			auto e = spandex::SparseMatrix<double>::FromGraph(5, 5, g);
			e.layout = spandex::Layout::LowerSymmetric;

			spandex::CholeskySolver<double> solver(5, 5);
			auto ld = solver.CholSym(ata);
			solver.CholTo(ata, ld);

			Assert::IsTrue(ld.Equals(e,
				[](double x, double y) { return std::abs(x - y) < 1e-8; }));
		}

		TEST_METHOD(Solve_1)
		{
			rope::CommonGraph<double> g(3);
			g.Insert(0, 0, 38);
			g.Insert(1, 0, 22);
			g.Insert(1, 1, 17);
			g.Insert(2, 0, 14);
			g.Insert(2, 1, 13);
			g.Insert(2, 2, 11);
			auto ata = spandex::SparseMatrix<double>::FromGraph(3, 3, g);
			ata.layout = spandex::Layout::LowerSymmetric;

			spandex::CholeskySolver<double> solver(3, 3);
			solver.permutation = spandex::Permutation::Type::NoPermutation;
			solver.normalization = spandex::Normalization::Type::Pivots;

			std::vector<double> b = { 1, 10, 5 };

			auto ld = solver.CholSym(ata);
			solver.CholTo(ata, ld);
			std::vector<double> x(3);
			solver.SolveTo(ld, b, x);

			Assert::AreEqual(3, (int)x.size());

			Assert::AreEqual(-9.5, x[0], 1e-10);
			Assert::AreEqual(34.166666666666, x[1], 1e-10);
			Assert::AreEqual(-27.833333333333, x[2], 1e-10);
		}

		TEST_METHOD(Solve_2)
		{
			rope::CommonGraph<double> g(5);
			g.Insert(0, 0, 0.339721892599889);
			g.Insert(1, 0, -0.117511897064494);
			g.Insert(1, 1, 0.61348986691309);
			g.Insert(2, 0, 0.0221940137659884);
			g.Insert(2, 1, 0.27504299861335);
			g.Insert(2, 2, 0.355516636030647);
			g.Insert(3, 0, -0.26882517335515);
			g.Insert(3, 1, 0.0684899239330446);
			g.Insert(3, 2, 0.0333434870019433);
			g.Insert(3, 3, 0.412101013254036);
			g.Insert(4, 0, -0.159648639043336);
			g.Insert(4, 1, 0.0401190234458681);
			g.Insert(4, 2, -0.0318276646882203);
			g.Insert(4, 3, -0.0108046250911296);
			g.Insert(4, 4, 0.399329937547505);
			auto ata = spandex::SparseMatrix<double>::FromGraph(5, 5, g);
			ata.layout = spandex::Layout::LowerSymmetric;

			spandex::CholeskySolver<double> solver(5, 5);
			solver.permutation = spandex::Permutation::Type::NoPermutation;
			solver.normalization = spandex::Normalization::Type::Pivots;

			std::vector<double> b = { 4, -1, -3, 4, -2 };

			auto ld = solver.CholSym(ata);
			solver.CholTo(ata, ld);
			std::vector<double> x(5);
			solver.SolveTo(ld, b, x);

			Assert::AreEqual(5, (int)x.size());

			Assert::AreEqual(84.3926371078906, x[0], 1e-10);
			Assert::AreEqual(20.6069097491701, x[1], 1e-10);
			Assert::AreEqual(-33.4133473479173, x[2], 1e-10);
			Assert::AreEqual(64.7118533875248, x[3], 1e-10);
			Assert::AreEqual(25.7485304673898, x[4], 1e-10);
		}

		TEST_METHOD(Solve_3)
		{
			rope::CommonGraph<double> g(10);

			g.Insert(0, 0, 0.360464443870286);
			g.Insert(2, 1, 0.965038079655014);
			g.Insert(9, 1, 0.806541221607173);
			g.Insert(0, 2, 0.156202523064209);
			g.Insert(2, 2, 0.70277194218269);
			g.Insert(6, 2, 0.398688926587124);
			g.Insert(8, 2, 0.158532504726658);
			g.Insert(9, 2, 0.070915819808533);
			g.Insert(6, 3, 0.552895404215196);
			g.Insert(3, 4, 0.97656582830328);
			g.Insert(5, 4, 0.362469500523493);
			g.Insert(1, 5, 0.510437505153131);
			g.Insert(2, 5, 0.473695871041683);
			g.Insert(4, 6, 0.477123911915246);
			g.Insert(7, 6, 0.582754540178946);
			g.Insert(3, 7, 0.828533162691592);
			g.Insert(4, 8, 0.612247361949774);
			g.Insert(5, 9, 0.570109021624869);

			auto a = spandex::SparseMatrix<double>::FromGraph(10, 10, g);

			spandex::CholeskySolver<double> solver(10, 10);
			solver.permutation = spandex::Permutation::Type::NoPermutation;
			solver.normalization = spandex::Normalization::Type::Pivots;

			solver.SolveSym(a);

			std::vector<double> b(10);
			std::iota(b.begin(), b.end(), 1);

			auto x = solver.Solve(a, b);

			Assert::AreEqual(4.6288540824669431, x[0], 1e-10);
			Assert::AreEqual(9.0912268747716602, x[1], 1e-10);
			Assert::AreEqual(-4.2799392703685015, x[2], 1e-10);
			Assert::AreEqual(15.746856145276686, x[3], 1e-10);
			Assert::AreEqual(-0.60521269977679459, x[4], 1e-10);
			Assert::AreEqual(-0.59628461522440024, x[5], 1e-10);
			Assert::AreEqual(13.727906774511695, x[6], 1e-10);
			Assert::AreEqual(5.5411542327921595, x[7], 1e-10);
			Assert::AreEqual(-2.5315136968936010, x[8], 1e-10);
			Assert::AreEqual(10.909090909090908, x[9], 1e-10);

			auto noPerm = solver.GetPermutation();

			solver.permutation = spandex::Permutation::Type::AMD;
			solver.SolveSym(a);

			auto perm = solver.GetPermutation();
			Assert::IsFalse(noPerm.Equals(perm));

			x = solver.Solve(a, b);

			Assert::AreEqual(4.6288540824669431, x[0], 1e-10);
			Assert::AreEqual(9.0912268747716602, x[1], 1e-10);
			Assert::AreEqual(-4.2799392703685015, x[2], 1e-10);
			Assert::AreEqual(15.746856145276686, x[3], 1e-10);
			Assert::AreEqual(0, x[4], 1e-10);
			Assert::AreEqual(-0.59628461522440024, x[5], 1e-10);
			Assert::AreEqual(13.727906774511695, x[6], 1e-10);
			Assert::AreEqual(4.8278091694066978, x[7], 1e-10);
			Assert::AreEqual(-2.5315136968936010, x[8], 1e-10);
			Assert::AreEqual(10.524302848075246, x[9], 1e-10);
		}

		TEST_METHOD(Solve_4)
		{
			rope::CommonGraph<double> g(3);
			g.Insert(0, 1, 1);
			g.Insert(0, 2, 1);
			g.Insert(1, 0, 2);
			g.Insert(1, 1, 4);
			g.Insert(1, 2, -2);
			g.Insert(2, 1, 3);
			g.Insert(2, 2, 15);
			auto a = spandex::SparseMatrix<double>::FromGraph(3, 3, g);

			spandex::CholeskySolver<double> solver(3, 3);
			solver.permutation = spandex::Permutation::Type::NoPermutation;
			solver.normalization = spandex::Normalization::Type::Pivots;

			solver.SolveSym(a);

			std::vector<double> b{ 17, 2.89, -3.3 };

			auto x = solver.Solve(a, b);

			Assert::AreEqual(-46.1300, x[0], 1e-8);
			Assert::AreEqual(21.5250, x[1], 1e-8);
			Assert::AreEqual(-4.5250, x[2], 1e-8);

			Assert::AreEqual(b[0], x[0] * a.GetRowwise(0, 0) + x[1] * a.GetRowwise(0, 1) + x[2] * a.GetRowwise(0, 2), 1e-8);
			Assert::AreEqual(b[1], x[0] * a.GetRowwise(1, 0) + x[1] * a.GetRowwise(1, 1) + x[2] * a.GetRowwise(1, 2), 1e-8);
			Assert::AreEqual(b[2], x[0] * a.GetRowwise(2, 0) + x[1] * a.GetRowwise(2, 1) + x[2] * a.GetRowwise(2, 2), 1e-8);
		}

		TEST_METHOD(Update_1)
		{
			rope::CommonGraph<double> g(3);
			g.Insert(0, 1, 1);
			g.Insert(0, 2, 1);
			g.Insert(1, 0, 2);
			g.Insert(1, 1, 4);
			g.Insert(1, 2, -2);
			g.Insert(2, 1, 3);
			g.Insert(2, 2, 15);
			auto a = spandex::SparseMatrix<double>::FromGraph(3, 3, g);

			spandex::CholeskySolver<double> solver(3, 3);
			solver.permutation = spandex::Permutation::Type::NoPermutation;
			solver.normalization = spandex::Normalization::Type::Pivots;

			solver.SolveSym(a);

			std::vector<double> b{ 17, 2.89, -3.3 };

			auto x = solver.Solve(a, b);

			auto u = solver.Update(SparseArray<double>(3, { {0, 7.0}, {1, -5.0}, {2, 1.0} }), 9);

			g.AddVertex();
			g.Insert(3, 0, 7);
			g.Insert(3, 1, -5);
			g.Insert(3, 2, 1);
			a = spandex::SparseMatrix<double>::FromGraph(4, 3, g);
			b.push_back(9);
			x = solver.Solve(a, b);

			double diff = SquareDiff(x, u);

			Assert::IsTrue(diff < 1e-8);
		}

		TEST_METHOD(Update_2)
		{
			rope::CommonGraph<double> g(3);
			g.Insert(0, 1, 1);
			g.Insert(0, 2, 1);
			g.Insert(1, 0, 2);
			g.Insert(1, 1, 4);
			g.Insert(1, 2, -2);
			g.Insert(2, 1, 3);
			g.Insert(2, 2, 15);
			auto a = spandex::SparseMatrix<double>::FromGraph(3, 3, g);

			spandex::CholeskySolver<double> solver(3, 3);
			solver.permutation = spandex::Permutation::Type::NoPermutation;
			solver.normalization = spandex::Normalization::Type::Pivots;

			solver.SolveSym(a);

			std::vector<double> b{ 17, 2.89, -3.3 };

			auto x = solver.Solve(a, b);

			auto u = solver.Update(SparseArray<double>(3, { {2, 1.0} }), 9);

			g.AddVertex();
			g.Insert(3, 2, 1);
			a = spandex::SparseMatrix<double>::FromGraph(4, 3, g);
			b.push_back(9);
			x = solver.Solve(a, b);

			double diff = SquareDiff(x, u);

			Assert::IsTrue(diff < 1e-8);
		}

	private:

		static double SquareDiff(std::vector<double> & x, std::vector<double> & y)
		{
			assert(x.size() == y.size());

			rope::Range iota((int)x.size());
			double diff = std::sqrt(std::accumulate(iota.begin(), iota.end(), 0.0,
				[&](double acc, int i) { return acc + std::pow(x[i] - y[i], 2.0); }));

			return diff;
		}
	};
}