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

#include "charconv.hpp"

namespace charconv {
    // Constructor: Set the initial codepage
    CharConverter::CharConverter(const char* codepage) {
        translationTable = getTranslationTable(codepage);
    }

    // Method to convert a string from the current codepage to UTF-8
    const char* CharConverter::convert(const char* input) const {
        if (!input) {
            throw std::invalid_argument("Input string cannot be null.");
        }

        std::string utf8Text;
        for (const char* p = input; *p; ++p) {
            unsigned char ch = static_cast<unsigned char>(*p);
            
            bool isAscii = (ch < 0x80);
            
            if (isAscii) {
                utf8Text += ch;
            } else {
                // subtract 0x80 to get the index for the translation table, 
                // it is "missing" the first 128 ASCII characters.
                utf8Text += translationTable[ch - 0x80];
            }
        }

        const char* utf8TextPtr = utf8Text.c_str();
        return utf8TextPtr;
    }

    // Helper method to select the translation table based on the codepage
    const char** CharConverter::getTranslationTable(const char* codepage) const {
        if (std::strcmp(codepage, "437") == 0) {
            return cp437_to_utf8;
        } else if (std::strcmp(codepage, "850") == 0) {
            return cp850_to_utf8;
        } else if (std::strcmp(codepage, "852") == 0) {
            return cp852_to_utf8;
        } else if (std::strcmp(codepage, "866") == 0) {
            return cp866_to_utf8;
        } else if (std::strcmp(codepage, "737") == 0) {
            return cp737_to_utf8;
        } else {
            throw std::runtime_error("Unsupported codepage: " + std::string(codepage));
        }
    }

