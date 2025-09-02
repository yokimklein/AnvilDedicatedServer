#include "hashtable.h"

byte const c_hash::k_hash_polynomials[]{ 3, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 54, 59 };
long const c_hash::k_hash_polynomial_count = NUMBEROF(c_hash::k_hash_polynomials);