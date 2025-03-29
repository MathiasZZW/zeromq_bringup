#include <zmq.hpp>
#include <string>
#include <thread>
#include "message.hpp"

class ModuleBase {
protected:
    std::string moduleId;
    zmq::context_t context;
    zmq::socket_t publisher;
    zmq::socket_t subscriber;
    std::thread receiveThread;
    bool running;
    
    // 消息处理函数
    virtual void handleMessage(const Message& message) = 0;
    
    // 接收消息线程
    void receiveLoop() {
        while (running) {
            zmq::message_t zmq_msg;
            try {
                subscriber.recv(zmq_msg);
                std::string data = zmq_msg.to_string();
                Message message = Message::deserialize(data);
                
                // 如果消息是发给所有人或特定此模块的，才处理
                if (message.target.empty() || message.target == moduleId) {
                    handleMessage(message);
                }
            }
            catch (const std::exception& e) {
                if (running) {
                    std::cerr << "[" << moduleId << "] 接收错误: " 
                              << e.what() << std::endl;
                }
            }
        }
    }
    
public:
    ModuleBase(const std::string& id) 
        : moduleId(id), context(1), 
          publisher(context, zmq::socket_type::pub),
          subscriber(context, zmq::socket_type::sub),
          running(false) {}
    
    virtual ~ModuleBase() {
        stop();
    }
    
    // 初始化连接
    virtual void init(const std::string& broker_address = "tcp://localhost") {
        publisher.connect(broker_address + ":5559");
        subscriber.connect(broker_address + ":5560");
        
        // 给连接一些建立的时间
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    // 开始接收消息
    virtual void start() {
        running = true;
        receiveThread = std::thread(&ModuleBase::receiveLoop, this);
    }
    
    // 停止接收消息
    virtual void stop() {
        running = false;
        if (receiveThread.joinable()) {
            receiveThread.join();
        }
    }
    
    // 订阅特定主题
    void subscribe(const std::string& topic) {
        subscriber.set(zmq::sockopt::subscribe, topic);
    }
    
    // 发送消息
    void sendMessage(const Message& message) {
        std::string data = message.serialize();
        publisher.send(zmq::buffer(data));
    }
}; 