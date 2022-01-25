#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>

/*#ifndef __x86_64__
#   error "Must build for x86_64"
#endif
*/
// Unnecessary - works on ARM64e (M1), with correct fixups (checked in IDA Pro 7.7)

bool assert_perror_dlfcn(int *val, char* fn) {
    if (val == 0) {
        perror(fn);
        return false;
    }
    return true;
}

typedef int (*extract_function)(const char* shared_cache_file_path, const char* extraction_root_path, void (^progress)(unsigned current, unsigned total));

int main(int argc, char* argv[]) {
    void* bundle = NULL;
    extract_function extract = NULL;

    if ( argc != 3 ) {
        fprintf(stderr, "usage: dsc_extractor <path-to-cache-file> <path-to-device-dir>\n");
        return -1;
    }

    bundle = dlopen("/usr/lib/dsc_extractor.bundle", RTLD_NOW);


    if (assert_perror_dlfcn(bundle, "dlopen dsc_extractor") != true) {
        exit(EXIT_FAILURE);
    }

    extract = (extract_function)dlsym(bundle, "dyld_shared_cache_extract_dylibs_progress");

    if (assert_perror_dlfcn((int *)extract, "dlsym dyld_shared_cache_extract_dylibs_progress") != true) {
        exit(EXIT_FAILURE);
    }

    void (^callback)(unsigned current, unsigned total) = ^(unsigned int current, unsigned int total) {
        printf("Progress: %d/%d\n", current, total);
    };

    int result = extract(argv[1], argv[2], callback);

    if (result) {
        printf("Failed to call extract (error %d)\n", result);
        exit(-1);
    }
    //What does result return on failure and exit? Might be better to leave well enough alone.
    //I'm lazy.

    printf("Finished!\n");
    return result;
}