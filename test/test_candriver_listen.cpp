#include "candriver.hpp"

#include <memory>
#include <iostream>

using RM_communication::CanDriver;

int main(){
    std::shared_ptr<CanDriver> canport = nullptr;

    try{
        canport = std::make_shared<CanDriver>("can0");
    } catch (const std::exception& e) {
        std::cerr << "Error initializing CanDriver: " << e.what() << std::endl;
        return 1;
    }
    std::cout << "CanDriver initialized successfully." << std::endl;

    while(1){
        can_frame frame;
        canport->receiveMessage(frame);

        // 输出can帧
        std::cout << "Can id " << frame.can_id;
        std::cout << " dlc " << static_cast<int>(frame.can_dlc);
        std::cout << " data ";
        for(int i=0; i< frame.can_dlc; i++){
            // 输出16进制
            std::cout << std::hex << static_cast<int>(frame.data[i]) << " ";
        }
        std::cout << std::endl;
    }
    
    
}