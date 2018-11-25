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

#include "QvdField.h"
#include "utils/conversions.h"

void QvdField::ParseNumberFormat(const xmlNode *node)
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

      if (strcmp((char *)child->name, "Type") == 0) {
        Type = (char *)nodeContent;
      } else if (strcmp((char *)child->name, "Dec") == 0) {
        Dec = (char *)nodeContent;
      } else if (strcmp((char *)child->name, "Thou") == 0) {
        Thou = (char *)nodeContent;
      } else if (strcmp((char *)child->name, "nDec") == 0) {
        nDec = atoi((char *)nodeContent);
      } else if (strcmp((char *)child->name, "UseThou") == 0) {
        UseThou = atoi((char *)nodeContent);
      } else {
        printf("Unprocessed NumberFormat tag: %s\n", child->name);
      }
      xmlFree(nodeContent);
    }
  }
}

void QvdField::ParseXml(const xmlNode *node)
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

      if (strcmp((char *)child->name, "FieldName") == 0) {
        FieldName = (char *)nodeContent;
      } else if (strcmp((char *)child->name, "BitOffset") == 0) {
        BitOffset = atoi((char *)nodeContent);
      } else if (strcmp((char *)child->name, "BitWidth") == 0) {
        BitWidth = atoi((char *)nodeContent);
      } else if (strcmp((char *)child->name, "Bias") == 0) {
        Bias = atoi((char *)nodeContent);
      } else if (strcmp((char *)child->name, "NoOfSymbols") == 0) {
        NoOfSymbols = atoi((char *)nodeContent);

        if (NoOfSymbols > 0) {
          Symbols.reserve(NoOfSymbols);
        }
      } else if (strcmp((char *)child->name, "Offset") == 0) {
        Offset = atoi((char *)nodeContent);
      } else if (strcmp((char *)child->name, "Length") == 0) {
        Length = atoi((char *)nodeContent);
      } else if (strcmp((char *)child->name, "NumberFormat") == 0) {
        ParseNumberFormat(child);
      } else {
        printf("Unprocessed QvdFieldHeader tag: %s\n", child->name);
      }
      xmlFree(nodeContent);
    }
  }
}
