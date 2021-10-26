# yolo_dsc
A late bound, hope-for-the-best dyld shared cache extractor

## why?

There are other `dsc_extract` utilities.  They usually require some combination of get matching headers for `dyld`

Apple is really late putting these out, and yes that is the _right_ way.

This is the wrong way, we `dlopen` the bundle at `/usr/lib/dsc_extract.bundle` and call the function
`dyld_shared_cache_extract_dylibs_progress` (the version that didn't require progress is no longer available)

## compiling?

Because we don't do proper header / early binding we really do just depend on `libdyld` (transitive via libSystem)
and the standard C library.

## will it crash?

Depends, did Apple change the API for `dsc_extract.bundle`?  Did you call it incorrectly and get bit by an egregious
lack of error handling?

# compiling

`$ clang -o yolo_dsc main.c` - yes - it is that simple

# running

`$ ./yolo_dsc /System/Library/dyld/dyld_shared_cache_x86_64h ~/some_output_dir`

## CMakeLists.txt?

For CLion

# contributing?

Yeah - if you want to make it more friendly that's great