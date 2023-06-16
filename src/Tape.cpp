//
// Created by Борис Новоселов on 12.06.2023.
//
#include <thread>
#include <sys/stat.h>
#include <iostream>
#include "../include/Tape.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

int FileTape::read_cell() {
    int value;
    file.read(reinterpret_cast<char *>(&value), sizeof(int));
    file.seekg(-sizeof(int), std::ios::cur);
    std::this_thread::sleep_for(std::chrono::milliseconds(rw_delay));
    return value;
}

void FileTape::write_cell(int value) {
    file.write(reinterpret_cast<char *>(&value), sizeof(int));
    if (size < file.tellg() / sizeof(int)) size++;
    file.seekg(-sizeof(int), std::ios::cur);
    std::this_thread::sleep_for(std::chrono::milliseconds(rw_delay));
}

void FileTape::move_forward() {
    file.seekg(sizeof(int), std::ios::cur);
    std::this_thread::sleep_for(std::chrono::milliseconds(fb_delay));
}

void FileTape::move_backward() {
    file.seekg(-sizeof(int), std::ios::cur);
    std::this_thread::sleep_for(std::chrono::milliseconds(fb_delay));
}

void FileTape::reset() {
    file.seekg(0, std::ios::beg);
    std::this_thread::sleep_for(std::chrono::milliseconds(rewind_delay));
}

int FileTape::get_length() {
    return size;
}

int FileTape::get_current_pos() {
    return file.tellg();
}

void FileTape::set_current_pos(int pos) {
    file.seekg(pos * sizeof(int), std::ios::beg);
}

FileTape::FileTape(const std::string &config) {
    boost::property_tree::ptree propertyTree;
    boost::property_tree::read_json(config, propertyTree);

    auto inp = propertyTree.get<std::string>("input_file");
    out_file = propertyTree.get<std::string>("output_file");
    fb_delay = propertyTree.get<uint8_t>("fb_delay");
    rewind_delay = propertyTree.get<uint8_t>("rewind_delay");
    rw_delay = propertyTree.get<uint8_t>("rw_delay");
    pos_delay = propertyTree.get<uint8_t>("pos_delay");

    file.open(inp, std::ios::in | std::ios::out | std::ios::binary);
    file.seekg(0, std::ios::end);
    size = file.tellg() / sizeof(int);
    file.seekg(0, std::ios::beg);
}


FileTape::~FileTape() {
    file.close();
    if (!tmp_fname.empty())
        std::remove(tmp_fname.c_str());
}

inline bool is_file_exist(const std::string &name) {
    struct stat buffer{};
    return (stat(name.c_str(), &buffer) == 0);
}

std::string create_tmp_file_name() {
    std::string name;

    do {
        uint32_t rand = random();
        name = "tmp/tmp" + std::to_string(rand);
    } while (is_file_exist(name));
    return name;
}

FileTape::FileTape(int delay, int rewind_delay, int rw_delay, int pos_delay, std::string &out_file) {
    tmp_fname = create_tmp_file_name();
    std::ofstream outfile(tmp_fname);
    outfile.close();

    file.open(tmp_fname, std::ios::in | std::ios::out | std::ios::binary);

    file.seekg(0, std::ios::end);
    if (file.tellg() != -1)
        size = file.tellg() / sizeof(int);
    else
        size = 0;
    file.seekg(0, std::ios::beg);

    this->fb_delay = delay;
    this->rewind_delay = rewind_delay;
    this->rw_delay = rw_delay;
    this->pos_delay = pos_delay;
    this->out_file = out_file;
}

Tape *FileTape::create_tape() {
    Tape *ptr = new FileTape(fb_delay, rewind_delay, rw_delay, pos_delay, out_file);
    return ptr;
}

void FileTape::save_to_file(const std::string &filename) {
    std::ofstream outfile(filename);
    outfile.close();

    std::fstream f(filename, std::ios::in | std::ios::out | std::ios::binary);

    if (!f) {
        std::cerr << "Failed to open the file for writing." << std::endl;
        return;
    }

    this->reset();
    for (int i = 0; i < size; ++i) {
        int value = this->read_cell();
        f.write(reinterpret_cast<const char *>(&value), sizeof(value));
        this->move_forward();
    }
    f.close();
}

void FileTape::save_to_file() {
    save_to_file(out_file);
}