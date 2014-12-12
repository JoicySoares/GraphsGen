#include "drawing.h"
#include<stdio.h>
#include<cairo/cairo-pdf.h>
#include <cairo/cairo.h>

int main("../test.json") {
     Graphic * graphic = loadGraphic(fileName);
    int width = graphic->width, height = graphic->height, Xorigem= width/10, Yorigem=height - (height/10);
	cairo_surface_t *surface;
    //Aqui comparamos o tipo de arquivo e assim diferenciamos a surface
    if (!(strcmp (graphic->format, "png"))){
		    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height); //passa pra png !
    }
    else if (!(strcmp (graphic->format, "pdf"))){
		    surface = cairo_pdf_surface_create(graphic->fileName,width,height); //passa pra pdf !
    }  
	
    cairo_t *context = cairo_create(surface);
    //Imagem de fundo
    designBackground(context, width, height);
    //Desenha os Eixos 
    designXAxis(context, width, height);
    designYAxis(context, width, height);
    //Função de  Texto	
    designText(context, graphic, width, height);
    
                /*----Escolher o tipo de Gráfico------- */
    
    
    switch(graphic->type){
    	case 0: designPoints(context, graphic, Xorigem, Yorigem); //Grafico de pontos.
    	break;
    	case 1: designLines(context, graphic, Xorigem, Yorigem); // Gráfico com linha 
    	break;
    	case 2: designArea(context, graphic, Yorigem); // Gráfico com área
    	break;
    
    }
    //Aqui realizamos outra comparação, agr para finalizar a passagem para pdf ou png !     
    if (!(strcmp (graphic->format, "png"))){
		cairo_surface_write_to_png(surface, graphic->fileName); //png !
    }
    else if (!(strcmp (graphic->format, "pdf"))){
		cairo_show_page(context); //pdf !
    }
    
    cairo_destroy(context);
    cairo_surface_destroy(surface);


	return 0;
}