    const char* CharConverter::cp850_to_utf8[128] = {
        /* 0x80 */ "\xC3\x87", /* 0x81 */ "\xC3\xBC", /* 0x82 */ "\xC3\xA9", /* 0x83 */ "\xC3\xA2", /* 0x84 */ "\xC3\xA4", /* 0x85 */ "\xC3\xA0", /* 0x86 */ "\xC3\xA5", /* 0x87 */ "\xC3\xA7", /* 0x88 */ "\xC3\xAA", /* 0x89 */ "\xC3\xAB",
        /* 0x8A */ "\xC3\xA8", /* 0x8B */ "\xC3\xAF", /* 0x8C */ "\xC3\xAE", /* 0x8D */ "\xC3\xAC", /* 0x8E */ "\xC3\x84", /* 0x8F */ "\xC3\x85", /* 0x90 */ "\xC3\x89", /* 0x91 */ "\xC3\xA6", /* 0x92 */ "\xC3\x86", /* 0x93 */ "\xC3\xB4",
        /* 0x94 */ "\xC3\xB6", /* 0x95 */ "\xC3\xB2", /* 0x96 */ "\xC3\xBB", /* 0x97 */ "\xC3\xB9", /* 0x98 */ "\xC3\xBF", /* 0x99 */ "\xC3\x96", /* 0x9A */ "\xC3\x9C", /* 0x9B */ "\xC3\xB8", /* 0x9C */ "\xC2\xA3", /* 0x9D */ "\xC3\x98",
        /* 0x9E */ "\xC3\x97", /* 0x9F */ "\xC6\x92", /* 0xA0 */ "\xC3\xA1", /* 0xA1 */ "\xC3\xAD", /* 0xA2 */ "\xC3\xB3", /* 0xA3 */ "\xC3\xBA", /* 0xA4 */ "\xC3\xB1", /* 0xA5 */ "\xC3\x91", /* 0xA6 */ "\xC2\xAA", /* 0xA7 */ "\xC2\xBA",
        /* 0xA8 */ "\xC2\xBF", /* 0xA9 */ "\xC2\xAE", /* 0xAA */ "\xC2\xAC", /* 0xAB */ "\xC2\xBD", /* 0xAC */ "\xC2\xBC", /* 0xAD */ "\xC2\xA1", /* 0xAE */ "\xC2\xAB", /* 0xAF */ "\xC2\xBB", /* 0xB0 */ "\xE2\x96\x91", /* 0xB1 */ "\xE2\x96\x92",
        /* 0xB2 */ "\xE2\x96\x93", /* 0xB3 */ "\xE2\x94\x82", /* 0xB4 */ "\xE2\x94\xA4", /* 0xB5 */ "\xC3\x81", /* 0xB6 */ "\xC3\x82", /* 0xB7 */ "\xC3\x80", /* 0xB8 */ "\xC2\xA9", /* 0xB9 */ "\xE2\x95\xA3", /* 0xBA */ "\xE2\x95\x91",
        /* 0xBB */ "\xE2\x95\x97", /* 0xBC */ "\xE2\x95\x9D", /* 0xBD */ "\xC2\xA2", /* 0xBE */ "\xC2\xA5", /* 0xBF */ "\xE2\x94\x90", /* 0xC0 */ "\xE2\x94\x94", /* 0xC1 */ "\xE2\x94\xB4", /* 0xC2 */ "\xE2\x94\xAC", /* 0xC3 */ "\xE2\x94\x9C",
        /* 0xC4 */ "\xE2\x94\x80", /* 0xC5 */ "\xE2\x94\xBC", /* 0xC6 */ "\xC3\xA3", /* 0xC7 */ "\xC3\x83", /* 0xC8 */ "\xE2\x95\x9A", /* 0xC9 */ "\xE2\x95\x94", /* 0xCA */ "\xE2\x95\xA9", /* 0xCB */ "\xE2\x95\xA6", /* 0xCC */ "\xE2\x95\xA0",
        /* 0xCD */ "\xE2\x95\x90", /* 0xCE */ "\xE2\x95\xAC", /* 0xCF */ "\xC2\xA4", /* 0xD0 */ "\xC3\xB0", /* 0xD1 */ "\xC3\x90", /* 0xD2 */ "\xC3\x8A", /* 0xD3 */ "\xC3\x8B", /* 0xD4 */ "\xC3\x88", /* 0xD5 */ "\xC4\xB1", /* 0xD6 */ "\xC3\x8D",
        /* 0xD7 */ "\xC3\x8E", /* 0xD8 */ "\xC3\x8F", /* 0xD9 */ "\xE2\x94\x98", /* 0xDA */ "\xE2\x94\x8C", /* 0xDB */ "\xE2\x96\x88", /* 0xDC */ "\xE2\x96\x84", /* 0xDD */ "\xC2\xA6", /* 0xDE */ "\xC3\x8C", /* 0xDF */ "\xE2\x96\x80", /* 0xE0 */ "\xC3\x93",
        /* 0xE1 */ "\xC3\x9F", /* 0xE2 */ "\xC3\x94", /* 0xE3 */ "\xC3\x92", /* 0xE4 */ "\xC3\xB5", /* 0xE5 */ "\xC3\x95", /* 0xE6 */ "\xC2\xB5", /* 0xE7 */ "\xC3\xBE", /* 0xE8 */ "\xC3\x9E", /* 0xE9 */ "\xC3\x9A", /* 0xEA */ "\xC3\x9B",
        /* 0xEB */ "\xC3\x99", /* 0xEC */ "\xC3\xBD", /* 0xED */ "\xC3\x9D", /* 0xEE */ "\xC2\xAF", /* 0xEF */ "\xC2\xB4", /* 0xF0 */ "\xC2\xAD", /* 0xF1 */ "\xC2\xB1", /* 0xF2 */ "\xE2\x80\x97", /* 0xF3 */ "\xC2\xBE", /* 0xF4 */ "\xC2\xB6",
        /* 0xF5 */ "\xC2\xA7", /* 0xF6 */ "\xC3\xB7", /* 0xF7 */ "\xC2\xB8", /* 0xF8 */ "\xC2\xB0", /* 0xF9 */ "\xC2\xA8", /* 0xFA */ "\xC2\xB7", /* 0xFB */ "\xC2\xB9", /* 0xFC */ "\xC2\xB3", /* 0xFD */ "\xC2\xB2", /* 0xFE */ "\xE2\x96\xA0",
        /* 0xFF */ "\xC2\xA0",
    };
    
