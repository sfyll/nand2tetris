use clap::{Arg, Command};

mod assembler;
pub use assembler::Assembler;

fn main() {
    let matches = Command::new("HackAssembler")
        .version("1.0")
        .author("sfyl, me@sfyl.xyz")
        .about("Assembler for Hack Assembly Language")
        .arg(
            Arg::new("input-file-asm")
                .short('i')
                .long("file")
                .value_name("FILE")
                .help("The .asm file to be assembled")
                .required(true),
        )
        .get_matches();

    let file_path = matches
        .get_one::<String>("input-file-asm")
        .unwrap()
        .to_string();

    println!("Parsing {}", file_path);

    let mut assembler = Assembler::new(file_path).unwrap();

    let parsed_vector = assembler.parse();

    assembler.write_file(parsed_vector).unwrap();
}

