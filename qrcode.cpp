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

#include <iostream>
#include <dos.h>
#include <conio.h>
#include "qrcodegen.hpp"
#include "charconv.hpp"

using qrcodegen::QrCode;
using qrcodegen::QrSegment;


// Function prototypes
static void printQrCGA(const QrCode &qr, const char *info);
static void setPixel(int x, int y, int color);
static void setVideoMode(unsigned char mode);
static uint8_t getVideoMode();
static void printUsage();

int main(int argc, char* argv[]) {
    // Check if the required parameters are provided
    if (argc != 3) {
        printUsage();
        return EXIT_FAILURE;
    }

    // Get the codepage parameter
    const char* codepage = argv[1];
    const char* text = argv[2];
	
	try {
        // Create a CharConverter instance with the specified codepage
        charconv::CharConverter converter(codepage);
        const char* utf8Text = converter.convert(text);

		// Generate the QR code
		const QrCode qr = QrCode::encodeText(utf8Text, QrCode::LOW);
		printQrCGA(qr, text);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// Function to print usage information
static void printUsage() {
    std::cout << "Usage: qrcode.exe <codepage> <text>" << std::endl
              << "  <codepage>   The codepage to use for text conversion (437 or 850)." << std::endl
              << "  <text>       The message to encode in the QR code." << std::endl
              << "Examples:" << std::endl
              << "  qrcode.exe 437 \"Hello, world!\"" << std::endl;
}

/*---- CGA Mode Helper ----*/

static void setVideoMode(uint8_t mode) {
	union REGS regs;
	regs.h.ah = 0x00;
	regs.h.al = mode;
	int86(0x10, &regs, &regs);
}

static uint8_t getVideoMode() {
	union REGS regs;
	regs.h.ah = 0x0F;
	int86(0x10, &regs, &regs);
	return regs.h.al;
}

// Function to set a pixel in CGA 320x200 4 color mode
static void setPixel(int x, int y, int color) {
	if (x < 0 || x >= 320 || y < 0 || y >= 200) {
	return;  // Ignore out-of-bounds pixels
	}
	
	uint8_t far* video = (uint8_t far*)0xB8000000L;
    
	// Calculate byte offset
	uint16_t offset = ((y & 1) << 13) + (y >> 1) * 80 + (x >> 2);
	uint8_t shift = (3 - (x & 3)) * 2; // shift amount for pixel within the byte
	
	uint8_t mask = 0x03 << shift;      // mask to clear pixel
	video[offset] = (video[offset] & ~mask) | ((color & 0x03) << shift);
}

// Function to render a centered QR code in CGA 320x200 mode
static void printQrCGA(const QrCode &qr, const char *info) {
	const int border = 4;  // Border size in QR modules
	const int scaleX = 2;  // Horizontal scale factor (pixels per module)
	const int scaleY = 2;  // Vertical scale factor (pixels per module)

	// Calculate the total QR code size in pixels
	int qrSize = qr.getSize() + 2 * border; // QR code size including border (in modules)
	int pixelWidth = qrSize * scaleX;       // Total width in pixels
	int pixelHeight = qrSize * scaleY;      // Total height in pixels

	// Ensure the QR code fits within the 320x200 resolution
	if (pixelWidth > 320 || pixelHeight > 200) {
		std::cerr << "Error: QR code is too large to fit on the screen." << std::endl;
		return;
	}

	// Calculate the starting position to center the QR code
	int startX = (320 - pixelWidth) / 2;  // Horizontal centering
	int startY = (200 - pixelHeight) / 2; // Vertical centering

	// remember current mode and set CGA 320x200 4 color mode
	uint8_t initialVideoMode = getVideoMode();
	setVideoMode(0x04);

	std::cout << info << std::endl;

	// Render the QR code
	for (int y = -border; y < qr.getSize() + border; y++) {  // rows
		for (int x = -border; x < qr.getSize() + border; x++) {  // columns
			int color = qr.getModule(x, y) ? 0 : 1;  // Black or white module
			for (int dy = 0; dy < scaleY; dy++) {  // vertical scaling
				for (int dx = 0; dx < scaleX; dx++) {  // horizontal scaling
					int xPos = startX + (x + border) * scaleX + dx;  // Horizontal position
					int yPos = startY + (y + border) * scaleY + dy;  // Vertical position
					setPixel(xPos, yPos, color);
				}
			}
		}
	}
	
	// Wait for a key press
	getch();
	
	// Restore previous mode
	setVideoMode(initialVideoMode);
}

