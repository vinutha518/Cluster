
#define BENCH_DRW2D_60  0
#define BENCH_CPU_60    1
#define BENCH_DRW2D_30  2
#define BENCH_CPU_30    3
#define BENCH_CPU_WAIT1 4
#define BENCH_CPU_WAIT2 5
#define BENCH_CPU_WAIT3 6
#define BENCH_CPU_WAIT4 7
/* #define BENCH_DEBUG2    6 */
/* #define BENCH_TMP       7 */

#define BENCH_US_MAX 100
#define BENCH_SLOT_MAX 8


void bench_start(uint32_t slot);

void bench_stop(uint32_t slot);

float64_t GetCurrentDuration(uint32_t slot);
