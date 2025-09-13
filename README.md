# SudokuSolver

A C++ library for solving
[Sudoku](https://en.wikipedia.org/wiki/Sudoku)
puzzles using human-style techniques.
In contrast to
[computer-centric algorithms](https://en.wikipedia.org/wiki/Sudoku_solving_algorithms)
like brute-force or stochastic search,
this library implements methods that a person with pencil and paper would use.

## Table of contents

- [About](#about)
- [Techniques](#techniques)
- [Resources](#resources)
- [License](#license)

## About

A small number of techniques have been implemented.
In concert, they are enough to solve most "hard" puzzles.

## Techniques

Techniques implemented thus far:

* naked/hidden singles
* naked/hidden subsets
* locked claiming/pointing candidates
* x-wings
* perfect swordfish
* perfect jellyfish

## Resources

The construction of this software was informed by the following:

- Bernhard Hobiger's [Hodoku - Solving Techniques](https://hodoku.sourceforge.net/en/techniques.php)
- Andrew Stuart's [SudokuWiki - Strategy Families](https://www.sudokuwiki.org/Strategy_Families)

## License

This software is made available under the [MIT License](./LICENSE.md). 
