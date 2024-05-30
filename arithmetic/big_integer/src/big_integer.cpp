#include <cstring>
#include <cmath>
#include <limits>
#include <algorithm>
#include "../include/big_integer.h"

big_integer &big_integer::trivial_multiplication::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier) const
{
    return first_multiplier *= second_multiplier;
}

big_integer &big_integer::Karatsuba_multiplication::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier) const
{
    throw not_implemented("big_integer &big_integer::Karatsuba_multiplication::multiply(big_integer &, big_integer const &)", "your code should be here...");
}

big_integer &big_integer::Schonhage_Strassen_multiplication::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier) const
{
    throw not_implemented("big_integer &big_integer::Schonhage_Strassen_multiplication::multiply(big_integer &, big_integer const &)", "your code should be here...");
}

big_integer &big_integer::trivial_division::divide(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::trivial_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::trivial_division::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::trivial_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Newton_division::divide(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Newton_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Newton_division::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Newton_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Burnikel_Ziegler_division::divide(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Burnikel_Ziegler_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Burnikel_Ziegler_division::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Burnikel_Ziegler_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

void big_integer::initialize_from(
        int const *digits,
        size_t digits_count)
{
    if (digits == nullptr)
    {
        throw std::logic_error("pointer to digits array must not be nullptr");
    }

    if (digits_count == 0)
    {
        throw std::logic_error("digits array length must  be GT 0");
    }

    _oldest_digit = digits[digits_count - 1];

    if (digits_count == 1)
    {
        return;
    }

    try
    {
        _other_digits = reinterpret_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), digits_count));
    }
    catch (std::bad_alloc const &)
    {
        clear();
        throw;
    }

    *_other_digits = (unsigned int)digits_count;

    std::memcpy(_other_digits + 1, digits, sizeof(unsigned int) * (digits_count - 1));
}

void big_integer::initialize_from(
        std::vector<int> const &digits,
        size_t digits_count)
{
    _other_digits = nullptr;

    if (digits.empty())
    {
        _oldest_digit = 0;

        return;
        // throw std::logic_error("std::vector<int> of digits should not be empty");
    }

    _oldest_digit = digits[digits_count - 1];

    if (digits_count == 1)
    {
        return;
    }

    try
    {
        _other_digits = reinterpret_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), digits_count));
    }
    catch (std::bad_alloc const &)
    {
        clear();
        throw;
    }

    *_other_digits = static_cast<unsigned int>(digits_count);

    for (auto i = 0; i < digits_count - 1; ++i)
    {
        if (i < digits.size())
        {
            _other_digits[1 + i] = *reinterpret_cast<unsigned int const *>(&digits[i]);
        }
        else
        {
            _other_digits[1 + i] = 0;
        }
    }
}

void big_integer::initialize_from(
        std::string const &value_as_string,
        size_t base)
{
    _oldest_digit = 0;
    _other_digits = nullptr;

    bool is_negative = false;
    size_t position = 0;

    if (value_as_string[position] == '-')
    {
        is_negative = true;
        ++position;
    }

    for (; position < value_as_string.size(); ++position)
    {
        unsigned int digit = char_to_int(value_as_string[position]);

        if (digit >= base)
        {
            throw std::logic_error("Invalid digit");
        }

        *this *= big_integer(base, this->_allocator);
        *this += big_integer(digit, this->_allocator);
    }

    if (is_negative)
    {
        change_sign();
    }
}

big_integer::big_integer(
        int digit,
        allocator *allocator):
        _oldest_digit(digit),
        _other_digits(nullptr),
        _allocator(allocator)
{ }

big_integer::big_integer(
    int const *digits,
    size_t digits_count,
    allocator *allocator):
    _allocator(allocator)
{
    initialize_from(digits, digits_count);
}

big_integer::big_integer(
    std::vector<int> const &digits,
    allocator *allocator):
    _allocator(allocator)
{
    initialize_from(digits, digits.size());
}

big_integer::big_integer(
    std::string const &value_as_string,
    size_t base,
    allocator *allocator):
    _allocator(allocator)

{
    initialize_from(value_as_string, base);
}

big_integer::~big_integer()
{
    clear();
    delete _allocator;
}

big_integer::big_integer(
    big_integer const &other)
{
    copy_from(other);
}

big_integer &big_integer::operator=(
    big_integer const &other)
{
    if (this != &other)
    {
        clear();
        copy_from(other);
    }

    return *this;
}

