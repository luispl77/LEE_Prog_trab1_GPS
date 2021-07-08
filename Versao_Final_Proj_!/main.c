#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define LAT 0
#define LON 1
#define MAX_STR 50
#define MAX_LOC 100
#define RAIO 6371.0
#define PIE 3.141593
#define I 0
#define I_REF 1                     
#define I_ANTERIOR 2                
#define PRIMEIRA_CIDADE 111
#define PRIMEIRA_ROTA 222

void identificacao(){ /*Identificacao do projecto e autores.*/

  printf("\t\tInstituto Superior Tecnico - DEEC - LEE\n\n");
  printf("Prof. Responsavel - Bertinho D'Andrade\n");
  printf("Realizado por: Luis Lopes (99791) & Miguel Lira (100721) -  Grupo 15\n");
  printf("Primeiro trabalho de Programacao\n----------------------------------------------------------------------------------------\n\n");
}

void mensagem_ao_utilizador(){ /*Mensagem com instrucoes para o utilizador.*/

  printf("\t\tBem vindo ao CALCULADOR DE DISTANCIAS DE ROTAS.\n\n");
  printf("Insira na mesma pasta do programa um ficheiro designado por 'localidades.txt'.\n");
  printf("O ficheiro deve conter o numero de cidades exitentes no mesmo, na primeira linha.\n");
  printf("As restantes linhas devem conter o seguinte formato:\n\n");
  printf("\tLocalidade latitude longitude\n\n");
  printf("A localidade nao pode ter espacos, a latitude e longitude sao numeros reais dentro dos parametros das mesmas.\n");
  printf("De seguida, insira um ficheiro denominado 'rotas.txt', onde serao colocadas as rotas que pretende calcular.\n");
  printf("O formato tem de ser o seguinte: a colocacao do identificador '#ROTA ' e obrigatoria, bem como o numero da rota.\nSe a rota nao tiver numero ira resultar num erro.\n");
  printf("De seguida, as localidades que pertencem a essa rota devem ser colocadas linha a linha, como no seguinte exemplo:\n");
  printf("\n\t#ROTA 36\n\tLisboa\n\tFaro\n\tBraga\n\n");
  printf("A localidade tem de comecar com maiuscula e as restantes letras minusculas.\n");
  printf("Quando estiver nas condicoes acima descritas, pressione ENTER para dar inicio ao programa.\n----------------------------------------------------------------------------------------\n");

  getchar(); /*Pausa no progama.*/
}

FILE * abrir_ficheiro(char ficheiro[], char mode){  /*Permite abrir ficheiros e escolher em que modo, apresentando uma msg de erro se nao abrir.*/

  FILE * apf;
  printf("--Abertura do ficheiro %s\n", ficheiro);

  if (mode == 'r')
    apf = fopen(ficheiro, "r");

  if (mode == 'w')
    apf = fopen(ficheiro, "w");

  if (apf == NULL)
  {
    printf("ERRO: Abertura de ficheiro %s mal sucedida!\n", ficheiro);
    fflush(stdout);
    getchar(); /*Pausa antres do programa terminar*/
    exit(0);
  }

  return apf;
}

int numero_de_cidades_localidades(FILE *ap_localidades){ /*Leitura do numero de cidades do ficheiro.*/

  int nc;
  int num_cidades;

  nc = fscanf(ap_localidades, "%d", &num_cidades);      /*Ler a primeira linha do ficheiro localidades.txt que contem o numero de localidades (20).*/

  printf("  --Leitura do numero de cidades no ficheiro localidades.");

  if (nc != 1)
  {
    printf("Linha 0: ERRO - Leitura do numero de cidades sem sucesso!\n");
    fflush(stdout);
    getchar(); /*Pausa antres do programa terminar*/
    exit (0);
  }

  if (num_cidades <= 0)
  {
    printf("Linha 0: ERRO - Numero de cidades tem um de 0 ou menor que 0!\n");
    fflush(stdout);
    getchar(); /*Pausa antres do programa terminar*/
    exit (0);
  }

  printf("\t>%d<\n", num_cidades);
  return num_cidades;
}

