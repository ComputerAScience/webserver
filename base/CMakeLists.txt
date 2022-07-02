set(LIB_SRC
        CountDownLatch.cpp
        Thread.cpp
        CurrentThread.h FileUtil.cpp FileUtil.h LogFile.cpp LogFile.h LogStream.cpp LogStream.h AsyncLogging.cpp AsyncLogging.h Logging.cpp Logging.h)

add_library(libserver_base ${LIB_SRC})
target_link_libraries(libserver_base pthread rt)
