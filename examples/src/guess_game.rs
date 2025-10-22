use rand::Rng;
use std::cmp::Ordering;
use std::io;

fn game() {
    let mut complex1 = types::complex::Complex::new(10, 10);
    let complex2 = types::complex::Complex::new(5, 6);
    println!("Complex number: {complex1}");
    complex1.add(&complex2);
    println!("Complex number: {complex1}");

    println!("Hello, world! Starting guess game...");

    let secret_num = rand::thread_rng().gen_range(1..=10);

    loop {
        println!("Enter your guess.");

        let mut guess = String::new();

        io::stdin()
            .read_line(&mut guess)
            .expect("Failed to read the line.");

        let guess: u32 = match guess.trim().parse() {
            Ok(num) => num,
            Err(_) => {
                println!("Please enter a valid number!");
                continue;
            }
        };

        println!("You have guessed: {guess}");

        match guess.cmp(&secret_num) {
            Ordering::Equal => {
                println!("You won!");
                break;
            }
            Ordering::Greater => println!("Your number is too big."),
            Ordering::Less => println!("Your number is too small."),
        }
    }
}

fn main() {
    // Run the game
    game();
}