    const char* CharConverter::cp437_to_utf8[128] = {
        /* 0x80 */ "\xC3\x87", /* 0x81 */ "\xC3\xBC", /* 0x82 */ "\xC3\xA9", /* 0x83 */ "\xC3\xA2", /* 0x84 */ "\xC3\xA4", /* 0x85 */ "\xC3\xA0", /* 0x86 */ "\xC3\xA5", /* 0x87 */ "\xC3\xA7", /* 0x88 */ "\xC3\xAA", /* 0x89 */ "\xC3\xAB",
        /* 0x8A */ "\xC3\xA8", /* 0x8B */ "\xC3\xAF", /* 0x8C */ "\xC3\xAE", /* 0x8D */ "\xC3\xAC", /* 0x8E */ "\xC3\x84", /* 0x8F */ "\xC3\x85", /* 0x90 */ "\xC3\x89", /* 0x91 */ "\xC3\xA6", /* 0x92 */ "\xC3\x86", /* 0x93 */ "\xC3\xB4",
        /* 0x94 */ "\xC3\xB6", /* 0x95 */ "\xC3\xB2", /* 0x96 */ "\xC3\xBB", /* 0x97 */ "\xC3\xB9", /* 0x98 */ "\xC3\xBF", /* 0x99 */ "\xC3\x96", /* 0x9A */ "\xC3\x9C", /* 0x9B */ "\xC2\xA2", /* 0x9C */ "\xC2\xA3", /* 0x9D */ "\xC2\xA5",
        /* 0x9E */ "\xE2\x82\xA7", /* 0x9F */ "\xC6\x92", /* 0xA0 */ "\xC3\xA1", /* 0xA1 */ "\xC3\xAD", /* 0xA2 */ "\xC3\xB3", /* 0xA3 */ "\xC3\xBA", /* 0xA4 */ "\xC3\xB1", /* 0xA5 */ "\xC3\x91", /* 0xA6 */ "\xC2\xAA", /* 0xA7 */ "\xC2\xBA",
        /* 0xA8 */ "\xC2\xBF", /* 0xA9 */ "\xE2\x8C\x90", /* 0xAA */ "\xC2\xAC", /* 0xAB */ "\xC2\xBD", /* 0xAC */ "\xC2\xBC", /* 0xAD */ "\xC2\xA1", /* 0xAE */ "\xC2\xAB", /* 0xAF */ "\xC2\xBB", /* 0xB0 */ "\xE2\x96\x91", /* 0xB1 */ "\xE2\x96\x92",
        /* 0xB2 */ "\xE2\x96\x93", /* 0xB3 */ "\xE2\x94\x82", /* 0xB4 */ "\xE2\x94\xA4", /* 0xB5 */ "\xE2\x95\xA1", /* 0xB6 */ "\xE2\x95\xA2", /* 0xB7 */ "\xE2\x95\x96", /* 0xB8 */ "\xE2\x95\x95", /* 0xB9 */ "\xE2\x95\xA3", /* 0xBA */ "\xE2\x95\x91",
        /* 0xBB */ "\xE2\x95\x97", /* 0xBC */ "\xE2\x95\x9D", /* 0xBD */ "\xE2\x95\x9C", /* 0xBE */ "\xE2\x95\x9B", /* 0xBF */ "\xE2\x94\x90", /* 0xC0 */ "\xE2\x94\x94", /* 0xC1 */ "\xE2\x94\xB4", /* 0xC2 */ "\xE2\x94\xAC", /* 0xC3 */ "\xE2\x94\x9C",
        /* 0xC4 */ "\xE2\x94\x80", /* 0xC5 */ "\xE2\x94\xBC", /* 0xC6 */ "\xE2\x95\x9E", /* 0xC7 */ "\xE2\x95\x9F", /* 0xC8 */ "\xE2\x95\x9A", /* 0xC9 */ "\xE2\x95\x94", /* 0xCA */ "\xE2\x95\xA9", /* 0xCB */ "\xE2\x95\xA6", /* 0xCC */ "\xE2\x95\xA0",
        /* 0xCD */ "\xE2\x95\x90", /* 0xCE */ "\xE2\x95\xAC", /* 0xCF */ "\xE2\x95\xA7", /* 0xD0 */ "\xE2\x95\xA8", /* 0xD1 */ "\xE2\x95\xA4", /* 0xD2 */ "\xE2\x95\xA5", /* 0xD3 */ "\xE2\x95\x99", /* 0xD4 */ "\xE2\x95\x98", /* 0xD5 */ "\xE2\x95\x92",
        /* 0xD6 */ "\xE2\x95\x93", /* 0xD7 */ "\xE2\x95\xAB", /* 0xD8 */ "\xE2\x95\xAA", /* 0xD9 */ "\xE2\x94\x98", /* 0xDA */ "\xE2\x94\x8C", /* 0xDB */ "\xE2\x96\x88", /* 0xDC */ "\xE2\x96\x84", /* 0xDD */ "\xE2\x96\x8C", /* 0xDE */ "\xE2\x96\x90",
        /* 0xDF */ "\xE2\x96\x80", /* 0xE0 */ "\xCE\xB1", /* 0xE1 */ "\xC3\x9F", /* 0xE2 */ "\xCE\x93", /* 0xE3 */ "\xCF\x80", /* 0xE4 */ "\xCE\xA3", /* 0xE5 */ "\xCF\x83", /* 0xE6 */ "\xC2\xB5", /* 0xE7 */ "\xCF\x84", /* 0xE8 */ "\xCE\xA6",
        /* 0xE9 */ "\xCE\x98", /* 0xEA */ "\xCE\xA9", /* 0xEB */ "\xCE\xB4", /* 0xEC */ "\xE2\x88\x9E", /* 0xED */ "\xCF\x86", /* 0xEE */ "\xCE\xB5", /* 0xEF */ "\xE2\x88\xA9", /* 0xF0 */ "\xE2\x89\xA1", /* 0xF1 */ "\xC2\xB1", /* 0xF2 */ "\xE2\x89\xA5",
        /* 0xF3 */ "\xE2\x89\xA4", /* 0xF4 */ "\xE2\x8C\xA0", /* 0xF5 */ "\xE2\x8C\xA1", /* 0xF6 */ "\xC3\xB7", /* 0xF7 */ "\xE2\x89\x88", /* 0xF8 */ "\xC2\xB0", /* 0xF9 */ "\xE2\x88\x99", /* 0xFA */ "\xC2\xB7", /* 0xFB */ "\xE2\x88\x9A",
        /* 0xFC */ "\xE2\x81\xBF", /* 0xFD */ "\xC2\xB2", /* 0xFE */ "\xE2\x96\xA0", /* 0xFF */ "\xC2\xA0",
    };
    
