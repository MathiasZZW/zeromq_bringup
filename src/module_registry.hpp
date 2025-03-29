class ModuleRegistry : public ModuleBase {
private:
    std::map<std::string, std::vector<std::string>> moduleTopics;
    
    void handleMessage(const Message& message) override {
        // 处理模块注册/注销请求
        if (message.topic == "SYSTEM.registry") {
            if (message.type == "REGISTER") {
                std::string moduleId = message.source;
                std::vector<std::string> topics = message.payload["topics"];
                moduleTopics[moduleId] = topics;
                
                // 发送确认
                Message response;
                response.topic = "SYSTEM.registry";
                response.source = "REGISTRY";
                response.target = moduleId;
                response.type = "REGISTERED";
                sendMessage(response);
            }
            else if (message.type == "UNREGISTER") {
                std::string moduleId = message.source;
                moduleTopics.erase(moduleId);
            }
        }
    }
    
public:
    ModuleRegistry() : ModuleBase("REGISTRY") {
        subscribe("SYSTEM.registry");
    }
    
    // 获取支持特定主题的所有模块
    std::vector<std::string> getModulesForTopic(const std::string& topic) {
        std::vector<std::string> result;
        for (const auto& pair : moduleTopics) {
            const auto& moduleId = pair.first;
            const auto& topics = pair.second;
            if (std::find(topics.begin(), topics.end(), topic) != topics.end()) {
                result.push_back(moduleId);
            }
        }
        return result;
    }
}; 