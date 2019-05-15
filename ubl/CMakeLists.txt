cmake_minimum_required(VERSION 3.13)

include_directories(${CMAKE_CURRENT_LIST_DIR})

# Actually, explicitly adding header and source files is better approach than implicit cmake re-run:
# https://stackoverflow.com/questions/1027247/is-it-better-to-specify-source-files-with-glob-or-each-file-individually-in-cmak
# https://crascit.com/2016/01/31/enhanced-source-file-handling-with-target_sources/

add_library(ubl STATIC "")

# Including headers and sources of detail folder:
target_sources(ubl 
    PRIVATE 
        detail/BitonicSortImpl.h
        detail/IRamMonitor.h
        detail/IRamMonitor.cpp
        detail/ParallelSearchAsync.h
        detail/ParallelSearchOpenMP.h
        detail/VariadicHelpersImpl.h
        detail/VariantImpl.h
)

target_sources(ubl 
    PRIVATE 
        detail/patterns/MasterSlavePattern.h
)

if (WIN32)
    set(OS_IDENTIFIER "windows")

    target_sources(ubl 
        PRIVATE 
            detail/os/${OS_IDENTIFIER}/WindowsRamMonitor.h
            detail/os/${OS_IDENTIFIER}/WindowsRamMonitor.cpp
    )
elseif(APPLE)
    set(OS_IDENTIFIER "macos")

    target_sources(ubl 
        PRIVATE 
            detail/os/${OS_IDENTIFIER}/MacosRamMonitor.h
            detail/os/${OS_IDENTIFIER}/MacosRamMonitor.cpp
    )
elseif(UNIX)
    set(OS_IDENTIFIER "unix")
else()
    message(WARNING "UNSUPPORTED OS version!")
endif()

message(STATUS "--------------------- The OS is ${OS_IDENTIFIER} ------------------------ ")


# Including headers and sources of utils folder:
target_sources(ubl 
    PRIVATE 
        utils/ElapsedTimer.h
        utils/ElapsedTimer.cpp
        utils/tests_helper.h
        utils/tests_helper.cpp
        utils/VariadicHelpers.h
)

# Including headers and sources of root folder
target_sources(ubl 
    PUBLIC 
        BitonicSort.h
        ParallelSearch.h
        SuffixTree.h
        ThreadPool.h
        ThreadSafeQueue.h
        ubl.h
        Variant.h
    PRIVATE
        main.cpp
        SuffixTree.cpp
        ThreadPool.cpp
)



set_target_properties(ubl PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
