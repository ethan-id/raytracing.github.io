cmake_minimum_required ( VERSION 3.16 ) # newer FindOpenMP works better
project ( RTWeekend LANGUAGES CXX )

# ---- Language standard ----
set ( CMAKE_CXX_STANDARD          20 )     # or 17
set ( CMAKE_CXX_STANDARD_REQUIRED ON )
set ( CMAKE_CXX_EXTENSIONS        OFF )

# ---- Sources ----
set ( EXTERNAL
  src/external/stb_image.h
)

set ( SOURCE_MY_WEEKEND
  src/myWeekend/camera.h
  src/myWeekend/color.h
  src/myWeekend/hittable.h
  src/myWeekend/hittable_list.h
  src/myWeekend/interval.h
  src/myWeekend/main.cc
  src/myWeekend/ray.h
  src/myWeekend/rtweekend.h
  src/myWeekend/sphere.h
  src/myWeekend/vec3.h
)

include_directories(src)

# ---- Warnings (as you had) ----
message (STATUS "Compiler ID: " ${CMAKE_CXX_COMPILER_ID})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options("/W4")
    add_compile_options("/we4265" "/we5204" "/wd4100")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wnon-virtual-dtor -Wreorder -Wmaybe-uninitialized -Wunused-variable)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wnon-virtual-dtor -Wreorder -Wsometimes-uninitialized -Wunused-variable)
endif()

# ---- Executable ----
add_executable(myWeekend ${EXTERNAL} ${SOURCE_MY_WEEKEND})

# --- OpenMP on AppleClang via Homebrew libomp ---
if (APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  # Ask Homebrew where libomp lives
  execute_process(COMMAND brew --prefix libomp
                  OUTPUT_VARIABLE LIBOMP_PREFIX
                  OUTPUT_STRIP_TRAILING_WHITESPACE
                  ERROR_QUIET)

  # Fall back to common prefixes if brew isn't available in PATH during CMake
  if (NOT LIBOMP_PREFIX)
    set(LIBOMP_PREFIX "/opt/homebrew/opt/libomp")
  endif()

  set(LIBOMP_INCLUDE_DIR "${LIBOMP_PREFIX}/include")
  set(LIBOMP_LIB_DIR     "${LIBOMP_PREFIX}/lib")

  # Make sure the files exist
  find_path(LIBOMP_INCLUDE omp.h HINTS "${LIBOMP_INCLUDE_DIR}")
  find_library(LIBOMP_LIBRARY NAMES omp HINTS "${LIBOMP_LIB_DIR}")

  if (LIBOMP_INCLUDE AND LIBOMP_LIBRARY)
    target_compile_options(myWeekend PRIVATE -Xpreprocessor -fopenmp)
    target_include_directories(myWeekend PRIVATE "${LIBOMP_INCLUDE_DIR}")
    # Add -L and link the dylib
    target_link_directories(myWeekend PRIVATE "${LIBOMP_LIB_DIR}")
    target_link_libraries(myWeekend PRIVATE "${LIBOMP_LIBRARY}")
    # Ensure runtime can find the dylib
    target_link_options(myWeekend PRIVATE "-Wl,-rpath,${LIBOMP_LIB_DIR}")
  else()
    message(FATAL_ERROR "libomp not found. Install via `brew install libomp` or set LIBOMP_PREFIX manually.")
  endif()
endif()

# If you rely on rtweekend.h being force-included:
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(myWeekend PRIVATE -include ${CMAKE_SOURCE_DIR}/src/myWeekend/rtweekend.h)
endif()

# ---- OpenMP (for #pragma omp parallel for) ----
# Works for GCC, LLVM Clang (Linux), and MSVC. For AppleClang we add a fallback.
find_package(OpenMP)

if (OpenMP_CXX_FOUND)
    target_link_libraries(myWeekend PRIVATE OpenMP::OpenMP_CXX)
else()
    # Fallback for Apple Clang (install libomp via Homebrew: `brew install libomp`)
    if (APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        message(STATUS "OpenMP not found via package, using manual flags for AppleClang")
        target_compile_options(myWeekend PRIVATE -Xpreprocessor -fopenmp)
        target_link_libraries(myWeekend PRIVATE omp)   # links libomp.dylib
        # Optionally add include/lib paths if Homebrew is in a nonstandard location:
        # target_include_directories(myWeekend PRIVATE /opt/homebrew/opt/libomp/include)
        # target_link_directories(myWeekend PRIVATE /opt/homebrew/opt/libomp/lib)
    endif()
endif()

# ---- Recommended optimizations (optional) ----
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(myWeekend PRIVATE -O3 -march=native -ffast-math)
elseif (MSVC)
    target_compile_options(myWeekend PRIVATE /O2)
endif()

