use regex::Regex;
use std::collections::HashMap;
use std::fs::File;
use std::io::Write;
use std::io::{self, Read};
use std::path::Path;

pub struct Assembler {
    pub file_path: String,
    pub file_content: String,
    symbol_map: HashMap<String, u16>,
    c_comp_instruction_map: HashMap<String, String>,
    c_dest_instruction_map: HashMap<String, String>,
    c_jump_instruction_map: HashMap<String, String>,
    regex_a_instruction_variable: Regex,
    regex_a_instruction_symbol: Regex,
    regex_c_instruction: Regex,
    regex_label_symbol: Regex,
}

enum Instruction {
    AInstruction(AInstruction),
    CInstruction(CInstruction),
}

pub struct AInstruction {
    pub value: String,
}

impl AInstruction {
    pub fn new(value: String) -> Self {
        println!("Value {:?}", value);
        let binary_value = format!("{:016b}", value.parse::<u16>().unwrap_or(0));

        println!("Binary Value {:?}", &binary_value);
        Self {
            value: binary_value,
        }
    }
}

pub struct CInstruction {
    pub dest: String,
    pub comp: String,
    pub jmp: String,
}

impl CInstruction {
    pub fn concatenate(&self) -> String {
        format!("111{}{}{}", self.comp, self.dest, self.jmp)
    }
}
impl Assembler {
    pub fn new(file_path: String) -> io::Result<Self> {
        let content = read_file(&file_path)?;

        let mut symbol_map = HashMap::new();
        symbol_map.insert("SP".to_string(), 0);
        symbol_map.insert("LCL".to_string(), 1);
        symbol_map.insert("ARG".to_string(), 2);
        symbol_map.insert("THIS".to_string(), 3);
        symbol_map.insert("THAT".to_string(), 4);
        symbol_map.insert("R0".to_string(), 0);
        symbol_map.insert("R1".to_string(), 1);
        symbol_map.insert("R2".to_string(), 2);
        symbol_map.insert("R3".to_string(), 3);
        symbol_map.insert("R4".to_string(), 4);
        symbol_map.insert("R5".to_string(), 5);
        symbol_map.insert("R6".to_string(), 6);
        symbol_map.insert("R7".to_string(), 7);
        symbol_map.insert("R8".to_string(), 8);
        symbol_map.insert("R9".to_string(), 9);
        symbol_map.insert("R10".to_string(), 10);
        symbol_map.insert("R11".to_string(), 11);
        symbol_map.insert("R12".to_string(), 12);
        symbol_map.insert("R13".to_string(), 13);
        symbol_map.insert("R14".to_string(), 14);
        symbol_map.insert("R15".to_string(), 15);
        symbol_map.insert("SCREEN".to_string(), 16384);
        symbol_map.insert("KBD".to_string(), 24576);

        let mut c_comp_instruction_map = HashMap::new();
        c_comp_instruction_map.insert("0".to_string(), "0101010".to_string());
        c_comp_instruction_map.insert("1".to_string(), "0111111".to_string());
        c_comp_instruction_map.insert("-1".to_string(), "0111010".to_string());
        c_comp_instruction_map.insert("D".to_string(), "0001100".to_string());
        c_comp_instruction_map.insert("A".to_string(), "0110000".to_string());
        c_comp_instruction_map.insert("M".to_string(), "1110000".to_string());
        c_comp_instruction_map.insert("!D".to_string(), "0001101".to_string());
        c_comp_instruction_map.insert("!A".to_string(), "0110001".to_string());
        c_comp_instruction_map.insert("!M".to_string(), "1110001".to_string());
        c_comp_instruction_map.insert("-D".to_string(), "0001111".to_string());
        c_comp_instruction_map.insert("-A".to_string(), "0110011".to_string());
        c_comp_instruction_map.insert("-M".to_string(), "1110011".to_string());
        c_comp_instruction_map.insert("D+1".to_string(), "0011111".to_string());
        c_comp_instruction_map.insert("A+1".to_string(), "0110111".to_string());
        c_comp_instruction_map.insert("M+1".to_string(), "1110111".to_string());
        c_comp_instruction_map.insert("D-1".to_string(), "0001110".to_string());
        c_comp_instruction_map.insert("A-1".to_string(), "0110010".to_string());
        c_comp_instruction_map.insert("M-1".to_string(), "1110010".to_string());
        c_comp_instruction_map.insert("D+A".to_string(), "0000010".to_string());
        c_comp_instruction_map.insert("D+M".to_string(), "1000010".to_string());
        c_comp_instruction_map.insert("D-A".to_string(), "0010011".to_string());
        c_comp_instruction_map.insert("D-M".to_string(), "1010011".to_string());
        c_comp_instruction_map.insert("A-D".to_string(), "0000111".to_string());
        c_comp_instruction_map.insert("M-D".to_string(), "1000111".to_string());
        c_comp_instruction_map.insert("D&A".to_string(), "0000000".to_string());
        c_comp_instruction_map.insert("D&M".to_string(), "1000000".to_string());
        c_comp_instruction_map.insert("D|A".to_string(), "0010101".to_string());
        c_comp_instruction_map.insert("D|M".to_string(), "1010101".to_string());

        let mut c_dest_instruction_map = HashMap::new();
        c_dest_instruction_map.insert("null".to_string(), "000".to_string());
        c_dest_instruction_map.insert("".to_string(), "000".to_string());
        c_dest_instruction_map.insert("M".to_string(), "001".to_string());
        c_dest_instruction_map.insert("D".to_string(), "010".to_string());
        c_dest_instruction_map.insert("MD".to_string(), "011".to_string());
        c_dest_instruction_map.insert("A".to_string(), "100".to_string());
        c_dest_instruction_map.insert("AM".to_string(), "101".to_string());
        c_dest_instruction_map.insert("AD".to_string(), "110".to_string());
        c_dest_instruction_map.insert("AMD".to_string(), "111".to_string());

        let mut c_jump_instruction_map = HashMap::new();
        c_jump_instruction_map.insert("null".to_string(), "000".to_string());
        c_jump_instruction_map.insert("".to_string(), "000".to_string());
        c_jump_instruction_map.insert("JGT".to_string(), "001".to_string());
        c_jump_instruction_map.insert("JEQ".to_string(), "010".to_string());
        c_jump_instruction_map.insert("JGE".to_string(), "011".to_string());
        c_jump_instruction_map.insert("JLT".to_string(), "100".to_string());
        c_jump_instruction_map.insert("JNE".to_string(), "101".to_string());
        c_jump_instruction_map.insert("JLE".to_string(), "110".to_string());
        c_jump_instruction_map.insert("JMP".to_string(), "111".to_string());

        const A_INSTRUCTION_INT_PATTERN: &str = r"^@(\d+)$";
        const A_INSTRUCTION_MIXED_PATTERN: &str =
            r"^@([a-zA-Z_][a-zA-Z0-9_.$]*)$|^@([0-9]*[a-zA-Z_.$][a-zA-Z0-9_.$]*)$";
        const C_INSTRUCTION_PATTERN: &str =
            r"^(?:([AMD]+)=)?([AMD01\+\-\&\|!]+)(?:;([JGTLEQNEMP]+))?$";
        const LABEL_SYMBOL_PATTERN: &str = r"^\(([^)]+)\)$";

        let regex_a_instruction_variable = Regex::new(A_INSTRUCTION_INT_PATTERN).unwrap();
        let regex_a_instruction_symbol = Regex::new(A_INSTRUCTION_MIXED_PATTERN).unwrap();
        let regex_c_instruction = Regex::new(C_INSTRUCTION_PATTERN).unwrap();
        let regex_label_symbol = Regex::new(LABEL_SYMBOL_PATTERN).unwrap();

        Ok(Self {
            file_path,
            file_content: content,
            symbol_map,
            c_comp_instruction_map,
            c_dest_instruction_map,
            c_jump_instruction_map,
            regex_a_instruction_variable,
            regex_a_instruction_symbol,
            regex_c_instruction,
            regex_label_symbol,
        })
    }

