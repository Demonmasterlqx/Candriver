#include "candriver.hpp"

namespace RM_communication{

CanDriver::CanDriver(const std::string ifname):interface_name(ifname) {
    // 检查 ifname 长度
    if (interface_name.size() >= IFNAMSIZ) {
        throw std::runtime_error("CAN interface name is too long");
    }

    if (!openCanSocket()) {
        throw std::runtime_error("Failed to open CAN socket");
    }

}

bool CanDriver::openCanSocket() {

    if (socket_fd >= 0 || isCanOk()) {
        throw std::runtime_error("Socket already opened");
    }

    setCanState(false);

    socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socket_fd < 0) {
        return false;
    }

    // 绑定CAN socket
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(socket_fd);
        socket_fd = -1;
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
    if (socket_fd < 0 || !isCanOk()) {
        return false;
    }

    int nbytes = write(socket_fd, &frame, sizeof(frame));
    return nbytes == sizeof(frame);
}

bool CanDriver::receiveMessage(can_frame& frame) {
    if (socket_fd < 0 || !isCanOk()) {
        return false;
    }

    int nbytes = read(socket_fd, &frame, sizeof(frame));
    return nbytes == sizeof(frame);
}

} // namespace RM_communication