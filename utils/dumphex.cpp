/*
 * Copyright (c) 2013 Devin Smith <devin@devinsmith.net>
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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dumphex.h"

void
dump_hex(int lvl, void *vp, size_t len)
{
	char linebuf[80];
	size_t i;
	int linebuf_dirty = 0;
	unsigned char *p = (unsigned char *)vp;

	memset(linebuf, ' ', sizeof(linebuf));
	linebuf[70] = '\0';

	for (i = 0; i < len; i++) {
		int x = i % 16;
		int ch = (unsigned)p[i];
		char hex[20];

		if (x >= 8)
			x = x * 3 + 1;
		else
			x = x * 3;
		snprintf(hex, sizeof(hex), "%02x", ch);
		linebuf[x] = hex[0];
		linebuf[x + 1] = hex[1];

		if (isprint(ch))
			linebuf[52 + (i % 16)] = ch;
		else
			linebuf[52 + (i % 16)] = '.';

		linebuf_dirty = 1;
		if (!((i + 1) % 16)) {
			fprintf(stderr, "%s\n", linebuf);
			memset(linebuf, ' ', sizeof(linebuf));
			linebuf[70] = '\0';
			linebuf_dirty = 0;
		}
	}
	if (linebuf_dirty == 1)
		fprintf(stderr, "%s\n", linebuf);
}
