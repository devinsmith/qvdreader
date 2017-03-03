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

#include <QvdTableHeader.h>
#include <utils/conversions.h>

void QvdTableHeader::ReadTag(const std::string &currentTag, const char *data,
  int len)
{
  if (currentTag.compare("QvBuildNo") == 0) {
    QvBuildNo.append(data, len);
  } else if (currentTag.compare("CreatorDoc") == 0) {
    CreatorDoc.append(data, len);
  } else if (currentTag.compare("CreateUtcTime") == 0) {
    CreateUtcTime.append(data, len);
  } else if (currentTag.compare("TableName") == 0) {
    TableName.append(data, len);
  } else if (currentTag.compare("SourceFileSize") == 0) {
    SourceFileSize += utils::mem_to_int(data, len);
  } else if (currentTag.compare("RecordByteSize") == 0) {
    RecordByteSize += utils::mem_to_uint(data, len);
  } else if (currentTag.compare("NoOfRecords") == 0) {
    NoOfRecords += utils::mem_to_uint(data, len);
  } else if (currentTag.compare("Offset") == 0) {
    Offset += utils::mem_to_uint(data, len);
  } else if (currentTag.compare("Length") == 0) {
    Length += utils::mem_to_uint(data, len);
  } else {
    printf("Unprocessed QvdTableHeader tag: %s\n", currentTag.c_str());
  }
}
