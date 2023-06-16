//
// Created by Борис Новоселов on 16.06.2023.
//
#define BOOST_TEST_MODULE MyTest

#include <boost/test/included/unit_test.hpp>
#include "../include/Tape.h"
#include "../include/SortingAlgorithm.h"
#include <vector>

BOOST_AUTO_TEST_CASE(preTest)
{
    std::string content = R"({
  "rewind_delay": 0,
  "fb_delay": 0,
  "pos_delay": 0,
  "rw_delay": 0,
  "input_file": "test_file.bin",
  "output_file": "test_output.bin"
})";

    std::ofstream file("config_test.json");
    if (file.is_open()) {
        file << content;
        file.close();
        std::cout << "File created successfully." << std::endl;
    } else {
        std::cout << "Unable to create file." << std::endl;
    }
}

BOOST_AUTO_TEST_CASE(oneReadTest) {
    std::ofstream tf("test_file.bin");
    tf.close();

    // Create an instance of the FileTape class
    FileTape tape("config_test.json");
    int number = 42;

    // Write some test data to the file
    std::ofstream file("test_file.bin");
    if (file.is_open()) {
        file.write(reinterpret_cast<const char *>(&number), sizeof(number));
        file.close();
        std::cout << "Integer written to binary file successfully." << std::endl;
    } else {
        std::cout << "Failed to open the binary file." << std::endl;
    }

    std::remove("test_file.bin");
    // Read the cell and check the result
    int value = tape.read_cell();
    BOOST_CHECK_EQUAL(value, number);
}


BOOST_AUTO_TEST_CASE(manyReadTest) {
    std::ofstream tf("test_file.bin");
    tf.close();
    int len = 50;
    FileTape tape("config_test.json");

    std::ofstream file("test_file.bin");
    for (int i = 1; i <= len; i++) {
        file.write(reinterpret_cast<char *>(&i), sizeof(i));
    }
    file.close();

    std::vector<int> result;
    std::vector<int> expected;

    for (int i = 1; i <= len; ++i) {
        result.push_back(tape.read_cell());
        tape.move_forward();
        expected.push_back(i);
    }
    std::remove("test_file.bin");
    BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(),
                                  expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(manyRWTest) {
    std::ofstream tf("test_file.bin");
    tf.close();
    int len = 50;
    FileTape tape("config_test.json");

    for (int i = 0; i < len; i++) {
        tape.write_cell(i);
        tape.move_forward();
    }
    tape.reset();

    std::vector<int> result;
    std::vector<int> expected;

    for (int i = 0; i < len; ++i) {
        result.push_back(tape.read_cell());
        tape.move_forward();
        expected.push_back(i);
    }
    std::remove("test_file.bin");
    BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(),
                                  expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(sortTest) {
    std::ofstream tf("test_file.bin");
    tf.close();
    int len = 50;
    FileTape tape("config_test.json");

    std::vector<int> expected;

    srand(time(NULL));
    for (int i = 0; i < len; i++) {
        int v = rand() % 100;
        tape.write_cell(v);
        tape.move_forward();
        expected.push_back(v);
    }

    std::sort(expected.begin(), expected.end());

    auto *sort_tape = dynamic_cast<FileTape *>(SortingAlgorithm::sort(&tape));
    std::vector<int> result;

    for (int i = 0; i < len; ++i) {
        result.push_back(sort_tape->read_cell());
        sort_tape->move_forward();
    }
    std::remove("test_file.bin");
    BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(),
                                  expected.begin(), expected.end());
}