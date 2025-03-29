#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

class MessageBroker {
private:
    zmq::context_t context;
    bool running;
    
    // 日志功能
    void log(const std::string& message) {
        std::cout << "[Broker] " << message << std::endl;
    }
    
public:
    MessageBroker() : context(1), running(false) {}
    
    void start() {
        running = true;
        
        // 创建XSUB前端 - 用于接收发布者消息
        zmq::socket_t frontend(context, zmq::socket_type::xsub);
        frontend.bind("tcp://*:5559");
        log("XSUB前端绑定到端口5559");
        
        // 创建XPUB后端 - 用于向订阅者转发消息
        zmq::socket_t backend(context, zmq::socket_type::xpub);
        backend.bind("tcp://*:5560");
        log("XPUB后端绑定到端口5560");
        
        // 启动代理
        log("消息代理开始运行...");
        try {
            zmq::proxy(frontend, backend, nullptr);
        }
        catch (const zmq::error_t& e) {
            log("代理错误: " + std::string(e.what()));
        }
    }
    
    void stop() {
        running = false;
        context.close();
    }
};

//int main() {
//    MessageBroker broker;
//    broker.start();
//    return 0;
//}