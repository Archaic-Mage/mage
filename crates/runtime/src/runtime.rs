pub trait Runnable {
    fn do_work(&mut self);
}

pub struct EventLoop {
    runnables: Vec<Box<dyn Runnable>>,
}

impl EventLoop {
    pub fn new() -> EventLoop {
        let empty_vec: Vec<Box<dyn Runnable>> = Vec::new();
        EventLoop {
            runnables: empty_vec,
        }
    }

    pub fn add_runnable(&mut self, runnable: Box<dyn Runnable>) {
        self.runnables.push(runnable);
    }

    pub fn run(&mut self) {
        loop {
            for runnable in &mut self.runnables {
                runnable.do_work();
            }
        }
    }
}

pub fn test() {
    println!("Using runtime.");
}
