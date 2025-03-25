#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

// ��Ϣ�������ģ�����
void broker() {
    zmq::context_t context(1);

    // ǰ���׽��֣����ڽ��շ����ߵ���Ϣ
    zmq::socket_t frontend(context, zmq::socket_type::xsub);
    frontend.bind("tcp://*:5559");

    // ����׽��֣�����������ת����Ϣ
    zmq::socket_t backend(context, zmq::socket_type::xpub);
    backend.bind("tcp://*:5560");

    std::cout << "Broker started..." << std::endl;

    // ��������
    zmq::proxy(frontend, backend, nullptr);
}

// �������߳�
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

// �������߳�
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
    // ���������߳�
    std::thread broker_thread(broker);

    // ������һЩ����ʱ��
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // ���������ߺͶ������߳�
    std::thread pub_thread(publisher);
    std::thread sub_thread(subscriber);

    // �ȴ��߳̽���������������в��������
    broker_thread.join();
    pub_thread.join();
    sub_thread.join();

    return 0;
}