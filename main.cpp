#include<iostream>
#include"./net/Server.h"
#include"./net/EventLoop.h"
#include"./base/Logging.h"
#include<string>
using namespace std;
int main(int argc, char** argv)
{
    // 修改进程的工作目录, 方便后续操作
    std::string logPath = "./WebServer.log";
    string ip = "127.0.0.1";
    string port = "8080";
    int threadNum = 4;

    // parse args
    int opt;
    const char *str = "t:l:p:";
    while ((opt = getopt(argc, argv, str)) != -1) {
        switch (opt) {
            case 't': {
                threadNum = atoi(optarg);
                break;
            }
            case 'l': {
                logPath = optarg;
                if (logPath.size() < 2 || optarg[0] != '/') {
                    printf("logPath should start with \"/\"\n");
                    abort();
                }
                break;
            }
            case 'p': {
                port = string(optarg);
                break;
            }
            default:
                break;
        }
    }
    Logger::setLogFileName(logPath);
    EventLoop loop;
    Server sever(&loop, ip, port, 4);
    sever.start(true);
    loop.loop();
    return 0;
}
