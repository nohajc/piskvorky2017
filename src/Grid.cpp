//
// Created by nohajc on 15.4.17.
//

#include "Grid.h"

Grid::row_iterator Grid::row_begin(grid_size_t rowIdx) {
    return data[rowIdx].begin();
}

Grid::row_iterator Grid::row_end(grid_size_t rowIdx) {
    return data[rowIdx].end();
}

Grid::row_const_iterator Grid::begin(grid_size_t rowIdx) const {
    return data[rowIdx].begin();
}

Grid::row_const_iterator Grid::end(grid_size_t rowIdx) const {
    return data[rowIdx].end();
}

Grid::col_iterator Grid::col_begin(grid_size_t colIdx) {
    return col_iterator(data.begin(), colIdx);
}

Grid::col_iterator Grid::col_end(grid_size_t colIdx) {
    return col_iterator(data.end(), colIdx);
}

Grid::col_const_iterator Grid::col_begin(grid_size_t colIdx) const {
    return col_const_iterator(data.begin(), colIdx);
}

Grid::col_const_iterator Grid::col_end(grid_size_t colIdx) const {
    return col_const_iterator(data.end(), colIdx);
}

Marking &Grid::operator[](Cell c) {
    return data[c.y][c.x];
}

const Marking &Grid::operator[](Cell c) const {
    return data[c.y][c.x];
}

grid_size_t Grid::size() const {
    return data.size();
}
