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

#include <cstring>

#include "QvdTableHeader.h"
#include "utils/conversions.h"

void QvdTableHeader::ParseFields(const xmlNode *node)
{
  for (const xmlNode *child = node->children; child; child = child->next) {
    if (child->type == XML_ELEMENT_NODE) {
      xmlChar *nodeContent = xmlNodeGetContent(child);
      if (nodeContent == NULL)
        continue;

      if (strlen((char *)nodeContent) == 0) {
        xmlFree(nodeContent);
        continue;
      }

      if (strcmp((char *)child->name, "QvdFieldHeader") == 0) {
        QvdField field;
        field.ParseXml(child);

        Fields.push_back(field);
      } else {
        printf("Unprocessed Fields tag: %s\n", child->name);
      }
      xmlFree(nodeContent);
    }
  }
}

void QvdTableHeader::ParseLineage(const xmlNode *node)
{
  for (const xmlNode *child = node->children; child; child = child->next) {
    if (child->type == XML_ELEMENT_NODE) {
      xmlChar *nodeContent = xmlNodeGetContent(child);
      if (nodeContent == NULL)
        continue;

      if (strlen((char *)nodeContent) == 0) {
        xmlFree(nodeContent);
        continue;
      }

      if (strcmp((char *)child->name, "LineageInfo") == 0) {
        QvdLineageInfo lin;
        lin.ParseXml(child);

        Lineages.push_back(lin);
      } else {
        printf("Unprocessed Lineage tag: %s\n", child->name);
      }
      xmlFree(nodeContent);
    }
  }
}

void QvdTableHeader::ParseXml(const xmlNode *node)
{
  for (const xmlNode *child = node->children; child; child = child->next) {
    if (child->type == XML_ELEMENT_NODE) {
      xmlChar *nodeContent = xmlNodeGetContent(child);
      if (nodeContent == NULL)
        continue;

      if (strlen((char *)nodeContent) == 0) {
        xmlFree(nodeContent);
        continue;
      }

      if (strcmp((char *)child->name, "QvBuildNo") == 0) {
        QvBuildNo = (char *)nodeContent;
      } else if (strcmp((char *)child->name, "CreatorDoc") == 0) {
        CreatorDoc = (char *)nodeContent;
      } else if (strcmp((char *)child->name, "CreateUtcTime") == 0) {
        CreateUtcTime = (char *)nodeContent;
      } else if (strcmp((char *)child->name, "TableName") == 0) {
        TableName = (char *)nodeContent;
      } else if (strcmp((char *)child->name, "SourceFileSize") == 0) {
        SourceFileSize = atoi((char *)nodeContent);
      } else if (strcmp((char *)child->name, "RecordByteSize") == 0) {
        RecordByteSize = atoi((char *)nodeContent);
      } else if (strcmp((char *)child->name, "NoOfRecords") == 0) {
        NoOfRecords = atoi((char *)nodeContent);
      } else if (strcmp((char *)child->name, "Offset") == 0) {
        Offset = atoi((char *)nodeContent);
      } else if (strcmp((char *)child->name, "Length") == 0) {
        Length = atoi((char *)nodeContent);
      } else if (strcmp((char *)child->name, "Lineage") == 0) {
        ParseLineage(child);
      } else if (strcmp((char *)child->name, "Fields") == 0) {
        ParseFields(child);
      } else {
        printf("Unprocessed QvdTableHeader tag: %s\n", child->name);
      }

      xmlFree(nodeContent);
    }
  }
}
