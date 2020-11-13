/**
 * @file tap.h
 * @author Joakim Bertils
 * @brief Linux kernel TAP Interface
 * @version 0.1
 * @date 2020-11-12
 *
 * Interface for the TAP device, which is a virtualized network device letting
 * the user tunnel layer 2 packets from any application. TAP simulates a link
 * layer device and operates carrying ethernet frames, working like a user space
 * network bridge.
 *
 */

#ifndef _TAP_H
#define _TAP_H

/**
 * @brief Initializes the device 
 * 
 * @param dev Interface name.
 * @return < 0 on error.
 */
int tap_init(char *dev);

/**
 * @brief Read from the interface, returning any bytes that may be present.
 * 
 * @param buf Buffer to store the read bytes in.
 * @param len Max size of the buffer.
 * @return Number of bytes read.
 * @return < 0 on error.
 */
int tap_read(char *buf, int len);

/**
 * @brief Write to the interface.
 * 
 * @param buf Buffer containing the bytes to write.
 * @param len Number of bytes in the buffer.
 * @return Bytes written to the interface. 
 */
int tap_write(char *buf, int len);

#endif

//==============================================================================
// End of file
//==============================================================================