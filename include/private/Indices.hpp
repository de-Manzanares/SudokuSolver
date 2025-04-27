#ifndef SUDOKU_SOLVER_INCLUDE_INDICES_HPP_
#define SUDOKU_SOLVER_INCLUDE_INDICES_HPP_

#include <array>

static constexpr std::array<int, 3> intersection(const std::array<int, 9> &a,
                                                 const std::array<int, 9> &b) {
  std::array<int, 3> result{0, 0, 0};
  int idx = 0;
  for (const int i : a) {
    for (const int j : b) {
      if (i == j) {
        result[idx++] = i;
      }
    }
  }
  return result;
}

static constexpr std::array<std::array<int, 9>, 9> initialize_rows() {
  std::array<std::array<int, 9>, 9> rows{};
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      rows[i][j] = i * 9 + j;
    }
  }
  return rows;
}

static constexpr std::array<std::array<int, 9>, 9> initialize_columns() {
  std::array<std::array<int, 9>, 9> rows{};
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      rows[i][j] = i + 9 * j;
    }
  }
  return rows;
}

static constexpr std::array<std::array<int, 9>, 9> initialize_boxes() {
  std::array<std::array<int, 9>, 9> boxes{};
  for (int i = 0; i < 9; i++) {
    const int brow = i / 3;
    const int bcol = i % 3;
    for (int y = 0; y < 3; ++y) {
      for (int x = 0; x < 3; ++x) {
        boxes[i][3 * y + x] = 9 * (3 * brow + y) + (3 * bcol + x);
      }
    }
  }
  return boxes;
}

static constexpr std::array<std::array<int, 3>, 81> initialize_associations() {
  std::array<std::array<int, 3>, 81> associations{};
  for (int i = 0; i < 81; i++) {
    const int row = i / 9;
    const int col = i % 9;
    const int box = (row / 3) * 3 + (col / 3);
    associations[i] = {row, col, box};
  }
  return associations;
}

/**
 * @brief Group cell indices into houses (rows, columns, boxes) and
 * intersections for easy access.
 */
struct Indices {
  /**
   * @brief Group cell indices in to rows \n
   * 0 (top of puzzle) to 8 (bottom of puzzle)
   */
  static constexpr auto rows = initialize_rows();

  /**
   * @brief Group cell indices into columns \n
   * 0 (left of puzzle) to 8 (right of puzzle)
   */
  static constexpr auto columns = initialize_columns();

  /**
   * @brief Group cell indices into boxes, left to right, top to bottom: 0 (top
   * left) to 8 (bottom right)
   */
  static constexpr auto boxes = initialize_boxes();

  /**
   * @brief A way to quickly find which houses a given cell belongs to: \n
   * associations[cell] = {cell's row, cell's column, cell's box}
   */
  static constexpr auto associations = initialize_associations();

