#pragma once

// 2-step macro to expand macro value
#define WIDEN2(x) L##x
#define WIDEN(x)  WIDEN2(x)
