#ifndef SUDOKU_SOLVER_INCLUDE_SUDOKU_HPP_
#define SUDOKU_SOLVER_INCLUDE_SUDOKU_HPP_

#include <array>
#include <bitset>
#include <iomanip>
#include <vector>

class Sudoku {
 public:
  /**
   * @brief Used as a parameter to tell functions which sets of cell indices to
   *        work with -- rows, columns, or boxes.
   */
  enum class House { row, column, box };

  /**
   * @brief   Construct a Sudoku object from a vector of integers representing
   *          the puzzle.
   * @param   puzzle 81 integers [0,9] -- 0 represents an unknown cell
   * @throws  std::exit if the puzzle is malformed. Should probably figure
   *          out a better way of handling this ... ?
   */
  explicit Sudoku(const std::vector<int> &puzzle) : _puzzle{puzzle} {
    check_puzzle();
    initialize_candidates();
  };

  /**
   * @brief   Construct a Sudoku object from a string of integers representing
   *          the puzzle.
   * @param   str 81 integers [0,9] -- 0 represents an unknown cell
   * @throws  std::exit if the puzzle is malformed. Should probably figure
   *          out a better way of handling this ... ?
   */
  explicit Sudoku(const std::string &str) {
    for (const auto ch : str) {
      _puzzle.push_back(ch - '0');
    }
    while (_puzzle.size() > 81) {
      _puzzle.pop_back();
    }
    check_puzzle();
    initialize_candidates();
  }

  /**
   * @brief   Verify that the puzzle does not contain duplicate values in houses
   *          -- pretty much the only rule of Sudoku.
   * @throws  std::exit if the puzzle is malformed. Should probably figure out
   *          a better way of handling this ... ?
   */
  void check_puzzle() const;

  /**
   * @brief Happens once per Sudoku object during construction. \n
   * We find all the "candidates" (possible values) for each unknown cell. \n
   * The whole purpose of the methods in this library is to reduce this list of
   * candidates until there are no more unknowns -- or until we are sad because
   * the puzzle is too difficult.
   */
  void initialize_candidates();

  /**
   * @brief   The most fundamental solving technique in Sudoku -- if a cell has
   *          only one possible value, it has only one possible value lol.
   * @return  True - at least one naked single has been found and solved \n
   *          False - a naked single could not be found
   */
  bool solve_naked_singles();

  /**
   * @brief   The second most fundamental solving technique in Sudoku -- if a
   *          value appears only once in a house, the cell in which it appears
   *          must take on that value.
   * @return  True - at least one hidden single has been found and solved \n
   *          False - a hidden single could not be found
   */
  bool solve_hidden_singles();

  bool solve();

  bool prune_naked_subset(std::size_t n);
  bool prune_hidden_subsets(std::size_t n);
  bool prune_locked_claiming_candidates();
  bool prune_locked_pointing_candidates();

  void print_puzzle() const;
  bool prune_x_wings(std::size_t n);
  static void print_puzzle(const std::vector<int> &vec);
  void print_unknown() const;
  static void print_candidates(const std::string &str,
                               const std::vector<std::vector<int>> &vec);
  void print_singles() const;

  void print_readout() const;

 private:
  static constexpr int UNKNOWN = 0;      ///< code for unknown cell value
  static constexpr int PUZZLE_SIZE = 81; ///< 81 cells in the puzzle

  std::vector<int> _puzzle;            ///< the puzzle
  std::bitset<PUZZLE_SIZE> _unknown{}; ///< tracks unknown cells
  std::vector<std::vector<int>> _candidates{PUZZLE_SIZE}; ///< tracks candidates
  std::array<int, 81> _singles{}; ///< tracks the location of hidden singles

  int _naked_singles{};  ///< number of naked singles solved
  int _hidden_singles{}; ///< number of hidden singles solved

  /**
   * @brief Tracks the number of candidates pruned by a given technique
   * @note Not completely implemented throughout all the techniques
   */
  struct _candidates_pruned_by {
    int _claiming_locked{};
    int _pointing_locked{};
    int _subset_hidden{};
    int _subset_naked{};
    int _xwing{};
  };

  _candidates_pruned_by _candidates_pruned_by{}; ///< track technique outcomes

  [[nodiscard]] bool puzzle_is_valid() const;
  [[nodiscard]] bool check_houses(House tag) const;

  // initialize candidates
  void find_unknown_Indices();

  /**
   * @brief A cell's candidates are the complement of the set of all known
   *        values among that cell's houses; the candidates for a cell are all
   *        the values that do not yet exist in that cell's row, column, and
   *        box.
   * @param cell The cell for which we wish to find the candidate values.
   */
  void initialize_candidates(int cell);

  /**
   * @brief Called whenever a cell is solved to remove that value from the
   *        candidate lists of all other cells the houses.
   * @param cell The cell that has been solved.
   */
  void update_candidates(int cell);

  /**
   * @brief   Search the whole puzzle for as many hidden singles as we can find.
   * @return  True - at least one hidden single has been found \n
   *          False - a hidden single has not been found
   */
  bool find_hidden_singles();

  /**
   * @brief   A hidden single a value that appears only once among all the
   *          candidates of a house.
   * @param   tag The houses to search
   * @return  True - a hidden single has been found \n
   *          False - a hidden single has not been found
   */
  bool find_hidden_singles(House tag);

  // locked claiming
  bool prune_locked_claiming_candidates(House tag);
};

#endif // SUDOKU_SOLVER_INCLUDE_SUDOKU_HPP_
