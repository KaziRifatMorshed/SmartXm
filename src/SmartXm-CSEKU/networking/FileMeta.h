#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <cstdint>
#include <cstring>
#include <stdexcept>

#define DEBUG_ON

#ifdef __linux__
#include <sys/socket.h>
#include <unistd.h>
#endif

#ifdef _WIN32
// Define these before any Windows includes to avoid conflicts
#define WIN32_LEAN_AND_MEAN
// #define NOMINMAX
#define NOGDI
#define NOSERVICE
#define NOMCX
#define NOIME
#define NORPC          // This specifically helps with the byte conflict

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include <iostream>

// Rest of your FileMeta class...


class FileMeta {
public:
    std::string filename;
    std::string extension;
    std::time_t sent_time;
    std::vector<char> file_data;
    std::string message;

    FileMeta() = default;
    FileMeta(const std::string& fname, const std::string& ext, std::time_t t,
             std::vector<char>&& data, const std::string& msg = "")
        : filename(fname), extension(ext), sent_time(t),
        file_data(std::move(data)), message(msg) {
#ifdef DEBUG_ON
        std::cout << "[FileMeta] FileMeta object created." << std::endl;
#endif
    }

    std::vector<char> serialize() const {
        std::vector<char> out;

        auto write_string = [&](const std::string& s) {
            size_t len = s.size();
            out.insert(out.end(), reinterpret_cast<const char*>(&len), reinterpret_cast<const char*>(&len) + sizeof(len));
            out.insert(out.end(), s.begin(), s.end());
        };

        write_string(filename);
#ifdef DEBUG_ON
        std::cout << "[FileMeta::serialize] Filename written." << std::endl;
#endif
        write_string(extension);
#ifdef DEBUG_ON
        std::cout << "[FileMeta::serialize] Extension written." << std::endl;
#endif
        write_string(message);
#ifdef DEBUG_ON
        std::cout << "[FileMeta::serialize] Message written." << std::endl;
#endif

        out.insert(out.end(), reinterpret_cast<const char*>(&sent_time), reinterpret_cast<const char*>(&sent_time) + sizeof(sent_time));
#ifdef DEBUG_ON
        std::cout << "[FileMeta::serialize] Sent time written." << std::endl;
#endif

        uint64_t datasz = file_data.size();
        out.insert(out.end(), reinterpret_cast<const char*>(&datasz), reinterpret_cast<const char*>(&datasz) + sizeof(datasz));
        out.insert(out.end(), file_data.begin(), file_data.end());
#ifdef DEBUG_ON
        std::cout << "[FileMeta::serialize] File data written." << std::endl;
#endif

#ifdef DEBUG_ON
        std::cout << "[FileMeta::serialize] Serialization complete." << std::endl;
#endif
        return out;
    }

