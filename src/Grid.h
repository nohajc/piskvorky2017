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

typedef unsigned grid_size_t;

enum class Marking {EMPTY = 0, NAUGHT = Player::NAUGHT, CROSS = Player::CROSS};

// Game board object which wraps 2D vector of Marking cells.
// It provides row, column and diagonal iterators
// but also operator[] for individual cell access.
class Grid {
    typedef std::vector<Marking> GridRowData;
    typedef std::vector<GridRowData> GridData;

    GridData data;

public:
    // Column iterator works in almost the same way as GridData (vector) iterator which iterates over grid rows.
    // We just need to remember the column index and access the corresponding element when dereferencing.
    // When we test two col_iters for equality, it depends on equality of the underlying
    // vector iterator but also on the equality of column indices.
    // So, we override the implementation of dereference and equal.
    template<typename Base, typename Derived = void> // Derived default cannot be set to col_iter<Base> here
    class col_iter : public boost::iterator_adaptor<
            std::conditional_t<
                    std::is_same<Derived, void>::value,
                    col_iter<Base>, // This is the CRTP default
                    Derived>, // but we also derive diag_iter from col_iter below
            Base,
            // This is the type of value we get when we dereference the outer (row) iterator
            // and then access a specific element in that row. It will either be const or non-const
            // but that depends on the type of Base (which can be iterator or const_iterator).
            decltype(std::declval<typename std::iterator_traits<Base>::reference>()[0])> {
    public:
        col_iter() = default;
        explicit col_iter(Base it, grid_size_t colIdx) : col_iter::iterator_adaptor_(it) {
            colIndex = colIdx;
        }

    protected:
        grid_size_t colIndex;

    private:
        friend class boost::iterator_core_access;

        typename col_iter::iterator_adaptor_::reference dereference() const {
            return (*this->base())[colIndex];
        }

        bool equal(const col_iter & other) const {
            return this->base() == other.base() && colIndex == other.colIndex;
        }
    };

    // Diagonal iterator shares a lot in common with column iterator.
    // We still iterate ove rows but not in a fixed column.
    // The column index changes in sync. Therefore, we just implement
    // increment, decrement and advance but reuse dereference and equality test.
    // To make this work with CRTP (which is used by boost::iterator_adaptor)
    // We need to pass the correct type of our newly derived class.
    // That's what col_iter's Derived parameter is for.
    template<typename Base, int ColIncrement>
    class diag_iter : public col_iter<Base, diag_iter<Base, ColIncrement>> {
    public:
        diag_iter() = default;
        explicit diag_iter(Base it, grid_size_t rowIdx, grid_size_t colIdx) : diag_iter::col_iter(it, colIdx) {
            std::advance(this->base_reference(), rowIdx);
        }

    private:
        friend class boost::iterator_core_access;

        void increment() {
            this->base_reference()++;
            this->colIndex += ColIncrement;
        }

        void decrement() {
            this->base_reference()--;
            this->colIndex -= ColIncrement;
        }

        void advance(typename diag_iter::iterator_adaptor_::difference_type n) {
            this->base_reference() += n;
            this->colIndex += n * ColIncrement;
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
    row_iterator row_at(grid_size_t rowIdx, grid_size_t colIdx);
    row_const_iterator row_begin(grid_size_t rowIdx) const;
    row_const_iterator row_end(grid_size_t rowIdx) const;
    row_const_iterator row_at(grid_size_t rowIdx, grid_size_t colIdx) const;

    col_iterator col_begin(grid_size_t colIdx);
    col_iterator col_end(grid_size_t colIdx);
    col_iterator col_at(grid_size_t rowIdx, grid_size_t colIdx);
    col_const_iterator col_begin(grid_size_t colIdx) const;
    col_const_iterator col_end(grid_size_t colIdx) const;
    col_const_iterator col_at(grid_size_t rowIdx, grid_size_t colIdx) const;

    main_diag_iterator main_diag_at(grid_size_t rowIdx, grid_size_t colIdx);
    main_diag_const_iterator main_diag_at(grid_size_t rowIdx, grid_size_t colIdx) const;

    anti_diag_iterator anti_diag_at(grid_size_t rowIdx, grid_size_t colIdx);
    anti_diag_const_iterator anti_diag_at(grid_size_t rowIdx, grid_size_t colIdx) const;

    // No bounds checking!
    Marking & operator[](Cell c);
    // No bounds checking!
    const Marking & operator[](Cell c) const;

    grid_size_t size() const;

    grid_size_t boundCheck(int x) const;

    Cell boundCheckMainDiag(Cell c, int shift) const;

    Cell boundCheckAntiDiag(Cell c, int shift) const;
};


#endif //PISKVORKY2017_GRID_H