  using subrange = std::array<int, 3>;
  static constexpr subrange box0row0 = intersection(boxes[0], rows[0]);
  static constexpr subrange box0row1 = intersection(boxes[0], rows[1]);
  static constexpr subrange box0row2 = intersection(boxes[0], rows[2]);
  static constexpr subrange box0column0 = intersection(boxes[0], columns[0]);
  static constexpr subrange box0column1 = intersection(boxes[0], columns[1]);
  static constexpr subrange box0column2 = intersection(boxes[0], columns[2]);
  //
  static constexpr subrange box1row0 = intersection(boxes[1], rows[0]);
  static constexpr subrange box1row1 = intersection(boxes[1], rows[1]);
  static constexpr subrange box1row2 = intersection(boxes[1], rows[2]);
  static constexpr subrange box1column3 = intersection(boxes[1], columns[3]);
  static constexpr subrange box1column4 = intersection(boxes[1], columns[4]);
  static constexpr subrange box1column5 = intersection(boxes[1], columns[5]);
  //
  static constexpr subrange box2row0 = intersection(boxes[2], rows[0]);
  static constexpr subrange box2row1 = intersection(boxes[2], rows[1]);
  static constexpr subrange box2row2 = intersection(boxes[2], rows[2]);
  static constexpr subrange box2column6 = intersection(boxes[2], columns[6]);
  static constexpr subrange box2column7 = intersection(boxes[2], columns[7]);
  static constexpr subrange box2column8 = intersection(boxes[2], columns[8]);
  //
  static constexpr subrange box3row3 = intersection(boxes[3], rows[3]);
  static constexpr subrange box3row4 = intersection(boxes[3], rows[4]);
  static constexpr subrange box3row5 = intersection(boxes[3], rows[5]);
  static constexpr subrange box3column0 = intersection(boxes[3], columns[0]);
  static constexpr subrange box3column1 = intersection(boxes[3], columns[1]);
  static constexpr subrange box3column2 = intersection(boxes[3], columns[2]);
  //
  static constexpr subrange box4row3 = intersection(boxes[4], rows[3]);
  static constexpr subrange box4row4 = intersection(boxes[4], rows[4]);
  static constexpr subrange box4row5 = intersection(boxes[4], rows[5]);
  static constexpr subrange box4column3 = intersection(boxes[4], columns[3]);
  static constexpr subrange box4column4 = intersection(boxes[4], columns[4]);
  static constexpr subrange box4column5 = intersection(boxes[4], columns[5]);
  //
  static constexpr subrange box5row3 = intersection(boxes[5], rows[3]);
  static constexpr subrange box5row4 = intersection(boxes[5], rows[4]);
  static constexpr subrange box5row5 = intersection(boxes[5], rows[5]);
  static constexpr subrange box5column6 = intersection(boxes[5], columns[6]);
  static constexpr subrange box5column7 = intersection(boxes[5], columns[7]);
  static constexpr subrange box5column8 = intersection(boxes[5], columns[8]);
  //
  static constexpr subrange box6row6 = intersection(boxes[6], rows[6]);
  static constexpr subrange box6row7 = intersection(boxes[6], rows[7]);
  static constexpr subrange box6row8 = intersection(boxes[6], rows[8]);
  static constexpr subrange box6column0 = intersection(boxes[6], columns[0]);
  static constexpr subrange box6column1 = intersection(boxes[6], columns[1]);
  static constexpr subrange box6column2 = intersection(boxes[6], columns[2]);
  //
  static constexpr subrange box7row6 = intersection(boxes[7], rows[6]);
  static constexpr subrange box7row7 = intersection(boxes[7], rows[7]);
  static constexpr subrange box7row8 = intersection(boxes[7], rows[8]);
  static constexpr subrange box7column3 = intersection(boxes[7], columns[3]);
  static constexpr subrange box7column4 = intersection(boxes[7], columns[4]);
  static constexpr subrange box7column5 = intersection(boxes[7], columns[5]);
  //
  static constexpr subrange box8row6 = intersection(boxes[8], rows[6]);
  static constexpr subrange box8row7 = intersection(boxes[8], rows[7]);
  static constexpr subrange box8row8 = intersection(boxes[8], rows[8]);
  static constexpr subrange box8column6 = intersection(boxes[8], columns[6]);
  static constexpr subrange box8column7 = intersection(boxes[8], columns[7]);
  static constexpr subrange box8column8 = intersection(boxes[8], columns[8]);

  /**
   * @brief Groups indices of the intersections between boxes and the other
   * two houses. \n
   * subranges[box] = { \n
   *  {box's top most row, middle row, bottom most row}, \n
   *  {box's left most column, middle column, right most column} \n
   *  }
   *
   *  Probably a bit more convoluted than it needs to be ... ?
   */
  static constexpr std::array<std::array<std::array<const subrange *, 3>, 2>, 9>
      subranges = {{{{{&box0row0, &box0row1, &box0row2},
                      {&box0column0, &box0column1, &box0column2}}},
                    {{{&box1row0, &box1row1, &box1row2},
                      {&box1column3, &box1column4, &box1column5}}},
                    {{{&box2row0, &box2row1, &box2row2},
                      {&box2column6, &box2column7, &box2column8}}},
                    {{{&box3row3, &box3row4, &box3row5},
                      {&box3column0, &box3column1, &box3column2}}},
                    {{{&box4row3, &box4row4, &box4row5},
                      {&box4column3, &box4column4, &box4column5}}},
                    {{{&box5row3, &box5row4, &box5row5},
                      {&box5column6, &box5column7, &box5column8}}},
                    {{{&box6row6, &box6row7, &box6row8},
                      {&box6column0, &box6column1, &box6column2}}},
                    {{{&box7row6, &box7row7, &box7row8},
                      {&box7column3, &box7column4, &box7column5}}},
                    {{{&box8row6, &box8row7, &box8row8},
                      {&box8column6, &box8column7, &box8column8}}}}};
};

#endif // SUDOKU_SOLVER_INCLUDE_INDICES_HPP_
