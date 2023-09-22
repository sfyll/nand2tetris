use regex::Regex;
use std::collections::HashMap;
use std::fs::File;
use std::io::Write;
use std::io::{self, Read};
use std::path::Path;

pub struct Assembler {
    pub file_path: String,
    pub file_content: String,
    c_comp_instruction_map: HashMap<String, String>,
    c_dest_instruction_map: HashMap<String, String>,
    c_jump_instruction_map: HashMap<String, String>,
    regex_a_instruction: Regex,
    regex_c_instruction: Regex,
}

pub struct AInstruction {
    pub value: String,
}

impl AInstruction {
    pub fn new(value: String) -> Self {
        let binary_value = format!("{:016b}", value.parse::<u16>().unwrap_or(0));
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

        const A_INSTRUCTION_PATTERN: &str = r"^@(\d+)$";
        const C_INSTRUCTION_PATTERN: &str =
            r"^(?:([AMD]+)=)?([AMD01\+\-\&\|!]+)(?:;([JGTLEQNEMP]+))?$";
        let regex_a_instruction = Regex::new(A_INSTRUCTION_PATTERN).unwrap();
        let regex_c_instruction = Regex::new(C_INSTRUCTION_PATTERN).unwrap();

        Ok(Self {
            file_path: file_path,
            file_content: content,
            c_comp_instruction_map,
            c_dest_instruction_map,
            c_jump_instruction_map,
            regex_a_instruction,
            regex_c_instruction,
        })
    }

    pub fn parse(&self) -> Vec<String> {
        let mut parsed_lines: Vec<String> = Vec::new();
        for line in self.file_content.lines() {
            println!("Parsed Line {:?}", line);
            let trimmed_line = line.trim();
            if trimmed_line.is_empty() || trimmed_line.starts_with("//") {
                continue;
            }
            let processed_line = self.process_line_into_instruction(trimmed_line);
            parsed_lines.push(processed_line);
        }
        parsed_lines
    }

    fn process_line_into_instruction(&self, line: &str) -> String {
        if let Some(caps) = self.regex_a_instruction.captures(line) {
            return AInstruction::new(caps[1].to_string()).value;
        }

        if let Some(caps) = self.regex_c_instruction.captures(line) {
            let dest_str = caps.get(1).map_or("", |m| m.as_str());
            let comp_str = &caps[2];
            let jmp_str = caps.get(3).map_or("", |m| m.as_str());
            println!(
                "Dest {:?}, COmp {:?}, Jmp {:?}",
                dest_str, comp_str, jmp_str
            );
            let c_instruction = CInstruction {
                dest: self.c_dest_instruction_map[dest_str].clone(),
                comp: self.c_comp_instruction_map[comp_str].clone(),
                jmp: self.c_jump_instruction_map[jmp_str].clone(),
            };

            println!("{:?}", self.c_jump_instruction_map[jmp_str].clone());
            return c_instruction.concatenate();
        }

        panic!("Unrecognized instruction: {}", line);
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
