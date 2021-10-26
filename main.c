#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>

static volatile bool finished = false;

typedef int (*extract_function)(const char* shared_cache_file_path, const char* extraction_root_path, void (^progress)(unsigned current, unsigned total));

int main(int argc, char* argv[]) {
    void* bundle = NULL;
    extract_function extract = NULL;

    bundle = dlopen("/usr/lib/dsc_extractor.bundle", RTLD_NOW);

    extract = (extract_function)dlsym(bundle, "dyld_shared_cache_extract_dylibs_progress");

    void (^callback)(unsigned current, unsigned total) = ^(unsigned int current, unsigned int total) {
        printf("Progress: %d/%d\n", current, total);

        if (current == total) {
            finished = true;
        }
    };

    extract(argv[1], argv[2], callback);

    while (finished == false) {
        sleep(1000);
    }

    printf("Finished!\n");
    return 0;
}