#include <cairo/cairo.h>
#include "storage.h"

void designBackground(cairo_t *context, int width, int height);

void designXAxis(cairo_t *context, int width, int height);

void designYAxis(cairo_t *context, int width, int height);

void designDot(cairo_t *context,Graphic * graphic, int x, int y);

void designPoints(cairo_t * context, Graphic * graphic, int Xorigem, int Yorigem);

void designLines(cairo_t * context, Graphic * graphic, int Xorigem, int Yorigem);

void designArea(cairo_t * context, Graphic * graphic, int Yorigem);

void designText(cairo_t * context, Graphic * graphic, int height, int width);

void createGraphic(char *fileName);
