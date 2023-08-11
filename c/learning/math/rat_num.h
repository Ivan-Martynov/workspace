#ifndef _RAT_NUM_H_
#define _RAT_NUM_H_

struct rat_num
{
    int numerator;
    int denominator;
};

struct rat_num make_rat_num(int, int);

int get_numerator(const struct rat_num* const);
int get_denominator(const struct rat_num* const);

int print_rat_num(const struct rat_num* const);

struct rat_num sum_two_rat_num(
    const struct rat_num* const, const struct rat_num* const);

struct rat_num sub_two_rat_num(
    const struct rat_num* const, const struct rat_num* const);

struct rat_num mul_two_rat_num(
    const struct rat_num* const, const struct rat_num* const);

struct rat_num div_two_rat_num(
    const struct rat_num* const, const struct rat_num* const);

#endif //_RAT_NUM_H_