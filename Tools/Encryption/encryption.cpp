/**
 * @file encryption.cpp
 * @brief Implementation and CLI entrypoint for a simple XOR file transformer.
 *
 * @details
 * This tool provides a minimal symmetric transform that XORs each byte of the
 * input with the low 8 bits of a provided 32-bit seed and writes the result to
 * the output. Running the tool again on the transformed output using the same
 * seed restores the original contents.
 *
 * Build (MinGW example):
 * @code{.cmd}
 * g++ -std=c++17 -O2 -Wall -Wextra -o Tools\Encryption\encryption.exe Tools\Encryption\encryption.cpp
 * @endcode
 *
 * Usage:
 * @code{.cmd}
 * Tools\Encryption\encryption.exe <input_file> <output_file> <seed> <encrypt|decrypt>
 * @endcode
 *
 * Notes and limitations:
 * - Not cryptographically secure; intended only for obfuscation/testing.
 * - Streams in 64 KiB chunks to handle large files with low memory overhead.
 * - Diagnostics are printed to stderr; returns non-zero exit codes on failure.
 */
#include "encryption.h"
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

/**
 * @brief Default-construct an `Encryption` instance with empty paths and zero seed.
 */
Encryption::Encryption() : inputFile(""), outputFile(""), seed(0) {}

/**
 * @brief Construct an `Encryption` instance bound to `input`, `output`, and `seed`.
 * @param input  Input file path to read from.
 * @param output Output file path to write to (overwritten if exists).
 * @param seed   32-bit seed (only the least significant 8 bits are used).
 */
Encryption::Encryption(const string& input, const string& output, unsigned int seed)
    : inputFile(input), outputFile(output), seed(seed) {}

/**
 * @brief Perform the XOR transformation from `inputFile` to `outputFile`.
 * @return `true` on success; `false` if opening either file fails.
 */
bool Encryption::encrypt()
{
    ifstream inFile(inputFile, ios::binary);

    if (!inFile) {
        cerr << "Error opening input file: " << inputFile << endl;

        return false;
    }

    ofstream outFile(outputFile, ios::binary);

    if (!outFile) {
        cerr << "Error opening output file: " << outputFile << endl;

        return false;
    }

    vector <char> buffer(64 * 1024);

    while (inFile) {
        inFile.read(buffer.data(), static_cast<streamsize>(buffer.size()));

        streamsize bytesRead = inFile.gcount();

        for (streamsize i = 0; i < bytesRead; ++i) {
            buffer[i] ^= static_cast<char> (seed & 0xFF);
        }

        outFile.write(buffer.data(), bytesRead);
    }

    return true;
}

/**
 * @brief Decrypt by reusing the symmetric XOR transformation.
 * @details Equivalent to `encrypt()` because XOR is its own inverse.
 */
bool Encryption::decrypt()
{
    return encrypt();
}

/**
 * @brief CLI entrypoint.
 *
 * @param argc Number of arguments; must be 5 including program name.
 * @param argv Arguments: `<input> <output> <seed> <encrypt|decrypt>`
 * @return 0 on success; 1 on invalid usage or operation failure.
 */
int main(int argc, char* argv[])
{
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file> <seed> <encrypt/decrypt>" << endl;

        return 1;
    }

    string input_file = argv[1];
    string output_file = argv[2];
    unsigned int seed = stoi(argv[3]);
    string mode = argv[4];
    
    if (mode != "encrypt" && mode != "decrypt") {
        cerr << "Invalid mode. Use 'encrypt' or 'decrypt'." << endl;

        return 1;
    }

    Encryption encryption = Encryption(input_file, output_file, seed);

    if (mode == "encrypt") {
        if (!encryption.encrypt()) {
            cerr << "Encryption failed." << endl;

            return 1;
        }
    }
    else {
        if (!encryption.decrypt()) {
            cerr << "Decryption failed." << endl;

            return 1;
        }
    }

    return 0;
}
