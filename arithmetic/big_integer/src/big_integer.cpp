#include <cstring>
#include <limits>
#include "../include/big_integer.h"

big_integer &big_integer::trivial_multiplication::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier) const
{
    throw not_implemented("big_integer &big_integer::trivial_multiplication::multiply(big_integer &, big_integer const &)", "your code should be here...");
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
    _other_digits = (digits_count == 1
                     ? nullptr
                     : new unsigned int[digits_count]);

    if (_other_digits == nullptr)
    {
        return;
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

    if (_allocator != nullptr)
    {
        _other_digits = static_cast<unsigned int *>(_allocator->allocate(sizeof(unsigned int), digits_count));
    }
    else
    {
        _other_digits = new unsigned int [digits_count];
    }

    *_other_digits = static_cast<unsigned int>(digits_count);

    for (auto i = 0; i < digits_count - 1; ++i)
    {
        _other_digits[1 + i] = *reinterpret_cast<unsigned int const *>(&digits[i]);
    }
}

void big_integer::initialize_from(
        std::string const &value_as_string,
        size_t base)
{
    _oldest_digit = 0;
    _other_digits = nullptr;

    bool negative_flag = false;
    size_t pos = 0;

    if (value_as_string[pos] == '-')
    {
        negative_flag = true;
        ++pos;
    }

    //TODO


    if (negative_flag)
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
    throw not_implemented("big_integer &big_integer::operator*=(big_integer const &)", "your code should be here...");
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

big_integer &big_integer::operator/=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator/=(big_integer const &)", "your code should be here...");
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
    throw not_implemented("big_integer &big_integer::operator%=(big_integer const &)", "your code should be here...");
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
        clear();
        std::vector<int> empty;
        initialize_from(empty, 1);
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

    auto value_sign = sign();
    if (value_sign == -1)
    {
        change_sign();
    }

    //TODO

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
    throw not_implemented("big_integer big_integer::operator<<(std::pair<size_t, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator>>=(
    size_t shift)
{
    throw not_implemented("big_integer &big_integer::operator>>=(size_t)", "your code should be here...");
}

big_integer big_integer::operator>>(
    size_t shift) const
{
    throw not_implemented("big_integer big_integer::operator>>(size_t) const", "your code should be here...");
}

big_integer big_integer::operator>>(
    std::pair<size_t, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator>>(std::pair<size_t, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier,
    allocator *allocator,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer &big_integer::multiply(big_integer &, big_integer const &, allocator *, big_integer::multiplication_rule)", "your code should be here...");
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
    throw not_implemented("std::ostream &operator<<(std::ostream &, big_integer const &)", "your code should be here...");
}

std::istream &operator>>(
    std::istream &stream,
    big_integer &value)
{
    throw not_implemented("std::istream &operator>>(std::istream &, big_integer &)", "your code should be here...");
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