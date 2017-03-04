/*
 * Copyright (c) 2017 Devin Smith <devin@devinsmith.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <cstdio>
#include <cstring>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <QvdFile.h>
#include <utils/dumphex.h>

bool QvdFile::Load(const char *filename)
{
  _fp = fopen(filename, "rb");
  if (_fp == NULL) {
    return false;
  }

  if (!parseXmlHeader(filename)) {
    return false;
  }

  if (_eof && _bufLen == 0) {
    // XML header parsed but missing symbol table and data.
    return false;
  }

  // The start of the dataPtr might have the ending \r and \n from
  // the XML portion, if so skip past that.
  char c;
  while (_bufLen > 0) {
    c = peekByte();
    if (c == '\r' || c == '\n' || c == '\0') {
      advanceBytes(1);
      continue;
    }
    break;
  }

  if (_eof && _bufLen == 0) {
    // XML header parsed but missing symbol table and data.
    return false;
  }

  fprintf(stdout, "%zu bytes left\n", _bufLen);

  parseSymbolAndData();

  // First 2 bytes seem to store a type:
  // 00 01 - INT
  // 00 04 - Text
  // 00 05 - Dual INT


  //dump_hex(0, _dataPtrStart, _bufLen);

  // Read rest of file.
  while (!_eof) {
    size_t len = readBytes();

    printf("Read %zu bytes.\n", len);
  }

  fclose(_fp);

  return true;
}

bool QvdFile::parseSymbolAndData()
{
  for (std::vector<QvdField>::iterator it = _hdr.Fields.begin();
      it != _hdr.Fields.end(); ++it) {
    printf("Parsing field: %s, need to read %d symbols\n",
      it->FieldName.c_str(), it->NoOfSymbols);

    for (unsigned int i = 0; i < it->NoOfSymbols; i++) {
      unsigned char typeByte = static_cast<unsigned char>(readByte());
      QvdSymbol symbol;

      switch (typeByte) {
      case 0x01: // INT (4 bytes)
        symbol.IntValue = readInt32();
        break;
//      case 0x02: // Date value (8 bytes)
      default:
        printf("Unknown byte: 0x%02x\n", typeByte);
      }

      it->Symbols.push_back(symbol);
    }
  }
  return true;
}

bool QvdFile::parseXmlHeader(const char *filename)
{
  const char *endQvdTableHeader = "</QvdTableHeader>";
  xmlParserCtxtPtr ctxt;

  ctxt = xmlCreatePushParserCtxt(NULL, NULL, NULL, 0, filename);

  // Read XML content of the file.
  int done;
  do {
    size_t len = readBytes();
    done = len < sizeof(_buf); // Did we reach end of file?

    // Unfortunately the Qvd file format includes an XML based
    // header followed by raw binary data.
    // We know that it occurs after </QvdTableHeader> so we need
    // to scan our read buffer each time to see if we have that tag.
    //
    // If we do, we alter the length read, set a pointer to the data just
    // after the tag and proceed to finish reading the XML portion of the
    // file.

    char *end;
    if ((end = strstr(_buf, endQvdTableHeader)) != NULL) {
      end += strlen(endQvdTableHeader);
      // For the XML portion, the dataPtrStart marks the end of the XML
      // and the start of raw data to be parsed later on.
      //
      // We treat this as an "End of File" situation for expat.
      _dataPtrStart = end;
      _bufLen = len;
      len = end - _buf;
      _bufLen -= len;
      done = 1;
    }

    if (xmlParseChunk(ctxt, _buf, len, done) != 0) {
      fprintf(stderr, "An error occurred parsing the XML\n");
      return false;
#if 0
      fprintf(stderr, "%s at line %lu\n",
        XML_ErrorString(XML_GetErrorCode(parser)),
        XML_GetCurrentLineNumber(parser));
#endif
    }
  } while (!done);

  if (ctxt->myDoc == NULL || !ctxt->wellFormed) {
    // XML header is not well formed.
    return false;
  }

  xmlDoc *doc = ctxt->myDoc; /* the resulting document tree */
  xmlNode *root_element = xmlDocGetRootElement(doc);

  for (xmlNode *node = root_element; node; node = node->next) {
    if (node->type == XML_ELEMENT_NODE &&
        strcmp((char *)node->name, "QvdTableHeader") == 0) {
      _hdr.ParseXml(node);
    }
  }

  xmlFreeDoc(doc);
  xmlFreeParserCtxt(ctxt);
  return true;
}

size_t QvdFile::readBytes()
{
  size_t len = fread(_buf, 1, sizeof(_buf), _fp);
  if (len < sizeof(_buf)) {
    _eof = true;
  }
  _dataPtrStart = _buf;
  _bufLen = len;
  return len;
}

char QvdFile::readByte()
{
  char c;

  if (_bufLen == 0) {
    if (!_eof) {
      readBytes();
    } else {
      return 0; // throw an error.
    }
  }

  _bufLen--;
  c = *_dataPtrStart++;

  return c;
}

int QvdFile::readInt32()
{
  int c;

  c = readByte();
  c += readByte() << 8;
  c += readByte() << 16;
  c += readByte() << 24;

  return c;
}

char QvdFile::peekByte()
{
  char c;

  if (_bufLen == 0) {
    if (!_eof) {
      readBytes();
    } else {
      return 0; // throw an error.
    }
  }

  c = *_dataPtrStart;

  return c;
}

void QvdFile::advanceBytes(size_t nBytes)
{
  if (_bufLen < nBytes) {
    if (!_eof) {
      readBytes();
    } else {
      return; // throw an error.
    }
  }

  if (_bufLen < nBytes) {
    return; // throw an error.
  }

  _bufLen -= nBytes;
  _dataPtrStart += nBytes;
}
