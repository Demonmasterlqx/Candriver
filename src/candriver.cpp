#include "candriver.hpp"
#include <iostream>

namespace RM_communication{

CanDriver::CanDriver(const std::string ifname):interface_name(ifname) {
    // 检查 ifname 长度
    if (interface_name.size() >= IFNAMSIZ) {
        throw std::runtime_error("CAN interface name is too long");
    }

    if (!openCanSocket()) {
        std::cerr << "Error opening CAN socket." << std::endl;
    }
}

bool CanDriver::openCanSocket() {

    if (isCanOk()) {
        throw std::runtime_error("Socket already opened");
    }

    setCanState(false);
    strncpy(ifr.ifr_name, interface_name.c_str(), IFNAMSIZ);

    // 创建套接字
    socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socket_fd < 0) {
        std::cerr << "Error: Could not create CAN socket. " << strerror(errno) << std::endl;
        setCanState(false);
        closeCanSocket();
        return false;
    }

    // 检查can网口标志
    if (ioctl(socket_fd, SIOCGIFFLAGS, &ifr) < 0) {
        std::cerr << "Error: Could not get interface flags for " << interface_name << ". " << strerror(errno) << std::endl;
        setCanState(false);
        closeCanSocket();
        return false;
    }

    bool up_status = (ifr.ifr_flags & IFF_UP) != 0;

    if (!up_status) {
        std::cerr << "Error: CAN interface " << interface_name << " is down." << std::endl;
        setCanState(false);
        closeCanSocket();
        return false;
    }

    // 指定can设备
    if(ioctl(socket_fd, SIOCGIFINDEX, &ifr) < 0) {
        std::cerr << "Error: Could not get interface index for " << interface_name << ". " << strerror(errno) << std::endl;
        setCanState(false);
        closeCanSocket();
        return false;
    }

    if(ifr.ifr_ifindex <= 0) {
        std::cerr << "Error: Invalid interface index for " << interface_name << "." << std::endl;
        setCanState(false);
        closeCanSocket();
        return false;
    }

    // 绑定CAN socket
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        std::cerr << "Error: Could not bind CAN socket. " << strerror(errno) << std::endl;
        closeCanSocket();
        setCanState(false);
        return false;
    }

    setCanState(true);
    return true;
}

void CanDriver::closeCanSocket() {
    if (socket_fd >= 0) {
        close(socket_fd);
        socket_fd = -1;
    }
    setCanState(false);
    return;
}

bool CanDriver::reopenCanSocket() {
    closeCanSocket();
    return openCanSocket();
}

bool CanDriver::sendMessage(const can_frame& frame) {
    if (!isCanOk()) {
        std::cerr << "Error: CAN socket is not open." << std::endl;
        return false;
    }

    int nbytes = write(socket_fd, &frame, sizeof(frame));
    if(nbytes != sizeof(frame)) {
        std::cerr << "Error: Could not send CAN message. " << strerror(errno) << std::endl;
        setCanState(false);
        return false;
    }
    setCanState(true);
    return true;
}

bool CanDriver::receiveMessage(can_frame& frame) {
    if (!isCanOk()) {
        std::cerr << "Error: CAN socket is not open." << std::endl;
        return false;
    }

    int nbytes = read(socket_fd, &frame, sizeof(frame));
    if(nbytes != sizeof(frame)) {
        std::cerr << "Error: Could not read CAN message. " << strerror(errno) << std::endl;
        setCanState(false);
        return false;
    }
    setCanState(true);
    return true;
}

CanDriver::~CanDriver() {
    closeCanSocket();
}

bool CanDriver::isCanOk() {

    if(can_is_ok == false) {
        return false;
    }

    // 检查 socket 是否正常
    if(!((socket_fd >= 0) && (fcntl(socket_fd, F_GETFD) != -1 || errno != EBADF))){
        std::cerr << "Error: CAN socket is not valid." << std::endl;
        setCanState(false);
        return false;
    }

    // 检查 can 网口 状态
    if (ioctl(socket_fd, SIOCGIFFLAGS, &ifr) < 0) {
        std::cerr << "Error: Could not get interface flags for " << interface_name << ". " << strerror(errno) << std::endl;
        setCanState(false);
        return false;
    }

    bool up_status = (ifr.ifr_flags & IFF_UP) != 0;

    if (!up_status) {
        std::cerr << "Error: CAN interface " << interface_name << " is down." << std::endl;
        setCanState(false);
        return false;
    }

    return true;
}

} // namespace RM_communication