/**
* @file apolonioB.c
* @author Mariana Goncalves Rodrigues, Anderson Soares da Silva
* @r.a: 2151014 e 2150875
* @date 06 MAI 2021
* @brief  ArvoreB Sr. Apolonio
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//ORDEM 5
#define MAX 4
#define MIN 2
/*
1- inserir as chaves em ordem crescente
  3 casos:
    1- no vazio;
      insere e faz esse valor virar raizes
    2- espaço vazio
      insere em ordem crescente
    3- overflow
      no cheio, entao, split 1 to 2, promote

          cria um vetor com tamanho maior que o orginal com as chaves do vetor original
          insere a nova chave em ordem crescente
          cria dois vetores de tamanho iguais
          passar para um vetor as duas primeiras chaves do vetor criado
          promove a chave da posicao 3
          passar as duas ultimas chaves para o outro vetor criado

2- antes de inserir é necessario verificar se a chave ja existe, duplicadas nao sao permitidas.
3- pesquisar valor
  pesquisa valor vai percorrer a arvore começando da raiz e procurando nas subarvores apropriadas
*/
typedef struct{
  int id;
  char nome[26];
  char rua[26];
  char cidade[21];
  char estado[3];
}arqdados;

typedef struct{
  int id;
  int rrn;
}indp;

typedef struct{
  char nome[26];
  int rrn;
}ind_sforte;

typedef struct{
  int id;
  char cidade[21];
}ind_sfraco;

typedef struct{
  char estado[3];
  char cidade[21];
  char rua[26];
}complemento;

//struct que referencia as caracteristicas da arvoreb
struct NoArvoreB {
  int val[MAX + 1], count;
  struct NoArvoreB *registro[MAX + 1];
};
//declaração da raiz como val global
struct NoArvoreB *raiz;

// Cria o nó
struct NoArvoreB *CriaArvorePrim(int val, struct NoArvoreB *filho) {
  struct NoArvoreB *NovoNo;
  NovoNo = (struct NoArvoreB *)malloc(sizeof(struct NoArvoreB));
  NovoNo->val[1] = val;
  NovoNo->count = 1;
  NovoNo->registro[0] = raiz;
  NovoNo->registro[1] = filho;
  printf("R[%03d]PROMOTE RAIZ\n", NovoNo->val[1]);
  return NovoNo;
}

// Insere nó
void InsereNo(int val, int pos, struct NoArvoreB *No, struct NoArvoreB *filho) {
  int j = No->count;//representa o quao cheio esta o no
  while (j > pos) {//joga o conteudo para a direita
    No->val[j + 1] = No->val[j];
    No->registro[j + 1] = No->registro[j];
    j--;
  }//while
  No->val[j + 1] = val;//insere no local apropriado
  No->registro[j + 1] = filho;
  No->count++;

  printf("F[%03d]\n", val);
}//InsereNo

// Divide No
void DivideNo(int val, int *pval, int pos, struct NoArvoreB *No, struct NoArvoreB *filho, struct NoArvoreB **NovoNo) {
  int mediano, j;

  if (pos > MIN)
  mediano = MIN + 1; //define o meio do no->val
  else
  mediano = MIN;//define o meio do no->val

  *NovoNo = (struct NoArvoreB *)malloc(sizeof(struct NoArvoreB));
  j = mediano + 1;
  while (j <= MAX) { //Insira os elementos em ordem crescente no novo nó verificamdo se o nó esta cheio
    (*NovoNo)->val[j - mediano] = No->val[j];
    (*NovoNo)->registro[j - mediano] = No->registro[j];
    j++;
  }
  No->count = mediano; //Agora, existem elementos maiores que seu limite. Então, divida na mediana.
  (*NovoNo)->count = MAX - mediano;
  //Empurre a chave do meio para cima e faça as chaves da esquerda como filho da esquerda e as chaves da direita como filho da direita.
  if (pos <= MIN) {//Inserindo no vetor1 do split 1 to 2
    InsereNo(val, pos, No, filho);
  } else {// Inserindo no veto2 do split 1 to 2
    InsereNo(val, pos - mediano, *NovoNo, filho);
  }
  *pval = No->val[No->count];//chave promovida
  printf("P[%03d]PROMOTE NO\n", No->val[No->count]);
  (*NovoNo)->registro[0] = No->registro[No->count];
  No->count--;
}

