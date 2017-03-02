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

#ifndef __QVDFILE_H__
#define __QVDFILE_H__

#include <cstdio>
#include <string>
#include <vector>
#include <expat.h>

#include <LineageInfo.h>
#include <QvdField.h>
#include <QvdTableHeader.h>

class QvdFile {
public:
  QvdFile() : _state(0), _prevState(0), _lastFieldIndex(0), _fp(NULL),
    _dataPtrStart(NULL), _bufLen(0), _eof(false) {}
  bool Load(const char *filename);

  void startElement(const XML_Char *name, const XML_Char **attrs);
  void endElement(const XML_Char *name);
  void charData(const XML_Char *s, int len);

  size_t NumFields() { return _fields.size(); }

private:
  bool parseXmlHeader();
  bool parseSymbolAndData();

  size_t readBytes();
  char peekByte();
  char readByte();
  int readInt32();
  void advanceBytes(size_t nBytes);

  QvdTableHeader _hdr;
  std::vector<QvdField> _fields;
  std::vector<QvdLineageInfo> _lineages;

  int _state;
  int _prevState;
  unsigned int _lastFieldIndex;
  std::string _currentTag;
  std::string _data;

  // Move to another class?
  FILE *_fp;
  char _buf[BUFSIZ];
  char *_dataPtrStart;
  size_t _bufLen;
  bool _eof;
};

#endif /* __QVDFILE_H__ */