big_integer::big_integer(
    big_integer &&other) noexcept
{
    move_from(std::move(other));
}

big_integer &big_integer::operator=(
    big_integer &&other) noexcept
{
    if (this != &other)
    {
        clear();
        move_from(std::move(other));
    }

    return *this;
}

bool big_integer::operator==(
    big_integer const &other) const
{
    if (get_digits_count() != other.get_digits_count())
    {
        return false;
    }

    auto digits_count = get_digits_count();

    for (int i = 0; i < digits_count; ++i)
    {
        unsigned int first_value_digit = get_digit(i);
        unsigned int second_value_digit = other.get_digit(i);

        if (first_value_digit != second_value_digit)
        {
            return false;
        }
    }

    return true;
}

bool big_integer::operator!=(
    big_integer const &other) const
{
    return !(*this == other);
}

bool big_integer::operator<(
    big_integer const &other) const
{
    if (sign() == -1 && other.sign() == -1)
    {
        return -*this > -other;
    }
    else if (sign() == -1)
    {
        return true;
    }
    else if (other.sign() == -1)
    {
        return false;
    }

    if (get_digits_count() < other.get_digits_count())
    {
        return true;
    }

    if (get_digits_count() > other.get_digits_count())
    {
        return false;
    }

    auto digits_count = get_digits_count();

    for (int i = digits_count - 1; i >= 0; --i)
    {
        unsigned int first_value_digit = get_digit(i);
        unsigned int second_value_digit = other.get_digit(i);

        if (first_value_digit < second_value_digit)
        {
            return true;
        }
        else if (first_value_digit != second_value_digit)
        {
            return false;
        }
    }

    return false;
}

bool big_integer::operator>(
    big_integer const &other) const
{
    if (sign() == -1 && other.sign() == -1)
    {
        return -*this < -other;
    }
    else if (sign() == -1)
    {
        return false;
    }
    else if (other.sign() == -1)
    {
        return true;
    }

    if (get_digits_count() > other.get_digits_count())
    {
        return true;
    }

    if (get_digits_count() < other.get_digits_count())
    {
        return false;
    }

    auto digits_count = get_digits_count();

    for (int i = digits_count - 1; i >= 0; --i)
    {
        unsigned int first_value_digit = get_digit(i);
        unsigned int second_value_digit = other.get_digit(i);

        if (first_value_digit > second_value_digit)
        {
            return true;
        }
        else if (first_value_digit != second_value_digit)
        {
            return false;
        }
    }

    return false;
}

bool big_integer::operator<=(
    big_integer const &other) const
{
    return !(*this > other);
}

bool big_integer::operator>=(
    big_integer const &other) const
{
    return !(*this < other);
}

big_integer big_integer::operator-() const
{
    return big_integer(*this).change_sign();
}

big_integer &big_integer::operator+=(
    big_integer const &other)
{
    if (other.is_equal_to_zero())
    {
        return *this;
    }

    if (is_equal_to_zero())
    {
        return *this = other;
    }

    if (sign() == -1)
    {
        change_sign();
        *this += -other;
        return change_sign();
    }

    if (other.sign() == -1)
    {
        return *this -= -other;
    }

    unsigned int operation_result = 0;

    auto const digits_count = std::max(
            get_digits_count(), other.get_digits_count());

    constexpr int shift = sizeof(unsigned int) << 2;
    constexpr int mask = (1 << shift) - 1;

    std::vector<int> result_digits(digits_count + 1);

    for (int i = 0; i < digits_count; ++i)
    {
        unsigned int first_value_digit = get_digit(i);
        unsigned int second_value_digit = other.get_digit(i);
        result_digits[i] = 0;

        for (int j = 0; j < 2; ++j)
        {
            operation_result += (first_value_digit & mask) + (second_value_digit & mask);
            first_value_digit >>= shift;
            second_value_digit >>= shift;
            *reinterpret_cast<unsigned int *>(&result_digits[i]) |= ((operation_result & mask) << shift * j);
            operation_result >>= shift;
        }
    }

    result_digits.back() += *reinterpret_cast<int *>(&operation_result);

    if (result_digits.back() == 0)
    {
        result_digits.pop_back();
    }

    if (result_digits.back() & (1 << ((sizeof(int) << 3) - 1)))
    {
        result_digits.push_back(0);
    }

    auto result_digits_count = result_digits.size();

    clear();
    initialize_from(result_digits, result_digits_count);

    return *this;
}

