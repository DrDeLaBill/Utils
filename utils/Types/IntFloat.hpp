/* Copyright © 2025 Georgy E. All rights reserved. */

#ifndef _INT_FLOAT_H_
#define _INT_FLOAT_H_


#include <limits>
#include <cstdint>

#include "gutils.h"

#include "TypeListService.h"


class IntFloat {
private:
    using type_t = int64_t;

public:
    static constexpr type_t FRACT_RESOLUTION = 1000000000;
    static constexpr int MAX_ITERATIONS = 20;
    static constexpr type_t EPSILON = 10;

private:
    static constexpr type_t INTEGER_TYPE_MAX = std::numeric_limits<type_t>::max();
    static constexpr type_t MAX_MULTIPLE_CNT = utl::NumbersCount<type_t, INTEGER_TYPE_MAX>::SIZE - 1;

    type_t integer;
    type_t fract;

    type_t calc_fract_resolution(const IntFloat& first, const IntFloat& second) const
    {
        type_t resolution = 1;
        while (resolution < FRACT_RESOLUTION) {
            type_t del = FRACT_RESOLUTION / resolution;
            if (first.fract % del || second.fract % del) {
                resolution *= 10;
                continue;
            }
            break;
        }
        return resolution;
    }

    type_t numbers_count(type_t value) const
    {
        type_t count = 0;
        while (value) {
            value /= 10;
            count++;
        }
        return count;
    }

public:
    IntFloat(const type_t& integer, const type_t& fract, const type_t& resolution): integer(integer), fract(fract)
    {
        if ((integer < 0 && fract > 0) || (integer > 0 && fract < 0)) {
            this->fract *= -1;
        }
        if (resolution) {
            this->fract *= (FRACT_RESOLUTION / resolution);
        } else {
            this->fract = 0;
        }
        if (__abs(this->fract) >= FRACT_RESOLUTION) {
            this->integer += this->fract / FRACT_RESOLUTION;
            this->fract %= FRACT_RESOLUTION;
        }
    }

    IntFloat(): IntFloat(0, 0, 0) {}

    IntFloat(const double& value)
    {
        this->operator=(value);
    }
 
    IntFloat(const uint32_t& value): IntFloat(value, 0, 0) 
    {}
    
    IntFloat(const IntFloat& other) 
    {
        integer = other.integer;
        fract = other.fract;
    }

    IntFloat& operator=(const double& value)
    {
        integer = (int)value;
        fract = ((int)((value - (double)integer) * FRACT_RESOLUTION)) % FRACT_RESOLUTION;
        return *this;
    }

    IntFloat& operator=(const IntFloat& other) 
    {
        integer = other.integer;
        fract = other.fract;
        return *this;
    }

    IntFloat operator+(const IntFloat& other) const
    {
        IntFloat res = *this;
        res.fract += other.fract;
        if (__abs(fract) >= FRACT_RESOLUTION) {
            res.integer += fract / FRACT_RESOLUTION;
            res.fract %= FRACT_RESOLUTION;
        }
        res.integer += other.integer;
        return res;
    }
    
    IntFloat operator+=(const IntFloat& other)
    {
        this->fract += other.fract;
        if (__abs(fract) >= FRACT_RESOLUTION) {
            this->integer += fract / FRACT_RESOLUTION;
            this->fract %= FRACT_RESOLUTION;
        }
        this->integer += other.integer;
        return *this;
    }
    
    IntFloat operator-() const
    {
        IntFloat res = *this;
        res.integer *= -1;
        res.fract *= -1;
        return res;
    }   

    IntFloat operator-(const IntFloat& other) const
    {
        IntFloat res = *this;
        if (res.fract < other.fract) {
            res.integer -= 1;
            res.fract = (FRACT_RESOLUTION - (other.fract - fract));
        } else {
            res.fract -= other.fract;
        }
        res.integer -= other.integer;
        return res;
    }
    
    IntFloat operator-=(const IntFloat& other)
    {
        if (this->fract < other.fract) {
            this->integer -= 1;
            this->fract = (FRACT_RESOLUTION - (other.fract - fract));
        } else {
            this->fract -= other.fract;
        }
        this->integer -= other.integer;
        return *this;
    }

    IntFloat operator*(const IntFloat& other) const 
    {
        IntFloat result(0, 0, 0);
        
        type_t resolution = calc_fract_resolution(*this, other);
        type_t fract_del = FRACT_RESOLUTION / resolution;
        type_t this_value = (type_t)integer * resolution + fract / fract_del;
        type_t other_value = (type_t)other.integer * resolution + other.fract / fract_del;

        type_t this_number_cnt = numbers_count(this_value);
        type_t other_number_cnt = numbers_count(other_value);
        bool flag = false;
        do {
            flag = false;
            if (this_number_cnt + other_number_cnt > MAX_MULTIPLE_CNT) {
                this_number_cnt--;
                other_number_cnt--;
                this_value /= 10;
                other_value /= 10;
                resolution /= 10;
                flag = true;
            }
        } while (flag);
        if (!resolution) {
            resolution = 1;
        }
        
        type_t product = this_value * other_value;
        
        type_t resolution_del = resolution * resolution;
        result.integer = (type_t)(product / resolution_del);
        if (FRACT_RESOLUTION >= resolution_del) {
            result.fract = (type_t)((product % resolution_del) * ((type_t)FRACT_RESOLUTION / resolution_del));
        } else {
            result.fract = (type_t)((product % resolution_del) / (resolution_del / (type_t)FRACT_RESOLUTION));
        }
        
        return result;
    }
    
