#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

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
    file_data(std::move(data)), message(msg) {}


    std::vector<char> serialize() const {
        std::vector<char> out;

        auto write_string = [&](const std::string& s) {
            uint32_t len = s.size();
            out.insert(out.end(), reinterpret_cast<const char*>(&len), reinterpret_cast<const char*>(&len) + sizeof(len));
            out.insert(out.end(), s.begin(), s.end());
        };

        write_string(filename);
        write_string(extension);
        write_string(message);

        out.insert(out.end(), reinterpret_cast<const char*>(&sent_time), reinterpret_cast<const char*>(&sent_time) + sizeof(sent_time));

        uint64_t datasz = file_data.size();
        out.insert(out.end(), reinterpret_cast<const char*>(&datasz), reinterpret_cast<const char*>(&datasz) + sizeof(datasz));
        out.insert(out.end(), file_data.begin(), file_data.end());

        return out;
    }

    static FileMeta deserialize(const std::vector<char>& bytes) {
        FileMeta m;
        size_t offset = 0;

        auto read_string = [&](std::string& out) {
            uint32_t len;
            std::memcpy(&len, bytes.data() + offset, sizeof(len));
            offset += sizeof(len);
            out.assign(bytes.data() + offset, len);
            offset += len;
        };

        read_string(m.filename);
        read_string(m.extension);
        read_string(m.message);

        std::memcpy(&m.sent_time, bytes.data() + offset, sizeof(m.sent_time));
        offset += sizeof(m.sent_time);

        uint64_t datasz;
        std::memcpy(&datasz, bytes.data() + offset, sizeof(datasz));
        offset += sizeof(datasz);
        m.file_data.resize(datasz);
        if (datasz > 0) {
            std::memcpy(m.file_data.data(), bytes.data() + offset, datasz);
            offset += datasz;
        }
        return m;
    }

#ifdef __linux__
    // Send the serialized FileMeta object on a socket
    bool send_on_socket(int sock_fd) const {
        std::vector<char> bytes = serialize();
        uint64_t total_size = bytes.size();
        // First send the total size of the serialized data
        if (::send(sock_fd, &total_size, sizeof(total_size), 0) != sizeof(total_size))
            return false;
        size_t sent = 0;
        while (sent < total_size) {
            ssize_t n = ::send(sock_fd, bytes.data() + sent, total_size - sent, 0);
            if (n <= 0) return false;
            sent += n;
        }
        return true;
    }

    // Receive and deserialize FileMeta object from a socket
    static FileMeta recv_from_socket(int sock_fd) {
        uint64_t total_size = 0;
        size_t recvd = 0;
        // Read the size of the incoming data
        while (recvd < sizeof(total_size)) {
            ssize_t n = ::recv(sock_fd, reinterpret_cast<char*>(&total_size) + recvd, sizeof(total_size) - recvd, 0);
            if (n <= 0) throw std::runtime_error("Socket closed or error on header read");
            recvd += n;
        }

        std::vector<char> buffer(total_size);
        recvd = 0;
        while (recvd < total_size) {
            ssize_t n = ::recv(sock_fd, buffer.data() + recvd, total_size - recvd, 0);
            if (n <= 0) throw std::runtime_error("Socket closed or error on data read");
            recvd += n;
        }
        return deserialize(buffer);
    }
#endif
};
