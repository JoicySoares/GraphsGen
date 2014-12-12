#include "storage.h"

// Abre o arquivo Json E verifica sem tem algum erro.
json_t * openJSONFile(char *fileName) {
    json_t *root = 0;

    json_error_t error;

    root = json_load_file(fileName, 0, &error);
    
    if(!root){
        fprintf(stderr, "FALHA AO LER ARQUIVO:\nError:%s\n", error.text);
        exit(1);
    } else {
        printf("ARQUIVO ABERTO COM SUCESSO!\n");
    }
    return root;
}

// Retorna o maior valor de X entre todas as cordenadas X dos pontos armazenados na estrutura Graphic
int BigX (Graphic * graphic){
	int i,  maior = 0;
	for (i = 0; i < graphic->sizeContent; i++){
		if (graphic->content[i].x >maior)
			maior= graphic->content[i].x;
	}	
	return maior;
}

// Retorna o maior valor de Y entre todas as cordenadas Y dos pontos armazenados na estrutura Graphic
int BigY (Graphic * graphic){
	int i, maior = 0;
	for (i = 0; i < graphic->sizeContent; i++){
		if (graphic->content[i].y >maior)
			maior = graphic->content[i].y;
	}
	return maior;
}

// Altera os valores de  X ,para que sejam proporcionais no gráfico.Para isso criaremos um fator auxiliar !
void modX (Graphic * graphic){
	int i, fatorX;
	fatorX = graphic->width/10;
	double proporcao = (double)(graphic->width - (2*fatorX))/graphic->maiorX;
	for(i = 0; i < graphic->sizeContent; i++){
		graphic->content[i].x = graphic->content[i].x*proporcao + fatorX;		
	}
}
// Altera os valores de Y ,para que sejam proporcionais no gráfico.Para isso também criaremos um fator auxiliar !
void modY (Graphic * graphic){
	int i, fatorY;
	fatorY = graphic->height/10;
	double proporcao = (double)(graphic->height - (2*fatorY))/graphic->maiorY;	
	for (i = 0; i < graphic->sizeContent; i++){
		graphic->content[i].y = (graphic->height - graphic->content[i].y*proporcao - fatorY) ;
	}

}

// Abre o arquivo Json dps da verificação de erro e passa os dados para a struct Graphic 
Graphic *loadGraphic(char *fileName) {
    
    Graphic * graphic= malloc(sizeof(Graphic));
    json_t *root = openJSONFile(fileName);
    
    json_t * jFileName = json_object_get(root, "fileName");  /*  Nome do Arquivo que será gerado */  
    graphic->fileName = json_string_value(jFileName);
    free(jFileName);
    json_t * jformat = json_object_get(root, "format");      /*  Formato do Arquivo que será gerado */ 
    graphic->format = json_string_value(jformat);
    free(jformat);
    json_t * jName = json_object_get(root,"name");           /*  Nome do Gráfico que será  */ 
    graphic->name = json_string_value(jName);
    free(jName);
    
    /*  Dimensões da Imagem ou Arquivo PDF */
     
    json_t * jWidth = json_object_get(root, "width");        /*  Largura do Arquivo que será gerado */    
    graphic->width = json_integer_value(jWidth);
    free(jWidth);
    json_t * jHeight = json_object_get(root,"height");       /*  Altura do Arquivo que será gerado */    
    graphic->height = json_integer_value(jHeight);
    free(jHeight);
     
    json_t * jType = json_object_get(root,"type");			 /*  Tipo do Arquivo que será gerado ( PNG ou PDF) */ 
    graphic->type = json_integer_value(jType);
    free(jType);  
    
    //Aqui serão coletadas as cores armazenadas em um array ,e que serão usadas no gráfico!   
    json_t * jArrayColor = json_object_get(root, "color");   
    int i;
    for(i = 0; i < 4; ++i){
        json_t *jFactor = json_array_get(jArrayColor, i);
        graphic->color[i] = json_integer_value(jFactor);
    }
    free(jArrayColor);
    
    // Aqui coletamos o conteúdo(pontos).
    json_t * jArrayContent = json_object_get(root, "content");
    Point *points = malloc(sizeof(Point) * json_array_size(jArrayContent)); //Passamos ele para Point
    for(i = 0; i < json_array_size(jArrayContent); ++i){
        json_t *jPoint = json_array_get(jArrayContent, i);
        json_t * jX = json_object_get(jPoint, "x");
        points[i].x = json_integer_value(jX);
        json_t * jY = json_object_get(jPoint, "y");
        points[i].y = json_integer_value(jY);
        free(jX);
        free(jY);
        free(jPoint);
    }
    
    //Algumas definições e atribuições das funções acima .
    graphic->sizeContent = json_array_size(jArrayContent);
    graphic->content = points;
    graphic->maiorX = BigX(graphic);
    graphic->maiorY = BigY(graphic);
    modX(graphic);
    modY(graphic);
    free(jArrayContent);
    
    //Vamos imprimir os dados Coletados :)
    printf("\n** Dados coletados **\n");
    printf("Nome do Arquivo: %s\n", graphic->fileName);
    printf("Formato: %s\n", graphic->format);
    printf("Título: %s\n\n", graphic->name);
    printf("**Dimensões**\n");
    printf("Width: %d\n", graphic->width);
    printf("Height: %d\n\n", graphic->height);
    printf("**Características**\n");
    printf("Type: %d\n", graphic->type);
    printf("**Cores RGBA (respectivamente)**\n");
    for(i = 0; i < 4; ++i){
		printf("%d\n", (int) graphic->color[i]);	
    }
    printf("\n**Conteúdo**\n");
    for(i = 0; i < json_array_size(jArrayContent); ++i){
        printf("Point(%d, %d)\n", points[i].x, points[i].y);
    }
    printf("Size of content: %d\n", graphic->sizeContent);
	printf("Higher value of X: %d\n", graphic->maiorX);
    printf("Higher value of Y: %d\n\n", graphic->maiorY);
    
    return graphic;
}