// Define valor
int DefineValor(int val, int *pval, struct NoArvoreB *No, struct NoArvoreB **filho) {
  int pos;
  if (!No) {//arvore vazia
    *pval = val;
    *filho = NULL;
    return 1;
  }

  if (val < No->val[1]) {//val menor que o primeiro valor?
    pos = 0;
  } else {
    for (pos = No->count;(val < No->val[pos] && pos > 1); pos--); //Atualize o número permitido de chaves no nó e verifica qual a melhor posicao para a chave queremos inserir
    if (val == No->val[pos]) {//se a chave for igual ao conteudo da melhor posicao
      //Chaves duplicadas nao sao permitidos
      return 0;
    }
  }
  if (DefineValor(val, pval, No->registro[pos], filho)) { //Procure o nó apropriado para inserção.
    if (No->count < MAX) {//se ainda tenho espaço para inserção
      InsereNo(*pval, pos, No, *filho);
    } else { //Se o nó estiver cheio
      DivideNo(*pval, pval, pos, No, *filho, filho);
      return 1;
    }
  }

  return 0;
}

// Insere o valor
void Insere(int val) {
  int sinalizador, i;
  struct NoArvoreB *filho;//ao inserir um chave, um filho é criado.

  sinalizador = DefineValor(val, &i, raiz, &filho);
  if (sinalizador) //Se a árvore estiver vazia, aloque um nó raiz e insira a chave.
  raiz = CriaArvorePrim(i, filho);
}

// Procura No
void Procura(int val, int *pos, struct NoArvoreB *meuNo, FILE*entrada){
  int byteoffset;
  arqdados temp;
  if (!meuNo){//chegou no fim e nao achou nada
    printf("A CHAVE NAO FOI ENCONTRADA!\n");
    printf("Entre com uma tecla para limpar a tela e exibir o menu\n");
    getchar();
    return;
  }
  if (val < meuNo->val[1]){//se meu val for menor que o valor da pos[1] do meuno que inicialmente é raiz
    *pos = 0;
  } else {//procura em qual posicao ele se encaixa melhor
    for (*pos = meuNo->count; (val < meuNo->val[*pos] && *pos > 1); (*pos)--);
    if (val == meuNo->val[*pos]) {//achou a chave na arvore
      printf("%03d foi encontrado\n", val);
      byteoffset = (val - 1) * 77;
      fseek(entrada, byteoffset, SEEK_SET);
      fscanf(entrada, "%03d %25[^\n] %25[^\n] %20[^\n] %2[^\n]", &temp.id, temp.nome, temp.rua, temp.cidade, temp.estado);
      printf("%03d %s %s %s %s\n", temp.id, temp.nome, temp.rua, temp.cidade, temp.estado);
      printf("Entre com uma tecla para limpar a tela e exibir o menu\n");
      getchar();

      return;
    }
  }
  Procura(val, pos, meuNo->registro[*pos], entrada);//recursao para subarvore apropriada
  return;
}

// funcao para printar raizes

void printraiz(struct NoArvoreB *meuNo, FILE*arqprimario) {
  int i;
  if (meuNo){
    for (i = 0; i < meuNo->count; i++) {
      printraiz(meuNo->registro[i], arqprimario);
      fprintf(arqprimario,"%03d\t%03d\n", meuNo->val[i + 1], i);
      fflush(arqprimario);
    }
    printraiz(meuNo->registro[i], arqprimario);
  }
}

int ordenaid_indp(const void *a, const void *b){
  const indp *x = (const indp*)a;
  const indp *y = (const indp*)b;

  if (x->id < y->id){
    return -1;
  }else if (y->id < x->id){
    return 1;
  }else{
    return 0;
  }//else
}//ordenaid_indp

int ordenaid_indsfraco(const void * a, const void * b){
  const ind_sfraco *x = (const ind_sfraco*)a;
  const ind_sfraco *y = (const ind_sfraco*)b;
  if (strcmpi(x->cidade, y->cidade) < 0){
    return -1;
  }else if (strcmpi(x->cidade, y->cidade) > 0){
    return 1;
  }else{
    return 0;
  }//else
}

