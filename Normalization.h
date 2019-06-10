#pragma once

#include "SparseMatrix.h"

#include <cmath>
#include <stdexcept>
#include <vector>

namespace spandex
{
	class Normalization
	{
	public:
		enum Type
		{
			NoNormalization,
			Pivots
		};

		template<class T>
		static std::vector<T> NormTo(Type type, SparseMatrix<T>& a)
		{
			if (Type::Pivots == type)
			{
				return std::move(NormPivotsTo(a));
			}
			else
			{
				std::vector<T> norm(a.columnCount, (T)1);
				return std::move(norm);
			}
		}

		template<class T>
		static std::vector<T> NormPivotsTo(SparseMatrix<T>& a)
		{
			T zero = (T)0;
			T one = (T)1;
			std::vector<T> norm(a.columnCount);

			for (int j = 0; j < a.columnCount; j++)
			{
				if (a.columns[j] == a.columns[j + 1])
				{
					throw std::runtime_error("should be positive defined");
				}
				int k = a.columns[j];

				if (a.values[k] == zero)
				{
					a.values[k] = one;
				}

				norm[j] = one / std::sqrt(a.values[k]);

				a.values[k] = one;
			}

			for (int j = 0; j < a.columnCount; j++)
			{
				double columnNorm = norm[j];

				for (int i = a.columns[j] + 1; i < a.columns[j + 1]; i++)
				{
					double rowNorm = norm[a.columnsRows[i]];

					a.values[i] *= columnNorm * rowNorm;
				}
			}

			return std::move(norm);
		}
	};
}