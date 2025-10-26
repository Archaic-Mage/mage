use std::cell::RefCell;
use std::io;
use std::rc::Rc;

struct Printer {
    times: Rc<RefCell<u32>>,
}

impl runtime::Runnable for Printer {
    fn do_work(&mut self) {
        let times = *self.times.borrow();
        for time in 1..=times {
            println!("Printing for {time}.");
        }
    }
}

struct InputTaker {
    times: Rc<RefCell<u32>>, //shared pointer
}

impl runtime::Runnable for InputTaker {
    fn do_work(&mut self) {
        let mut input = String::new();
        io::stdin().read_line(&mut input).expect("Fail!");
        let input = match input.trim().parse::<u32>() {
            Ok(num) => num,
            Err(_) => 0,
        };
        *self.times.borrow_mut() = input;
    }
}

fn main() {
    runtime::test();

    let times = Rc::new(RefCell::new(0));
    let printer = Printer {
        times: Rc::clone(&times),
    };
    let input_taker = InputTaker {
        times: Rc::clone(&times),
    };
    let mut program = runtime::EventLoop::new();
    program.add_runnable(Box::new(printer)); // Box is an unique ptr
    program.add_runnable(Box::new(input_taker));
    program.run();
}
