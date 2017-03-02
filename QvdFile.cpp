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

#include <expat.h>

#include <QvdFile.h>
#include <utils/dumphex.h>

static void XMLCALL
charDataProc(void *userData, const XML_Char *s, int len)
{
  QvdFile *file = static_cast<QvdFile *>(userData);

  if (file != NULL) {
    file->charData(s, len);
  }

}

static void XMLCALL
startElementProc(void *userData, const XML_Char *name, const XML_Char **attrs)
{
  QvdFile *file = static_cast<QvdFile *>(userData);

  if (file != NULL) {
    file->startElement(name, attrs);
  }

}

static void XMLCALL
endElementProc(void *userData, const XML_Char *name)
{
  QvdFile *file = static_cast<QvdFile *>(userData);

  if (file != NULL) {
    file->endElement(name);
  }
}

static bool
blankStr(const char *str, int len)
{
  int i = 0;
  const char *p = str;
  for (; i < len; p++, i++) {
    if (*p != ' ' && *p != '\t' && *p != '\n' && *p != '\r')
      return false;
  }
  return true;
}

enum FileTagState {
  Unknown = 0,
  QvdTableHeader = 1,
  Fields = 2,
  QvdFieldHeader = 3,
  NumberFormat = 4,
  Lineage = 5,
  LineageInfo = 6
};

void QvdFile::charData(const XML_Char *data, int len)
{
  // Did expat give us all whitespace ?
  if (blankStr(data, len)) {
    return;
  }

  switch (_state) {
  case QvdTableHeader:
    _hdr.ReadTag(_currentTag, data, len);
    break;
  case QvdFieldHeader:
  case NumberFormat: {
    QvdField *field = &_fields.back();
    field->ReadTag(_currentTag, data, len);
    break;
  }
  case LineageInfo: {
    QvdLineageInfo *lineage = &_lineages.back();
    lineage->ReadTag(_currentTag, data, len);
    break;
  }
  default:
    printf("Unhandled state: %d - %s\n", _state, _currentTag.c_str());
    break;
  }
}

void QvdFile::endElement(const XML_Char *name)
{
  if (strcmp(name, "QvdTableHeader") == 0) {
    _state = Unknown;
  } else if (strcmp(name, "Fields") == 0) {
    _state = QvdTableHeader;
  } else if (strcmp(name, "QvdFieldHeader") == 0) {
    _state = Fields;
  } else if (strcmp(name, "NumberFormat") == 0) {
    _state = QvdFieldHeader;
  } else if (strcmp(name, "Lineage") == 0) {
    _state = QvdTableHeader;
  } else if (strcmp(name, "LineageInfo") == 0) {
    _state = Lineage;
  }
}

void QvdFile::startElement(const XML_Char *name, const XML_Char **attrs)
{
  if (strcmp(name, "QvdTableHeader") == 0) {
    _state = QvdTableHeader;
  } else if (strcmp(name, "Fields") == 0) {
    _state = Fields;
  } else if (strcmp(name, "QvdFieldHeader") == 0) {
    _state = QvdFieldHeader;
    QvdField field;
    _fields.push_back(field);
  } else if (strcmp(name, "NumberFormat") == 0) {
    _state = NumberFormat;
  } else if (strcmp(name, "Lineage") == 0) {
    _state = Lineage;
  } else if (strcmp(name, "LineageInfo") == 0) {
    _state = LineageInfo;
    QvdLineageInfo info;
    _lineages.push_back(info);
  }

  _currentTag = name;
}

bool QvdFile::Load(const char *filename)
{
  _fp = fopen(filename, "rb");
  if (_fp == NULL) {
    return false;
  }

  _state = _prevState = Unknown;
  if (!parseXmlHeader()) {
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

  // First 2 bytes seem to store a type:
  // 00 01 - INT
  // 00 04 - Text
  // 00 05 - Dual INT


  dump_hex(0, _dataPtrStart, _bufLen);

  // Read rest of file.
  while (!_eof) {
    size_t len = readBytes();

    printf("Read %zu bytes.\n", len);
  }

  fclose(_fp);

  return true;
}

bool QvdFile::parseXmlHeader()
{
  const char *endQvdTableHeader = "</QvdTableHeader>";

  XML_Parser parser = XML_ParserCreate(NULL);

  XML_SetUserData(parser, this);
  XML_SetElementHandler(parser, startElementProc, endElementProc);
  XML_SetCharacterDataHandler(parser, charDataProc);

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

    if (XML_Parse(parser, _buf, len, done) == XML_STATUS_ERROR) {
      fprintf(stderr, "%s at line %lu\n",
        XML_ErrorString(XML_GetErrorCode(parser)),
        XML_GetCurrentLineNumber(parser));
      return false;
    }
  } while (!done);

  XML_ParserFree(parser);
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
