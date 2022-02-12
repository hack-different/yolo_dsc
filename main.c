#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>

const char* app_name = "yolo_dsc";

typedef int (*extract_function)(const char* shared_cache_file_path, const char* extraction_root_path, void (^progress)(unsigned current, unsigned total));

int main(int argc, char* argv[]) {
    void* bundle = NULL;
    extract_function extract = NULL;

    if (argc != 3) {
        if (argc >= 1) {
            app_name = argv[0];
        }
        fprintf(stderr, "usage: %s <path-to-cache-file> <path-to-device-dir>\n", app_name);
        return -1;
    }

    bundle = dlopen("/usr/lib/dsc_extractor.bundle", RTLD_NOW);

    if (bundle == NULL) {
        fprintf(stderr, "dsc_extractor.bundle could not be loaded from /usr/lib/\n");
        return -1;
    }

    extract = (extract_function)dlsym(bundle, "dyld_shared_cache_extract_dylibs_progress");

    if (extract == NULL) {
        fprintf(stderr, "dsc_extractor.bundle did not have dyld_shared_cache_extract_dylibs_progress symbol.\n\nThey must have changed the API and we're not that brave...\n");
        return 1;
    }


    void (^callback)(unsigned current, unsigned total) = ^(unsigned int current, unsigned int total) {
        printf("Progress: %d/%d\n", current, total);
    };

    int result = extract(argv[1], argv[2], callback);

    if (result) {
        printf("Failed to call extract (error %d)\n", result);
        exit(-1);
    }
    printf("Finished!\n");
    return result;
}
