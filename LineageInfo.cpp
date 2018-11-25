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

#include "LineageInfo.h"

void QvdLineageInfo::ParseXml(const xmlNode *node)
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

      if (strcmp((char *)child->name, "Discriminator") == 0) {
        Discriminator = (char *)nodeContent;
      } else if (strcmp((char *)child->name, "Statement") == 0) {
        Statement = (char *)nodeContent;
      } else {
        printf("Unprocessed LineageInfo tag: %s\n", child->name);
      }
      xmlFree(nodeContent);
    }
  }
}
