set(LIB_SRC
        CountDownLatch.cpp
        Thread.cpp
        FileUtil.cpp
        LogFile.cpp
        LogStream.cpp
        AsyncLogging.cpp
        Logging.cpp
        TimeSinceGMT.h)

add_library(libserver_base ${LIB_SRC})
target_link_libraries(libserver_base pthread rt)
