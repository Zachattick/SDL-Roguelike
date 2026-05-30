#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

int rand_int(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

struct Vector2D normalize_vector(struct Vector2D vector)
{
    struct Vector2D normalized = {0, 0};
    float length = sqrtf((vector.x * vector.x) + (vector.y * vector.y));

    if (length != 0)
    {
        normalized.x = vector.x / length;
        normalized.y = vector.y / length;
    }

    return normalized;
}