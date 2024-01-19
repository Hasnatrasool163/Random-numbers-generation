#include "random_generator.h"
#include<stdio.h>

int main() {
    int random_number = generate_true_random_number();
    if (random_number != -1) {
        printf("True Random Number: %d\n", random_number);
    }

    return 0;
}