void leitura_localidades(FILE * apf, int num_cidades, char array_cidades[][MAX_STR], float coordenadas[][2]){ /*Guarda valores lidos da localidades.txt e deteta erros.*/

  int i;
  int nc;
  
  printf("   --Leitura das localidades e respetivas coordenadas.\n");

  for(i = 0; i < num_cidades; ++i)           /*Loop que preenche os arrays das cidades e coordenadas, linha a linha a partir do ficheiro localidades.txt.*/
  {
    nc = fscanf(apf, "%s %f %f", array_cidades[i], &(coordenadas[i][LAT]), &(coordenadas[i][LON]));
    if (nc != 3)
    {
      printf("\nLinha %d: ERRO - Numero de parametros fora do esperado!", i+1);
      fflush(stdout);
      getchar(); /*Pausa antres do programa terminar*/
      exit (0);
    }

    /*Teste para determiar se os valores de lat e lon estao dentro dos parametros.*/

    if ((coordenadas[i][LAT] < -90.0) || (coordenadas[i][LAT] > 90.0) || (coordenadas[i][LON] < -180.0) || (coordenadas[i][LON] > 180.0))
    {

      if ((coordenadas[i][LAT] < -90.0) || (coordenadas[i][LAT] > 90.0))
        printf("Linha %d: ERRO - O valor da latitude esta fora dos limites!\n", i+1);

      if ((coordenadas[i][LON] < -180.0) || (coordenadas[i][LON] > 180.0))
        printf("Linha %d: ERRO - O valor da longitude esta fora dos limites!\n", i+1);

      fflush(stdout);
      getchar(); /*Pausa antres do programa terminar*/      
      exit (0);
    }
  }

  fclose(apf);

  return;
}

void mostrar_dados(int num_cidades, char array_cidades[][MAX_STR], float coordenadas[][2]){ /*Imprime os dados obtidos*/

  int i;
  int j;

  putchar('\n');

  for( i = 0; i < num_cidades; i++)      /*Loop de impressao dos arrays.*/
  {
    putchar('\t');
    for( j = 0; array_cidades[i][j] != '\0'; j++)
      printf("%c", array_cidades [i][j]);

    printf("  %f  %f\n" , coordenadas[i][LAT], coordenadas[i][LON]);
  }

  putchar('\n');

  return;
}

void imprime_rota(int rota, FILE * ap_rotas_pro, int scanned_dec, char scanned_string[]){  /*Imprime a rota no ficheiro rotas_processadas*/

   /*Para evitar o uso de mais variveis defenimos o indice i anterior com o codigo 222 (primeira rota) */
   /*Sendo que apenas vai ter valor 222 durante a impressao da primeira rota*/
   /*Nas restantes rotas o indices[I_ANTERIOR] vai esta sempre entre 0 e 19 ate o final do prog*/
   /*Isto e util para a formatacao do texto no rotas processadas.*/

  if (rota == PRIMEIRA_ROTA)       /*Se for a primeira rota entao é imprimida sem \n inicial.*/
    fprintf(ap_rotas_pro, "%s %d \n", scanned_string, scanned_dec);

    else{
      fprintf(ap_rotas_pro, "\n%s %d \n", scanned_string, scanned_dec);}   /*As restantes rotas tem o \n inicial.*/

  printf("\n%s %d \n", scanned_string, scanned_dec);  /*Print do conteudo para o terminal.*/
}