    IntFloat operator*=(const IntFloat& other)
    {
        type_t resolution = calc_fract_resolution(*this, other);
        type_t fract_del = FRACT_RESOLUTION / resolution;
        type_t this_value = (type_t)integer * resolution + fract / fract_del;
        type_t other_value = (type_t)other.integer * resolution + other.fract / fract_del;

        type_t this_number_cnt = numbers_count(this_value);
        type_t other_number_cnt = numbers_count(other_value);
        bool flag = false;
        do {
            flag = false;
            if (this_number_cnt + other_number_cnt > MAX_MULTIPLE_CNT) {
                this_number_cnt--;
                other_number_cnt--;
                this_value /= 10;
                other_value /= 10;
                resolution /= 10;
                flag = true;
            }
        } while (flag);
        if (!resolution) {
            resolution = 1;
        }
        
        type_t product = this_value * other_value;
        
        type_t resolution_del = resolution * resolution;
        this->integer = (type_t)(product / resolution_del);
        if (FRACT_RESOLUTION >= resolution_del) {
            this->fract = (type_t)((product % resolution_del) * ((type_t)FRACT_RESOLUTION / resolution_del));
        } else {
            this->fract = (type_t)((product % resolution_del) / (resolution_del / (type_t)FRACT_RESOLUTION));
        }
        
        return *this;
    }

    IntFloat operator/(const IntFloat& other) const
    {
        IntFloat result(0, 0, 0);
        
        if (other.integer == 0 && other.fract == 0) {
            return result;
        }

        type_t resolution = calc_fract_resolution(*this, other);
        type_t fract_del = FRACT_RESOLUTION / resolution;
        type_t this_value = (type_t)integer * resolution + fract / fract_del;
        type_t other_value = (type_t)other.integer * resolution + other.fract / fract_del;
        
        type_t mult1 = FRACT_RESOLUTION;
        type_t mult2 = 1;
        if (numbers_count(this_value) + numbers_count(FRACT_RESOLUTION) > MAX_MULTIPLE_CNT) {
            type_t this_count = numbers_count(this_value);
            while (mult1 > 1 && this_count + numbers_count(mult1) > MAX_MULTIPLE_CNT) {
                mult1 /= 10;
                mult2 *= 10;
            }
        }
        type_t quotient = ((this_value * mult1) / other_value) * mult2;
        
        result.integer = (type_t)(quotient / (type_t)FRACT_RESOLUTION);
        result.fract = (type_t)(quotient % (type_t)FRACT_RESOLUTION);
        
        return result;
    }
    
    IntFloat operator/=(const IntFloat& other) 
    {
        if (other.integer == 0 && other.fract == 0) {
            this->integer = 0;
            this->fract = 0;
            return *this;
        }

        type_t resolution = calc_fract_resolution(*this, other);
        type_t fract_del = FRACT_RESOLUTION / resolution;
        type_t this_value = (type_t)integer * resolution + fract / fract_del;
        type_t other_value = (type_t)other.integer * resolution + other.fract / fract_del;
        
        type_t quotient = (this_value * FRACT_RESOLUTION) / other_value;
        
        this->integer = (type_t)(quotient / (type_t)FRACT_RESOLUTION);
        this->fract = (type_t)(quotient % (type_t)FRACT_RESOLUTION);
        
        return *this;
    }
    
    bool operator==(const IntFloat& other) const 
    {
        return integer == other.integer && fract == other.fract;
    }

    bool operator!=(const IntFloat& other) const 
    {
        return !(*this == other);
    }

    bool operator<(const IntFloat& other) const 
    {
        if (integer < other.integer) return true;
        if (integer > other.integer) return false;
        return fract < other.fract;
    }

    bool operator>(const IntFloat& other) const 
    {
        return other < *this;
    }

    bool operator<=(const IntFloat& other) const 
    {
        return !(other < *this);
    }

    bool operator>=(const IntFloat& other) const 
    {
        return !(*this < other);
    }

    type_t getInteger() const
    {
        return integer;
    }

    type_t getFract() const
    {
        return __abs(fract);
    }

    float toFloat() const {
        return (float)integer + (float)((float)fract / (float)FRACT_RESOLUTION);
    }
     
    type_t toInt64() const {
        return (type_t)integer * FRACT_RESOLUTION + fract;
    }
    
    static IntFloat zero() { return IntFloat(0, 0, 0); }
    static IntFloat one() { return IntFloat(1, 0, 0); }
    static IntFloat two() { return IntFloat(2, 0, 0); }
    static IntFloat half() { return IntFloat(0, 5, 10); }

    static IntFloat pi()
    {
        return IntFloat(3, 141592654, IntFloat::FRACT_RESOLUTION); // π ≈ 3.141592654
    }
    
    static IntFloat half_pi()
    {
        return IntFloat(1, 570796327, IntFloat::FRACT_RESOLUTION); // π/2 ≈ 1.570796327
    }
    
    static IntFloat quarter_pi()
    { 
        return IntFloat(0, 785398163, IntFloat::FRACT_RESOLUTION); // π/4 ≈ 0.785398163
    }
    
    static IntFloat gravity_earth()
    {
        return IntFloat(9, 80665, 100000); // g = 9.80665g
    }
    
    bool isZero() const 
    {
        return integer == 0 && fract < EPSILON;
    }

    IntFloat get_abs() const 
    {
        if (*this >= zero()) {
            return *this;
        }
        IntFloat result = *this;
        result.integer = -result.integer;
        result.fract = -result.fract;
        if (result.fract < 0) {
            result.integer -= 1;
            result.fract += FRACT_RESOLUTION;
        }
        return result;
    }
};


IntFloat sqrt(const IntFloat& value);
IntFloat atan(const IntFloat& x);
IntFloat atan2(const IntFloat& y, const IntFloat& x);


#endif