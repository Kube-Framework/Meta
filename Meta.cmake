project(KubeMeta)

get_filename_component(KubeMetaDir ${CMAKE_CURRENT_LIST_FILE} PATH)

set(KubeMetaSources
    ${KubeMetaDir}/Meta.hpp
    ${KubeMetaDir}/Meta.cpp
    ${KubeMetaDir}/Base.hpp
    ${KubeMetaDir}/Type.hpp
    ${KubeMetaDir}/Type.cpp
)

add_library(${PROJECT_NAME} ${KubeMetaSources})

target_link_libraries(${PROJECT_NAME}
PUBLIC
    KubeCore
)

if(${KF_TESTS})
    include(${KubeMetaDir}/Tests/MetaTests.cmake)
endif()

if(${KF_BENCHMARKS})
    include(${KubeMetaDir}/Benchmarks/MetaBenchmarks.cmake)
endif()