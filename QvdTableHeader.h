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

#ifndef __QVD_TABLEHEADER_H__
#define __QVD_TABLEHEADER_H__

#include <string>
#include <vector>

#include <libxml/tree.h>

#include "LineageInfo.h"
#include "QvdField.h"

struct QvdTableHeader {
  QvdTableHeader() : SourceFileSize(0), RecordByteSize(0), NoOfRecords(0),
    Offset(0), Length(0) { }

  void ParseFields(const xmlNode *node);
  void ParseLineage(const xmlNode *node);
  void ParseXml(const xmlNode *node);

  std::string QvBuildNo;
  std::string CreatorDoc;
  std::string CreateUtcTime;
  int SourceFileSize;
  std::string TableName;
  unsigned int RecordByteSize;
  unsigned int NoOfRecords;
  unsigned int Offset;
  unsigned int Length;

  std::vector<QvdField> Fields;
  std::vector<QvdLineageInfo> Lineages;

  std::vector<int> Indices;
};

#endif /* __QVD_TABLEHEADER_H__ */
