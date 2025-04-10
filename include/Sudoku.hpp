#ifndef SUDOKU_SOLVER_INCLUDE_SUDOKU_HPP_
#define SUDOKU_SOLVER_INCLUDE_SUDOKU_HPP_

#include <array>
#include <bitset>
#include <iomanip>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/**
 * @enum House
 * @brief Used as a parameter to tell functions which sets of cell indices to
 *        work with -- rows, columns, or boxes.
 */
enum class House { row, column, box };

/**
 * @struct Subset
 * @brief Holds the cell indices and values of a naked subset
 */
struct Subset {
  std::unordered_set<int> set_cells{};      ///< the cells in the subset
  std::unordered_set<int> set_candidates{}; ///< the values in the subset
};

/**
 * @enum SetSize
 * @brief Used as a parameter to tell functions the size of subset we are
 *        looking for.
 */
enum class SetSize : std::size_t { pair = 2, triple = 3, quad = 4 };

/**
 * @class Sudoku
 * @brief Represents a Sudoku puzzle and tracks all information needed to
 *        (attempt to) solve that puzzle.
 */
class Sudoku {
 public:
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

  /**
   * @brief   Remove erroneous candidates identified by naked subsets
   * @param   set_size Pair, triple, or quad?
   * @return  True - at least one candidate was removed \n
   *          False - no candidates were removed
   */
  bool prune_naked_subsets(SetSize set_size);

  /**
   * @brief   Remove erroneous candidates identified by hidden subsets
   * @param   set_size Pair, triple, or quad?
   * @return  True - at least one candidate was removed \n
   *          False - no candidates were removed
   */
  bool prune_hidden_subsets(SetSize set_size);

  /**
   * @brief   Remove erroneous candidates identified by box line reduction
   * @return  True - at least one candidate was removed \n
   *          False - no candidates were removed
   */
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

  /// _candidates[cell] = {all candidates for that cell}
  std::vector<std::vector<int>> _candidates{PUZZLE_SIZE};

  /// tracks the location and values of hidden singles
  std::array<int, 81> _singles{};

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
   * @brief Happens once per Sudoku object during construction. \n
   * We find all the "candidates" (possible values) for each unknown cell. \n
   * The whole purpose of the methods in this library is to reduce this list of
   * candidates until there are no more unknowns -- or until we are sad because
   * the puzzle is too difficult.
   */
  void initialize_candidates();

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

  /**
   * @brief   Search the puzzle for a naked subset of size set_size
   * @param   good_cells Cells with number of candidates n
   *          such that 2 <= n <= set_size
   * @param   set_size Are we looking for a pair (2), triple (3), or quad (4)?
   * @return  An optional Subset object which contains the indices of the
   *          cells in which the subset is located and the values in the subset;
   *          std::nullopt if no naked subset is found.
   */
  std::optional<Subset> find_naked_subset(const std::vector<int> &good_cells,
                                          SetSize set_size);

  /**
   * @brief   Search the puzzle for a hidden subset of size set_size
   * @param   good_candidates Candidates with a house frequency of n
   *          such that 2 <= n <= set_size
   * @param   candidates_to_cells Associative map of a candidate to the house
   *          cells in which it appears.
   * @param   set_size Are we looking for a pair (2), triple (3), or quad (4)?
   * @return  An optional Subset object which contains the indices of the
   *          cells in which the subset is located and the values in the subset;
   *          std::nullopt if no hidden subset is found.
   * @note    It may be better to maintain a map of candidates to cells in which
   *          they appear, grouped by houses, so that the caller of this
   *          function doesn't have to evaluate such a map each time this
   *          function is called.
   */
  static std::optional<Subset> find_hidden_subsets(
      const std::vector<int> &good_candidates,
      const std::unordered_map<int, std::vector<int>> &candidates_to_cells,
      SetSize set_size);

  /**
   * @brief   A helper for the public method -- the current implementation
   *          requires the type of house to be known in order to figure out
   *          which box we are working with
   * @param   tag rows or columns?
   * @return  True - at least one candidate was removed \n
   *          False - no candidates were removed
   */
  bool prune_locked_claiming_candidates(House tag);

  /**
   * @brief   Search all rows, or all columns, for claimed candidates.
   * @param   houses Rows or columns?
   * @param   house Which row or column?
   * @return  An optional 2d vector containing the claimed candidates -- the
   *          index of the sub vector corresponds to the intersection of the
   *          house and the box that claims that candidate. [0,2] represents
   *          boxes left to right if a row is being used, or top to bottom if a
   *          column is being used; returns std::nullopt if no claimed
   *          candidates are found
   */
  std::optional<std::vector<std::vector<int>>>
  find_claimed_candidates(const std::array<std::array<int, 9>, 9> &houses,
                          int house);
};

#endif // SUDOKU_SOLVER_INCLUDE_SUDOKU_HPP_