void conteudo(FILE* entrada, arqdados aux, indp* prim, ind_sforte *forte, ind_sfraco *fraco, complemento *comp, FILE *arqprimario){
  int i=0;
  while(fscanf(entrada, "%3d %25[^\n] %25[^\n] %20[^\n] %2[^\n]", &aux.id, aux.nome, aux.rua, aux.cidade, aux.estado) != EOF){
    prim[i].id = aux.id;
    fraco[i].id = aux.id;
    strncpy(forte[i].nome, aux.nome, 26);
    strncpy(comp[i].rua, aux.rua, 26);
    strncpy(fraco[i].cidade, aux.cidade, 21);
    strncpy(comp[i].cidade, aux.cidade, 21);
    strncpy(comp[i].estado, aux.estado, 3);
    prim[i].rrn = i;
    forte[i].rrn = i;
    i++;
    Insere(aux.id);

  }//end while

  printraiz(raiz, arqprimario);  //funcao para printar raizes
  qsort(prim, 500, sizeof(indp), ordenaid_indp);//ordenacao do id no indice primario por quicksort
  qsort(fraco, 500, sizeof(ind_sfraco), ordenaid_indsfraco);//ordenacao do id no indice secundario por quicksort
}

void listarTodosAmigos(indp* prim, ind_sforte* forte, ind_sfraco* fraco, complemento* comp){
  for(int i = 0; i<500; i++){
    printf("%03d %s %s %s %s \n", prim[i].id, forte[i].nome, comp[i].rua, comp[i].cidade, comp[i].estado);
  }
  printf("Pressione qualquer tecla para voltar ao menu\n");
  getchar();
}

/*
void arqprimario(FILE* primario, struct NoArvoreB *meuNo){
for(int i = 0; i < 500; i++){
fprintf(primario, "%03d\t%05d\n", prim[i].id, prim[i].rrn);
}
fflush(primario);
}
*/
void arqforte(FILE* secundarioforte, ind_sforte *forte){
  for(int i = 0; i < 500; i++){
    fprintf(secundarioforte, "%s\t%05d\n", forte[i].nome, forte[i].rrn);
  }
  fflush(secundarioforte);
}

void arqfraco(FILE* secundariofraco, ind_sfraco *fraco){
  for(int i = 0; i < 500; i++){
    fprintf(secundariofraco, "%s\t%03d\n", fraco[i].cidade, fraco[i].id);
  }
  fflush(secundariofraco);
}

void pesquisaNome(FILE* entrada, FILE* secundarioforte, ind_sforte *forte){
  int byteoffset;
  int aux = 0;
  char nome[26];
  arqdados temp;
  int contador=0;
  char name[contador];

  printf("Insira um nome para realizar a busca\n");
  fgets(nome, 26, stdin);
  setbuf(stdin, NULL);
  nome[strcspn(nome,"\n")]='\0';

  for(int i = 0; i<500;i++){
    for(int j = 0; j<strlen(forte[i].nome); j++){//remover espacos desnecessarios
      if((forte[i].nome[j] == ' ' && forte[i].nome[j+1] == ' ')  || (j == strlen(forte[i].nome) - 1) || (j == strlen(forte[i].nome))){
        contador = j;
        break;
      }
    }
    if(memcmp(forte[i].nome, nome, contador) == 0){
      aux = 1;
      byteoffset = forte[i].rrn * 77;
    }
  }
  if(aux == 0){
    printf("Nome nao encontrado\n");
  }else{
    fseek(entrada, byteoffset, SEEK_SET);
    fscanf(entrada, "%03d %25[^\n] %25[^\n] %20[^\n] %2[^\n]", &temp.id, temp.nome, temp.rua, temp.cidade, temp.estado);
    printf("%03d %s %s %s %s\n", temp.id, temp.nome, temp.rua, temp.cidade, temp.estado);
  }
  printf("Pressione qualquer tecla para voltar ao menu\n");
  getchar();
}

