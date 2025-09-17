#include "cmath.h"

#include <math.h>

int numdig(long n)
{
    if (n == 0)
        return 1;
    return (n < 0) ? (int)log10((double)-n) + 2 : (int)log10((double)n) + 1;
};