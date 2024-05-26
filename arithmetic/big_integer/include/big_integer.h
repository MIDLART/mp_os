#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_BIGINT_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_BIGINT_H

#include <iostream>
#include <map>
#include <vector>

#include <allocator.h>
#include <allocator_guardant.h>
#include <not_implemented.h>

//#include <logger.h>
//#include <logger_guardant.h>

class big_integer final:
    allocator_guardant
{

public:
    
    enum class multiplication_rule
    {
        trivial,
        Karatsuba,
        SchonhageStrassen
    };

private:
    
    class multiplication
    {
    
    public:
        
        virtual ~multiplication() noexcept = default;
    
    public:
        
        virtual big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const = 0;
        
    };
    
    class trivial_multiplication final:
        public multiplication
    {
    
    public:
        
        big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const override;
        
    };
    
    class Karatsuba_multiplication final:
        public multiplication
    {

    public:
        
        big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const override;
        
    };
    
    class Schonhage_Strassen_multiplication final:
        public multiplication
    {

    public:
        
        big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const override;
        
    };

public:
    
    enum class division_rule
    {
        trivial,
        Newton,
        BurnikelZiegler
    };

private:
    
    class division
    {
    
    public:
        
        virtual ~division() noexcept = default;
    
    public:
        
        virtual big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const = 0;
        
        virtual big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const = 0;
        
    };
    
    class trivial_division final:
        public division
    {
    
    public:
        
        big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;
        
        big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;
        
    };
    
    class Newton_division final:
        public division
    {
    
    public:
        
        big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;
        
        big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;
        
    };
    
    class Burnikel_Ziegler_division final:
        public division
    {
    
    public:
        
        big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;
        
        big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;
        
    };

private:

    int _oldest_digit;
    unsigned int *_other_digits;
    allocator *_allocator;
    //TODO logger

public:

    big_integer(
            int digit,
            allocator *allocator);

    big_integer(
        int const *digits,
        size_t digits_count,
        allocator *allocator = nullptr);

    explicit big_integer(
        std::vector<int> const &digits,
        allocator *allocator = nullptr);

    explicit big_integer(
        std::string const &value_as_string,
        size_t base = 10,
        allocator *allocator = nullptr);

public:

    ~big_integer() noexcept;
    
    big_integer(
        big_integer const &other);

    big_integer &operator=(
        big_integer const &other);
    
    big_integer(
        big_integer &&other) noexcept;
    
    big_integer &operator=(
        big_integer &&other) noexcept;

public:

    bool operator==(
        big_integer const &other) const;

    bool operator!=(
        big_integer const &other) const;

public:

    bool operator<(
        big_integer const &other) const;

    bool operator>(
        big_integer const &other) const;

    bool operator<=(
        big_integer const &other) const;

    bool operator>=(
        big_integer const &other) const;

public:

    big_integer operator-() const;
    
    big_integer &operator+=(
        big_integer const &other);

    big_integer operator+(
        big_integer const &other) const;

    big_integer operator+(
        std::pair<big_integer, allocator *> const &other) const;
    
    big_integer &operator-=(
        big_integer const &other);

    big_integer operator-(
        big_integer const &other) const;

    big_integer operator-(
        std::pair<big_integer, allocator *> const &other) const;
    
    big_integer &operator*=(
        big_integer const &other);

    big_integer operator*(
        big_integer const &other) const;

    big_integer operator*(
        std::pair<big_integer, allocator *> const &other) const;
    
    big_integer &operator/=(
        big_integer const &other);

    big_integer operator/(
        big_integer const &other) const;

    big_integer operator/(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator%=(
        big_integer const &other);

    big_integer operator%(
        big_integer const &other) const;

    big_integer operator%(
        std::pair<big_integer, allocator *> const &other) const;

public:

    big_integer operator~() const;

    big_integer &operator&=(
        big_integer const &other);

    big_integer operator&(
        big_integer const &other) const;

    big_integer operator&(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator|=(
        big_integer const &other);

    big_integer operator|(
        big_integer const &other) const;

    big_integer operator|(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator^=(
        big_integer const &other);

    big_integer operator^(
        big_integer const &other) const;

    big_integer operator^(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator<<=(
        size_t shift);

    big_integer operator<<(
        size_t shift) const;

    big_integer operator<<(
        std::pair<size_t, allocator *> const &shift) const;

    big_integer &operator>>=(
        size_t shift);

    big_integer operator>>(
        size_t shift) const;

    big_integer operator>>(
        std::pair<size_t, allocator *> const &shift) const;

public:

    static big_integer &multiply(
        big_integer &first_multiplier,
        big_integer const &second_multiplier,
        allocator *allocator = nullptr,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer multiply(
        big_integer const &first_multiplier,
        big_integer const &second_multiplier,
        allocator *allocator = nullptr,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer &divide(
        big_integer &dividend,
        big_integer const &divisor,
        allocator *allocator = nullptr,
        big_integer::division_rule division_rule = big_integer::division_rule::trivial,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer divide(
        big_integer const &dividend,
        big_integer const &divisor,
        allocator *allocator = nullptr,
        big_integer::division_rule division_rule = big_integer::division_rule::trivial,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer &modulo(
        big_integer &dividend,
        big_integer const &divisor,
        allocator *allocator = nullptr,
        big_integer::division_rule division_rule = big_integer::division_rule::trivial,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer modulo(
        big_integer const &dividend,
        big_integer const &divisor,
        allocator *allocator = nullptr,
        big_integer::division_rule division_rule = big_integer::division_rule::trivial,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

public:
    
    friend std::ostream &operator<<(
        std::ostream &stream,
        big_integer const &value);
    
    friend std::istream &operator>>(
        std::istream &stream,
        big_integer &value);

private:

    [[nodiscard]] allocator *get_allocator() const noexcept override;

private:

    big_integer &change_sign();

    inline int get_digits_count() const noexcept;

    inline int sign() const noexcept;

    inline bool is_equal_to_zero() const noexcept;

    inline unsigned int get_digit(
            int position) const noexcept;

private:

    void clear();

    void copy_from(
            big_integer const &other);

    void move_from(
            big_integer &&other);

private:

    void initialize_from(
            int const *digits,
            size_t digits_count);

    void initialize_from(
            std::vector<int> const &digits,
            size_t digits_count);

    void initialize_from(
            std::string const &value_as_string,
            size_t base);

private:

    void print_byte(
            std::ostream &stream,
            unsigned char byte_value) const;

    void dump_int_value(
            std::ostream &stream,
            int value) const;

public:

    void dump_value(
            std::ostream &stream) const;
    
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_BIGINT_H