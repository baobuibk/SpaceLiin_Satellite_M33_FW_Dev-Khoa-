#ifndef MEM_H
#define MEM_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Reads data from the flash memory.
 *
 * @param partition The partition of the flash memory to read from.
 * @param addr The address within the partition to start reading from.
 * @param data Pointer to the buffer where the read data will be stored.
 * @param len The number of bytes to read.
 * @return int 0 on success, or an error code on failure.
 */
int mem_read_flash(uint8_t partition, uint32_t addr, uint8_t *data, uint16_t len);

/**
 * @brief Writes data to the flash memory.
 *
 * @param partition The partition of the flash memory to write to.
 * @param addr The address within the partition to start writing to.
 * @param data Pointer to the buffer containing the data to write.
 * @param len The number of bytes to write.
 * @return int 0 on success, or an error code on failure.
 */
int mem_write_flash(uint8_t partition, uint32_t addr, const uint8_t *data, uint16_t len);

/**
 * @brief Erases a section of the flash memory.
 *
 * @param partition The partition of the flash memory to erase.
 * @param addr The address within the partition to start erasing.
 * @return int 0 on success, or an error code on failure.
 */
int mem_erase_flash(uint8_t partition, uint32_t addr);

/**
 * @brief Writes data to the FRAM memory.
 *
 * @param add The address in FRAM memory to write to.
 * @param value Pointer to the data to write.
 * @param len The number of bytes to write.
 * @return int 0 on success, or an error code on failure.
 */
int mem_write_fram(uint32_t add, void *value, size_t len);

/**
 * @brief Reads data from the FRAM memory.
 *
 * @param add The address in FRAM memory to read from.
 * @param value Pointer to the buffer where the read data will be stored.
 * @param len The number of bytes to read.
 * @return int 0 on success, or an error code on failure.
 */
int mem_read_fram(uint32_t add, void *value, size_t len);

/**
 * Write the line in buffer the end of the file.
 *
 * @param filename Path to a file
 * @param buffer Buffer containing the data to write. It should be a null-terminated string.
 * If no line separator is found at the end of the buffer, it will be added automatically.
 * @param len Buffer length. Maximum len bytes will be written to the file, plus a line separator if not present.
 * @return int 0 on success, or an error code on failure.
 */
int mem_write_file_line(char* filename, uint8_t* buffer, size_t len);

/**
 * Reads the line at the given index from the file specified by filename.
 * If the file contains fewer lines than index, an error code is returned.
 * The buffer will contain a null-terminated string of the line content (excluding the line separator).
 * This function modifies the buffer even if it returns an error code.
 *
 * @param filename Path to the file.
 * @param index Line number to read. If index is 0, reads the first line.
 * If index is greater than or equal to the number of lines in the file -1 is returned.
 * @param buffer Buffer to store the read line. Should be large enough to hold the line content and a null terminator.
 * If the line is longer than len-1, it will be truncated.
 * This buffer will be modified even if the function returns an error code.
 * @param len Length of the buffer. At most len-1 characters will be copied, plus a null terminator.
 * @return int 0 on success, -1 if the line does not exist, or an error code on failure.
 */
int mem_read_file_line(char* filename, int index, uint8_t* buffer, size_t len);

/**
 * Delete a file from the filesystem.
 * @param filename Path to the file to delete.
 * @return 0 on success, or an error code on failure.
 */
int mem_erase_file(char* filename);

#endif //MEM_H