    pub fn parse(&mut self) -> Vec<String> {
        self.first_pass(self.file_content.clone());
        return self.second_pass(self.file_content.clone());
    }

    fn first_pass(&mut self, file_content: String) {
        let mut instruction_counter = 0;
        for line in file_content.lines() {
            if line.is_empty()
                || line.starts_with("//")
            {
                continue;
            }
            
            let trimmed_line = if let Some(idx) = line.find("//") {
                &line[0..idx]
            } else {
                line
            }
            .trim();
            if let Some(caps) = self.regex_label_symbol.captures(line) {
                //pointer to next instruction
                println!("caps: {:?}", caps[1].to_string());
                self.symbol_map
                    .insert(caps[1].to_string(), instruction_counter);
                continue
            };
            instruction_counter += 1;
        }
    }

    fn second_pass(&mut self, file_content: String) -> Vec<String> {
        let mut instruction_number: u16 = 16;
        let mut parsed_lines: Vec<String> = Vec::new();
        for line in file_content.lines() {
            if line.is_empty()
                || line.starts_with("//")
                || line.starts_with("(")
            {
                continue;
            }
            
            let trimmed_line = if let Some(idx) = line.find("//") {
                &line[0..idx]
            } else {
                line
            }
            .trim();
            let (processed_instruction, new_instruction_number) =
                self.process_line_into_instruction(trimmed_line, instruction_number);
            instruction_number = new_instruction_number;
            match processed_instruction {
                Instruction::AInstruction(a_instruction) => {
                    println!("A Instruction {:?}", &a_instruction.value);
                    parsed_lines.push(a_instruction.value);
                }
                Instruction::CInstruction(c_instruction) => {
                    parsed_lines.push(c_instruction.concatenate());
                    println!("B Instruction {:?}", c_instruction.concatenate());
                }
            }
        }
        parsed_lines
    }

