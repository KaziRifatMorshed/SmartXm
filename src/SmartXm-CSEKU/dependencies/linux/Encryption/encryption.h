/**
 * @file encryption.h
 * @brief Declaration of a simple stream XOR file obfuscation helper.
 *
 * @details
 * The `Encryption` class performs a very lightweight, symmetric byte-wise XOR
 * transformation on a file stream. It reads from an input file and writes to an
 * output file in chunks, XOR-ing each byte with the least significant 8 bits of
 * a provided 32-bit `seed`. Because XOR is its own inverse, the same operation
 * can be used for both "encryption" and "decryption" as long as the same seed
 * is supplied.
 *
 * Important: This is not cryptographic security. XOR with a single-byte key is
 * trivially reversible and should only be used for basic obfuscation in test or
 * prototyping scenarios.
 *
 * Typical usage is via the CLI defined in `encryption.cpp`, e.g.:
 * @code{.cmd}
 * Tools\Encryption\encryption.exe input.bin output.bin 123 encrypt
 * Tools\Encryption\encryption.exe output.bin roundtrip.bin 123 decrypt
 * @endcode
 */
#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>

using namespace std;

/**
 * @class Encryption
 * @brief Streams bytes from an input file to an output file applying XOR.
 *
 * @details
 * - Buffer size: 64 KiB per chunk (see implementation).
 * - Keying: Uses only the least significant 8 bits of the 32-bit `seed` for
 *   XOR, repeated for every byte.
 * - Symmetry: Applying the operation twice with the same `seed` restores the
 *   original data.
 * - Diagnostics: Methods return `false` on open failures and print messages to
 *   `stderr`. They don't throw exceptions.
 */
class Encryption {
private:
    string inputFile;
    string outputFile;
    unsigned int seed;

public:
    /**
     * @brief Default-construct an unusable instance.
     *
     * @details
     * Initializes with empty file paths and `seed = 0`. You will typically
     * want to use the parameterized constructor instead. If you do use this
     * default constructor, you must assign valid paths and a non-zero seed to
     * the data members before calling `encrypt()`/`decrypt()`.
     */
    Encryption();

    /**
     * @brief Construct an instance bound to specific files and seed.
     *
     * @param input  Path to the source file to read (opened in binary mode).
     * @param output Path to the destination file to write (opened in binary mode).
     *               The file will be created or overwritten.
     * @param seed   32-bit seed; only the least significant 8 bits are used as
     *               the XOR key for every byte.
     */
    Encryption(const string& input, const string& output, unsigned int seed);
    
    /**
     * @brief Apply the XOR transformation from `inputFile` to `outputFile`.
     *
     * @details
     * Reads the input file in 64 KiB chunks, XORs each byte with
     * `static_cast<char>(seed & 0xFF)`, and writes the result to the output
     * file. Because XOR is symmetric, this function doubles as both the
     * encryptor and decryptor depending on your perspective.
     *
     * @return `true` on success; `false` if either input or output cannot be
     *         opened. Any diagnostics are printed to `stderr`.
     *
     * @note This routine does not guarantee detection of short writes or other
     *       I/O errors beyond initial open failures. For robust production use,
     *       check stream states after writes.
     */
    bool encrypt();

    /**
     * @brief Decrypt using the same XOR operation and seed.
     *
     * @details
     * Since XOR is its own inverse, decryption simply re-invokes the same
     * transformation performed by `encrypt()`.
     *
     * @return `true` on success; `false` on open failures (see `encrypt()`).
     */
    bool decrypt();
};

#endif // ENCRYPTION_H