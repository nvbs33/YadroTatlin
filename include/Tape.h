//
// Created by Борис Новоселов on 12.06.2023.
//

#ifndef TAPE_TAPE_H
#define TAPE_TAPE_H

#include <fstream>

class Tape {
public:
    virtual ~Tape() = default;

    virtual int read_cell() = 0;

    virtual void write_cell(int value) = 0;

    virtual void reset() = 0;

    virtual void move_forward() = 0;

    virtual void move_backward() = 0;

    virtual int get_current_pos() = 0;

    virtual void set_current_pos(int pos) = 0;

    virtual int get_length() = 0;

    virtual Tape *create_tape() = 0;
};


class FileTape : public Tape {
    unsigned long long size;
    std::fstream file;
    std::string tmp_fname;
    std::string out_file;
    int fb_delay;
    int rewind_delay;
    int pos_delay;
    int rw_delay;

    FileTape(int delay, int rewind_delay, int rw_delay, int pos_delay, std::string &out_file);

public:

    FileTape(const std::string &filename);

    ~FileTape() override;

    int read_cell() override;

    void write_cell(int value) override;

    void move_forward() override;

    void move_backward() override;

    void reset() override;

    int get_length() override;

    int get_current_pos() override;

    void set_current_pos(int pos) override;

    Tape *create_tape() override;

    void save_to_file(const std::string &filename);

    void save_to_file();
};

#endif //TAPE_TAPE_H
