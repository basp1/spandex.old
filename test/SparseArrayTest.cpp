#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <spandex/SparseArray.h>

namespace spandex::test
{
	TEST_CLASS(SparseArray)
	{
	public:

		TEST_METHOD(Insert_1)
		{
			spandex::SparseArray<char> a(10);

			a.Insert(1, 'a');
			a.Insert(5, 'e');
			a.Insert(3, 'c');

			Assert::AreEqual(3, a.nnz);

			a.Insert(2, 'b');
			a.Insert(4, 'd');

			Assert::AreEqual(5, a.nnz);
		}

		TEST_METHOD(Insert_2)
		{
			spandex::SparseArray<char> a(10);

			a.Insert(1, 'a');
			Assert::AreEqual(1, a.nnz);

			a.Insert(1, 'a');
			Assert::AreEqual(1, a.nnz);

			a.Insert(1, 'b');
			Assert::AreEqual(1, a.nnz);
		}

		TEST_METHOD(Contains_1)
		{
			spandex::SparseArray<char> a(10);

			a.Insert(1, 'a');
			a.Insert(5, 'e');
			a.Insert(3, 'c');

			Assert::IsTrue(a.Contains(1));
			Assert::IsTrue(a.Contains(3));
			Assert::IsTrue(a.Contains(5));
			Assert::IsFalse(a.Contains(0));
			Assert::IsFalse(a.Contains(2));
			Assert::IsFalse(a.Contains(4));
			Assert::IsFalse(a.Contains(6));

			a.Insert(2, 'b');
			a.Insert(4, 'd');

			Assert::IsTrue(a.Contains(1));
			Assert::IsTrue(a.Contains(2));
			Assert::IsTrue(a.Contains(3));
			Assert::IsTrue(a.Contains(4));
			Assert::IsTrue(a.Contains(5));
			Assert::IsFalse(a.Contains(0));
			Assert::IsFalse(a.Contains(6));
		}

		TEST_METHOD(Contains_2)
		{
			spandex::SparseArray<char> a(10);

			a.Insert(1, 'a');
			Assert::IsTrue(a.Contains(1));
			Assert::IsFalse(a.Contains(0));
			Assert::IsFalse(a.Contains(9));

			a.Insert(1, 'a');
			Assert::IsTrue(a.Contains(1));
			Assert::IsFalse(a.Contains(0));
			Assert::IsFalse(a.Contains(9));

			a.Insert(1, 'b');
			Assert::IsTrue(a.Contains(1));
			Assert::IsFalse(a.Contains(0));
			Assert::IsFalse(a.Contains(9));
		}

		TEST_METHOD(At_1)
		{
			spandex::SparseArray<char> a(10);

			a.Insert(1, 'a');
			a.Insert(5, 'e');
			a.Insert(3, 'c');

			Assert::AreEqual('a', a.At(1));
			Assert::AreEqual('c', a.At(3));
			Assert::AreEqual('e', a.At(5));

			a.Insert(2, 'b');
			a.Insert(4, 'd');

			Assert::AreEqual('a', a.At(1));
			Assert::AreEqual('b', a.At(2));
			Assert::AreEqual('c', a.At(3));
			Assert::AreEqual('d', a.At(4));
			Assert::AreEqual('e', a.At(5));
		}

		TEST_METHOD(At_2)
		{
			spandex::SparseArray<char> a(10);

			a.Insert(1, 'a');
			Assert::AreEqual('a', a.At(1));

			a.Insert(1, 'a');
			Assert::AreEqual('a', a.At(1));

			a.Insert(1, 'b');
			Assert::AreEqual('b', a.At(1));
		}

		TEST_METHOD(Equals_1)
		{
			spandex::SparseArray<char> a(10, { { 1, 'a' }, { 2, 'b'}, {3, 'c'}, {4, 'd'}, {5, 'e'} });

			spandex::SparseArray<char> b(10);
			b.Insert(1, 'a');
			b.Insert(5, 'e');
			b.Insert(3, 'c');
			b.Insert(2, 'b');
			b.Insert(4, 'd');

			Assert::IsTrue(a.Equals(b));
		}

		TEST_METHOD(Equals_2)
		{
			spandex::SparseArray<char> a(10);
			a.Insert(1, 'a');
			a.Insert(2, 'b');
			a.Insert(3, 'c');
			a.Insert(4, 'd');
			a.Insert(5, 'e');

			spandex::SparseArray<char> b(10);
			b.Insert(1, 'a');
			b.Insert(5, 'e');
			b.Insert(3, 'c');
			b.Insert(2, 'b');
			b.Insert(4, 'd');

			Assert::IsTrue(b.Equals(a));
		}

		TEST_METHOD(Equals_3)
		{
			spandex::SparseArray<char> a(20);
			a.Insert(1, 'a');
			a.Insert(2, 'b');
			a.Insert(3, 'c');
			a.Insert(4, 'd');
			a.Insert(5, 'e');

			spandex::SparseArray<char> b(10);
			b.Insert(1, 'a');
			b.Insert(5, 'e');
			b.Insert(3, 'c');
			b.Insert(2, 'b');
			b.Insert(4, 'd');

			Assert::IsFalse(b.Equals(a));
		}

		TEST_METHOD(Equals_4)
		{
			spandex::SparseArray<char> a(20);
			a.Insert(1, 'a');

			spandex::SparseArray<char> b(10);
			b.Insert(1, 'b');

			Assert::IsFalse(b.Equals(a));
		}

		TEST_METHOD(Equals_5)
		{
			spandex::SparseArray<char> a(4, { { 0, 'a' }, { 1, 'b'}, {2, 'c'}, {3, 'd'} });
			spandex::SparseArray<char> b({ 'a', 'b', 'c', 'd' });

			Assert::IsTrue(a.Equals(b));
		}
	};
}