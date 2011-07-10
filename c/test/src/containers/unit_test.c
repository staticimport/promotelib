
#include <stdlib.h>
#include <time.h>

#include "avl_tree_set_test.h"

int main(void)
{
    srand(time(NULL));
    pmt_unit_test_avl_tree_set();
    return 0;
}
