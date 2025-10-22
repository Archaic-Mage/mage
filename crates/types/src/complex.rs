pub struct Complex {
    real: u32,
    imaginary: u32,
}

// functions on the type
impl Complex {
    pub fn new(real: u32, imaginary: u32) -> Complex {
        Complex { real, imaginary }
    }
}

// functions on the object
impl Complex {
    pub fn add(&mut self, other: &Complex) {
        self.real += other.real;
        self.imaginary += other.imaginary;
    }
}

impl std::fmt::Display for Complex {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{} + {}i", self.real, self.imaginary)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_new() {
        let c = Complex::new(3, 4);
        assert_eq!(c.real, 3);
        assert_eq!(c.imaginary, 4);
    }

    #[test]
    fn test_add() {
        let mut c1 = Complex::new(3, 4);
        let c2 = Complex::new(1, 2);
        c1.add(&c2);
        assert_eq!(c1.real, 4);
        assert_eq!(c1.imaginary, 6);
    }

    #[test]
    fn test_add_multiple() {
        let mut c1 = Complex::new(1, 1);
        let c2 = Complex::new(2, 2);
        let c3 = Complex::new(3, 3);
        c1.add(&c2);
        c1.add(&c3);
        assert_eq!(c1.real, 6);
        assert_eq!(c1.imaginary, 6);
    }

    #[test]
    fn test_display() {
        let c = Complex::new(3, 4);
        assert_eq!(format!("{}", c), "3 + 4i");
    }

    #[test]
    fn test_display_with_large_numbers() {
        let c = Complex::new(12345, 67890);
        assert_eq!(format!("{}", c), "12345 + 67890i");
    }
}
