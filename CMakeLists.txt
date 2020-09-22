cmake_minimum_required(VERSION 3.10)

project(Brotli_Demo VERSION 1.0 LANGUAGES C)

add_subdirectory(brotli)
list(APPEND BR_LIBS brotlidec-static brotlienc-static)

add_executable(br_demo br_demo.c)

target_link_libraries(br_demo PUBLIC ${BR_LIBS})

install(TARGETS br_demo DESTINATION bin)
