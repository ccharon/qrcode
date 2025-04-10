/*
 * MIT License
 *
 * Copyright (c) ccharon
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef CHARCONV_HPP
#define CHARCONV_HPP

#include <string>

namespace charconv {

/*
 * This class provides methods to convert characters from different codepages
 * (like CP437, CP850, etc.) to UTF-8. It uses translation tables for the conversion.
 */
class CharConverter {
   public:
    // Constructor to set the codepage (default to CP437)
    explicit CharConverter(uint16_t codepage = 437);

    // Method to convert a string from the current codepage to UTF-8
    void CharConverter::convert(const char* input, char* utf8TextPtr, size_t bufferSize) const;

   private:
    // Pointer to the current translation table
    const char** translationTable;

    // Helper method to select the translation table based on the codepage
    const char** getTranslationTable(uint16_t codepage) const;

    // Translation tables for different codepages (only the last 128 non ASCII characters)
    static const char* cp437_to_utf8[128];
    static const char* cp850_to_utf8[128];
    static const char* cp858_to_utf8[128];
    static const char* cp852_to_utf8[128];
    static const char* cp866_to_utf8[128];
    static const char* cp737_to_utf8[128];
};
}  // namespace charconv

#endif  // CHARCONV_HPP