    const char* CharConverter::cp852_to_utf8[128] = {
        /* 0x80 */ "\xC3\x87", /* 0x81 */ "\xC3\xBC", /* 0x82 */ "\xC3\xA9", /* 0x83 */ "\xC3\xA2", /* 0x84 */ "\xC3\xA4", /* 0x85 */ "\xC5\xAF", /* 0x86 */ "\xC4\x87", /* 0x87 */ "\xC3\xA7", /* 0x88 */ "\xC5\x82", /* 0x89 */ "\xC3\xAB",
        /* 0x8A */ "\xC5\x90", /* 0x8B */ "\xC5\x91", /* 0x8C */ "\xC3\xAE", /* 0x8D */ "\xC5\xB9", /* 0x8E */ "\xC3\x84", /* 0x8F */ "\xC4\x86", /* 0x90 */ "\xC3\x89", /* 0x91 */ "\xC4\xB9", /* 0x92 */ "\xC4\xBA", /* 0x93 */ "\xC3\xB4",
        /* 0x94 */ "\xC3\xB6", /* 0x95 */ "\xC4\xBD", /* 0x96 */ "\xC4\xBE", /* 0x97 */ "\xC5\x9A", /* 0x98 */ "\xC5\x9B", /* 0x99 */ "\xC3\x96", /* 0x9A */ "\xC3\x9C", /* 0x9B */ "\xC5\xA4", /* 0x9C */ "\xC5\xA5", /* 0x9D */ "\xC5\x81",
        /* 0x9E */ "\xC3\x97", /* 0x9F */ "\xC4\x8D", /* 0xA0 */ "\xC3\xA1", /* 0xA1 */ "\xC3\xAD", /* 0xA2 */ "\xC3\xB3", /* 0xA3 */ "\xC3\xBA", /* 0xA4 */ "\xC4\x84", /* 0xA5 */ "\xC4\x85", /* 0xA6 */ "\xC5\xBD", /* 0xA7 */ "\xC5\xBE",
        /* 0xA8 */ "\xC4\x98", /* 0xA9 */ "\xC4\x99", /* 0xAA */ "\xC2\xAC", /* 0xAB */ "\xC5\xBA", /* 0xAC */ "\xC4\x8C", /* 0xAD */ "\xC5\x9F", /* 0xAE */ "\xC2\xAB", /* 0xAF */ "\xC2\xBB", /* 0xB0 */ "\xE2\x96\x91", /* 0xB1 */ "\xE2\x96\x92",
        /* 0xB2 */ "\xE2\x96\x93", /* 0xB3 */ "\xE2\x94\x82", /* 0xB4 */ "\xE2\x94\xA4", /* 0xB5 */ "\xC3\x81", /* 0xB6 */ "\xC3\x82", /* 0xB7 */ "\xC4\x9A", /* 0xB8 */ "\xC5\x9E", /* 0xB9 */ "\xE2\x95\xA3", /* 0xBA */ "\xE2\x95\x91",
        /* 0xBB */ "\xE2\x95\x97", /* 0xBC */ "\xE2\x95\x9D", /* 0xBD */ "\xC5\xBB", /* 0xBE */ "\xC5\xBC", /* 0xBF */ "\xE2\x94\x90", /* 0xC0 */ "\xE2\x94\x94", /* 0xC1 */ "\xE2\x94\xB4", /* 0xC2 */ "\xE2\x94\xAC", /* 0xC3 */ "\xE2\x94\x9C",
        /* 0xC4 */ "\xE2\x94\x80", /* 0xC5 */ "\xE2\x94\xBC", /* 0xC6 */ "\xC4\x82", /* 0xC7 */ "\xC4\x83", /* 0xC8 */ "\xE2\x95\x9A", /* 0xC9 */ "\xE2\x95\x94", /* 0xCA */ "\xE2\x95\xA9", /* 0xCB */ "\xE2\x95\xA6", /* 0xCC */ "\xE2\x95\xA0",
        /* 0xCD */ "\xE2\x95\x90", /* 0xCE */ "\xE2\x95\xAC", /* 0xCF */ "\xC2\xA4", /* 0xD0 */ "\xC4\x91", /* 0xD1 */ "\xC4\x90", /* 0xD2 */ "\xC4\x8E", /* 0xD3 */ "\xC3\x8B", /* 0xD4 */ "\xC4\x8F", /* 0xD5 */ "\xC5\x87", /* 0xD6 */ "\xC3\x8D",
        /* 0xD7 */ "\xC3\x8E", /* 0xD8 */ "\xC4\x9B", /* 0xD9 */ "\xE2\x94\x98", /* 0xDA */ "\xE2\x94\x8C", /* 0xDB */ "\xE2\x96\x88", /* 0xDC */ "\xE2\x96\x84", /* 0xDD */ "\xC5\xA2", /* 0xDE */ "\xC5\xAE", /* 0xDF */ "\xE2\x96\x80", /* 0xE0 */ "\xC3\x93",
        /* 0xE1 */ "\xC3\x9F", /* 0xE2 */ "\xC3\x94", /* 0xE3 */ "\xC5\x83", /* 0xE4 */ "\xC5\x84", /* 0xE5 */ "\xC5\x88", /* 0xE6 */ "\xC5\xA0", /* 0xE7 */ "\xC5\xA1", /* 0xE8 */ "\xC5\x94", /* 0xE9 */ "\xC3\x9A", /* 0xEA */ "\xC5\x95",
        /* 0xEB */ "\xC5\xB0", /* 0xEC */ "\xC3\xBD", /* 0xED */ "\xC3\x9D", /* 0xEE */ "\xC5\xA3", /* 0xEF */ "\xC2\xB4", /* 0xF0 */ "\xC2\xAD", /* 0xF1 */ "\xCB\x9D", /* 0xF2 */ "\xCB\x9B", /* 0xF3 */ "\xCB\x87", /* 0xF4 */ "\xCB\x98",
        /* 0xF5 */ "\xC2\xA7", /* 0xF6 */ "\xC3\xB7", /* 0xF7 */ "\xC2\xB8", /* 0xF8 */ "\xC2\xB0", /* 0xF9 */ "\xC2\xA8", /* 0xFA */ "\xCB\x99", /* 0xFB */ "\xC5\xB1", /* 0xFC */ "\xC5\x98", /* 0xFD */ "\xC5\x99", /* 0xFE */ "\xE2\x96\xA0",
        /* 0xFF */ "\xC2\xA0",
    };
    
