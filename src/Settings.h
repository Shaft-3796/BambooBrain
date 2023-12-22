#pragma once

#define VERBOSE 1  // whether to show verbose information

#if VERBOSE == 1
#define bbprintf(...) printf(__VA_ARGS__)
#else
#define bbprintf(...)
#endif