big_integer big_integer::operator+(
    big_integer const &other) const
{
    return big_integer(*this) += other;
}

big_integer big_integer::operator+(
    std::pair<big_integer, allocator *> const &other) const
{
    return big_integer(0, other.second) += big_integer(*this) += other.first;
}

big_integer &big_integer::operator-=(
    big_integer const &other)
{
    if (other.is_equal_to_zero())
    {
        return *this;
    }

    if (is_equal_to_zero())
    {
        *this = other;
        change_sign();
        return *this;
    }

    if (sign() == -1 && other.sign() == -1)
    {
        change_sign();
        *this += -other;
        change_sign();
        return *this;
    }
    else if (sign() == -1)
    {
        change_sign();
        *this += other;
        return change_sign();
    }
    else if (other.sign() == -1)
    {
        return *this += -other;
    }

    if (*this < other)
    {
        big_integer res(other);
        res -= *this;

        clear();
        copy_from(res);
        return change_sign();;
    }

    auto const digits_count = std::max(
            get_digits_count(), other.get_digits_count());

    std::vector<int> result_digits(digits_count);

    unsigned int taken = 0;
    for (int i = 0; i < digits_count; ++i)
    {
        unsigned int first_value_digit = get_digit(i);
        unsigned int second_value_digit = other.get_digit(i);
        result_digits[i] = 0;

        if (second_value_digit == UINT_MAX)
        {
            if (first_value_digit == UINT_MAX)
            {
                *reinterpret_cast<unsigned int *>(&result_digits[i]) = taken ? UINT_MAX : 0;
            }
            else
            {
                *reinterpret_cast<unsigned int *>(&result_digits[i]) = first_value_digit - taken + 1;
                taken = 1;
            }
        }
        else if (first_value_digit < second_value_digit + taken)
        {
            *reinterpret_cast<unsigned int *>(&result_digits[i]) =
                    first_value_digit + (UINT_MAX - second_value_digit - taken) + 1;

            taken = 1;
        }
        else
        {
            *reinterpret_cast<unsigned int *>(&result_digits[i]) = first_value_digit - second_value_digit - taken;
            taken = 0;
        }
    }

    while (result_digits.back() == 0)
    {
        result_digits.pop_back();
    }

    if (result_digits.back() & (1 << ((sizeof(int) << 3) - 1)))
    {
        result_digits.push_back(0);
    }

    auto result_digits_count = result_digits.size();

    clear();
    initialize_from(result_digits, result_digits_count);

    return *this;
}

big_integer big_integer::operator-(
    big_integer const &other) const
{
    return big_integer(*this) -= other;
}

big_integer big_integer::operator-(
    std::pair<big_integer, allocator *> const &other) const
{
    return big_integer(0, other.second) += big_integer(*this) -= other.first;
}

