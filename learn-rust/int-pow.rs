fn int_pow(mut a: f32, mut n: i32) -> f32 {
    if n < 0 {
        return 1.0 / int_pow(a, -n);
    }

    // Performing this check here to run the algorithm until n is 1. Otherwise
    // there is an additional step, performing operations.
    if (n == 0) || (a == 1.0) {
        return 1.0;
    }

    let mut res = 1.0;
    while n > 1 {
        if n % 2 != 0 {
            res *= a;
        }
        n /= 2;
        a *= a;
    }

    return res * a;
}

fn main() {
    println!("{}", int_pow(2.0, 6));
    println!("{}", int_pow(2.0, 7));
    println!("{}", int_pow(2.0, 10));
    println!("{}", int_pow(3.0, 7));
    println!("{}", int_pow(0.0, 7));
    println!("{}", int_pow(4.0, 0));
    println!("{}", int_pow(4.0, -1));
    println!("{}", int_pow(1.0, -8));
    println!("{}", int_pow(0.0, -1));
}
