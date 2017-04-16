//
// Created by nohajc on 15.4.17.
//

#ifndef PISKVORKY2017_GRID_H
#define PISKVORKY2017_GRID_H

#include <iterator>
#include <vector>
#include <type_traits>
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
            decltype(std::declval<typename std::iterator_traits<Base>::reference>()[0])> {
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

    template<typename Base, int ColIncrement>
    class diag_iter : public boost::iterator_adaptor<
            diag_iter<Base, ColIncrement>,
            Base,
            decltype(std::declval<typename std::iterator_traits<Base>::reference>()[0])> {
    public:
        diag_iter() = default;
        explicit diag_iter(Base it, grid_size_t rowIdx, grid_size_t colIdx) : diag_iter::iterator_adaptor_(it) {
            std::advance(this->base_reference(), rowIdx);
            colIndex = colIdx;
        }

    private:
        friend class boost::iterator_core_access;

        grid_size_t colIndex;

        typename diag_iter::iterator_adaptor_::reference dereference() const {
            return (*this->base())[colIndex];
        }

        bool equal(const diag_iter & other) const {
            return this->base() == other.base() && colIndex == other.colIndex;
        }

        void increment() {
            this->base_reference()++;
            colIndex += ColIncrement;
        }

        void decrement() {
            this->base_reference()--;
            colIndex -= ColIncrement;
        }

        void advance(typename diag_iter::iterator_adaptor_::difference_type n) {
            this->base_reference() += n;
            colIndex += n * ColIncrement;
        }
    };

    typedef GridRowData::iterator row_iterator;
    typedef GridRowData::const_iterator row_const_iterator;

    typedef col_iter<GridData::iterator> col_iterator;
    typedef col_iter<GridData::const_iterator> col_const_iterator;

    typedef diag_iter<GridData::iterator, 1> main_diag_iterator;
    typedef diag_iter<GridData::const_iterator, 1> main_diag_const_iterator;

    typedef diag_iter<GridData::iterator, -1> anti_diag_iterator;
    typedef diag_iter<GridData::const_iterator, -1> anti_diag_const_iterator;

    Grid(grid_size_t n) : data(n, std::vector<Marking>(n)) {}

    row_iterator row_begin(grid_size_t rowIdx);
    row_iterator row_end(grid_size_t rowIdx);
    row_const_iterator row_begin(grid_size_t rowIdx) const;
    row_const_iterator row_end(grid_size_t rowIdx) const;

    col_iterator col_begin(grid_size_t colIdx);
    col_iterator col_end(grid_size_t colIdx);
    col_const_iterator col_begin(grid_size_t colIdx) const;
    col_const_iterator col_end(grid_size_t colIdx) const;

    main_diag_iterator main_diag_at(grid_size_t rowIdx, grid_size_t colIdx);
    main_diag_const_iterator main_diag_at(grid_size_t rowIdx, grid_size_t colIdx) const;

    anti_diag_iterator anti_diag_at(grid_size_t rowIdx, grid_size_t colIdx);
    anti_diag_const_iterator anti_diag_at(grid_size_t rowIdx, grid_size_t colIdx) const;

    // No bounds checking!
    Marking & operator[](Cell c);
    // No bounds checking!
    const Marking & operator[](Cell c) const;

    grid_size_t size() const;
};


#endif //PISKVORKY2017_GRID_H
