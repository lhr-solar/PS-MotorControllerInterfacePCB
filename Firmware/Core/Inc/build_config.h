// Selects the per-variant config based on the build command:
//   `make moco` -> -DBUILD_MOCO -> config_moco.h
//   `make mppt` -> -DBUILD_MPPT -> config_mppt.h
#pragma once

#if defined(BUILD_MOCO)
#include "config_moco.h"
#elif defined(BUILD_MPPT)
#include "config_mppt.h"
#else
#error "No build variant defined. Build with 'make moco' or 'make mppt'."
#endif
