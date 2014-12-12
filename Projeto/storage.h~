#include <stdio.h>
#include <jansson.h>

struct _point {
    int x;
    int y;
};

typedef struct _point Point;

struct graphic {
    const char *fileName;
    const char *name;
    const char *format;
    int width;
    int height;
    int type;
    int sizeContent;
    int maiorX;
    int maiorY;
    unsigned char color[4];
    Point *content;
    
};

typedef struct graphic Graphic;
    
json_t * openJSONFile(char *fileName);
Graphic *loadGraphic(char *fileName);
int BigY (Graphic * graphic);
int BigX (Graphic * graphic);
void modX (Graphic * graphic);
void modY (Graphic * graphic);
