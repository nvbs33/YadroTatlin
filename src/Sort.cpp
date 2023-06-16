//
// Created by Борис Новоселов on 13.06.2023.
//

#include <vector>
#include "../include/SortingAlgorithm.h"

#define CHUNK_SIZE 256


void in_place_sort(Tape *tape) {
    std::vector<int> list(tape->get_length());

    for (int i = 0; i < tape->get_length(); ++i) {
        list[i] = tape->read_cell();
        if (i != tape->get_length() - 1) tape->move_forward();
    }
    std::make_heap(list.begin(), list.end());
    std::sort_heap(list.begin(), list.end());

    for (int i = tape->get_length() - 1; i >= 0; --i) {
        tape->write_cell(list[i]);
        if (i != 0) tape->move_backward();
    }
}

void merge2(Tape *tape1, Tape *tape2, Tape *tape3) {
    int i = 0, j = 0;
    while (i < tape1->get_length() && j < tape2->get_length()) {
        int v1 = tape1->read_cell();
        int v2 = tape2->read_cell();
        if (v1 < v2) {
            tape3->write_cell(v1);
            i++;
            tape1->move_forward();
        } else {
            tape3->write_cell(v2);
            j++;
            tape2->move_forward();
        }
        tape3->move_forward();
    }

    while (i < tape1->get_length()) {
        tape3->write_cell(tape1->read_cell());
        tape3->move_forward();
        tape1->move_forward();
        i++;
    }

    while (j < tape2->get_length()) {
        tape3->write_cell(tape2->read_cell());
        tape3->move_forward();
        tape2->move_forward();
        j++;
    }
}


Tape *merge_sort(std::vector<Tape *> tapes) {
    int num_tapes = tapes.size();

    // Step 1: Sort each chunk
    for (int i = 0; i < num_tapes; i++) {
        in_place_sort(tapes.at(i));
    }
    if (num_tapes == 1) return tapes[0];
    // Step 2: Merge the sorted chunks
    auto tape_res = tapes[0]->create_tape();
    for (int i = 1; i < num_tapes; ++i) {
        merge2(tapes.at(i - 1), tapes.at(i), tape_res);
    }
    for (int i = 0; i < num_tapes; ++i)
        delete tapes[i];

    tape_res->reset();
    return tape_res;
}

Tape *SortingAlgorithm::sort(Tape *t) {
    t->reset();
    int num_tapes = t->get_length() / CHUNK_SIZE;
    int remainder = t->get_length() % CHUNK_SIZE;

    std::vector<Tape *> tapes;
    for (int i = 0; i < num_tapes; ++i) {
        Tape *tape = t->create_tape();
        for (int j = 0; j < CHUNK_SIZE; ++j) {
            int v = t->read_cell();
            tape->write_cell(v);
            t->move_forward();
            tape->move_forward();
        }
        tape->reset();
        tapes.push_back(tape);
    }

    if (remainder != 0) {
        Tape *tape = t->create_tape();
        for (int i = 0; i < remainder; ++i) {
            int v = t->read_cell();
            tape->write_cell(v);
            t->move_forward();
            tape->move_forward();
        }
        tapes.push_back(tape);
        tape->reset();
    }

    t->reset();
    tapes[0]->reset();
    auto res = merge_sort(tapes);
    return res;
}