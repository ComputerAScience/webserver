set(LIB_SRC
        Buffer.cpp
        Channel.cpp
        Connection.cpp
        EPoll.cpp
        EventLoop.cpp
        Timer.cpp
        Server.cpp
        Socket.cpp
        EventLoopThreadPool.cpp
        EventLoopThread.cpp )

add_library(libserver_net ${LIB_SRC})
target_link_libraries(libserver_net pthread)