big_integer &big_integer::operator*=(
    big_integer const &other)
{
    if (other.is_equal_to_zero())
    {
        return *this = other;
    }

    if (is_equal_to_zero())
    {
        return *this;
    }

    if (other.is_equal_to_one())
    {
        return *this;
    }

    if (is_equal_to_one())
    {
        return *this = other;
    }

    if (sign() == -1 && other.sign() == -1)
    {
        change_sign();
        *this *= -other;
        return *this;
    }
    else if (sign() == -1)
    {
        change_sign();
        *this *= other;
        return change_sign();
    }
    else if (other.sign() == -1)
    {
        *this *= -other;
        return change_sign();
    }

    unsigned int operation_result = 0;

    auto const first_digits_count = get_digits_count();
    auto const second_digits_count = other.get_digits_count();
    auto const max_digits_count = first_digits_count + second_digits_count;

    constexpr int shift = sizeof(unsigned int) << 2;
    constexpr int mask = (1 << shift) - 1;

    std::vector<int> result_digits(max_digits_count, 0);
    std::vector<unsigned int> half_digits_result(2 * max_digits_count, 0);

    for (int i = 0; i < 2 * first_digits_count; ++i)
    {
        unsigned int first_value_digit = get_digit(i/2);
        unsigned int first = i & 1 ?
                first_value_digit >> shift :
                first_value_digit & mask;

        for (int j = 0; j < 2 * second_digits_count; ++j)
        {
            unsigned int second_value_digit = other.get_digit(j/2);
            unsigned int second = j & 1 ?
                    second_value_digit >> shift :
                    second_value_digit & mask;

            operation_result += first * second;
            half_digits_result[i + j] += (operation_result & mask);
            operation_result >>= shift;
        }
    }

    constexpr auto left_max = UINT_MAX >> shift;
    constexpr auto right_max = 1 << shift;
    unsigned int left;
    unsigned int right;

    for (int i = 0; i < max_digits_count; ++i)
    {
//        std::cout << half_digits_result[2 * i + 1] << "  " << half_digits_result[2 * i] << std::endl;
//        auto tmp_1 = half_digits_result[2 * i + 1] / left_max;
//        if (tmp_1)
//        {
//            left = half_digits_result[2 * i + 1] % left_max;
//            half_digits_result[2 * (i + 1)] += tmp_1;
//
//            //std::cout << " _1 " << std::endl;
//        }
//        else
//        {
//            left = half_digits_result[2 * i + 1];
//
//            //std::cout << " _2 " << std::endl;
//        }
//
//        left <<= shift;
//
//        if (tmp_1 * right_max > left)
//        {
//            half_digits_result[2 * (i + 1)] -= 1;
//            left += UINT_MAX - tmp_1 * right_max + 1;
//
//            //std::cout << " _3 " << std::endl;
//        }
//        else
//        {
//            left -= tmp_1 * right_max;
//
//            //std::cout << " _4 " << std::endl;
//        }
//
//        right = half_digits_result[2 * i];

        result_digits[i] = (half_digits_result[2 * i + 1] << shift) + half_digits_result[2 * i];
//        *reinterpret_cast<unsigned int *>(&result_digits[i]) = left + right;
    }

    result_digits.back() += *reinterpret_cast<int *>(&operation_result);

    std::cout << "_"<< operation_result << std::endl;

    while (result_digits.back() == 0)
    {
        result_digits.pop_back();
    }

    if (result_digits.back() & (1 << ((sizeof(int) << 3) - 1)))
    {
        result_digits.push_back(0);
    }

    auto result_digits_count = result_digits.size();

    clear();
    initialize_from(result_digits, result_digits_count);

    return *this;
}

big_integer big_integer::operator*(
    big_integer const &other) const
{
    return big_integer(*this) *= other;
}

big_integer big_integer::operator*(
    std::pair<big_integer, allocator *> const &other) const
{
    return big_integer(0, other.second) += big_integer(*this) *= other.first;
}

std::pair<big_integer, big_integer> big_integer::division_with_remainder(
        big_integer const &divisible,
        big_integer const &divisor)
{
    if (divisor.is_equal_to_zero())
    {
        throw std::logic_error("There was an attempt to divide by zero!");
    }

    if (divisible.is_equal_to_zero())
    {
        return std::make_pair(big_integer(0), big_integer(0));
    }

    if (divisor.is_equal_to_one())
    {
        return std::make_pair(divisible, big_integer(0));
    }

    if (divisible.sign() == -1 && divisor.sign() == -1)
    {
        return division_with_remainder(-divisible, -divisor);
    }
    else if (divisible.sign() == -1)
    {
        auto [res, rem] = division_with_remainder(-divisible, divisor);
        return std::make_pair(-res, -rem);
    }
    else if (divisor.sign() == -1)
    {
        auto [res, rem] = division_with_remainder(divisible, -divisor);
        return std::make_pair(-res, -rem);
    }

    if (divisible == divisor)
    {
        return std::make_pair(big_integer(1), big_integer(0));
    }

    if (divisible < divisor)
    {
        return std::make_pair(big_integer(0), divisible);
    }

    unsigned int operation_result = 0;
    auto bit_count = (sizeof(int) * 8);

    auto const first_digits_count = divisible.get_digits_count();
    auto const second_digits_count = divisor.get_digits_count();

    std::vector<int> result_digits(1, 0);
    big_integer minuend(std::vector<int>{0});

    for (int i = first_digits_count - 1; i >= 0; --i)
    {
        unsigned int first_value_digit = divisible.get_digit(i);

        minuend <<= bit_count;
        if ((1 << ((sizeof(int) << 3) - 1)) & first_value_digit)
        {
            minuend += big_integer (std::vector<int>{*reinterpret_cast<int*>(&first_value_digit), 0});
        }
        else
        {
            minuend += big_integer (std::vector<int>{*reinterpret_cast<int*>(&first_value_digit)});
        }
        big_integer subtrahend(std::vector<int>{0});

        if (minuend >= divisor)
        {
            unsigned int digit = 0;

            for (unsigned int k = 1 << (bit_count - 1); k > 0; k >>= 1)
            {
                big_integer tmp = divisor * big_integer(std::vector<int>({*reinterpret_cast<int *>(&k), 0}));

                if (subtrahend + tmp <= minuend)
                {
                    subtrahend += tmp;
                    digit += k;
                }
            }

            minuend -= subtrahend;

            result_digits.push_back(*reinterpret_cast<int*>(&digit));
        }
        else
        {
            result_digits.push_back(0);
        }
    }

    std::reverse(result_digits.begin(), result_digits.end());

    while (result_digits.back() == 0)
    {
        result_digits.pop_back();
    }

    if (result_digits.back() & (1 << ((sizeof(int) << 3) - 1)))
    {
        result_digits.push_back(0);
    }

//    auto result_digits_count = result_digits.size();
//    initialize_from(result_digits, result_digits_count);

    return std::make_pair(big_integer(result_digits), minuend);
}

