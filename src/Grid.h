//
// Created by nohajc on 15.4.17.
//

#ifndef PISKVORKY2017_GRID_H
#define PISKVORKY2017_GRID_H

#include <iterator>
#include <vector>
#include <boost/iterator/iterator_adaptor.hpp>

#include "Player.h"

typedef unsigned long grid_size_t;

enum class Marking {EMPTY = 0, NAUGHT = Player::NAUGHT, CROSS = Player::CROSS};

// Game board object which wraps 2D vector of Marking cells.
// It provides row, column and diagonal iterators
// but also operator[] for individual cell access.
class Grid {
    typedef std::vector<Marking> GridRowData;
    typedef std::vector<GridRowData> GridData;

    GridData data;
public:
    template<typename Base>
    class col_iter : public boost::iterator_adaptor<
            col_iter<Base>,
            Base,
            typename std::iterator_traits<
                    typename std::iterator_traits<Base>::value_type::iterator
            >::value_type> {
    public:
        col_iter() = default;
        explicit col_iter(Base it, grid_size_t colIdx) : col_iter::iterator_adaptor_(it) {
            colIndex = colIdx;
        }

    private:
        friend class boost::iterator_core_access;

        grid_size_t colIndex;

        typename col_iter::iterator_adaptor_::reference dereference() const {
            return (*this->base())[colIndex];
        }

        bool equal(const col_iter & other) const {
            return this->base() == other.base() && colIndex == other.colIndex;
        }
    };

    typedef col_iter<GridData::iterator> col_iterator;
    typedef col_iter<GridData::const_iterator> col_const_iterator;
    typedef GridRowData::iterator row_iterator;
    typedef GridRowData::const_iterator row_const_iterator;

    Grid(grid_size_t n) : data(n, std::vector<Marking>(n)) {}

    row_iterator row_begin(grid_size_t rowIdx);
    row_iterator row_end(grid_size_t rowIdx);
    row_const_iterator begin(grid_size_t rowIdx) const;
    row_const_iterator end(grid_size_t rowIdx) const;

    col_iterator col_begin(grid_size_t colIdx);
    col_iterator col_end(grid_size_t colIdx);
    col_const_iterator col_begin(grid_size_t colIdx) const;
    col_const_iterator col_end(grid_size_t colIdx) const;

    // No bounds checking!
    Marking & operator[](Cell c);
    // No bounds checking!
    const Marking & operator[](Cell c) const;

    grid_size_t size() const;
};


#endif //PISKVORKY2017_GRID_H