    fn process_line_into_instruction(
        &mut self,
        line: &str,
        mut instruction_number: u16,
    ) -> (Instruction, u16) {
        let mut instruction: Option<Instruction> = None;

        if let Some(caps) = self.regex_a_instruction_variable.captures(line) {
            instruction = Some(Instruction::AInstruction(AInstruction::new(
                caps[1].to_string(),
            )));
        }

        if let Some(caps) = self.regex_a_instruction_symbol.captures(line) {
            let value = caps[1].to_string();
            if let Some(&map_value) = self.symbol_map.get(&value) {
                instruction = Some(Instruction::AInstruction(AInstruction::new(
                    map_value.to_string(),
                )));
            } else {
                self.symbol_map
                    .insert(value.to_string(), instruction_number);
                instruction = Some(Instruction::AInstruction(AInstruction::new(
                    instruction_number.to_string(),
                )));
                instruction_number += 1;
            }
        }

        if let Some(caps) = self.regex_c_instruction.captures(line) {
            let dest_str = caps.get(1).map_or("", |m| m.as_str());
            let comp_str = &caps[2];
            let jmp_str = caps.get(3).map_or("", |m| m.as_str());
            println!(
                "Dest {:?}, COmp {:?}, Jmp {:?}",
                dest_str, comp_str, jmp_str
            );
            instruction = Some(Instruction::CInstruction(CInstruction {
                dest: self.c_dest_instruction_map[dest_str].clone(),
                comp: self.c_comp_instruction_map[comp_str].clone(),
                jmp: self.c_jump_instruction_map[jmp_str].clone(),
            }));
        }

        return (instruction.unwrap(), instruction_number);
    }

    pub fn write_file(&self, parsed_vector: Vec<String>) -> std::io::Result<()> {
        let output_content = parsed_vector.join("\n");

        let path = Path::new(&self.file_path);
        let file_stem = path.file_stem().unwrap().to_str().unwrap();
        let parent = path.parent().unwrap();
        let new_file_name = format!("{}.hack", file_stem);
        let new_file_path = parent.join(new_file_name);

        // Create and write to the new file
        let mut file = File::create(new_file_path)?;
        file.write_all(output_content.as_bytes())?;

        Ok(())
    }
}

fn read_file(file_path: &str) -> io::Result<String> {
    let mut file = File::open(file_path)?;
    let mut content = String::new();
    file.read_to_string(&mut content)?;
    Ok(content)
}