void processa_cidade(int indices[],char scanned_string[], char array_cidades[][MAX_STR], int num_cidades){  /*Determina indices (linhas) usados nos calculos*/

  for(indices[I] = 0; indices[I] <= num_cidades; ++indices[I])
  {
    /*Ciclo de 0 a num_cidades*/

    if(strcmp(scanned_string, array_cidades[indices[I]]) == 0)
    {  /*Comparacao em cada ciclo ate ser encontrada o indice i onde esta guardado as coordenadas da cidade lida.*/

      if(indices[I_REF] == PRIMEIRA_CIDADE) /*Quando o prog leu uma rota coloca o indices[I_REF] a 111 (codigo que seginifica 1a cidade).*/
      {                                     /*Estamos a reutilizar os indices para evitar criar novas variveis com uma finalidade esclusiva de
                                            guardar a informacao de que o programa esteve a ler uma rota no ciclo de leitura anterior, aproveitando
                                            o facto de os indices na sua finalidade principal so terem valores entre 0 e 19.*/
        indices[I_REF] = indices[I]; /*O indice do ponto de refrencia (tinha o codigo primeira_cidade 111) agora vai conter o indice da localidade lida no fscaf.*/
      }                              /*Quando e a 1a cidade da rota, entao o ponto de refrencia = ao ponto lido pela fscanf(distancia calculada = 0).*/

      else {
        indices[I_REF] = indices[I_ANTERIOR];  /*Se nao for a primeira cidade da rota entao o ponto lido anteiormente passa a ser refrencia atual para os calc*/
      }

      indices[I_ANTERIOR] = indices[I];   /*Gravar o i lido neste ciclo de leitura para passar a ser o ponto de refrencia no proximo ciclo.*/
      break;                              /*Forca o ciclo do for a parar porque ja compriu o objectivo.*/
    }

    if (num_cidades == indices[I]) /*Se nao for encontrada uma cidade durante (0 a num_cidade -1) ou seja ja precorreu as cidades todas.*/
    {                              /*No proximo ciclo do for é dado o erro ao utilizador.*/

      printf("\nERRO: Cidade de %s nao foi encontrada em localidades.txt ou ficheiro rotas.txt fora dos parametros!\n", scanned_string);
      fflush(stdout);
      getchar(); /*Pausa antres do programa terminar*/      
      exit (0);
    }
  }
}

void imprime_cidade(FILE * ap_rotas_pro, char scanned_string[], double distancia, double distancia_acumulada){

  fprintf(ap_rotas_pro, "%-17s %.2f km\n", scanned_string, distancia_acumulada);    /*Print da cidade e distancia respetiva no ficheiro.*/
  printf("%-17s %.2f km = %.2f + %.2f\n", scanned_string, distancia_acumulada, (distancia_acumulada - distancia), distancia);   /*Print no terminal, bem como o calculo parcial das distancias.*/
}

void fim_do_prog(FILE* ap_rotas, FILE* ap_rotas_pro){ /*Fecha os ficheiros e msg de fim.*/

  fclose(ap_rotas);
  fclose(ap_rotas_pro);

  printf("   \n--Fim do processamento de rotas.\n");
  printf("    Consulte o ficheiro das rotas processadas.\n\n\n\n\n");
  printf("Pressione ENTER para sair do programa.\n");
  getchar();   /*Pausa antes do programa terminar*/
}

double converter_em_rad(double graus){   /*Convert valores reais em graus para radianos*/

  double rad = 0;

  rad = (graus * PIE) / 180;

  return rad;
}

double distancia_calculada(double ponto_ref_lat, double ponto_ref_lon, double ponto_pro_lat, double ponto_pro_lon){  /*Calcula a distancia entre duas cidades*/
/*Calculo da distancia entre um ponto de referencia e um ponto proximo.*/

  double distancia_final = 0;
  double x = 0;
  double y = 0;

  ponto_ref_lat = converter_em_rad(ponto_ref_lat);
  ponto_ref_lon = converter_em_rad(ponto_ref_lon);
  ponto_pro_lat = converter_em_rad(ponto_pro_lat);
  ponto_pro_lon = converter_em_rad(ponto_pro_lon);

  x = RAIO * cos(ponto_pro_lat) * sin(ponto_pro_lon - ponto_ref_lon);
  y = RAIO * tan(ponto_pro_lat - ponto_ref_lat);
  distancia_final = sqrt(x*x + y*y);

  return distancia_final;
}

