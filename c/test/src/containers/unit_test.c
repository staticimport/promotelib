
#include <stdlib.h>
#include <time.h>

#include "avl_tree_set_test.h"
#include "vector_test.h"

int main(void)
{
    srand(time(NULL));
    pmt_unit_test_avl_tree_set();
    pmt_unit_test_vector();
    return 0;
}
