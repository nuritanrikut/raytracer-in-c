cmake_minimum_required(VERSION 3.5)

project(raytracer LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

set(SOURCES
    src/camera.c
    src/dielectric.c
    src/hit_record.c
    src/hittable_list.c
    src/lambertian.c
    src/main.c
    src/material.c
    src/metal.c
    src/ray.c
    src/rng.c
    src/sphere.c
    src/utils.c
    src/vec3.c
)

add_executable(raytracer ${SOURCES})

target_link_libraries(raytracer PRIVATE OpenMP::OpenMP_C pthread m)
target_link_libraries(raytracer PUBLIC OpenMP::OpenMP_C)