big_integer &big_integer::operator/=(
    big_integer const &other)
{
    auto tmp = division_with_remainder(*this, other);
    return *this = tmp.first;
//    if (other.is_equal_to_zero())
//    {
//        throw std::logic_error("There was an attempt to divide by zero!");
//    }
//
//    if (is_equal_to_zero())
//    {
//        return *this;
//    }
//
//    if (other.is_equal_to_one())
//    {
//        return *this;
//    }
//
//    if (*this < other)
//    {
//        clear();
//        initialize_from(std::vector<int>{0}, 1);
//
//        return *this = other;
//    }
//
//    if (*this == other)
//    {
//        clear();
//        initialize_from(std::vector<int>{1}, 1);
//
//        return *this = other;
//    }
//
//    if (sign() == -1 && other.sign() == -1)
//    {
//        change_sign();
//        *this /= -other;
//        return *this;
//    }
//    else if (sign() == -1)
//    {
//        change_sign();
//        *this /= other;
//        return change_sign();
//    }
//    else if (other.sign() == -1)
//    {
//        *this /= -other;
//        return change_sign();
//    }
//
//    unsigned int operation_result = 0;
//    auto bit_count = (sizeof(int) * 8);
//
//    auto const first_digits_count = get_digits_count();
//    auto const second_digits_count = other.get_digits_count();
//
////    constexpr int shift = sizeof(unsigned int) << 2;
////    constexpr int mask = (1 << shift) - 1;
//
//    std::vector<int> result_digits(1, 0);
//    big_integer minuend(std::vector<int>{0});
//
//    for (int i = first_digits_count - 1; i >= 0; --i)
//    {
//        unsigned int first_value_digit = get_digit(i);
//
//        minuend <<= bit_count;
//        if ((1 << ((sizeof(int) << 3) - 1)) & first_value_digit)
//        {
//            minuend += big_integer (std::vector<int>{*reinterpret_cast<int*>(&first_value_digit), 0});
//        }
//        else
//        {
//            minuend += big_integer (std::vector<int>{*reinterpret_cast<int*>(&first_value_digit)});
//        }
//        big_integer subtrahend(std::vector<int>{0});
//
//        if (minuend >= other)
//        {
//            unsigned int digit = 0;
//
//            for (unsigned int k = 1 << (bit_count - 1); k > 0; k >>= 1)
//            {
//                big_integer tmp = other * big_integer(std::vector<int>({*reinterpret_cast<int *>(&k), 0}));
//
//                if (subtrahend + tmp <= minuend)
//                {
//                    subtrahend += tmp;
//                    digit += k;
//                }
//            }
//
//            minuend -= subtrahend;
//
//            result_digits.push_back(*reinterpret_cast<int*>(&digit));
//        }
//        else
//        {
//            result_digits.push_back(0);
//        }
//    }
//
//    std::reverse(result_digits.begin(), result_digits.end());
//
//    while (result_digits.back() == 0)
//    {
//        result_digits.pop_back();
//    }
//
//    if (result_digits.back() & (1 << ((sizeof(int) << 3) - 1)))
//    {
//        result_digits.push_back(0);
//    }
//
//    auto result_digits_count = result_digits.size();
//
//    clear();
//    initialize_from(result_digits, result_digits_count);
//
//    return *this;
}

big_integer big_integer::operator/(
    big_integer const &other) const
{
    return big_integer(*this) /= other;
}

big_integer big_integer::operator/(
    std::pair<big_integer, allocator *> const &other) const
{
    return big_integer(0, other.second) += big_integer(*this) /= other.first;
}

