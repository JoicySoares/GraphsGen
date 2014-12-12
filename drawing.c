#include "drawing.h"
#include<string.h>
#include<cairo/cairo-pdf.h>
#include <cairo/cairo.h>

#define PI 3.1416

//Esta função cria a imagem de fundo (É a "folha" onde será desenhado o gráfico)
void designBackground(cairo_t *context, int width, int height) {
    cairo_save(context);
    cairo_rectangle (context, 0, 0, width, height);
	cairo_set_source_rgba(context,255,255,255,255);
	cairo_fill(context);
    cairo_restore(context);
}

//Desenha o Eixo X 
void designXAxis(cairo_t *context, int width, int height) {
    cairo_save(context);
	cairo_set_line_width(context, 3);
	cairo_set_line_cap(context, CAIRO_LINE_CAP_ROUND);
	cairo_move_to(context, width/10, height - (height/10)); 
	cairo_line_to(context, width - (width/10), height - (height/10));
	/* A partir daqui ,serão desenhadas as setas deste eixo!*/
	cairo_move_to(context, width - (width/12), height - (height/10));
	cairo_line_to(context, width - (width/10), height - (height/8));
	cairo_move_to(context, width - (width/12), height - (height/10));
	cairo_line_to(context, width - (width/10), height - (height/12));
	cairo_line_to(context, width - (width/10), height - (height/8));
	cairo_set_source_rgba(context,0,0,0,255);
	cairo_stroke(context);
    cairo_restore(context);
}

void designYAxis(cairo_t *context, int width, int height) {
    cairo_save(context);
	cairo_set_line_width(context, 3);
	cairo_set_line_cap(context, CAIRO_LINE_CAP_ROUND);
	cairo_move_to(context, width/10, height/10);
	cairo_line_to(context, width/10, height - (height/10));
	/* A partir daqui ,serão desenhadas as setas deste eixo!*/
	cairo_move_to(context, width/10, height/12);
	cairo_line_to(context, width/12, height/10);
	cairo_move_to(context, width/10, height/12); 
	cairo_line_to(context, width/8.5, height/10);
	cairo_line_to(context, width/12, height/10);
	cairo_set_source_rgba(context,0,0,0,255);
	cairo_stroke(context);
    cairo_restore(context);
}

//Essa função é responsável pelo texto!
void designText (cairo_t * context, Graphic * graphic, int height, int width){
 	cairo_set_source_rgba(context,0,0,0,255); 

  	cairo_select_font_face(context, "Purisa",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_BOLD);

  	cairo_set_font_size(context, width/25);
	cairo_move_to(context, width - (width/2), height/20);
  	cairo_show_text(context, graphic->name);
  	cairo_select_font_face(context, "Times New Roman",CAIRO_FONT_SLANT_ITALIC,CAIRO_FONT_WEIGHT_BOLD);
  	cairo_set_font_size(context, width/40);
    cairo_move_to(context, width - (width/8), height - (height/13));
	cairo_show_text(context, "x"); 
    cairo_set_font_size(context, width/40);
  	cairo_move_to(context, width/14, height/8);
    cairo_show_text(context, "y"); 
    cairo_set_font_size(context, width/40);
	cairo_move_to(context, width/12, height - (height/12)); 
    cairo_show_text(context, "0"); 

}
void designDot(cairo_t *context,Graphic * graphic, int x, int y) {
    cairo_set_source_rgba(context,graphic->color[0],graphic->color[1],graphic->color[2],graphic->color[3]);
    cairo_arc (context,x,y,3,0,2*PI);
    cairo_fill(context);
}
//Essa função pega os pontos de Graphic e os desenha,para isso utilizza a função designDot !
void designPoints (cairo_t * context, Graphic * graphic, int Xorigem, int Yorigem){
	cairo_save(context);
	static const double dashed[] = {4.0};
	int i;
	//Os dois loops são responsáveis por ligar os ontos aos eixos x e y !
    for(i = 0; i< graphic->sizeContent; i++){
    	cairo_move_to(context, Xorigem, graphic->content[i].y);
    	cairo_line_to(context, graphic->content[i].x, graphic->content[i].y);	
    }
    for (i = 0; i < graphic->sizeContent;i++){
    	cairo_move_to(context, graphic->content[i].x, graphic->content[i].y);
    	cairo_line_to(context, graphic->content[i].x, Yorigem);
    }
   	cairo_set_line_width(context, 1.5);
   	cairo_set_dash(context, dashed, 1, 0);
   	cairo_set_source_rgba(context, 0,  0,  0,  255); 
	cairo_stroke(context);
	//Aqui são desenhados os pontos
   	for (i = 0; i < graphic->sizeContent; i++){
   		designDot(context,graphic, graphic->content[i].x, graphic->content[i].y);
   	}     	
   	cairo_set_source_rgba(context,graphic->color[0],graphic->color[1],graphic->color[2],graphic->color[3]);   	    	
	cairo_stroke(context);   	
   	cairo_restore(context);   
}

//Aqui está a função será responsável por criar o gráfico de linhas.Ela também usará a função designDot pra mostrar a localização dos pontos
void designLines(cairo_t * context, Graphic * graphic, int Xorigem, int Yorigem){
	cairo_save(context);
	static const double dashed[] = {1.0};
	int j;
    cairo_set_line_width(context, 2);  //Espessura da linha
    cairo_set_dash(context, dashed, 0, 0); //Afastamento
    for (j = 0; j < graphic->sizeContent; j++){  	
   		if (j == 0) {
   			cairo_move_to(context, Xorigem, Yorigem);
   		} 
   		cairo_line_to(context, graphic->content[j].x, graphic->content[j].y);
   	}
    cairo_set_source_rgba(context,graphic->color[0],graphic->color[1],graphic->color[2],graphic->color[3]);
    cairo_stroke(context);
    for (j = 0; j < graphic->sizeContent; j++){
    	designDot(context,graphic, graphic->content[j].x, graphic->content[j].y);
    }   
    cairo_restore(context);
}
//esta função é responsável por criar o gráfico de área
void designArea(cairo_t * context, Graphic * graphic, int Yorigem){
	cairo_save(context);
	int count;
    for (count = 0; count < graphic->sizeContent; count++){	
    	if (count == 0) {
    		cairo_move_to(context, graphic->content[count].x, graphic->content[count].y);
    	} else if  (count == graphic->sizeContent - 1) {
    		cairo_line_to(context, graphic->content[count].x, graphic->content[count].y);
    		cairo_line_to(context, graphic->content[count].x, Yorigem);
    		cairo_line_to(context, graphic->content[0].x, Yorigem);
    		cairo_move_to(context, graphic->content[0].x, graphic->content[0].y);
    	} else {
    		cairo_line_to(context, graphic->content[count].x, graphic->content[count].y);
    	}		
    }
	cairo_set_source_rgba(context,graphic->color[0],graphic->color[1],graphic->color[2],graphic->color[3]);
    cairo_fill(context);
    cairo_restore(context);
}

//Vamos criar o Gráfico !
void createGraphic(char *fileName){
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


}
