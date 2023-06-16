#include <iostream>
#include "include/Tape.h"
#include "include/SortingAlgorithm.h"

int main() {
    FileTape tape("./config.json");

    auto *res = dynamic_cast<FileTape *>(SortingAlgorithm::sort(&tape));

    for (int i = 0; i < res->get_length(); ++i) {
        std::cout << res->read_cell() << std::endl;
        res->move_forward();
    }
    res->save_to_file();
    delete res;
    return 0;
}
