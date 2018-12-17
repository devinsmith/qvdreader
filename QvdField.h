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

#ifndef __QVD_FIELD_H__
#define __QVD_FIELD_H__

#include <string>
#include <vector>

#include <libxml/tree.h>

#include "QvdSymbol.h"

struct QvdField {
  QvdField() : BitOffset(0), BitWidth(0), Bias(0), nDec(0), UseThou(0),
    NoOfSymbols(0), Offset(0), Length(0) { }

  void ParseNumberFormat(const xmlNode *node);
  void ParseXml(const xmlNode *node);

  std::string FieldName;
  unsigned int BitOffset;
  unsigned int BitWidth;
  int Bias;

  // NumberFormat stuff
  std::string Type;
  unsigned int nDec; // Number of decimals?
  unsigned int UseThou; // Use Thousands
  std::string Dec; // Decimal format?
  std::string Thou; // Thousand's format?

  unsigned int NoOfSymbols;
  unsigned int Offset;
  unsigned int Length;

  std::vector<QvdSymbol> Symbols;
};

#endif /* __QVD_FIELD_H__ */