    const char* CharConverter::cp866_to_utf8[128] = {
        /* 0x80 */ "\xD0\x90", /* 0x81 */ "\xD0\x91", /* 0x82 */ "\xD0\x92", /* 0x83 */ "\xD0\x93", /* 0x84 */ "\xD0\x94", /* 0x85 */ "\xD0\x95", /* 0x86 */ "\xD0\x96", /* 0x87 */ "\xD0\x97", /* 0x88 */ "\xD0\x98", /* 0x89 */ "\xD0\x99",
        /* 0x8A */ "\xD0\x9A", /* 0x8B */ "\xD0\x9B", /* 0x8C */ "\xD0\x9C", /* 0x8D */ "\xD0\x9D", /* 0x8E */ "\xD0\x9E", /* 0x8F */ "\xD0\x9F", /* 0x90 */ "\xD0\xA0", /* 0x91 */ "\xD0\xA1", /* 0x92 */ "\xD0\xA2", /* 0x93 */ "\xD0\xA3",
        /* 0x94 */ "\xD0\xA4", /* 0x95 */ "\xD0\xA5", /* 0x96 */ "\xD0\xA6", /* 0x97 */ "\xD0\xA7", /* 0x98 */ "\xD0\xA8", /* 0x99 */ "\xD0\xA9", /* 0x9A */ "\xD0\xAA", /* 0x9B */ "\xD0\xAB", /* 0x9C */ "\xD0\xAC", /* 0x9D */ "\xD0\xAD",
        /* 0x9E */ "\xD0\xAE", /* 0x9F */ "\xD0\xAF", /* 0xA0 */ "\xD0\xB0", /* 0xA1 */ "\xD0\xB1", /* 0xA2 */ "\xD0\xB2", /* 0xA3 */ "\xD0\xB3", /* 0xA4 */ "\xD0\xB4", /* 0xA5 */ "\xD0\xB5", /* 0xA6 */ "\xD0\xB6", /* 0xA7 */ "\xD0\xB7",
        /* 0xA8 */ "\xD0\xB8", /* 0xA9 */ "\xD0\xB9", /* 0xAA */ "\xD0\xBA", /* 0xAB */ "\xD0\xBB", /* 0xAC */ "\xD0\xBC", /* 0xAD */ "\xD0\xBD", /* 0xAE */ "\xD0\xBE", /* 0xAF */ "\xD0\xBF", /* 0xB0 */ "\xE2\x96\x91", /* 0xB1 */ "\xE2\x96\x92",
        /* 0xB2 */ "\xE2\x96\x93", /* 0xB3 */ "\xE2\x94\x82", /* 0xB4 */ "\xE2\x94\xA4", /* 0xB5 */ "\xE2\x95\xA1", /* 0xB6 */ "\xE2\x95\xA2", /* 0xB7 */ "\xE2\x95\x96", /* 0xB8 */ "\xE2\x95\x95", /* 0xB9 */ "\xE2\x95\xA3", /* 0xBA */ "\xE2\x95\x91",
        /* 0xBB */ "\xE2\x95\x97", /* 0xBC */ "\xE2\x95\x9D", /* 0xBD */ "\xE2\x95\x9C", /* 0xBE */ "\xE2\x95\x9B", /* 0xBF */ "\xE2\x94\x90", /* 0xC0 */ "\xE2\x94\x94", /* 0xC1 */ "\xE2\x94\xB4", /* 0xC2 */ "\xE2\x94\xAC", /* 0xC3 */ "\xE2\x94\x9C",
        /* 0xC4 */ "\xE2\x94\x80", /* 0xC5 */ "\xE2\x94\xBC", /* 0xC6 */ "\xE2\x95\x9E", /* 0xC7 */ "\xE2\x95\x9F", /* 0xC8 */ "\xE2\x95\x9A", /* 0xC9 */ "\xE2\x95\x94", /* 0xCA */ "\xE2\x95\xA9", /* 0xCB */ "\xE2\x95\xA6", /* 0xCC */ "\xE2\x95\xA0",
        /* 0xCD */ "\xE2\x95\x90", /* 0xCE */ "\xE2\x95\xAC", /* 0xCF */ "\xE2\x95\xA7", /* 0xD0 */ "\xE2\x95\xA8", /* 0xD1 */ "\xE2\x95\xA4", /* 0xD2 */ "\xE2\x95\xA5", /* 0xD3 */ "\xE2\x95\x99", /* 0xD4 */ "\xE2\x95\x98", /* 0xD5 */ "\xE2\x95\x92",
        /* 0xD6 */ "\xE2\x95\x93", /* 0xD7 */ "\xE2\x95\xAB", /* 0xD8 */ "\xE2\x95\xAA", /* 0xD9 */ "\xE2\x94\x98", /* 0xDA */ "\xE2\x94\x8C", /* 0xDB */ "\xE2\x96\x88", /* 0xDC */ "\xE2\x96\x84", /* 0xDD */ "\xE2\x96\x8C", /* 0xDE */ "\xE2\x96\x90",
        /* 0xDF */ "\xE2\x96\x80", /* 0xE0 */ "\xD1\x80", /* 0xE1 */ "\xD1\x81", /* 0xE2 */ "\xD1\x82", /* 0xE3 */ "\xD1\x83", /* 0xE4 */ "\xD1\x84", /* 0xE5 */ "\xD1\x85", /* 0xE6 */ "\xD1\x86", /* 0xE7 */ "\xD1\x87", /* 0xE8 */ "\xD1\x88",
        /* 0xE9 */ "\xD1\x89", /* 0xEA */ "\xD1\x8A", /* 0xEB */ "\xD1\x8B", /* 0xEC */ "\xD1\x8C", /* 0xED */ "\xD1\x8D", /* 0xEE */ "\xD1\x8E", /* 0xEF */ "\xD1\x8F", /* 0xF0 */ "\xD0\x81", /* 0xF1 */ "\xD1\x91", /* 0xF2 */ "\xD0\x84",
        /* 0xF3 */ "\xD1\x94", /* 0xF4 */ "\xD0\x87", /* 0xF5 */ "\xD1\x97", /* 0xF6 */ "\xD0\x8E", /* 0xF7 */ "\xD1\x9E", /* 0xF8 */ "\xC2\xB0", /* 0xF9 */ "\xE2\x88\x99", /* 0xFA */ "\xC2\xB7", /* 0xFB */ "\xE2\x88\x9A", /* 0xFC */ "\xE2\x84\x96",
        /* 0xFD */ "\xC2\xA4", /* 0xFE */ "\xE2\x96\xA0", /* 0xFF */ "\xC2\xA0",
    };
    
