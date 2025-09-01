#include "candriver.hpp"

#include <memory>
#include <iostream>
#include <thread>

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

    can_frame send_frame = {{0}};

    send_frame.can_id = 0x1FF;
    send_frame.can_dlc = 8;
    send_frame.data[0] = 0x17;
    send_frame.data[1] = 0x77;

    while(1){
        
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));

        if (canport->sendMessage(send_frame)) {
            std::cout << "Message sent successfully." << std::endl;
        } else {
            std::cerr << "Error sending message." << std::endl;
            bool reopened = canport->reopenCanSocket();
            if(!reopened) {
                std::cerr << "Error reopening CAN socket." << std::endl;
            }
        }

        can_frame frame;
        bool rec = canport->receiveMessage(frame);
        if(!rec){
            std::cerr << "Error receiving CAN message. try to reopen" << std::endl;
            bool reopened = canport->reopenCanSocket();
            if(!reopened) {
                std::cerr << "Error reopening CAN socket." << std::endl;
            }
            continue;
        }

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