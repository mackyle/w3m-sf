/* $Id: w3mimgdisplay.c,v 1.5 2002/07/18 06:49:39 ukai Exp $ */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "config.h"
#include "w3mimg/w3mimg.h"

w3mimg_op *w_op;
static char *background = NULL;
static int offset_x = 0, offset_y = 0;
static int defined_bg = 0, defined_x = 0, defined_y = 0, defined_test = 0;
static int defined_debug = 0;

#define MAX_IMAGE 1000
static W3MImage *imageBuf = NULL;
static int maxImage = 0;

static void GetOption(int argc, char **argv);
static void DrawImage(char *buf, int redraw);
static void ClearImage(void);

int
main(int argc, char **argv)
{
    int len;
    char buf[1024 + 128];

    GetOption(argc, argv);
    if (!defined_debug)
	fclose(stderr);

    w_op = w3mimg_open();
    if (w_op == NULL)
	exit(1);
    if (defined_x)
	w_op->offset_x = offset_x;
    if (defined_y)
	w_op->offset_y = offset_y;

    if (defined_test) {
	printf("%d %d\n", w_op->width - w_op->offset_x, 
	       w_op->height - w_op->offset_y);
	exit(0);
    }

    w_op->set_background(w_op, background);

    while (fgets(buf, sizeof(buf), stdin) != NULL) {
	if (!(isdigit(buf[0]) && buf[1] == ';')) {
	    fputc('\n', stdout);
	    fflush(stdout);
	    continue;
	}
	len = strlen(buf);
	if (buf[len - 1] == '\n') {
	    buf[--len] = '\0';
	    if (buf[len - 1] == '\r')
		buf[--len] = '\0';
	}
	/*
	 * w3mimg protocol
	 *  0  1  2 ....
	 * +--+--+--+--+ ...... +--+--+
	 * |op|; |args             |\n|
	 * +--+--+--+--+ .......+--+--+
	 *
	 * args is separeted by ';'
	 * op	args
	 *  0;  params		draw image
	 *  1;  params		redraw image
	 *  2;  -none-		clear image
	 *  3;  -none-		sync drawing
	 *  4;  -none-		nop, sync communication
	 *			response '\n'
	 *  5;  path		get size of image,
	 *			response "<width> <height>\n"
	 *
	 * params
	 *	<n>;<x>;<y>;<w>;<h>;<sx>;<sy>;<sw>;<sh>;<path>
	 *   
	 */
	switch (buf[0]) {
	case '0':
	    DrawImage(&buf[2], 0);
	    break;
	case '1':
	    DrawImage(&buf[2], 1);
	    break;
	case '2':
	    ClearImage();
	    break;
	case '3':
	    w_op->sync(w_op);
	    break;
	case '4':
	    fputs("\n", stdout);
	    fflush(stdout);
	    break;
	case '5':
	    if (w_op->init(w_op)) {
		W3MImage img;
		if (w_op->load_image(w_op, &img, &buf[2], 0, 0)) {
		    fprintf(stdout, "%d %d\n", img.width, img.height);
		    fflush(stdout);
		    w_op->free_image(w_op, &img);
		}
	    }
	    break;
	}
    }
    ClearImage();
    w_op->close(w_op);
    exit(0);
}

static void
GetOption(int argc, char **argv)
{
    int i;

    for (i = 1; i < argc; i++) {
	if (!strcmp("-bg", argv[i])) {
	    if (++i >= argc)
		exit(1);
	    background = argv[i];
	    defined_bg = 1;
	}
	else if (!strcmp("-x", argv[i])) {
	    if (++i >= argc)
		exit(1);
	    offset_x = atoi(argv[i]);
	    defined_x = 1;
	}
	else if (!strcmp("-y", argv[i])) {
	    if (++i >= argc)
		exit(1);
	    offset_y = atoi(argv[i]);
	    defined_y = 1;
	}
	else if (!strcmp("-test", argv[i])) {
	    defined_test = 1;
	}
	else if (!strcmp("-debug", argv[i])) {
	    defined_debug = 1;
	}
	else {
	    exit(1);
	}
    }
}

void
DrawImage(char *buf, int redraw)
{
    char *p = buf;
    int n = 0, x = 0, y = 0, w = 0, h = 0, sx = 0, sy = 0, sw = 0, sh = 0;

    if (!p)
	return;
    for (; isdigit(*p); p++)
	n = 10 * n + (*p - '0');
    if (*(p++) != ';')
	return;
    for (; isdigit(*p); p++)
	x = 10 * x + (*p - '0');
    if (*(p++) != ';')
	return;
    for (; isdigit(*p); p++)
	y = 10 * y + (*p - '0');
    if (*(p++) != ';')
	return;
    for (; isdigit(*p); p++)
	w = 10 * w + (*p - '0');
    if (*(p++) != ';')
	return;
    for (; isdigit(*p); p++)
	h = 10 * h + (*p - '0');
    if (*(p++) != ';')
	return;
    for (; isdigit(*p); p++)
	sx = 10 * sx + (*p - '0');
    if (*(p++) != ';')
	return;
    for (; isdigit(*p); p++)
	sy = 10 * sy + (*p - '0');
    if (*(p++) != ';')
	return;
    for (; isdigit(*p); p++)
	sw = 10 * sw + (*p - '0');
    if (*(p++) != ';')
	return;
    for (; isdigit(*p); p++)
	sh = 10 * sh + (*p - '0');
    if (*(p++) != ';')
	return;

    n--;
    if (n < 0 || n >= MAX_IMAGE)
	return;
    if (redraw) {
	if (! w_op->active(w_op) || n >= maxImage || !imageBuf[n].pixmap)
	    return;
	goto draw_image;
    }
    w_op->init(w_op);

    if (n >= maxImage) {
	int i = maxImage;
	maxImage = i ? (i * 2) : 2;
	if (maxImage > MAX_IMAGE)
	    maxImage = MAX_IMAGE;
	else if (n >= maxImage)
	    maxImage = n + 1;
	imageBuf = (W3MImage *) realloc((void *)imageBuf,
				     sizeof(W3MImage) * maxImage);
	for (; i < maxImage; i++)
	    imageBuf[i].pixmap = NULL;
    }
    if (imageBuf[n].pixmap) {
	w_op->free_image(w_op, &imageBuf[n]);
	imageBuf[n].pixmap = NULL;
    }

    w_op->load_image(w_op, &imageBuf[n], p, w, h);

  draw_image:
    w_op->show_image(w_op, &imageBuf[n], sx, sy, sw, sh, x, y);
}

void
ClearImage(void)
{
    w_op->finish(w_op);
    if (imageBuf) {
	int i;
	for (i = 0; i < maxImage; i++) {
	    w_op->free_image(w_op, &imageBuf[i]);
	}
	free(imageBuf);
	imageBuf = NULL;
    }
    maxImage = 0;
}
