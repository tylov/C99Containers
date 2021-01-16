#include <stc/csmap.h>
#include <stc/cstr.h>
#include <stc/crand.h>
#include <stdio.h>

//using_csset(i, int);
//using_csmap(s, cstr, cstr, cstr_del, cstr_clone, cstr_compare_ref, cstr_del, cstr_clone);

using_csmap(i, int, int);

#include <time.h>

int main(int argc, char **argv)
{
    csmap_i map = csmap_i_init();
    time_t seed = time(NULL);

    size_t n = 1000000;
    uint64_t mask = (1ull << 20) - 1;
    stc64_srandom(seed);
    for (size_t i = 0; i < n; ++i) {
        csmap_i_emplace(&map, stc64_random() & mask, i);
    }
    puts("inserted");
    stc64_srandom(seed);
    for (unsigned int i = 0; i < n - 50; ++i) {
        csmap_i_erase(&map, stc64_random() & mask);
    }

    csmap_i_emplace(&map, 500000, 5);

    c_foreach (i, csmap_i, map)
        printf("-- %d: %d\n", i.ref->first, i.ref->second);
    puts("");

    csmap_i_iter_t it;
    printf("min/max: %d -- %d: %d\n\n", csmap_i_front(&map)->first, csmap_i_back(&map)->first, csmap_i_find(&map, 500000, &it) != NULL);

    c_foreach (i, csmap_i, it, csmap_i_end(&map))
        printf("-- %d: %d\n", i.ref->first, i.ref->second);

    csmap_i_del(&map);
    puts("done");
    return 0;
}
