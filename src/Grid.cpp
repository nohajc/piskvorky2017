//
// Created by nohajc on 15.4.17.
//

#include <algorithm>

#include "Game.h"
#include "Grid.h"

Grid::row_iterator Grid::row_begin(grid_size_t rowIdx) {
    return data[rowIdx].begin();
}

Grid::row_iterator Grid::row_end(grid_size_t rowIdx) {
    return data[rowIdx].end();
}

Grid::row_iterator Grid::row_at(grid_size_t rowIdx, grid_size_t colIdx) {
    return std::next(row_begin(rowIdx), colIdx);
}

Grid::row_const_iterator Grid::row_begin(grid_size_t rowIdx) const {
    return data[rowIdx].begin();
}

Grid::row_const_iterator Grid::row_end(grid_size_t rowIdx) const {
    return data[rowIdx].end();
}

Grid::row_const_iterator Grid::row_at(grid_size_t rowIdx, grid_size_t colIdx) const {
    return std::next(row_begin(rowIdx), colIdx);
}


Grid::col_iterator Grid::col_begin(grid_size_t colIdx) {
    return col_iterator(data.begin(), colIdx);
}

Grid::col_iterator Grid::col_end(grid_size_t colIdx) {
    return col_iterator(data.end(), colIdx);
}

Grid::col_iterator Grid::col_at(grid_size_t rowIdx, grid_size_t colIdx) {
    return std::next(col_begin(colIdx), rowIdx);
}

Grid::col_const_iterator Grid::col_begin(grid_size_t colIdx) const {
    return col_const_iterator(data.begin(), colIdx);
}

Grid::col_const_iterator Grid::col_end(grid_size_t colIdx) const {
    return col_const_iterator(data.end(), colIdx);
}

Grid::col_const_iterator Grid::col_at(grid_size_t rowIdx, grid_size_t colIdx) const {
    return std::next(col_begin(colIdx), rowIdx);
}


Marking &Grid::operator[](Cell c) {
    return data[c.y][c.x];
}

const Marking &Grid::operator[](Cell c) const {
    return data[c.y][c.x];
}

grid_size_t Grid::size() const {
    return (grid_size_t)data.size();
}

Grid::main_diag_iterator Grid::main_diag_at(grid_size_t rowIdx, grid_size_t colIdx) {
    return main_diag_iterator(data.begin(), rowIdx, colIdx);
}

Grid::main_diag_const_iterator Grid::main_diag_at(grid_size_t rowIdx, grid_size_t colIdx) const {
    return main_diag_const_iterator(data.begin(), rowIdx, colIdx);
}

Grid::anti_diag_iterator Grid::anti_diag_at(grid_size_t rowIdx, grid_size_t colIdx) {
    return anti_diag_iterator(data.begin(), rowIdx, colIdx);
}

Grid::anti_diag_const_iterator Grid::anti_diag_at(grid_size_t rowIdx, grid_size_t colIdx) const {
    return anti_diag_const_iterator(data.begin(), rowIdx, colIdx);
}

grid_size_t Grid::boundCheck(int x) const {
    if (x < 0) {
        return 0ul;
    }
    else if (x >= size()) {
        return size() - 1;
    }
    return (grid_size_t)x;
}

Cell Grid::boundCheckMainDiag(Cell c, int shift) const {
    auto gridSize = size();
    if (shift > 0) {
        unsigned actualShift = std::min({gridSize - 1 - c.x, gridSize - 1 - c.y, (unsigned)shift});
        return {c.x + actualShift, c.y + actualShift};
    }

    unsigned actualShift = std::min({c.x, c.y, (unsigned)(-shift)});
    return {c.x - actualShift, c.y - actualShift};
}

Cell Grid::boundCheckAntiDiag(Cell c, int shift) const {
    auto gridSize = size();
    if (shift > 0) {
        unsigned actualShift = std::min({c.x, gridSize - 1 - c.y, (unsigned)shift});
        return {c.x - actualShift, c.y + actualShift};
    }

    unsigned actualShift = std::min({gridSize - 1 - c.x, c.y, (unsigned)(-shift)});
    return {c.x + actualShift, c.y - actualShift};
}

unsigned Grid::longestRowAround(Cell c, unsigned radius) const {
    // Do four sweeps around the newly filled cell
    // (horizontal, vertical and two diagonal)
    // and search for a row of crosses or naughts
    Marking marking = (*this)[c];

//    printf("c.x = %u, c.y = %u, leftX = %d, rightX = %d, topY = %d, bottomY = %d\n",
//           c.x, c.y, leftX, rightX, topY, bottomY);

    auto left = row_at(c.y, boundCheck(c.x - radius));
    auto right = row_at(c.y, boundCheck(c.x + radius));

    auto top = col_at(boundCheck(c.y - radius), c.x);
    auto bottom = col_at(boundCheck(c.y + radius), c.x);

    Cell chkTopLeft = boundCheckMainDiag(c, -radius);
    auto topLeft = main_diag_at(chkTopLeft.y, chkTopLeft.x);
    Cell chkBottomRight = boundCheckMainDiag(c, radius);
    auto bottomRight = main_diag_at(chkBottomRight.y, chkBottomRight.x);

    Cell chkTopRight = boundCheckAntiDiag(c, -radius);
    auto topRight = anti_diag_at(chkTopRight.y, chkTopRight.x);
    Cell chkBottomLeft = boundCheckAntiDiag(c, radius);
    auto bottomLeft = anti_diag_at(chkBottomLeft.y, chkBottomLeft.x);

    return std::max({checkForRowInRange(left, right, marking),
                     checkForRowInRange(top, bottom, marking),
                     checkForRowInRange(topLeft, bottomRight, marking),
                     checkForRowInRange(topRight, bottomLeft, marking)});
}
