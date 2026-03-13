#include "bigint.h"
#include <algorithm>
#include <cctype>

// Конструкторы
BigInt::BigInt() : digits("0"), negative(false) {}

BigInt::BigInt(long long num) : negative(false) {
    if (num < 0) {
        negative = true;
        num = -num;
    }
    
    if (num == 0) {
        digits = "0";
        return;
    }
    
    while (num > 0) {
        digits += static_cast<char>('0' + (num % 10));
        num /= 10;
    }
}

BigInt::BigInt(const std::string& str) : negative(false) {
    if (str.empty()) {
        digits = "0";
        return;
    }
    
    size_t startIdx = 0;
    
    // Проверка на знак минус
    if (str[0] == '-') {
        negative = true;
        startIdx = 1;
    } else if (str[0] == '+') {
        startIdx = 1;
    }
    
    // Валидация: проверка только цифр
    for (size_t i = startIdx; i < str.length(); ++i) {
        if (!std::isdigit(str[i])) {
            throw std::invalid_argument("Недопустимые символы в строке");
        }
    }
    
    if (startIdx >= str.length()) {
        digits = "0";
        negative = false;
        return;
    }
    
    // Копирование цифр в обратном порядке
    for (size_t i = str.length(); i > startIdx; --i) {
        digits += str[i - 1];
    }
    
    removeLeadingZeros();
}

BigInt::BigInt(const BigInt& other) : digits(other.digits), negative(other.negative) {}

// Оператор присваивания
BigInt& BigInt::operator=(const BigInt& other) {
    if (this != &other) {
        digits = other.digits;
        negative = other.negative;
    }
    return *this;
}

// Вспомогательные методы
void BigInt::removeLeadingZeros() {
    while (digits.length() > 1 && digits.back() == '0') {
        digits.pop_back();
    }
    
    if (digits == "0") {
        negative = false;
    }
}

int BigInt::compareAbsolute(const std::string& a, const std::string& b) {
    if (a.length() != b.length()) {
        return a.length() < b.length() ? -1 : 1;
    }
    
    for (size_t i = 0; i < a.length(); ++i) {
        if (a[i] != b[i]) {
            return a[i] < b[i] ? -1 : 1;
        }
    }
    
    return 0;
}

// Операторы сложения
BigInt BigInt::operator+(const BigInt& other) const {
    BigInt result;
    
    // Если знаки одинаковые
    if (negative == other.negative) {
        result.negative = negative;
        std::string sum;
        int carry = 0;
        size_t maxLen = std::max(digits.length(), other.digits.length());
        
        for (size_t i = 0; i < maxLen || carry; ++i) {
            int digit1 = (i < digits.length()) ? (digits[i] - '0') : 0;
            int digit2 = (i < other.digits.length()) ? (other.digits[i] - '0') : 0;
            int total = digit1 + digit2 + carry;
            
            sum += static_cast<char>('0' + (total % 10));
            carry = total / 10;
        }
        
        result.digits = sum;
        result.removeLeadingZeros();
    } else {
        // Разные знаки - вычитание
        int cmp = compareAbsolute(digits, other.digits);
        
        if (cmp == 0) {
            result.digits = "0";
            result.negative = false;
        } else if (cmp > 0) {
            result.negative = negative;
            std::string diff;
            int borrow = 0;
            
            for (size_t i = 0; i < digits.length(); ++i) {
                int digit1 = digits[i] - '0';
                int digit2 = (i < other.digits.length()) ? (other.digits[i] - '0') : 0;
                
                int res = digit1 - digit2 - borrow;
                if (res < 0) {
                    res += 10;
                    borrow = 1;
                } else {
                    borrow = 0;
                }
                
                diff += static_cast<char>('0' + res);
            }
            
            result.digits = diff;
            result.removeLeadingZeros();
        } else {
            result.negative = other.negative;
            std::string diff;
            int borrow = 0;
            
            for (size_t i = 0; i < other.digits.length(); ++i) {
                int digit1 = (i < digits.length()) ? (digits[i] - '0') : 0;
                int digit2 = other.digits[i] - '0';
                
                int res = digit2 - digit1 - borrow;
                if (res < 0) {
                    res += 10;
                    borrow = 1;
                } else {
                    borrow = 0;
                }
                
                diff += static_cast<char>('0' + res);
            }
            
            result.digits = diff;
            result.removeLeadingZeros();
        }
    }
    
    return result;
}

BigInt& BigInt::operator+=(const BigInt& other) {
    *this = *this + other;
    return *this;
}

// Операторы инкремента
BigInt& BigInt::operator++() {
    *this = *this + BigInt(1);
    return *this;
}

BigInt BigInt::operator++(int) {
    BigInt temp = *this;
    *this = *this + BigInt(1);
    return temp;
}

// Операторы умножения
BigInt BigInt::operator*(const BigInt& other) const {
    if (digits == "0" || other.digits == "0") {
        return BigInt(0);
    }
    
    BigInt result;
    result.digits.clear();
    result.digits.resize(digits.length() + other.digits.length(), '0');
    result.negative = (negative != other.negative);
    
    for (size_t i = 0; i < digits.length(); ++i) {
        int carry = 0;
        int digit1 = digits[i] - '0';
        
        for (size_t j = 0; j < other.digits.length() || carry; ++j) {
            int digit2 = (j < other.digits.length()) ? (other.digits[j] - '0') : 0;
            int prod = digit1 * digit2 + carry + (result.digits[i + j] - '0');
            
            result.digits[i + j] = static_cast<char>('0' + (prod % 10));
            carry = prod / 10;
        }
    }
    
    result.removeLeadingZeros();
    return result;
}

BigInt& BigInt::operator*=(const BigInt& other) {
    *this = *this * other;
    return *this;
}

// Операторы сравнения
bool BigInt::operator<(const BigInt& other) const {
    if (negative != other.negative) {
        return negative;
    }
    
    int cmp = compareAbsolute(digits, other.digits);
    
    if (negative) {
        return cmp > 0;
    } else {
        return cmp < 0;
    }
}

bool BigInt::operator>(const BigInt& other) const {
    return other < *this;
}

bool BigInt::operator==(const BigInt& other) const {
    return digits == other.digits && negative == other.negative;
}

bool BigInt::operator!=(const BigInt& other) const {
    return !(*this == other);
}

bool BigInt::operator<=(const BigInt& other) const {
    return *this < other || *this == other;
}

bool BigInt::operator>=(const BigInt& other) const {
    return *this > other || *this == other;
}

// Операторы ввода-вывода
std::ostream& operator<<(std::ostream& os, const BigInt& num) {
    if (num.negative) {
        os << '-';
    }
    
    for (size_t i = num.digits.length(); i > 0; --i) {
        os << num.digits[i - 1];
    }
    
    return os;
}

std::istream& operator>>(std::istream& is, BigInt& num) {
    std::string str;
    is >> str;
    num = BigInt(str);
    return is;
}

// Вспомогательный метод
std::string BigInt::toString() const {
    std::ostringstream oss;
    oss << *this;
    return oss.str();
}