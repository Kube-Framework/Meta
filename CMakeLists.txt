kube_add_library(KubeMeta
    SOURCES
        Meta.hpp
        Meta.cpp
        Base.hpp
        Utils.hpp
        Type.hpp
        Type.cpp
        Resolver.hpp
        Resolver.cpp

    LIBRARIES
        KubeCore
)
