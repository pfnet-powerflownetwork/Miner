#ifndef MINER_H
#define MINER_H

// Package and Version Information
#define PACKAGE_NAME "pfnet_miner"
#define MINER_VERSION "0.9.6"
#define USER_AGENT PACKAGE_NAME "/" MINER_VERSION

// System Configuration
#define MAX_CPUS 16
#define MAX_POW_PER_BLOCK 50

#include <curl/curl.h>
#include <jansson.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
    #include <windows.h>
    #define sleep(secs) Sleep((secs) * 1000)
#else
    #include <unistd.h>
#endif

#include "elist.h"
#include "PFNETPL/PFNETPL.h"
#include "PFNETPL/PFNETPLFunctions.h"
#include "crypto/sha2.h"

#ifdef _MSC_VER
    #define strdup(...) _strdup(__VA_ARGS__)
    #define strcasecmp(x, y) _stricmp(x, y)
    #define strncasecmp(x, y, z) _strnicmp(x, y, z)
    #define __thread __declspec(thread)
    #define ALIGN(x) __declspec(align(x))
#else
    #define ALIGN(x) __attribute__((aligned(x)))
#endif

#if JANSSON_MAJOR_VERSION >= 2
    #define JSON_LOADS(str, err_ptr) json_loads(str, 0, err_ptr)
    #define JSON_LOAD_FILE(path, err_ptr) json_load_file(path, 0, err_ptr)
#else
    #define JSON_LOADS(str, err_ptr) json_loads(str, err_ptr)
    #define JSON_LOAD_FILE(path, err_ptr) json_load_file(path, err_ptr)
#endif

// Aligned Thread-Local Variables
extern __thread ALIGN(64) uint32_t *vm_m;
extern __thread ALIGN(64) int32_t *vm_i;
extern __thread ALIGN(64) uint32_t *vm_u;
extern __thread ALIGN(64) int64_t *vm_l;
extern __thread ALIGN(64) uint64_t *vm_ul;
extern __thread ALIGN(64) float *vm_f;
extern __thread ALIGN(64) double *vm_d;
extern __thread ALIGN(64) uint32_t *vm_s;

// Configuration Flags and Options
extern bool use_PFNETpl_math;
extern bool opt_debug, opt_debug_epl, opt_debug_vm;
extern bool opt_protocol, opt_quiet;
extern int opt_timeout, opt_n_threads;
extern bool opt_test_vm, opt_opencl;
extern int opt_opencl_gthreads, opt_opencl_vwidth;

// Global Variables
extern struct work_package *g_work_package;
extern volatile int g_work_package_cnt, g_work_package_idx;
extern struct work_restart *work_restart;

// Base85 Powers Array
static const int BASE85_POW[] = {1, 85, 7225, 614125, 52200625};

enum submit_commands {
    SUBMIT_BOUNTY,
    SUBMIT_POW,
    SUBMIT_COMPLETE
};

// Structures Definition
struct work_package {
    uint64_t block_id, work_id;
    unsigned char work_str[22], work_nm[50];
    uint64_t WCET, bounty_limit, pow_reward, bty_reward;
    int pending_bty_cnt, iterations;
    bool blacklisted, active;

    // VM Memory
    uint32_t vm_ints, vm_uints, vm_longs, vm_ulongs;
    uint32_t vm_floats, vm_doubles;

    // Data Submissions and Storage
    uint32_t submit_sz, submit_idx;
    uint32_t iteration_id, storage_id, storage_sz;
    uint32_t storage_idx, storage_cnt;
    uint32_t *storage;
};

struct work {
    int thr_id, package_id;
    uint64_t block_id, work_id;
    uint32_t iteration_id;
    unsigned char work_str[22], work_nm[50];
    uint32_t pow_target[4], pow_hash[4], vm_input[12];
    unsigned char multiplicator[32];
};

struct thr_info {
    int id;
    char name[6];
    pthread_t pth;
    pthread_attr_t attr;
    struct thread_q *q;
};

struct work_restart {
    volatile uint8_t restart;
    char padding[128 - sizeof(uint8_t)];
};

struct submit_req {
    int thr_id;
    bool bounty;
    enum submit_commands req_type;
    time_t start_tm, delay_tm;
    int retries;
    char mult[65], hash[33];
    uint64_t work_id;
    unsigned char work_str[22];
    uint32_t iteration_id, storage_id, submit_data_sz;
    uint32_t *submit_data;
};

struct instance {
#ifdef WIN32
    HINSTANCE hndl;
    int32_t (__cdecl *initialize)(uint32_t *, int32_t *, uint32_t *, int64_t *, uint64_t *, float *, double *, uint32_t *);
    int32_t (__cdecl *execute)(uint64_t, uint32_t *, uint32_t, uint32_t *, uint32_t *, uint32_t *);
    int32_t (__cdecl *verify)(uint64_t, uint32_t *, uint32_t, uint32_t *, uint32_t *, uint32_t *);
#else
    void *hndl;
    int32_t (*initialize)(uint32_t *, int32_t *, uint32_t *, int64_t *, uint64_t *, float *, double *, uint32_t *);
    int32_t (*execute)(uint64_t, uint32_t *, uint32_t, uint32_t *, uint32_t *, uint32_t *);
    int32_t (*verify)(uint64_t, uint32_t *, uint32_t, uint32_t *, uint32_t *, uint32_t *);
#endif
};

// Prototypes
extern bool add_work_package(struct work_package *work_package);
extern void applog(int prio, const char *fmt, ...);
extern json_t* json_rpc_call(CURL *curl, const char *url, const char *userpass, const char *req, int *curl_err);
extern unsigned long genrand_int32(void);
extern void init_genrand(unsigned long s);
extern void clear_all_workpackages();

#endif // MINER_H