big_integer &big_integer::operator%=(
    big_integer const &other)
{
    auto tmp = division_with_remainder(*this, other);
    return *this = tmp.second;
}

big_integer big_integer::operator%(
    big_integer const &other) const
{
    return big_integer(*this) %= other;
}

big_integer big_integer::operator%(
    std::pair<big_integer, allocator *> const &other) const
{
    return big_integer(0, other.second) += big_integer(*this) %= other.first;
}

big_integer big_integer::operator~() const
{
    auto const digits_count = get_digits_count();

    std::vector<int> result_digits(digits_count);

    for (int i = 0; i < digits_count; ++i)
    {
        unsigned int value_digit = get_digit(i);

        *reinterpret_cast<unsigned int *>(&result_digits[i]) = ~value_digit;
    }

    while (result_digits.back() == 0)
    {
        result_digits.pop_back();
    }

    return big_integer(result_digits);
}

big_integer &big_integer::operator&=(
    big_integer const &other)
{
    if (other.is_equal_to_zero())
    {
        *this = other;
        return *this;
    }

    if (is_equal_to_zero())
    {
        return *this;
    }

    auto const digits_count = std::max(
            get_digits_count(), other.get_digits_count());

    std::vector<int> result_digits(digits_count);

    for (int i = 0; i < digits_count; ++i)
    {
        unsigned int first_value_digit = get_digit(i);
        unsigned int second_value_digit = other.get_digit(i);

        *reinterpret_cast<unsigned int *>(&result_digits[i]) = first_value_digit & second_value_digit;
    }

    while (result_digits.back() == 0)
    {
        result_digits.pop_back();
    }

    auto result_digits_count = result_digits.size();

    clear();
    initialize_from(result_digits, result_digits_count);

    return *this;
}

big_integer big_integer::operator&(
    big_integer const &other) const
{
    return big_integer(*this) &= other;
}

big_integer big_integer::operator&(
    std::pair<big_integer, allocator *> const &other) const
{
    return big_integer(0, other.second) += big_integer(*this) &= other.first;
}

big_integer &big_integer::operator|=(
    big_integer const &other)
{
    if (other.is_equal_to_zero())
    {
        return *this;
    }

    if (is_equal_to_zero())
    {
        copy_from(other);
        return *this;
    }

    auto const digits_count = std::max(
            get_digits_count(), other.get_digits_count());

    std::vector<int> result_digits(digits_count);

    for (int i = 0; i < digits_count; ++i)
    {
        unsigned int first_value_digit = get_digit(i);
        unsigned int second_value_digit = other.get_digit(i);

        *reinterpret_cast<unsigned int *>(&result_digits[i]) = first_value_digit | second_value_digit;
    }

    auto result_digits_count = result_digits.size();

    clear();
    initialize_from(result_digits, result_digits_count);

    return *this;
}

big_integer big_integer::operator|(
    big_integer const &other) const
{
    return big_integer(*this) |= other;
}

big_integer big_integer::operator|(
    std::pair<big_integer, allocator *> const &other) const
{
    return big_integer(0, other.second) += big_integer(*this) |= other.first;
}

big_integer &big_integer::operator^=(
    big_integer const &other)
{
    auto const digits_count = std::max(
            get_digits_count(), other.get_digits_count());

    std::vector<int> result_digits(digits_count);

    for (int i = 0; i < digits_count; ++i)
    {
        unsigned int first_value_digit = get_digit(i);
        unsigned int second_value_digit = other.get_digit(i);

        *reinterpret_cast<unsigned int *>(&result_digits[i]) = first_value_digit ^ second_value_digit;
    }

    while (result_digits.back() == 0)
    {
        result_digits.pop_back();
    }

    auto result_digits_count = result_digits.size();

    clear();
    initialize_from(result_digits, result_digits_count);

    return *this;
}

big_integer big_integer::operator^(
    big_integer const &other) const
{
    return big_integer(*this) ^= other;
}

big_integer big_integer::operator^(
    std::pair<big_integer, allocator *> const &other) const
{
    return big_integer(0, other.second) += big_integer(*this) ^= other.first;
}