int main(){

  FILE *ap_localidades, *ap_rotas, *ap_rotas_pro;   /*Apontadores de ficheiros*/
  int num_cidades;                                  /*Numero de cidades (lido na primeira linha do ficheiro localididades).*/
  char scanned_string[MAX_STR];                     /*Vetor para guardar temporariamente uma string que foi lida do ficheiro rotas.*/
  int scanned_dec;                                  /*Variavel para guardar temporariamente um numero que foi lida do ficheiro rotas.*/
  int nc = 0, comp;                                 /*Numero de conversões; valor da comparacao entre a cidade nas rotas e o ficheiro localidades.*/
  double distancia = 0, distancia_acumulada = 0;    /*Distancia calculada entre cada par de cidades.*/
  int indices[3] = {0, 0, PRIMEIRA_ROTA};           /*Vetor de Indices, contem i, i_ref, i_anterior. O i_anterior é inicializado com um indicador (numero 222) para indicar a primeira rota.*/
  char array_cidades[MAX_LOC][MAX_STR];             /*Inicialização dos arrays das cidades, e coordenadas, baseado no numero de cidades lido.*/
  float coordenadas[MAX_LOC][2];
  char linha[200];								     /*Variavel de aux usada no fgets*/

  identificacao();                              /*Prints de identificação*/
  mensagem_ao_utilizador();                     /*Prints de um pequeno tutorial sobre o funcionamento do prog.*/

  ap_localidades = abrir_ficheiro("localidades.txt", 'r');            /*Abertura do ficheiro localidades.txt .*/

  num_cidades = numero_de_cidades_localidades(ap_localidades);        /*Scan do numero de cidades no localidades.txt (primeira linha).*/

  leitura_localidades(ap_localidades, num_cidades, array_cidades, coordenadas);   /*Leitura do ficheiro localidades.txt e guardar dados (coordenadas e cidades) dentro dos arrays.*/
  mostrar_dados(num_cidades, array_cidades, coordenadas);                         /*Print dos dados lidos (localidades.txt).*/

  ap_rotas = abrir_ficheiro("rotas.txt", 'r');                   /*Abertura dos ficheiros rotas e rotas_processadas.*/
  ap_rotas_pro = abrir_ficheiro("rotas_processadas.txt", 'w');

  printf("  --Processamento das rotas:\n");         /*Print para indicar o inicio do processamento do rotas.txt e escrita no rotas_processadas.txt .*/

  while(feof(ap_rotas) == 0) /*Ciclo principal que analisa o ficheiro rotas.txt linha a linha, e acaba quando chegar ao fim do ficheiro (feof).*/
  {

    fgets(linha, 200, ap_rotas);
    nc = sscanf(linha, "%s %d", scanned_string, &scanned_dec);


    comp = strncmp(scanned_string, "#ROTA", 4);                    /*Averigua se e uma rota ou uma cidade.*/

      if ((comp == 0) && (nc == 2))       /*Se for uma rota e leu um %d que coresponde ao numero da rota , entao executar o codigo.*/
      {
        imprime_rota(indices[I_ANTERIOR], ap_rotas_pro, scanned_dec, scanned_string);  /*Print do que foi lido para o ficheiro rotas_processadas.txt .*/
        distancia_acumulada = 0.0;        /*Reset da distancia acumulada*/
        indices[I_REF] = PRIMEIRA_CIDADE; /*Indicador "111" que sera alocada ao I_REF,
                                           para evitar criar uma nova variavel so para guardar a informacao que a cidade a ser pocessada apos a leitura de uma rota,
                                           vai ser a primeira da mesma.*/
      }

      if ((comp == 0) && (nc != 2))       /*msgERRO: Se lemos #ROTA e o nc != 2 entao nao consegimos ler o numero da rota.*/
      {
        printf("ERR0: Nao foi detetado um numero de rota!");
        fflush(stdout);
        getchar(); /*Pausa antres do programa terminar*/   
        return 0;
      }

    if ((comp != 0) && (nc >= 1) && (scanned_string[0] != '\n'))  /*Processamento Localidade*/ /*Se a string lida nao for uma #ROTA entao vamos processalo como sendo uma cidade.*/
    {
      processa_cidade(indices, scanned_string, array_cidades, num_cidades); /*Funcao que encontra os indices nos arrays do ficheiro localidades.txt associados a rota lida (loop de comparacao).*/
      /*Calculo da distancia atual.*/
      distancia = distancia_calculada(coordenadas[(indices[I_REF])][LAT], coordenadas[(indices[I_REF])][LON], coordenadas[(indices[I])][LAT],coordenadas[(indices[I])][LON]);
      distancia_acumulada = distancia_acumulada + distancia;                           /*calculo da distancia acumulada*/

      imprime_cidade(ap_rotas_pro, scanned_string, distancia, distancia_acumulada);    /*Imprime no ficheiro rotas_processadas.txt as localidades e distancias em km, bem como imprimir no terminal o calculo das mesmas.*/
    }
  }
  fim_do_prog(ap_rotas, ap_rotas_pro); /*Fecha os ficheiros e faz print das mensagens de fim.*/

  return 0;
}
