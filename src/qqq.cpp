//
//  qqq.cpp
//  ??? (Programming Language)
//
//  Created by Alex A. and BrainSteel
//  Based heavily on https://gist.github.com/migimunz/964338
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>


typedef char CELL;
typedef std::vector<CELL> instruction_t;
typedef instruction_t::iterator instruction_pointer;

typedef std::vector<CELL> memory_t;
typedef memory_t::iterator memory_pointer;


struct environment {
    memory_t memory;
    instruction_t instructions;

    instruction_pointer ip;
    memory_pointer mp;

    environment() : memory(3000) {
        clear();
        mp = memory.begin();
    }

    void clear() {
        instructions.clear();
        ip = instructions.begin();
    }
};


void interpret(environment &env);
int from_line(environment &env, std::string &line, int open_quotes = 0);
int from_stream(environment &env, std::istream &stream, int open_quotes = 0);
void interactive_mode(environment &env);
void print_cell(CELL cell);


int main(int argc, const char* argv[]) {

    std::string filename;

    if (argc < 2) {
        std::cout << "Enter a file:\n";
        std::cin >> filename;
    } else {
        filename = argv[1];
    }

    std::ifstream infile(filename);

    if (infile.is_open()) {

        environment env;

        int unmatched_quotes = from_stream(env, infile);

        if (unmatched_quotes == 0)
            interpret(env);
        else
            throw std::runtime_error("Um, match up your quotes and try again.");

        infile.close();

    } else {
        throw std::runtime_error("Input file does not exist!");
    }

    return 0;
}


/*
 * Prints a memory cell, or hex representation if unprintable
 */
void print_cell(CELL cell) {

    if (isprint(cell))
        std::cout << cell;
    else
        std::cout << "0x" << std::hex << (int)cell << std::dec;
}


/*
 * Reads code from a line, returns number of unmatched quotes
 */
int from_line(environment &env, std::string &line, int open_quotes) {

    std::istringstream stream(line);

    return from_stream(env, stream, open_quotes);
}


/*
 * Reads code from a stream into env.instructions
 */
int from_stream(environment &env, std::istream &stream, int open_quotes) {

    if (open_quotes == 0)
        env.instructions.push_back('\0');

    CELL cell;

    bool is_open = true;

    while (true) {
        stream >> cell;

        if (!stream)
            break;

        switch (cell) {
            case '!':
            case '?':
            case '.':
            case ',':
            case '-':
            case ';':
                env.instructions.push_back(cell);
                break;

            case '\'':
                is_open = !is_open;
                break;

            case '"':
                if (is_open) {
                    open_quotes++;
                    env.instructions.push_back('[');
                } else {
                    open_quotes--;
                    env.instructions.push_back(']');
                }
                break;

            default:
                break;
        }
    }

    if (open_quotes == 0)
        env.instructions.push_back('\0');

    return open_quotes;
}

/*
 * Finds the closing bracket, moves the ip to it
 */
void find_closing(environment &env)
{
    int balance = 1;
    do {
        env.ip++;
        if (*env.ip == '[')
            balance++;
        else if (*env.ip == ']')
            balance--;
    } while (balance != 0);
}

/*
 * Finds the opening bracket, moves the ip to one cell before it
 */
void find_opening(environment &env)
{
    int balance = 0;
    do {
        if (*env.ip == '[')
            balance++;
        else if (*env.ip == ']')
            balance--;
        env.ip--;
    } while (balance != 0);
}

/*
 * Interprets code
 */
void interpret(environment &env) {

    env.ip = env.instructions.begin();

    while (env.ip != env.instructions.end()) {

        switch (*env.ip) {
            case '.':
                (*env.mp)++;
                env.ip++;
                break;

            case ',':
                (*env.mp)--;
                env.ip++;
                break;

            case ';':
                if (env.mp != (env.memory.end()--))
                    env.mp++;
                env.ip++;
                break;

            case '-':
                if (env.mp != env.memory.begin())
                    env.mp--;
                env.ip++;
                break;

            case '!':
                print_cell(*env.mp);
                env.ip++;
                break;

            case '?':
                CELL cell;
                std::cin >> cell;
                (*env.mp) = cell;
                env.ip++;
                break;

            case '[':
                if (!(*env.mp))
                    find_closing(env);
                env.ip++;
                break;

            case ']':
                find_opening(env);
                env.ip++;
                break;

            case '\0':
                env.ip++;
                break;
        }
    }
}
