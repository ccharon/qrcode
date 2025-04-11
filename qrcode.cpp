/*
 * QR Code generator
 *
 * Copyright (c) ccharon
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * - The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * - The Software is provided "as is", without warranty of any kind, express or
 *   implied, including but not limited to the warranties of merchantability,
 *   fitness for a particular purpose and noninfringement. In no event shall the
 *   authors or copyright holders be liable for any claim, damages or other
 *   liability, whether in an action of contract, tort or otherwise, arising from,
 *   out of or in connection with the Software or the use or other dealings in the
 *   Software.
 *
 */

#include <conio.h>
#include <dos.h>

#include <iostream>

#include "charconv.hpp"
#include "qrcodegen.hpp"

#define QR_CODE_VERSION "1.0.3"

using charconv::CharConverter;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;

// Function prototypes
static void printQrCGA(const QrCode& qr);
static void setPixel(int x, int y, int color);
static void setVideoMode(unsigned char mode);
static uint8_t getVideoMode();
static void printUsage();
static uint16_t getSystemCodepage();

int main(int argc, char* argv[]) {
    // Check if the required parameters are provided
    if (argc != 2) {
        printUsage();
        return EXIT_FAILURE;
    }

    const char* text = argv[1];

    try {
        // Convert the text to UTF-8 using the system codepage
        char utf8Text[2048];
        const CharConverter converter(getSystemCodepage());
        converter.convert(text, utf8Text, 2048);

        // Generate the QR Code
        const QrCode qr = QrCode::encodeText(utf8Text, QrCode::LOW);

        // remember current mode and set CGA 320x200 4 color mode
        uint8_t initialVideoMode = getVideoMode();
        setVideoMode(0x04);

        printQrCGA(qr);

        // Wait for a key press and restore previous mode
        getch();
        setVideoMode(initialVideoMode);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// Function to print usage information
static void printUsage() {
    std::cout << "QRCode Generator " << QR_CODE_VERSION << " for MS-DOS CGA systems. (c) 2025 Christian Charon" << std::endl
              << "Contact: ccharon@mailbox.org | Licensed under the MIT License." << std::endl
              << "Sources: https://github.com/ccharon/qrcode" << std::endl
              << std::endl
              << "Usage: qrcode.exe \"<text>\"" << std::endl
              << std::endl
              << "Options:" << std::endl
              << "  <text>       The message to encode in the QR Code." << std::endl
              << std::endl
              << "Examples:" << std::endl
              << "  qrcode.exe \"Hello, world!\"" << std::endl
              << "  qrcode.exe \"WIFI:T:WPA;S:examplenet;P:secret;;\"" << std::endl
              << std::endl
              << "The text for the QR Code is encoded to UTF-8. Automatic conversion from" << std::endl
              << "codepages 437, 850, 852, 858, 866, and 737 is supported, 437 is the fallback." << std::endl
              << std::endl;
}

/*---- CGA Mode Helper ----*/

// BIOS Interrupt call to set the video mode
static void setVideoMode(uint8_t mode) {
    union REGS regs;
    regs.h.ah = 0x00;
    regs.h.al = mode;
    int86(0x10, &regs, &regs);
}

// BIOS Interrupt call to get the current video mode
static uint8_t getVideoMode() {
    union REGS regs;
    regs.h.ah = 0x0F;
    int86(0x10, &regs, &regs);
    return regs.h.al;
}

// Render a centered QR code in CGA 320x200 mode
static void printQrCGA(const QrCode& qr) {
    const int border = 4;  // Border size in QR modules
    const int scaleX = 2;  // Default horizontal scale factor (pixels per module)
    const int scaleY = 2;  // Default vertical scale factor (pixels per module)

    const int qrSize = qr.getSize() + 2 * border;  // QR code size including border (in modules)
    const int pixelWidth = qrSize * scaleX;        // Total width in pixels
    const int pixelHeight = qrSize * scaleY;       // Total height in pixels

    const bool fits = (pixelWidth <= 320 && pixelHeight <= 200);

    if (!fits) {
        throw std::runtime_error("QR code is too large to fit on the screen.");
    }

    // Calculate the starting position to center the QR code
    int startX = (320 - pixelWidth) / 2;   // Horizontal centering
    int startY = (200 - pixelHeight) / 2;  // Vertical centering

    // Render the QR code
    for (int y = -border; y < qr.getSize() + border; y++) {          // rows
        for (int x = -border; x < qr.getSize() + border; x++) {      // columns
            int color = qr.getModule(x, y) ? 0 : 1;                  // Black or white module
            for (int dy = 0; dy < scaleY; dy++) {                    // vertical scaling
                for (int dx = 0; dx < scaleX; dx++) {                // horizontal scaling
                    int xPos = startX + (x + border) * scaleX + dx;  // Horizontal position
                    int yPos = startY + (y + border) * scaleY + dy;  // Vertical position
                    setPixel(xPos, yPos, color);
                }
            }
        }
    }
}

// Set a pixel in CGA 320x200 4 color mode
static void setPixel(int x, int y, int color) {
    if (x < 0 || x >= 320 || y < 0 || y >= 200) {
        return;  // Ignore out-of-bounds pixels
    }

    uint8_t far* video = (uint8_t far*)0xB8000000L;

    // Calculate byte offset
    uint16_t offset = ((y & 1) << 13) + (y >> 1) * 80 + (x >> 2);
    uint8_t shift = (3 - (x & 3)) * 2;  // shift amount for pixel within the byte

    uint8_t mask = 0x03 << shift;  // mask to clear pixel
    video[offset] = (video[offset] & ~mask) | ((color & 0x03) << shift);
}

// MS-DOS Interrupt call to get the system codepage
static uint16_t getSystemCodepage() {
    union REGS regs;
    regs.h.ah = 0x66;
    regs.h.al = 0x01;
    int86(0x21, &regs, &regs); 

    if (regs.x.cflag) {
        return 0;  // Return 0 to indicate an error
    }
    return regs.x.dx;  // Return the system (default) codepage
}
