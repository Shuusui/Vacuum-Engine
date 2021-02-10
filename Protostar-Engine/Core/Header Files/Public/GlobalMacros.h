#pragma once
#define PRINTF(VALUE, ...) Protostar::Printf(VALUE, __VA_ARGS__)


#define WIDE1(x) L##x
#define WIDE2(x) WIDE1(x)
#define __WFUNCTION__ WIDE2(__FUNCTION__)