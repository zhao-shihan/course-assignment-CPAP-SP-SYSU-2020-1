#include <iostream>
#include <string>
using namespace std;

#ifndef LENGTH
#define LENGTH 167
#endif
#ifndef ENOBS
#define ENOBS 50
#endif

class high_float {
public:
    bool sign = 0;   //true: positive, false: negative
    bool binary[LENGTH] = {};  //Numbers in binary form
    int32_t exponent = 0;    //The binary exponent of this number

    high_float() {}
    high_float(string number) {
        //Get the sign.
        int32_t exponent_bias = 0;
        uint32_t k = 0;
        if (number[0] == '-' || number[0] == '+') {
            if (number[0] == '-') {
                this->sign = false;
            }
            k++;
        }

        //Get integer part.
        string int_part = "";
        string frac_part = "";
        while (number[k] != '.' && number[k] != 'e') {
            int_part.push_back((char)(number[k]));
            exponent_bias++;
            k++;
        }
        if (number[k] == '.') {
            k++;
            while (number[k] != 'e') {
                frac_part.push_back((char)(number[k]));
                k++;
            }
        }
        while (int_part[0] == '0') {
            exponent_bias--;
            int_part.erase(0, 1);
        }
        while (int_part.length() < ENOBS) {
            int_part.push_back('0');
        }

        //Convert to binary.


        //Get exponent.
        string exponent_str;
        k++;
        while (number[k] != 0) {
            exponent_str.push_back(number[k]);
            k++;
        }
        this->exponent = stoi(exponent_str);
        this->exponent += exponent_bias;
    }

    void print() {
        cout << this->sign;
        cout << endl;
        for (uint32_t i = 0; i < LENGTH; i++) {
            cout << this->binary[i];
        }
        cout << endl;
        cout << this->exponent;
        cout << endl;
    }

    high_float operator+(high_float rhs) {
        high_float result;
        bool flag = false;
        for (uint32_t i = 0; i < LENGTH; i++) {
            result.binary[i] = (this->binary[i] ^ rhs.binary[i]) ^ flag;
            if (flag) {
                if (!(this->binary[i] || rhs.binary[i])) {
                    flag = false;
                }
            } else {
                if (this->binary[i] && rhs.binary[i]) {
                    flag = true;
                }
            }
        }
        if (flag) {
            for (uint32_t i = 0; i < LENGTH; i++) {

            }
        }
    }
private:
    void convertToBinary(string number_str) {
        uint32_t length = number_str.length();
        for (uint32_t j = LENGTH - 1; j >= 0; j--) {
            uint32_t temp = 0;
            uint32_t remainder = 0;
            for (uint32_t i = 0; i < ENOBS; i++) {
                if (number_str[i] == '0') {
                    if (remainder == 1) {
                        number_str[i] = '5';
                        remainder = 0;
                    }
                    continue;
                }
                if ((temp = 10 * remainder + (uint32_t)number_str[i] - 48) == 1) {
                    if (i != ENOBS - 1) {
                        temp = 10 + (uint32_t)number_str[i + 1] - 48;
                        number_str[i] = '0';
                        i++;
                    } else {
                        number_str[i] = '0';
                        remainder = 1;
                        break;
                    }
                }
                number_str[i] = (char)(temp / 2 + 48);
                remainder = temp - ((uint32_t)number_str[i] - 48) * 2;
            }
            if (remainder == 1) {
                this->binary[j] = true;
            }
        }
    }
};

int main() {
    high_float a("0.4e3");
    high_float b("0.4e3");
    high_float c;
    a.print();
    b.print();
    c.print();
    return 0;
}