big_integer &big_integer::operator<<=(
    size_t shift)
{
    if (is_equal_to_zero() || shift == 0)
    {
        return *this;
    }

    if (sign() == -1)
    {
        change_sign();
        *this <<= shift;
        return change_sign();
    }

    auto bit_count = (sizeof(int) * 8);
    auto big_shift = shift / bit_count;
    shift %= bit_count;

    auto digits_count = get_digits_count();
    std::vector<int> result_digits(digits_count + big_shift);

    for (int i = 0; i < big_shift; ++i)
    {
        result_digits[i] = 0;
    }

    unsigned int remain = 0;

    for (int i = 0; i < digits_count; ++i)
    {
        unsigned int value_digit = get_digit(i);

        *reinterpret_cast<unsigned int *>(&result_digits[i + big_shift]) = (value_digit << shift) | remain;
        remain = bit_count - shift == 32 ?
                0 :
                value_digit >> (bit_count - shift);
//        remain = value_digit >> (bit_count - shift);
//        std::cout << value_digit << "  " << bit_count - shift << "  " << remain << std::endl;
    }

    if (remain != 0)
    {
        result_digits.push_back(static_cast<int>(remain));
    }

    if (result_digits.back() & (1 << ((sizeof(int) << 3) - 1)))
    {
        result_digits.push_back(0);
    }

    auto result_digits_count = result_digits.size();

    clear();
    initialize_from(result_digits, result_digits_count);

    return *this;
}

big_integer big_integer::operator<<(
    size_t shift) const
{
    return big_integer(*this) <<= shift;
}

big_integer big_integer::operator<<(
    std::pair<size_t, allocator *> const &shift) const
{
    return big_integer(0, shift.second) += big_integer(*this) <<= shift.first;
}

big_integer &big_integer::operator>>=(
    size_t shift)
{
    if (is_equal_to_zero() || shift == 0)
    {
        return *this;
    }

//    auto value_sign = sign();
//    if (value_sign == -1)
//    {
//        change_sign();
//    }

    auto bit_count = (sizeof(int) * 8);
    auto big_shift = shift / bit_count;
    shift %= bit_count;

    auto digits_count = get_digits_count();
    if (digits_count <= big_shift)
    {
        clear();
        initialize_from(std::vector<int>{0}, 1);

        return *this;
    }

    std::vector<int> result_digits(digits_count - big_shift);

    unsigned int remain = 0;

    for (int i = static_cast<int>(big_shift); i < digits_count; ++i)
    {
        unsigned int value_digit = get_digit(i);

        *reinterpret_cast<unsigned int *>(&result_digits[i - big_shift]) = (value_digit >> shift) | remain;
//        remain = value_digit << (bit_count - shift);
        remain = bit_count - shift == 32 ?
                 0 :
                 value_digit << (bit_count - shift);
    }

    while (result_digits.back() == 0)
    {
        result_digits.pop_back();
    }

    auto result_digits_count = result_digits.size();

    clear();
    initialize_from(result_digits, result_digits_count);

    return *this;
}

big_integer big_integer::operator>>(
    size_t shift) const
{
    return big_integer(*this) >>= shift;
}

big_integer big_integer::operator>>(
    std::pair<size_t, allocator *> const &shift) const
{
    return big_integer(0, shift.second) += big_integer(*this) >>= shift.first;
}

big_integer &big_integer::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier,
    allocator *allocator,
    big_integer::multiplication_rule multiplication_rule)
{
    switch (multiplication_rule)
    {
        case multiplication_rule::trivial:
        {
//            return trivial_multiplication::multiply(first_multiplier, second_multiplier);
            return first_multiplier *= second_multiplier;
        }
        case multiplication_rule::Karatsuba:
        {
            break;
        }
        case multiplication_rule::SchonhageStrassen:
        {
            break;
        }
        default:
        {
            break;
        }
    }
}

