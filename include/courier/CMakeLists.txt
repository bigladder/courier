file(GLOB_RECURSE public_headers "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/[a-zA-Z]*.h")

add_library(${PROJECT_NAME} INTERFACE ${public_headers})
target_include_directories(${PROJECT_NAME} INTERFACE "${PROJECT_SOURCE_DIR}/include")
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)


if (${PROJECT_NAME}_COVERAGE)
    add_coverage(${PROJECT_NAME})
endif ()