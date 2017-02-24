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

#include <QvdFile.h>

#include <expat.h>

enum FileTagState {
  Unknown = 0,
  QvdTableHeader = 1,
  Fields = 2,
  QvdFieldHeader = 3,
  NumberFormat = 4,
  Lineage = 5,
  LineageInfo = 6
};

static void XMLCALL
charDataProc(void *userData, const XML_Char *s, int len)
{
}

static void XMLCALL
startElementProc(void *userData, const char *name, const char **attrs)
{
  QvdFile *file = static_cast<QvdFile *>(userData);

  file->_prevState = file->_state;
  if (strcmp(name, "QvdTableHeader") == 0) {
    file->_state = QvdTableHeader;
  } else if (strcmp(name, "Fields") == 0) {
    file->_state = Fields;
  } else if (strcmp(name, "QvdFieldHeader") == 0) {
    file->_state = QvdFieldHeader;
  } else if (strcmp(name, "NumberFormat") == 0) {
    file->_state = NumberFormat;
  } else if (strcmp(name, "Lineage") == 0) {
    file->_state = Lineage;
  } else if (strcmp(name, "LineageInfo") == 0) {
    file->_state = LineageInfo;
  }

  file->_currentTag = name;
  printf("S: %s\n", name);
}

static void XMLCALL
endElementProc(void *userData, const char *name)
{
  QvdFile *file = static_cast<QvdFile *>(userData);

  file->_state = file->_prevState;
  printf("E: %s\n", name);
}

bool QvdFile::Load(const char *filename)
{
  char buf[BUFSIZ];
  const char *endQvdTableHeader = "</QvdTableHeader>";
  char *dataPtrStart;

  FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
    return false;
  }

  XML_Parser parser = XML_ParserCreate(NULL);

  XML_SetUserData(parser, this);
  XML_SetElementHandler(parser, startElementProc, endElementProc);
  XML_SetCharacterDataHandler(parser, charDataProc);

  // Read XML content of the file.
  int done;
  do {
    size_t len = fread(buf, 1, sizeof(buf), fp);
    done = len < sizeof(buf); // Did we reach end of file?

    // Unfortunately the Qvd file format includes an XML based
    // header followed by raw binary data.
    // We know that it occurs after </QvdTableHeader> so we need
    // to scan our read buffer each time to see if we have that tag.
    //
    // If we do, we alter the length read, set a pointer to the data just
    // after the tag and proceed to finish reading the XML portion of the
    // file.

    char *end;
    if ((end = strstr(buf, endQvdTableHeader)) != NULL) {
      end += strlen(endQvdTableHeader);
      dataPtrStart = end;
      len = end - buf;
      done = 1;
    }

    if (XML_Parse(parser, buf, len ,done) == XML_STATUS_ERROR) {
      fprintf(stderr, "%s at line %lu\n",
        XML_ErrorString(XML_GetErrorCode(parser)),
        XML_GetCurrentLineNumber(parser));
      return false;
    }
  } while (!done);

  XML_ParserFree(parser);

  // Parse dataPtr portion.


  fclose(fp);

  return true;
}

