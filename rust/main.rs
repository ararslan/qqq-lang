use std::fs::File;
use std::io::{Read, Write, BufReader};
use std::path::Path;

fn parse_instructions(code: String) -> Vec<char> {
    let mut instructions = Vec::new();
    let mut is_open = true;
    let mut bracket_depth = 0;
    while let Some(c) = code.chars().next() {
        match c {
            '!' | '?' | '.' | ',' | '-' | ';' => instructions.push(c),
            '\'' => {
                is_open = !is_open;
            },
            '"' if is_open => {
                bracket_depth += 1;
                instructions.push('[');
            },
            '"' if !is_open => {
                bracket_depth -= 1;
                instructions.push(']');
            },
            _ => continue,
        }
    }
    if bracket_depth != 0 {
        panic!("Unmatched quotes detected!")
    }
    instructions
}

fn evaluate(instructions: Vec<char>) {
    if instructions.is_empty() {
        return;
    }
    let mut tape = vec![0x00];
    let mut ins_ptr = 0; // instruction pointer: index in `instructions` of current instruction
    let mut mem_ptr = 0; // memory pointer: index in `tape` of current memory cell
    let mut input = std::io::stdin().bytes();
    while ins_ptr != instructions.len() - 1 {
        match instructions[ins_ptr] {
            '-' => {
                // can't travel left of the starting cell
                if mem_ptr != 0 { mem_ptr -= 1 }
            },
            ';' => {
                mem_ptr += 1;
                // lazily grow the tape as needed
                if mem_ptr > tape.len() - 1 { tape.resize(mem_ptr + 1, 0x00) }
            },
            '.' => {
                tape[mem_ptr] += 0x01;
            },
            ',' => {
                tape[mem_ptr] -= 0x01;
            },
            '?' => {
                let next_from_stdin = input.next().and_then(|r| r.ok()).map(|b| b as u8);
                tape[mem_ptr] = next_from_stdin.unwrap_or(0x00);
            },
            '!' => {
                print!("{}", tape[mem_ptr] as char);
                std::io::stdout().flush().unwrap();
            },
            '[' => {
                if tape[mem_ptr] == 0x00 {
                    let mut balance = 1;
                    // find the corresponding closing bracket, move the ip to it
                    loop {
                        ins_ptr += 1;
                        if instructions[ins_ptr] == '[' {
                            balance += 1
                        } else if instructions[ins_ptr] == ']' {
                            balance -= 1
                        }
                        if balance == 0 { break }
                    }
                }
            },
            ']' => {
                let mut balance = 0;
                // find the corresponding opening bracket, move the ip just before it
                loop {
                    if instructions[ins_ptr] == '[' {
                        balance += 1
                    } else if instructions[ins_ptr] == ']' {
                        balance -= 1
                    }
                    ins_ptr -= 1;
                    if balance == 0 { break }
                }
            }
            _ => unreachable!(),
        }
        ins_ptr += 1
    }
}

fn main() {
    let args: Vec<String> = std::env::args().collect();

    // TODO: We end up allocating a string up here regardless of whether a file is
    // provided at the command line due to lifetime/borrowing weirdness. Surely there's
    // a better way than this.
    let mut input = String::new();

    let path = if args.len() == 1 {
        println!("Enter a file:");
        std::io::stdin().read_line(&mut input).expect("Failed to read input");
        Path::new(input.trim())
    } else {
        Path::new(&args[1])
    };

    let file = File::open(&path).expect("File not found");
    let mut buf_reader = BufReader::new(&file);
    let mut code = String::new();
    buf_reader.read_to_string(&mut code).expect("Failed to read code from file");

    let instr = parse_instructions(code);
    evaluate(instr);
}
