# spandex
Fast and simple sparse matrix library

```c++
spandex::misc::CommonGraph<double> g(3);
g.Insert(0, 1, 1);
g.Insert(0, 2, 1);
g.Insert(1, 0, 2);
g.Insert(1, 1, 4);
g.Insert(1, 2, -2);
g.Insert(2, 1, 3);
g.Insert(2, 2, 15);
auto a = spandex::SparseMatrix<double>::FromGraph(3, 3, g);

spandex::CholeskySolver<double> solver(3, 3);
solver.permutation = spandex::Permutation::Type::AMD;
solver.normalization = spandex::Normalization::Type::Pivots;

solver.SolveSym(a);

std::vector<double> b{ 17, 2.89, -3.3 };

auto x = solver.Solve(a, b);

spandex::SparseArray<double> mod({ 7.0, -5.0, 1.0 });
auto u = solver.Update(mod, 9);
auto v = solver.Downdate(mod, 9);
```
