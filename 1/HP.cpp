#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;

#pragma region HPbasicAlgorithm

class HPbasicAlgorithm {
protected:
    HPbasicAlgorithm();
    ~HPbasicAlgorithm();
    string digitWise_plus(const string lhs, const string rhs)const;
    string digitWise_minus(const string lhs, const string rhs)const;
    string digitWise_multiply(const string lhs, const string rhs)const;
    string digitWise_divide(const string lhs, const string rhs, string* remainder)const;
    int16_t compare(const string lhs, const string rhs)const;
    bool compare_boolean(const string lhs, const string rhs)const;

private:
    string digitWise_single_multiply(const string lhs, const uint8_t rhs_digit)const;
    string kill_zeros_and_copy(const string str)const;
    size_t align_with_zeros(string* plhs, string* prhs)const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPbasicAlgorithm::HPbasicAlgorithm() {}
HPbasicAlgorithm::~HPbasicAlgorithm() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

string HPbasicAlgorithm::digitWise_plus(const string lhs, const string rhs)const {
    string lhs_cp = kill_zeros_and_copy(lhs);
    string rhs_cp = kill_zeros_and_copy(rhs);
    if (lhs_cp.empty()) {
        return rhs;
    }
    if (rhs_cp.empty()) {
        return lhs;
    }

    size_t length = align_with_zeros(&lhs_cp, &rhs_cp);

    string result(length, '0');
    bool carry = false;
    for (size_t i = length; i > 0; i--) {
        i--;
        result[i] = lhs_cp[i] + rhs_cp[i] - 48;
        if (carry) {
            result[i]++;
        }
        if (result[i] >= 58) {
            result[i] -= 10;
            carry = true;
        } else {
            carry = false;
        }
        i++;
    }
    if (carry) {
        result.insert(0, 1, '1');
    }
    return result;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

string HPbasicAlgorithm::digitWise_minus(const string lhs, const string rhs)const {
    string lhs_cp = kill_zeros_and_copy(lhs);
    string rhs_cp = kill_zeros_and_copy(rhs);
    if (rhs_cp.empty()) {
        return lhs;
    }
    if (lhs_cp.empty()) {
        rhs_cp.push_back('-');
        return rhs_cp;
    }
    if (lhs_cp == rhs_cp) {
        return "";
    }

    size_t length = align_with_zeros(&lhs_cp, &rhs_cp);

    bool convert_sign = false;
    size_t lhs_first_not_of_0 = lhs_cp.find_first_not_of('0');
    size_t rhs_first_not_of_0 = rhs_cp.find_first_not_of('0');
    if (lhs_first_not_of_0 > rhs_first_not_of_0) {
        lhs_cp.swap(rhs_cp);
        convert_sign = true;
    } else if (lhs_first_not_of_0 == rhs_first_not_of_0) {
        if (lhs_cp[lhs_first_not_of_0] < rhs_cp[lhs_first_not_of_0]) {
            lhs_cp.swap(rhs_cp);
            convert_sign = true;
        }
    }

    string result(length, '0');
    bool borrow = false;
    for (size_t i = length; i > 0; i--) {
        i--;
        result[i] = lhs_cp[i] - rhs_cp[i] + 48;
        if (borrow) {
            result[i] -= 1;
        }
        if (result[i] < 48) {
            result[i] += 10;
            borrow = true;
        } else {
            borrow = false;
        }
        i++;
    }
    if (result[0] == '0') {
        result.erase(0, 1);
    }
    if (convert_sign) {
        result.push_back('-');
    }
    return result;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

string HPbasicAlgorithm::digitWise_single_multiply(const string lhs, const uint8_t rhs_digit)const {
    if (rhs_digit == '0') {
        return "";
    }
    string result;
    for (uint8_t i = 0; i < rhs_digit - '0'; i++) {
        result = digitWise_plus(result, lhs);
    }
    return result;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

string HPbasicAlgorithm::digitWise_multiply(const string lhs, const string rhs)const {
    string lhs_cp = kill_zeros_and_copy(lhs);
    string rhs_cp = kill_zeros_and_copy(rhs);
    if (lhs_cp.empty() || rhs_cp.empty()) {
        return "";
    }
    if (lhs_cp == (string)"1") {
        return rhs_cp;
    }
    if (rhs_cp == (string)"1") {
        return lhs_cp;
    }
    string result;
    for (size_t i = rhs_cp.length(); i > 0; i--) {
        result = digitWise_plus(digitWise_single_multiply(lhs_cp, rhs_cp[i - 1]).append(rhs_cp.length() - i, '0'), result);
    }
    return result;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

string HPbasicAlgorithm::digitWise_divide(const string lhs, const string rhs, string* remainder)const {
    string lhs_cp(lhs);
    string rhs_cp(rhs);
    string result;
    while (compare_boolean(lhs_cp, rhs_cp)) {
        lhs_cp = digitWise_minus(lhs_cp, rhs_cp);
        result = digitWise_plus(result, "1");
    }
    *remainder = lhs_cp;
    return result;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int16_t HPbasicAlgorithm::compare(const string lhs, const string rhs)const {
    size_t lhs_length = lhs.length();
    size_t rhs_length = rhs.length();
    if (lhs_length > rhs_length) {
        return 1;
    } else {
        if (lhs_length < rhs_length) {
            return -1;
        } else {
            size_t i = 0;
            while (lhs[i] == rhs[i] && i < lhs_length) {
                i++;
            }
            if (i == lhs_length) {
                return 0;
            }
            if (lhs[i] > rhs[i]) {
                return 1;
            } else {
                return -1;
            }
        }
    }
}

bool HPbasicAlgorithm::compare_boolean(const string lhs, const string rhs)const {
    size_t lhs_length = lhs.length();
    size_t rhs_length = rhs.length();
    if (lhs_length > rhs_length) {
        return true;
    } else {
        if (lhs_length < rhs_length) {
            return false;
        } else {
            size_t i = 0;
            while (lhs[i] == rhs[i] && i < lhs_length) {
                i++;
            }
            if (i == lhs_length) {
                return true;
            }
            if (lhs[i] > rhs[i]) {
                return true;
            } else {
                return false;
            }
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

string HPbasicAlgorithm::kill_zeros_and_copy(const string str)const {
    string str_cp(str);
    return str_cp.erase(0, str.find_first_not_of('0'));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

size_t HPbasicAlgorithm::align_with_zeros(string* plhs, string* prhs)const {
    size_t lhs_length = plhs->length();
    size_t rhs_length = prhs->length();
    size_t length = 0;
    int64_t zero_num = 0;
    if ((zero_num = lhs_length - rhs_length) > 0) {
        length = lhs_length;
        prhs->insert(0, zero_num, '0');
    } else {
        if ((zero_num = rhs_length - lhs_length) > 0) {
            length = rhs_length;
            plhs->insert(0, zero_num, '0');
        } else {
            length = lhs_length;
        }
    }
    return length;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#pragma endregion

#pragma region HPfloat

#ifndef HP_ENOBS
#define HP_ENOBS 50
#endif

class HPfloat :private HPbasicAlgorithm {
private:
    bool sign = true;   // true: positive, false: negative.
    string digits;  // Effective numbers in string form.
    int16_t exponent = INT16_MIN;    // The exponent of this number.

public:
    HPfloat();
    HPfloat(const double number_double);
    HPfloat(const float number_float);
    HPfloat(const string number_str);
    HPfloat(const int64_t number_int);
    ~HPfloat();
    void print()const;
    void print(const size_t precision)const;
    HPfloat get_zero()const;
    HPfloat get_zero(const bool sign)const;
    bool zeroQ(const HPfloat value)const;
    HPfloat get_infinity()const;
    HPfloat get_infinity(const bool sign)const;
    bool infinityQ(const HPfloat value)const;

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    HPfloat& operator=(const double number_double) {
        stringstream ss;
        ss.precision(15);
        ss << number_double << flush;
        string number_str = ss.str();
        construct(number_str);
        return *this;
    }

    HPfloat& operator=(const float number_float) {
        stringstream ss;
        ss.precision(6);
        ss << number_float << flush;
        string number_str = ss.str();
        construct(number_str);
        return *this;
    }

    HPfloat& operator=(const string number_str) {
        construct(number_str);
        return *this;
    }

    HPfloat& operator=(const int64_t number_int) {
        stringstream ss;
        ss << number_int << flush;
        string number_str = ss.str();
        construct(number_str);
        return *this;
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    bool operator==(const HPfloat rhs)const {
        return
            this->sign == rhs.sign
            && this->digits == rhs.digits
            && this->exponent == rhs.exponent;
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    bool operator!=(const HPfloat rhs)const {
        return
            this->sign != rhs.sign
            || this->digits != rhs.digits
            || this->exponent != rhs.exponent;
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    HPfloat operator+(const HPfloat rhs)const {
        if (zeroQ(rhs)) {
            return *this;
        }
        if (zeroQ(*this)) {
            return rhs;
        }
        if (infinityQ(rhs)) {
            if (rhs.sign) {
                return get_infinity();
            } else {
                return get_infinity(false);
            }
        }
        if (infinityQ(*this)) {
            if (this->sign) {
                return get_infinity();
            } else {
                return get_infinity(false);
            }
        }

        return construct_result_of_plus_minus(*this, rhs, true);
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    HPfloat operator-(const HPfloat rhs)const {
        if (*this == rhs) {
            return get_zero();
        }
        if (zeroQ(rhs)) {
            return *this;
        }
        if (zeroQ(*this)) {
            HPfloat rhs_cp = rhs;
            rhs_cp.sign = !rhs_cp.sign;
            return rhs;
        }
        if (infinityQ(rhs)) {
            if (rhs.sign) {
                return get_infinity(false);
            } else {
                return get_infinity();
            }
        }
        if (infinityQ(*this)) {
            if (this->sign) {
                return get_infinity();
            } else {
                return get_infinity(false);
            }
        }

        return construct_result_of_plus_minus(*this, rhs, false);
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    HPfloat operator*(const HPfloat rhs)const {
        bool result_sign = !(this->sign ^ rhs.sign);
        if (zeroQ(*this) || zeroQ(rhs)) {
            return get_zero(result_sign);
        }
        if (infinityQ(*this) || infinityQ(rhs)) {
            return get_infinity(result_sign);
        }
        if (*this == (string)"1") {
            return rhs;
        }
        if (rhs == (string)"1") {
            return *this;
        }

        HPfloat result;
        HPfloat lhs_cp = *this;
        HPfloat rhs_cp = rhs;
        result.sign = result_sign;
        result.exponent = lhs_cp.exponent + rhs_cp.exponent;
        result.digits = digitWise_multiply(lhs_cp.digits, rhs_cp.digits);
        int16_t extra_exp = (int16_t)result.digits.length() - 2 * HP_ENOBS;
        result.digits.erase(HP_ENOBS);
        result.exponent += extra_exp;
        return result;
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    HPfloat operator/(const HPfloat rhs)const {
        if (rhs == (string)"1") {
            return *this;
        }
        if (*this == rhs) {
            return (string)"1";
        }
        bool result_sign = !(this->sign ^ rhs.sign);
        if (zeroQ(*this) && !zeroQ(rhs)) {
            return get_zero(result_sign);
        }
        if (!zeroQ(*this) && zeroQ(rhs)) {
            return get_infinity(result_sign);
        }
        if (infinityQ(*this) && !infinityQ(rhs)) {
            return get_infinity(result_sign);
        }
        if (!infinityQ(*this) && infinityQ(rhs)) {
            return get_zero(result_sign);
        }

        HPfloat result;
        HPfloat lhs_cp = *this;
        HPfloat rhs_cp = rhs;
        string remainder;
        size_t count = 0;
        HPfloat result_prev;
        while (!compare_boolean(lhs_cp.digits, rhs_cp.digits)) {
            lhs_cp.digits.push_back('0');
            count++;
        }
        do {
            result_prev = result;
            result += (HPfloat)powf64(0.1, (double)count) * (HPfloat)digitWise_divide(lhs_cp.digits, rhs_cp.digits, &remainder);
            lhs_cp.digits = remainder;
            while (!compare_boolean(lhs_cp.digits, rhs_cp.digits) && remainder != (string)"") {
                lhs_cp.digits.push_back('0');
                count++;
            }
        } while (remainder != (string)"" && result_prev != result);
        result.exponent += this->exponent - rhs.exponent;
        result.sign = result_sign;
        return result;
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    HPfloat& operator+=(const HPfloat value) {
        *this = *this + value;
        return *this;
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    HPfloat& operator-=(const HPfloat value) {
        *this = *this - value;
        return *this;
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    HPfloat& operator*=(const HPfloat value) {
        *this = *this * value;
        return *this;
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    HPfloat& operator/=(const HPfloat value) {
        *this = *this / value;
        return *this;
    }

private:
    void construct(const string number_str);
    void move(string* digits, const size_t mov)const;
    int16_t align(HPfloat* lhs, HPfloat* rhs)const;
    HPfloat construct_result_of_plus_minus(const HPfloat lhs, const HPfloat rhs, const bool symbol)const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HPfloat::construct(const string number_str) {
    // Get the sign.
    /* (positive) The exponent of the integer part.
    (negative) The exponent bias from the "0"s in fraction part. */
    int32_t exponent_bias = 0;
    // index.
    size_t k = 0;
    /* If there is a sign, set sign.
    Else, ignore. (because the default value is positive)*/
    if (number_str[0] == '-' || number_str[0] == '+') {
        if (number_str[0] == '-') {
            this->sign = false;
        }
        k++;
    }

    // Get digits.
    while (number_str[k] != '.' && number_str[k] != 'e' && number_str[k] != 0) {
        this->digits.push_back((char)(number_str[k]));
        exponent_bias++;
        k++;
    }
    if (number_str[k] == '.') {
        k++;
        while (number_str[k] != 'e' && number_str[k] != 0) {
            this->digits.push_back((char)(number_str[k]));
            k++;
        }
    }
    while (this->digits[0] == '0') {
        exponent_bias--;
        this->digits.erase(0, 1);
    }
    this->digits.resize(HP_ENOBS, '0');
    string zeros(HP_ENOBS, '0');
    if (this->digits == zeros) {
        this->exponent = INT16_MIN;
        return;
    } else {
        this->exponent = exponent_bias;
    }

    // Get exponent. If there are.
    if (number_str[k] == 'e') {
        int16_t e_exponent = 0;
        stringstream ss(number_str.substr(k + 1));
        ss >> e_exponent;
        this->exponent += e_exponent;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPfloat::HPfloat() {
    this->digits.append(HP_ENOBS, '0');
}

HPfloat::HPfloat(const double number_double) {
    stringstream ss;
    ss.precision(15);
    ss << number_double << flush;
    string number_str = ss.str();
    construct(number_str);
}

HPfloat::HPfloat(const float number_float) {
    stringstream ss;
    ss.precision(6);
    ss << number_float << flush;
    string number_str = ss.str();
    construct(number_str);
}

HPfloat::HPfloat(const string number_str) {
    construct(number_str);
}

HPfloat::HPfloat(const int64_t number_int) {
    stringstream ss;
    ss << number_int << flush;
    string number_str = ss.str();
    construct(number_str);
}

HPfloat::~HPfloat() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HPfloat::print()const {
    if (!(this->sign)) {
        cout << '-';
    }
    cout << "0." << this->digits << 'e' << exponent;
}

void HPfloat::print(const size_t precision)const {
    if (this->digits[precision] - 48 >= 5) {
        string round_str = "0.1e";
        stringstream ss;
        ss << (int64_t)this->exponent - (int64_t)precision << flush;
        round_str += ss.str();
        HPfloat round = round_str;
        HPfloat this_cp = *this;
        if (this->sign) {
            this_cp = this_cp + round;
            cout << "0." << this_cp.digits.substr(0, precision) << 'e' << this_cp.exponent;
        } else {
            this_cp = this_cp - round;
            cout << "-0." << this_cp.digits.substr(0, precision) << 'e' << this_cp.exponent;
        }
    } else {
        if (!(this->sign)) {
            cout << '-';
        }
        cout << "0." << this->digits.substr(0, precision) << 'e' << exponent;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPfloat HPfloat::get_zero()const {
    HPfloat zero;
    return zero;
}

HPfloat HPfloat::get_zero(const bool sign)const {
    HPfloat zero;
    zero.sign = sign;
    return zero;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool HPfloat::zeroQ(const HPfloat value)const {
    string zero_digits(HP_ENOBS, '0');
    return value.digits == zero_digits;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPfloat HPfloat::get_infinity()const {
    HPfloat infinity;
    string infinity_str(HP_ENOBS, '9');
    infinity.digits = infinity_str;
    infinity.exponent = INT16_MAX;
    return infinity;
}

HPfloat HPfloat::get_infinity(const bool sign)const {
    HPfloat infinity;
    infinity.sign = sign;
    string infinity_str(HP_ENOBS, '9');
    infinity.digits = infinity_str;
    infinity.exponent = INT16_MAX;
    return infinity;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool HPfloat::infinityQ(const HPfloat value)const {
    HPfloat infinity = get_infinity();
    if (value == infinity) {
        return true;
    } else {
        infinity.sign = false;
        if (value == infinity) {
            return true;
        }
    }
    return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HPfloat::move(string* digits, const size_t mov)const {
    if (mov == 0) {
        return;
    }
    digits->erase(HP_ENOBS - mov);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int16_t HPfloat::align(HPfloat* lhs, HPfloat* rhs)const {
    int16_t exp_diff = 0;
    if ((exp_diff = lhs->exponent - rhs->exponent) >= 0) {
        move(&(rhs->digits), exp_diff);
        return lhs->exponent;
    } else {
        move(&(lhs->digits), -exp_diff);
        return rhs->exponent;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPfloat HPfloat::construct_result_of_plus_minus(const HPfloat lhs, const HPfloat rhs, const bool symbol)const {
    HPfloat result;
    HPfloat lhs_cp = lhs;
    HPfloat rhs_cp = rhs;
    result.exponent = align(&lhs_cp, &rhs_cp);
    bool result_sign = lhs_cp.sign;
    if ((lhs_cp.sign ^ rhs_cp.sign) ^ symbol) {
        result.digits = digitWise_plus(lhs_cp.digits, rhs_cp.digits);
        if (result.digits.length() > HP_ENOBS) {
            result.digits.erase(HP_ENOBS);
            result.exponent++;
        }
    } else {
        result.digits = digitWise_minus(lhs_cp.digits, rhs_cp.digits);
        if (result.digits.rfind('-') != -1) {
            result_sign = !result_sign;
            result.digits.erase(result.digits.length() - 1);
        }
        result.exponent -= HP_ENOBS - result.digits.length();
        result.digits.resize(HP_ENOBS, '0');
    }
    result.sign = result_sign;
    return result;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#pragma endregion

#pragma region Main

int main() {
    HPfloat a = 9.;
    HPfloat b = 8.;
    HPfloat c = 40.;
    HPfloat d = 12.;
    HPfloat e = 34.;
    HPfloat f = a + b - c * d / e;
    a.print();cout << endl;
    b.print();cout << endl;
    c.print();cout << endl;
    d.print();cout << endl;
    e.print();cout << endl;
    f.print();cout << endl;
    return 0;
}

#pragma endregion
