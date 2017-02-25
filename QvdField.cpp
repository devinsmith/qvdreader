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

#include <QvdField.h>
#include <utils/conversions.h>

void QvdField::ReadTag(const std::string &currentTag, const char *data,
  int len)
{
  if (currentTag.compare("FieldName") == 0) {
    FieldName = std::string(data, len);
  } else if (currentTag.compare("BitOffset") == 0) {
    BitOffset = utils::mem_to_uint(data, len);
  } else if (currentTag.compare("BitWidth") == 0) {
    BitWidth = utils::mem_to_uint(data, len);
  } else if (currentTag.compare("Bias") == 0) {
    Bias = utils::mem_to_uint(data, len);
  } else if (currentTag.compare("Type") == 0) {
    Type = std::string(data, len);
  } else if (currentTag.compare("nDec") == 0) {
    nDec = utils::mem_to_uint(data, len);
  } else if (currentTag.compare("UseThou") == 0) {
    UseThou = utils::mem_to_uint(data, len);
  } else if (currentTag.compare("Dec") == 0) {
    Dec = std::string(data, len);
  } else if (currentTag.compare("Thou") == 0) {
    Thou = std::string(data, len);
  } else if (currentTag.compare("NoOfSymbols") == 0) {
    NoOfSymbols = utils::mem_to_uint(data, len);
  } else if (currentTag.compare("Offset") == 0) {
    Offset = utils::mem_to_uint(data, len);
  } else if (currentTag.compare("Length") == 0) {
    Length = utils::mem_to_uint(data, len);
  } else {
    printf("Unprocessed QvdField tag: %s\n", currentTag.c_str());
  }
}
