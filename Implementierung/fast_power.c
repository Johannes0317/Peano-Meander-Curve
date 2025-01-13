long long fast_power(int base, int exponent) {
    //more accurate than pow() for large exponents
    // Handle negative exponents
    if (exponent < 0) {
        return 0; // or handle as needed in your application
    }

    // Base case: exponent is 0
    if (exponent == 0) {
        return 1;
    }
    long long result = 1;
    long long power = base;

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result *= power;
        }
        power *= power;
        exponent /= 2;
    }

    return result;
}