    const char* CharConverter::cp737_to_utf8[128] = {
        /* 0x80 */ "\xCE\x91", /* 0x81 */ "\xCE\x92", /* 0x82 */ "\xCE\x93", /* 0x83 */ "\xCE\x94", /* 0x84 */ "\xCE\x95", /* 0x85 */ "\xCE\x96", /* 0x86 */ "\xCE\x97", /* 0x87 */ "\xCE\x98", /* 0x88 */ "\xCE\x99", /* 0x89 */ "\xCE\x9A",
        /* 0x8A */ "\xCE\x9B", /* 0x8B */ "\xCE\x9C", /* 0x8C */ "\xCE\x9D", /* 0x8D */ "\xCE\x9E", /* 0x8E */ "\xCE\x9F", /* 0x8F */ "\xCE\xA0", /* 0x90 */ "\xCE\xA1", /* 0x91 */ "\xCE\xA3", /* 0x92 */ "\xCE\xA4", /* 0x93 */ "\xCE\xA5",
        /* 0x94 */ "\xCE\xA6", /* 0x95 */ "\xCE\xA7", /* 0x96 */ "\xCE\xA8", /* 0x97 */ "\xCE\xA9", /* 0x98 */ "\xCE\xB1", /* 0x99 */ "\xCE\xB2", /* 0x9A */ "\xCE\xB3", /* 0x9B */ "\xCE\xB4", /* 0x9C */ "\xCE\xB5", /* 0x9D */ "\xCE\xB6",
        /* 0x9E */ "\xCE\xB7", /* 0x9F */ "\xCE\xB8", /* 0xA0 */ "\xCE\xB9", /* 0xA1 */ "\xCE\xBA", /* 0xA2 */ "\xCE\xBB", /* 0xA3 */ "\xCE\xBC", /* 0xA4 */ "\xCE\xBD", /* 0xA5 */ "\xCE\xBE", /* 0xA6 */ "\xCE\xBF", /* 0xA7 */ "\xCF\x80",
        /* 0xA8 */ "\xCF\x81", /* 0xA9 */ "\xCF\x83", /* 0xAA */ "\xCF\x82", /* 0xAB */ "\xCF\x84", /* 0xAC */ "\xCF\x85", /* 0xAD */ "\xCF\x86", /* 0xAE */ "\xCF\x87", /* 0xAF */ "\xCF\x88", /* 0xB0 */ "\xE2\x96\x91", /* 0xB1 */ "\xE2\x96\x92",
        /* 0xB2 */ "\xE2\x96\x93", /* 0xB3 */ "\xE2\x94\x82", /* 0xB4 */ "\xE2\x94\xA4", /* 0xB5 */ "\xE2\x95\xA1", /* 0xB6 */ "\xE2\x95\xA2", /* 0xB7 */ "\xE2\x95\x96", /* 0xB8 */ "\xE2\x95\x95", /* 0xB9 */ "\xE2\x95\xA3", /* 0xBA */ "\xE2\x95\x91",
        /* 0xBB */ "\xE2\x95\x97", /* 0xBC */ "\xE2\x95\x9D", /* 0xBD */ "\xE2\x95\x9C", /* 0xBE */ "\xE2\x95\x9B", /* 0xBF */ "\xE2\x94\x90", /* 0xC0 */ "\xE2\x94\x94", /* 0xC1 */ "\xE2\x94\xB4", /* 0xC2 */ "\xE2\x94\xAC", /* 0xC3 */ "\xE2\x94\x9C",
        /* 0xC4 */ "\xE2\x94\x80", /* 0xC5 */ "\xE2\x94\xBC", /* 0xC6 */ "\xE2\x95\x9E", /* 0xC7 */ "\xE2\x95\x9F", /* 0xC8 */ "\xE2\x95\x9A", /* 0xC9 */ "\xE2\x95\x94", /* 0xCA */ "\xE2\x95\xA9", /* 0xCB */ "\xE2\x95\xA6", /* 0xCC */ "\xE2\x95\xA0",
        /* 0xCD */ "\xE2\x95\x90", /* 0xCE */ "\xE2\x95\xAC", /* 0xCF */ "\xE2\x95\xA7", /* 0xD0 */ "\xE2\x95\xA8", /* 0xD1 */ "\xE2\x95\xA4", /* 0xD2 */ "\xE2\x95\xA5", /* 0xD3 */ "\xE2\x95\x99", /* 0xD4 */ "\xE2\x95\x98", /* 0xD5 */ "\xE2\x95\x92",
        /* 0xD6 */ "\xE2\x95\x93", /* 0xD7 */ "\xE2\x95\xAB", /* 0xD8 */ "\xE2\x95\xAA", /* 0xD9 */ "\xE2\x94\x98", /* 0xDA */ "\xE2\x94\x8C", /* 0xDB */ "\xE2\x96\x88", /* 0xDC */ "\xE2\x96\x84", /* 0xDD */ "\xE2\x96\x8C", /* 0xDE */ "\xE2\x96\x90",
        /* 0xDF */ "\xE2\x96\x80", /* 0xE0 */ "\xCF\x89", /* 0xE1 */ "\xCE\xAC", /* 0xE2 */ "\xCE\xAD", /* 0xE3 */ "\xCE\xAE", /* 0xE4 */ "\xCF\x8A", /* 0xE5 */ "\xCE\xAF", /* 0xE6 */ "\xCF\x8C", /* 0xE7 */ "\xCF\x8D", /* 0xE8 */ "\xCF\x8B",
        /* 0xE9 */ "\xCF\x8E", /* 0xEA */ "\xCE\x86", /* 0xEB */ "\xCE\x88", /* 0xEC */ "\xCE\x89", /* 0xED */ "\xCE\x8A", /* 0xEE */ "\xCE\x8C", /* 0xEF */ "\xCE\x8E", /* 0xF0 */ "\xCE\x8F", /* 0xF1 */ "\xC2\xB1", /* 0xF2 */ "\xE2\x89\xA5",
        /* 0xF3 */ "\xE2\x89\xA4", /* 0xF4 */ "\xCE\xAA", /* 0xF5 */ "\xCE\xAB", /* 0xF6 */ "\xC3\xB7", /* 0xF7 */ "\xE2\x89\x88", /* 0xF8 */ "\xC2\xB0", /* 0xF9 */ "\xE2\x88\x99", /* 0xFA */ "\xC2\xB7", /* 0xFB */ "\xE2\x88\x9A", /* 0xFC */ "\xE2\x81\xBF",
        /* 0xFD */ "\xC2\xB2", /* 0xFE */ "\xE2\x96\xA0", /* 0xFF */ "\xC2\xA0",
    };
}
