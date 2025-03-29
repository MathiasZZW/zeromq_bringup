#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// 消息格式定义
struct Message {
    std::string topic;    // 消息主题，用于路由
    std::string source;   // 发送者ID
    std::string target;   // 目标ID (可选，空表示广播)
    std::string type;     // 消息类型
    json payload;         // 消息内容
    
    // 序列化消息为字符串
    std::string serialize() const {
        json j;
        j["topic"] = topic;
        j["source"] = source;
        j["target"] = target;
        j["type"] = type;
        j["payload"] = payload;
        return j.dump();
    }
    
    // 从字符串反序列化消息
    static Message deserialize(const std::string& data) {
        json j = json::parse(data);
        Message msg;
        msg.topic = j["topic"];
        msg.source = j["source"];
        msg.target = j["target"];
        msg.type = j["type"];
        msg.payload = j["payload"];
        return msg;
    }
}; 