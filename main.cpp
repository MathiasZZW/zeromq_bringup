#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

// 消息队列中心（代理）
void broker() {
    zmq::context_t context(1);

    // 前端套接字，用于接收发布者的消息
    zmq::socket_t frontend(context, zmq::socket_type::xsub);
    frontend.bind("tcp://*:5559");

    // 后端套接字，用于向订阅者转发消息
    zmq::socket_t backend(context, zmq::socket_type::xpub);
    backend.bind("tcp://*:5560");

    std::cout << "Broker started..." << std::endl;

    // 启动代理
    zmq::proxy(frontend, backend, nullptr);
}

// 发布者线程
void publisher() {
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::pub);
    socket.connect("tcp://localhost:5559");

    int count = 0;
    while (true) {
        std::string message = "Message " + std::to_string(count++);
        socket.send(zmq::buffer(message), zmq::send_flags::none);
        std::cout << "Published: " << message << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

// 订阅者线程
void subscriber() {
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::sub);
    socket.connect("tcp://localhost:5560");
    socket.set(zmq::sockopt::subscribe, "");

    while (true) {
        zmq::message_t message;
        socket.recv(message, zmq::recv_flags::none);
        std::cout << "Received: " << message.to_string() << std::endl;
    }
}

int main() {
    // 启动代理线程
    std::thread broker_thread(broker);

    // 给代理一些启动时间
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 启动发布者和订阅者线程
    std::thread pub_thread(publisher);
    std::thread sub_thread(subscriber);

    // 等待线程结束（在这个例子中不会结束）
    broker_thread.join();
    pub_thread.join();
    sub_thread.join();

    return 0;
}