    static FileMeta deserialize(const std::vector<char>& byte_s) {
        FileMeta m;
        size_t offset = 0;

        auto read_string = [&](std::string& out) {
            uint32_t len;
            std::memcpy(&len, byte_s.data() + offset, sizeof(len));
            offset += sizeof(len);
            out.assign(byte_s.data() + offset, len);
            offset += len;
        };

        read_string(m.filename);
#ifdef DEBUG_ON
        std::cout << "[FileMeta::deserialize] Filename read: " << m.filename << std::endl;
#endif
        read_string(m.extension);
#ifdef DEBUG_ON
        std::cout << "[FileMeta::deserialize] Extension read: " << m.extension << std::endl;
#endif
        read_string(m.message);
#ifdef DEBUG_ON
        std::cout << "[FileMeta::deserialize] Message read: " << m.message << std::endl;
#endif

        std::memcpy(&m.sent_time, byte_s.data() + offset, sizeof(m.sent_time));
        offset += sizeof(m.sent_time);
#ifdef DEBUG_ON
        std::cout << "[FileMeta::deserialize] Sent time read: " << m.sent_time << std::endl;
#endif

        uint64_t datasz;
        std::memcpy(&datasz, byte_s.data() + offset, sizeof(datasz));
        offset += sizeof(datasz);
        m.file_data.resize(datasz);
        if (datasz > 0) {
            std::memcpy(m.file_data.data(), byte_s.data() + offset, datasz);
            offset += datasz;
        }
#ifdef DEBUG_ON
        std::cout << "[FileMeta::deserialize] File data read, size: " << datasz << std::endl;
#endif

#ifdef DEBUG_ON
        std::cout << "[FileMeta::deserialize] Deserialization complete." << std::endl;
#endif
        return m;
    }

#ifdef __linux__
    bool send_on_socket(int sock_fd) const {
        std::vector<char> byte_s = serialize();
#ifdef DEBUG_ON
        std::cout << "[FileMeta::send_on_socket] Serialized data." << std::endl;
#endif
        uint64_t total_size = byte_s.size();
        if (::send(sock_fd, &total_size, sizeof(total_size), 0) != sizeof(total_size)) {
#ifdef DEBUG_ON
            std::cerr << "[FileMeta::send_on_socket] Error sending total size." << std::endl;
#endif
            return false;
        }
#ifdef DEBUG_ON
        std::cout << "[FileMeta::send_on_socket] Total size sent: " << total_size << std::endl;
#endif
        size_t sent = 0;
        while (sent < total_size) {
            ssize_t n = ::send(sock_fd, byte_s.data() + sent, total_size - sent, 0);
            if (n <= 0) {
#ifdef DEBUG_ON
                std::cerr << "[FileMeta::send_on_socket] Error sending data." << std::endl;
#endif
                return false;
            }
            sent += n;
        }
#ifdef DEBUG_ON
        std::cout << "[FileMeta::send_on_socket] Data sent successfully." << std::endl;
#endif
        return true;
    }
    static FileMeta recv_from_socket(int sock_fd) {
#ifdef DEBUG_ON
        std::cout << "[FileMeta::recv_from_socket] Processing a received file from server" << std::endl;
#endif
        uint64_t total_size = 0;
        size_t recvd = 0;
        while (recvd < sizeof(total_size)) {
            ssize_t n = ::recv(sock_fd, reinterpret_cast<char*>(&total_size) + recvd, sizeof(total_size) - recvd, 0);
            if (n <= 0) throw std::runtime_error("Socket closed or error on header read");
            recvd += n;
        }
#ifdef DEBUG_ON
        std::cout << "[FileMeta::recv_from_socket] Total size received: " << total_size << std::endl;
#endif
        std::vector<char> buffer(total_size);
        recvd = 0;
        while (recvd < total_size) {
            ssize_t n = ::recv(sock_fd, buffer.data() + recvd, total_size - recvd, 0);
            if (n <= 0) throw std::runtime_error("Socket closed or error on data read");
            recvd += n;
        }
#ifdef DEBUG_ON
        std::cout << "[FileMeta::recv_from_socket] Data received." << std::endl;
#endif
        return deserialize(buffer);
    }
#endif

#ifdef _WIN32
    bool send_on_socket(SOCKET sock_fd) const {
        std::vector<char> byte_s = serialize();
#ifdef DEBUG_ON
        std::cout << "[FileMeta::send_on_socket] Serialized data." << std::endl;
#endif
        uint64_t total_size = byte_s.size();
        if (::send(sock_fd, reinterpret_cast<const char*>(&total_size), sizeof(total_size), 0) != sizeof(total_size)) {
#ifdef DEBUG_ON
            std::cerr << "[FileMeta::send_on_socket] Error sending total size." << std::endl;
#endif
            return false;
        }
#ifdef DEBUG_ON
        std::cout << "[FileMeta::send_on_socket] Total size sent: " << total_size << std::endl;
#endif
        size_t sent = 0;
        while (sent < total_size) {
            int n = ::send(sock_fd, byte_s.data() + sent, static_cast<int>(total_size - sent), 0);
            if (n <= 0) {
#ifdef DEBUG_ON
                std::cerr << "[FileMeta::send_on_socket] Error sending data." << std::endl;
#endif
                return false;
            }
            sent += n;
        }
#ifdef DEBUG_ON
        std::cout << "[FileMeta::send_on_socket] Data sent successfully." << std::endl;
#endif
        return true;
    }
    static FileMeta recv_from_socket(SOCKET sock_fd) {
#ifdef DEBUG_ON
        std::cout << "[FileMeta::recv_from_socket] Processing a received file from server" << std::endl;
#endif
        uint64_t total_size = 0;
        size_t recvd = 0;
        while (recvd < sizeof(total_size)) {
            int n = ::recv(sock_fd, reinterpret_cast<char*>(&total_size) + recvd, static_cast<int>(sizeof(total_size) - recvd), 0);
            if (n <= 0) throw std::runtime_error("Socket closed or error on header read");
            recvd += n;
        }
#ifdef DEBUG_ON
        std::cout << "[FileMeta::recv_from_socket] Total size received: " << total_size << std::endl;
#endif
        std::vector<char> buffer(total_size);
        recvd = 0;
        while (recvd < total_size) {
            int n = ::recv(sock_fd, buffer.data() + recvd, static_cast<int>(total_size - recvd), 0);
            if (n <= 0) throw std::runtime_error("Socket closed or error on data read");
            recvd += n;
        }
#ifdef DEBUG_ON
        std::cout << "[FileMeta::recv_from_socket] Data received." << std::endl;
#endif
        return deserialize(buffer);
    }
#endif
};