big_integer big_integer::multiply(
    big_integer const &first_multiplier,
    big_integer const &second_multiplier,
    allocator *allocator,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer big_integer::multiply(big_integer const &, big_integer const &, allocator *, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::divide(
    big_integer &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer &big_integer::divide(big_integer &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer big_integer::divide(
    big_integer const &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer big_integer::divide(big_integer const &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer &big_integer::modulo(big_integer &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer big_integer::modulo(
    big_integer const &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer big_integer::modulo(big_integer const &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

std::ostream &operator<<(
    std::ostream &stream,
    big_integer const &value)
{
    return stream << value.to_string();
}

std::istream &operator>>(
    std::istream &stream,
    big_integer &value)
{
    std::string value_as_string;
    stream >> value_as_string;

    value.clear();
    value.initialize_from(value_as_string, 10);

    return stream;
}

[[nodiscard]] allocator *big_integer::get_allocator() const noexcept
{
    return _allocator;
}

big_integer &big_integer::change_sign()
{
    _oldest_digit ^= (1 << ((sizeof(int) << 3) - 1));

    return *this;
}

inline int big_integer::get_digits_count() const noexcept
{
    return static_cast<int>(_other_digits == nullptr
                            ? 1
                            : *_other_digits);
}

inline int big_integer::sign() const noexcept
{
    if (is_equal_to_zero())
    {
        return 0;
    }

    return 1 - (static_cast<int>((*reinterpret_cast<unsigned int const *>(&_oldest_digit) >> ((sizeof(int) << 3) - 1))) << 1);
}

inline bool big_integer::is_equal_to_zero() const noexcept
{
    return _oldest_digit == 0 && _other_digits == nullptr;
}

inline unsigned int big_integer::get_digit(
        int position) const noexcept
{
    if (_other_digits == nullptr)
    {
        return position == 0
               ? _oldest_digit
               : 0;
    }

    int const digits_count = get_digits_count();
    if (position < digits_count - 1)
    {
        return _other_digits[position + 1];
    }

    if (position == digits_count - 1)
    {
        return _oldest_digit;
    }

    return 0;
}

void big_integer::clear()
{
    _allocator = nullptr;
    _oldest_digit = 0;
    delete[] _other_digits;
    _other_digits = nullptr;
}

void big_integer::copy_from(
        big_integer const &other)
{
    _allocator = other._allocator;

    _oldest_digit = other._oldest_digit;
    _other_digits = nullptr;
    if (other._other_digits == nullptr)
    {
        return;
    }

    _other_digits = new unsigned int[*other._other_digits];
    std::memcpy(_other_digits, other._other_digits, sizeof(unsigned int) * (*other._other_digits));
}

void big_integer::move_from(
        big_integer &&other)
{
    _oldest_digit = other._oldest_digit;
    _other_digits = other._other_digits;
    _allocator = other._allocator;

    other._oldest_digit = 0;
    other._other_digits = nullptr;
    other._allocator = nullptr;
}

void big_integer::print_byte(
        std::ostream &stream,
        unsigned char byte_value) const
{
    for (int i = 0; i < 8; i++)
    {
        stream << ((byte_value >> (7 - i)) & 1);
    }
}

void big_integer::dump_int_value(
        std::ostream &stream,
        int value) const
{
    auto *p = reinterpret_cast<unsigned char *>(&value);
    for (int i = 0; i < sizeof(int); i++)
    {
        print_byte(stream, *p++);
        stream << ' ';
    }
}

void big_integer::dump_value(
        std::ostream &stream) const
{
    for (auto i = 0; i < get_digits_count(); ++i)
    {
        auto digit = get_digit(i);
        dump_int_value(stream, *reinterpret_cast<int *>(&digit));
        stream << ' ';
    }
}

inline bool big_integer::is_equal_to_one() const noexcept
{
    return _oldest_digit == 1 && _other_digits == nullptr;
}

unsigned int big_integer::char_to_int(
        char ch)
{
    if ('0' <= ch && ch <= '9')
    {
        return ch - '0';
    }
    else if ('A' <= ch && ch <= 'Z')
    {
        return ch - 'A' + 10;
    }
    else if ('a' <= ch && ch <= 'z')
    {
        return ch - 'a' + 36;
    }

    throw std::logic_error("Invalid digit");
}

std::string big_integer::to_string() const
{
    std::string str;
    str.reserve(10 * get_digits_count() + 1);

    big_integer big_num(*this);

    int sign = big_num.sign();
    if (sign == -1)
    {
        big_num.change_sign();
    }

    size_t big_mod_zeros = 9;
    size_t big_mod = std::pow(10, big_mod_zeros);

    while (!big_num.is_equal_to_zero())
    {
        auto [num, rem] = division_with_remainder(big_num, big_integer(big_mod));
        unsigned int tmp = rem.get_digit(0);

        for (size_t i = 0; i < big_mod_zeros; ++i)
        {
            if (!tmp && num.is_equal_to_zero())
            {
                break;
            }

            str.push_back(tmp % 10 + '0');
            tmp /= 10;
        }

        big_num = num;
    }

    if (is_equal_to_zero())
    {
        str.push_back('0');
    }

    if (sign == -1)
    {
        str.push_back('-');
    }

    std::reverse(str.begin(), str.end());

    return str;
}