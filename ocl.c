/*
*
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the Free
* Software Foundation; either version 2 of the License, or (at your option)
* any later version.
*/

#ifdef USE_OPENCL

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define _GNU_SOURCE

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include "miner.h"

#define CL_CHECK(x) x
#define MAX_OPENCL_PLATFORMS 100
#define BUFFER_SIZE 1024
#define MAX_SOURCE_SIZE (1024 * 1024)

static inline bool opencl_err_check(int err, const char *desc) {
    if (err != CL_SUCCESS) {
        applog(LOG_ERR, "Error: %s (Code: %d)", desc, err);
        return false;
    }
    return true;
}

extern int opencl_init_devices() {
    size_t i, j;
    cl_platform_id platforms[MAX_OPENCL_PLATFORMS];
    cl_uint num_platforms = 0, num_devices = 0;
    cl_int err;
    char buffer[BUFFER_SIZE];
    int gpu_cnt = 0;

    err = clGetPlatformIDs(MAX_OPENCL_PLATFORMS, platforms, &num_platforms);
    if (!opencl_err_check(err, "No OpenCL platforms found")) return 0;

    gpu = malloc(num_platforms * sizeof(struct opencl_device));
    if (!gpu) {
        applog(LOG_ERR, "ERROR: Unable to allocate GPU devices!");
        return 0;
    }

    applog(LOG_DEBUG, "=== %d OpenCL platform(s) found ===", num_platforms);

    for (i = 0; i < num_platforms; i++) {
        bool found = false;

        applog(LOG_DEBUG, "  -- Platform %zu --", i);
        clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, sizeof(buffer), buffer, NULL);
        applog(LOG_DEBUG, "  PROFILE = %s", buffer);
        clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, sizeof(buffer), buffer, NULL);
        applog(LOG_DEBUG, "  VERSION = %s", buffer);
        clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(buffer), buffer, NULL);
        applog(LOG_DEBUG, "  NAME = %s", buffer);
        clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, sizeof(buffer), buffer, NULL);
        applog(LOG_DEBUG, "  VENDOR = %s", buffer);
        clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, sizeof(buffer), buffer, NULL);
        applog(LOG_DEBUG, "  EXTENSIONS = %s", buffer);

        err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
        if (!opencl_err_check(err, "Unable to get devices from OpenCL Platform")) continue;

        if (num_devices > 0) {
            applog(LOG_DEBUG, "  DEVICES:");
            cl_device_id *devices = malloc(num_devices * sizeof(cl_device_id));
            if (!devices) {
                applog(LOG_ERR, "ERROR: Unable to allocate memory for device IDs!");
                break;
            }

            err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, num_devices, devices, NULL);
            if (!opencl_err_check(err, "Unable to get GPU devices from device list")) {
                free(devices);
                continue;
            }

            for (j = 0; j < num_devices; j++) {
                clGetDeviceInfo(devices[j], CL_DEVICE_NAME, sizeof(buffer), buffer, NULL);
                applog(LOG_DEBUG, "    %zu - %s", j, buffer);

                clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, sizeof(buffer), buffer, NULL);
                if (!strstr(buffer, "cl_khr_fp64")) {
                    applog(LOG_DEBUG, "        *Device does not support 64-bit Floating Point math");
                    continue;
                }

                gpu[gpu_cnt].platform_id = platforms[i];
                gpu[gpu_cnt].device_id = devices[j];
                strncpy(gpu[gpu_cnt].name, buffer, sizeof(gpu[gpu_cnt].name) - 1);

                gpu[gpu_cnt].context = clCreateContext(NULL, 1, &gpu[gpu_cnt].device_id, NULL, NULL, &err);
                if (!gpu[gpu_cnt].context || !opencl_err_check(err, "Unable to create OpenCL context")) {
                    applog(LOG_ERR, "ERROR: Failed to create context for device");
                    continue;
                }

                gpu[gpu_cnt].queue = clCreateCommandQueue(gpu[gpu_cnt].context, gpu[gpu_cnt].device_id, 0, &err);
                if (!gpu[gpu_cnt].queue || !opencl_err_check(err, "Unable to create OpenCL command queue")) {
                    applog(LOG_ERR, "ERROR: Failed to create command queue for device");
                    clReleaseContext(gpu[gpu_cnt].context);
                    continue;
                }

                gpu_cnt++;
                found = true;
                break;
            }

            free(devices);
        }

        if (!found) {
            applog(LOG_DEBUG, "  No suitable GPUs found on this platform");
        }
    }

    return gpu_cnt;
}

// Continue refactoring other functions similarly
