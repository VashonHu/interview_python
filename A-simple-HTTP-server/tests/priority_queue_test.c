#include <priority_queue.h>
#include <dbg.h>

static int comp(void *i, void *j) {
    size_t si = (size_t)i;
    size_t sj = (size_t)j;

    return (si < sj)? 1: 0;
}

size_t testdata[] = {112, 1634, 2151, 1345, 362, 1557, 1622, 461, 427, 1587, 1836, 2902, 1059, 501, 1391, 1223, 1590, 1931, 2004, 155, 2629, 2796, 2324, 323, 2931, 460, 1270, 1683, 89, 837, 100, 1399, 2110, 156, 24, 1814, 206, 751, 1837, 2438, 2670, 398, 526, 568, 197, 2917, 2430, 245, 1390, 2571, 659, 1246, 1763, 490, 64, 1768, 1710, 1243, 2187, 949, 2604, 2216, 2241, 1073, 2013, 1799, 1860, 1823, 345, 2514, 2008, 1520, 2088, 2841, 231, 351, 2047, 2887, 1389, 1876, 2246, 772, 420, 919, 2068, 2652, 1258, 1962, 2934, 649, 691, 839, 456, 1646, 1268, 2355, 608, 2062, 1808, 858, 421, 2883, 771, 1317, 521, 1125, 2655, 1209, 1991, 2221, 2264, 2227, 1794, 2161, 746, 1496, 2135, 228, 433, 2574, 2725, 1969, 1084, 551, 2454, 1472, 355, 1720, 2864, 1342, 279, 359, 2222, 1769, 2634, 618, 495, 2444, 953, 2265, 2930, 1065, 668, 486, 2166, 2798, 157, 2982, 1204, 2284, 1982, 1659, 2251, 1211, 2316, 2517, 1567, 487, 1098, 106, 853, 1016, 48, 2683, 1033, 2044, 354, 1013, 2964, 1241, 1310, 1666, 2180, 2178, 2846, 1835, 1942, 357, 334, 1018, 2083, 2666, 1930, 1103, 836, 2804, 453, 110, 2128, 1967, 2692, 1902, 2353, 917, 1849, 2060, 2471, 1006, 46, 2253, 384, 2214, 867, 237, 2349, 2562, 2932, 1862, 78, 258, 1053, 1436, 941, 72, 2094, 872, 1396, 121, 711, 1017, 1756, 1611, 2189, 2534, 973, 1387, 141, 2317, 2025, 1180, 1159, 2220, 845, 1238, 11, 1041, 1581, 242, 2953, 1653, 1924, 758, 2587, 787, 1996, 2540, 122, 2530, 2001, 2777, 1549, 1187, 1827, 2755, 1375, 1515, 2790, 1037, 2038, 898, 2197, 2870, 2101, 1420, 2579, 1119, 1485, 2678, 449, 1092, 376, 2714, 2803, 2215, 318, 330, 1941, 1791, 866, 1035, 1192, 1993, 2095, 1868, 225, 918, 2366, 1347, 549, 591, 1562, 2737, 2719, 1577, 2164, 955, 2968, 1432, 732, 368};

size_t resultdata[] = {11, 24, 46, 48, 64, 72, 78, 89, 100, 106, 110, 112, 121, 122, 141, 155, 156, 157, 197, 206, 225, 228, 231, 237, 242, 245, 258, 279, 318, 323, 330, 334, 345, 351, 354, 355, 357, 359, 362, 368, 376, 384, 398, 420, 421, 427, 433, 449, 453, 456, 460, 461, 486, 487, 490, 495, 501, 521, 526, 549, 551, 568, 591, 608, 618, 649, 659, 668, 691, 711, 732, 746, 751, 758, 771, 772, 787, 836, 837, 839, 845, 853, 858, 866, 867, 872, 898, 917, 918, 919, 941, 949, 953, 955, 973, 1006, 1013, 1016, 1017, 1018, 1033, 1035, 1037, 1041, 1053, 1059, 1065, 1073, 1084, 1092, 1098, 1103, 1119, 1125, 1159, 1180, 1187, 1192, 1204, 1209, 1211, 1223, 1238, 1241, 1243, 1246, 1258, 1268, 1270, 1310, 1317, 1342, 1345, 1347, 1375, 1387, 1389, 1390, 1391, 1396, 1399, 1420, 1432, 1436, 1472, 1485, 1496, 1515, 1520, 1549, 1557, 1562, 1567, 1577, 1581, 1587, 1590, 1611, 1622, 1634, 1646, 1653, 1659, 1666, 1683, 1710, 1720, 1756, 1763, 1768, 1769, 1791, 1794, 1799, 1808, 1814, 1823, 1827, 1835, 1836, 1837, 1849, 1860, 1862, 1868, 1876, 1902, 1924, 1930, 1931, 1941, 1942, 1962, 1967, 1969, 1982, 1991, 1993, 1996, 2001, 2004, 2008, 2013, 2025, 2038, 2044, 2047, 2060, 2062, 2068, 2083, 2088, 2094, 2095, 2101, 2110, 2128, 2135, 2151, 2161, 2164, 2166, 2178, 2180, 2187, 2189, 2197, 2214, 2215, 2216, 2220, 2221, 2222, 2227, 2241, 2246, 2251, 2253, 2264, 2265, 2284, 2316, 2317, 2324, 2349, 2353, 2355, 2366, 2430, 2438, 2444, 2454, 2471, 2514, 2517, 2530, 2534, 2540, 2562, 2571, 2574, 2579, 2587, 2604, 2629, 2634, 2652, 2655, 2666, 2670, 2678, 2683, 2692, 2714, 2719, 2725, 2737, 2755, 2777, 2790, 2796, 2798, 2803, 2804, 2841, 2846, 2864, 2870, 2883, 2887, 2902, 2917, 2930, 2931, 2932, 2934, 2953, 2964, 2968, 2982};


int main() {
    nx_pq_t pq;
    int rc;

    rc = nx_pq_init(&pq, comp, NX_PQ_DEFAULT_SIZE);
    check_exit(rc == 0, "nx_pq_init error");

    rc = nx_pq_is_empty(&pq);
    check_exit(rc == 1, "nx_pq_is_empty error");

    size_t sz;
    sz = nx_pq_size(&pq);
    check_exit(sz == 0, "nx_pq_size");

    void *min;
    min = nx_pq_min(&pq);
    check_exit(min == NULL, "nx_pq_min");

    rc = nx_pq_delmin(&pq);
    check_exit(rc == 0, "nx_pq_delmin error");

    int n = sizeof(testdata)/sizeof(size_t);
    int i;
    for (i = 0; i < n; i++) {
        rc = nx_pq_insert(&pq, (void *)testdata[i]);
        check_exit(rc == 0, "nx_pq_insert error");
        
        check_exit(nx_pq_size(&pq) == (size_t)i+1, "nx_pq_size error");
    }

    i = 0;
    while (!nx_pq_is_empty(&pq)) {
        min = nx_pq_min(&pq);
        check_exit(min != NULL, "nx_pq_min error");
        check_exit((size_t)min == (size_t)resultdata[i], "nx_pq_min error, min=%zu, rd[i]=%zu", (size_t)min, (size_t)resultdata[i]);
        i++;

        rc = nx_pq_delmin(&pq);
        check_exit(rc == 0, "nx_pq_delmin error");
    }

    check_exit(i == n, "size not match");
    printf("pass priority_queue_test\n");
    return 0;
}
