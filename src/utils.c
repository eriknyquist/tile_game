#include "defs.h"

int min_positive(int a, int b)
{
    int r;

    r = INT_MAX;

    if (a > 0) r = MIN(r, a);
    if (b > 0) r = MIN(r, b);

    if (r == INT_MAX) r = MIN(a, b);
    return r;
}