void pesquisaCidade(FILE* entrada, FILE* secundariofraco, ind_sfraco *fraco){

  char cidade[26];
  int aux = 0;
  int contador = 0;
  int byteoffset;
  int cont=0;
  arqdados temp[cont];

  printf("Insira um cidade para realizar a busca\n");
  fgets(cidade, 26, stdin);
  setbuf(stdin, NULL);
  cidade[strcspn(cidade,"\n")]='\0';

  for(int i = 0; i < 500; i++){
    for(int j = 0; j<strlen(fraco[i].cidade); j++){//remover espacos desnecessarios
      if((fraco[i].cidade[j] == ' ' && fraco[i].cidade[j+1] == ' ')  || (j == strlen(fraco[i].cidade) - 1) || (j == strlen(fraco[i].cidade))){
        contador = j;
        break;
      }
    }
    if(memcmp(fraco[i].cidade, cidade, contador) == 0){
      aux = 1;
      byteoffset = (fraco[i].id - 1) * 77;
      fseek(entrada, byteoffset, SEEK_SET);
      fscanf(entrada, "%3d %25[^\n] %25[^\n] %20[^\n] %2[^\n]", &temp[i].id, temp[i].nome, temp[i].rua, temp[i].cidade, temp[i].estado);
      printf("\n%03d %s %s %s %s\n", temp[i].id, temp[i].nome, temp[i].rua, temp[i].cidade, temp[i].estado);
    }
  }
  if(aux == 0){
    printf("A cidade nao foi encontrada!\n");
  }
  printf("Pressione qualquer tecla para voltar ao menu\n");
  getchar();
}

int main(int argc, char const *argv[]) {

  FILE* entrada; //ponteiro para arquivo que vou usar como entrada
  FILE* primario; //ponteiro para arquivo do indice primario
  FILE* secundarioforte; //ponteiro para arquivo do indice secundario forte
  FILE* secundariofraco; //ponteiro para arquivo do indice secundario fraco

  arqdados aux;
  indp prim[500];
  ind_sfraco fraco[500];
  ind_sforte forte[500];
  complemento comp[500];
  int menu;
  int ch; //pos na funcao procura
  entrada = fopen("cadastro.txt", "r");
  primario = fopen("primario.txt", "r");
  secundarioforte = fopen("secforte.txt", "r");
  secundariofraco = fopen("secfraco.txt", "r");

  if(entrada == NULL){
    printf("erro ao ler arquivo");
    return 0;
  }else{
    fclose(primario);
    fclose(secundarioforte);
    fclose(secundariofraco);
    primario = fopen("primario.txt", "w");
    secundarioforte = fopen("secforte.txt", "w");
    secundariofraco = fopen("secfraco.txt", "w");
    //printf("Pegando conteudo\n");
    conteudo(entrada, aux, prim, forte, fraco, comp, primario);
    //printf("Pegando arqprim\n");
    //arqprimario(primario, prim);
    //printf("Pegando arqfort\n");
    arqforte(secundarioforte, forte);
    arqfraco(secundariofraco, fraco);
  }//else if
  printf("Entre com uma tecla para limpar a tela e exibir o menu\n");
  getchar();
  do{
    system("cls");
    printf("=======Amigos do Sr Polonio=======\n");
    printf("1-Listar todos os dados dos amigos\n");
    printf("2-Pesquisar por id\n");
    printf("3-Pesquisar por cidade\n");
    printf("4-Sair do programa\n");
    printf("\nDigite uma opcao:\n");
    do{
      scanf("%d", &menu);
      setbuf(stdin, NULL);
    }while((menu > 4) || (menu < 1));

    switch (menu) {
      case 1:
      printf("A opcao escolhida foi: 'Listar todos os dados dos amigos'\n");
      listarTodosAmigos(prim, forte, fraco, comp);
      break;
      case 2:
      printf("A opcao escolhida foi: 'Pesquisar por ID'\n");
      int pesquisa;
      //pesquisaNome(entrada, secundarioforte, forte);
      printf("Insira o ID que deseja pesquisar\n");
      scanf("%d", &pesquisa);
      setbuf(stdin, NULL);
      Procura(pesquisa, &ch, raiz, entrada);//raiz global
      //Acessar o arq secundario forte e identificar o byteoffset relacionado a aquele nome,
      //então fazer um seek no arq de dados com o byteoffset encontrado e printar os dados
      //daquela linha
      break;
      case 3:
      printf("A opcao escolhida foi: 'Pesquisar por cidade'\n");
      pesquisaCidade(entrada, secundariofraco, fraco);
      //Acessar o arq secundario fraco e buscar o id relacionado a cidade, acessar o arq
      //primario e buscar o byteoffset daquele id, fazer seek no arq de dados com o byteoffset
      //encontrado e printar os dados daquela linha
      break;
    }
  }while(menu != 4);

  return 0;
}
