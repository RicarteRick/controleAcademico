/**
 * @file ControleAcademico.c
 * @brief Um programa que mantenha registros e realize uma análise estatística
 *	de uma turma de estudantes. 
 * @author Mateus Ferreira Silva
 * @author Rick Gomes Ricarte
 * @date 03/07/2019
 * @bugs Nenhum conhecido
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * Para compilar e executar corretamente o código utilizando o GCC, no terminal do linux:
 * 1º > gcc -std=c11 -c ControleAcademico.c
 * 2º > gcc -std=c11 -o ControleAcademico.exe ControleAcademico.o
 * 3º > ./ControleAcademico.exe
 */

typedef struct 
{
	char titulo[93]; // é a soma de todos caracteres abaixo +1 que é o caractere nulo
	char linha1[93];
	char dados[93];
	char linha2[93];
} Cabecalho; 

typedef struct
{	
	char nome[51]; // 50 caracteres para o nome, +1 para o '\0', o mesmo caso acontece nas outras.
	char matricula[13]; // +1 '\0' e +1 para o espaço, porque senão pega um caractere a menos
	double provas[3];
	double trabalhos[2];
	int faltas;
	double notaF;
	char situacao;
} Estudante;

typedef struct 
{
	char nomeArquivo[400]; // Nome do arquivo ou do caminho p/ o diretório + o nome 
	int qtdAlunos; // Para contabilizar o número de alunos, usado para validações e cálculos
} Arquivo;

typedef struct 
{
	double maiorP1;
	double maiorP2;
	double maiorP3;
	double maiorT1;
	double maiorT2;
	double maiorFinal;
	double menorP1;
	double menorP2;
	double menorP3;
	double menorT1;
	double menorT2;
	double menorFinal;

	double totalP1;
	double totalP2;
	double totalP3;
	double totalT1;
	double totalT2;
	double total;
	double mediaP1;
	double mediaP2;
	double mediaP3;
	double mediaT1;
	double mediaT2;
	double mediaTotal;
} Notas;

typedef struct 
{
	int zero10;
	int onze20;
	int vinteUm30;
	int trintaUm40;
	int quarentaUm50;
	int cinquentaUm60;
	int sessentaUm70;
	int setentaUm80;
	int oitentaUm90;
	int noventaUm100;

	int aprovados;
	int reprovados;
	int reprovadosFalta; 	
} Estatistica;

// Cabeçalhos:
void menuPrincipal(Estudante estudante[80], Arquivo arquivo);
void lerTurma(Estudante estudante[80], Arquivo arquivo);
	void lerTurmaExistente(Estudante estudante[80], Arquivo arquivo);
	void criarTurmaNova(Estudante estudante[80], Arquivo arquivo);
void imprimirTurma(Estudante estudante[80], Arquivo arquivo);
void imprimirEstatistica(Estudante estudante[80], Arquivo arquivo);
void cadastrarEstudante(Estudante estudante[80], Arquivo arquivo);
void editarInfoEstudante(Estudante estudante[80], Arquivo arquivo);
	void validaMatricula(Estudante estudante[80], Arquivo arquivo);
	void validaNome(Estudante estudante[80], Arquivo arquivo);
	void editaInformacoes(int indice, Estudante estudante[80], Arquivo arquivo);
void relerTurma(Estudante estudante[80], Arquivo arquivo);
void salvaSair(Estudante estudante[80], Arquivo arquivo);

/**
 * @brief Imprime uma linha de '-', simplesmente um detalhe estético
 *
 */
void imprimeLinha(void){
	for(int c = 0; c != 110; c++)
			printf("-");
		puts("");
} //imprimiLinha

/**
 * @brief Imprime um cabeçalho em formato de tabela para mostrar os dados do arquivo de uma forma mais fácil de ser interpretada
 *
 */
void imprimeCabecalho(void){
	printf("%80s\n","Turma de Estudantes");
		imprimeLinha();
		printf("%-50s", "Nome");
		printf("%-14s", "Matrícula");
		printf("%-5s", "Nota");
		printf("%-5s", "Nota");
		printf("%-5s", "Nota");
		printf("%-5s", "Trab");
		printf("%-5s", "Trab");
		printf("%-7s", "Faltas");
		printf("%-7s", "Nota");
		printf("%s", "Situação");
		puts("");
		printf("%64c %4c %4c %4c %4c", '1', '2', '3', '1', '2');
		printf("%16s", "final");
		puts("");
		imprimeLinha();
		puts("");
} // imprimeCabecalho

/**
 * @brief Imprime um cabeçalho para o menu estatística em formato de tabela para mostrar a estatísca de 
 *	uma forma mais fácil de ser interpretada.
 *
 */
void cabecalhoEstatisca(void){
	printf("                                   Estatísticas\n");
		printf("-----------------------------------------------------------------\n");
		printf("                                   %-5s", "N1");
		printf("%-5s", "N2");
		printf("%-5s", "N3");
		printf("%-5s", "T1");
		printf("%-5s", "T2");
		printf("%-7s", "Final");
		puts("");
		printf("-----------------------------------------------------------------\n");
		puts("");
} // imprimeCabecalho

/**
 * @brief Lê a estrututa com as notas e separa as maiores e menores, e também faz o cálculo da média da turma.
 *
 * @param estudante, estrutura com os dados do estudante: nome, matrícula, notas da provas e trabalhos, faltas e sua situação.
 * @param arquivo, estrutura com o nome do arquivo que será utilizado em alguma operação de manipulação de arquivo.
 * @param notas, estrutura com as todas as notas.
 * @return estrutura com as notas, após serem organizadas entre maiores, menores e as médias de notas da turma.
 */
Notas filtraNotas(Estudante estudante[80], Arquivo arquivo, Notas notas){
	for(int i = 0; i < arquivo.qtdAlunos; i++){
			for(int j = 0; j < sizeof(estudante[i].provas)/sizeof(double); j++){
				notas.total += estudante[i].provas[j]; // obtém o total para ser utilizado no cálculo de média
				if(j == 0)
					notas.totalP1 += estudante[i].provas[j];
				if(j == 1)
					notas.totalP2 += estudante[i].provas[j];
				if(j == 2)
					notas.totalP3 += estudante[i].provas[j];
			} // for
			for(int j = 0; j < sizeof(estudante[i].trabalhos)/sizeof(double); j++){
				notas.total += estudante[i].trabalhos[j]; // obtém o total para ser utilizado no cálculo de média
				if(j == 1)
					notas.totalT1 += estudante[i].trabalhos[j];
				if(j == 0)
					notas.totalT2 += estudante[i].trabalhos[j];
			} // for

			if(estudante[i].provas[0] > notas.maiorP1 || i == 0) // obtém quais foram as maiores notas
				notas.maiorP1 = estudante[i].provas[0];
			if(estudante[i].provas[1] > notas.maiorP2 || i == 0)
				notas.maiorP2 = estudante[i].provas[1];
			if(estudante[i].provas[2] > notas.maiorP3 || i == 0)
				notas.maiorP3 = estudante[i].provas[2];
			if(estudante[i].trabalhos[0] > notas.maiorT1 || i == 0)
				notas.maiorT1 = estudante[i].trabalhos[0];
			if(estudante[i].trabalhos[1] > notas.maiorT2 || i == 0)
				notas.maiorT2 = estudante[i].trabalhos[1];
			if(estudante[i].notaF > notas.maiorFinal || i == 0)
				notas.maiorFinal = estudante[i].notaF;

			if(estudante[i].provas[0] < notas.menorP1 || i == 0) // obtém quais as menores notas
				notas.menorP1 = estudante[i].provas[0];
			if(estudante[i].provas[1] < notas.menorP2 || i == 0)
				notas.menorP2 = estudante[i].provas[1];
			if(estudante[i].provas[2] < notas.menorP3 || i == 0)
				notas.menorP3 = estudante[i].provas[2];
			if(estudante[i].trabalhos[0] < notas.menorT1 || i == 0)
				notas.menorT1 = estudante[i].trabalhos[0];
			if(estudante[i].trabalhos[1] < notas.menorT2 || i == 0)
				notas.menorT2 = estudante[i].trabalhos[1];
			if(estudante[i].notaF < notas.menorFinal || i == 0)
				notas.menorFinal = estudante[i].notaF;
		} // for

		notas.mediaP1 = notas.totalP1/arquivo.qtdAlunos;
		notas.mediaP2 = notas.totalP2/arquivo.qtdAlunos;
		notas.mediaP3 = notas.totalP3/arquivo.qtdAlunos;
		notas.mediaT1 = notas.totalT1/arquivo.qtdAlunos;
		notas.mediaT2 = notas.totalT2/arquivo.qtdAlunos;
		notas.mediaTotal = notas.total/arquivo.qtdAlunos;

		return notas;
} // filtraNotas

/**
 * @brief Mostra uma tabela com as notas separadas entre as maiores, menores e também as médias da turma. 
 *
 * @param estudante, estrutura com os dados do estudante: nome, matrícula, notas da provas e trabalhos, faltas e sua situação.
 * @param arquivo, estrutura com o nome do arquivo que será utilizado em alguma operação de manipulação de arquivo.
 * @param notas, estrutura com as todas as notas.
 */
void tabelaNotas(Estudante estudante[80], Arquivo arquivo, Notas notas){
		printf("Maiores notas da turma\t\t");
		printf("%5.0lf", notas.maiorP1);
		printf("%5.0lf", notas.maiorP2);
		printf("%5.0lf", notas.maiorP3);
		printf("%5.0lf", notas.maiorT1);
		printf("%5.0lf", notas.maiorT2);
		printf("%8.0lf", notas.maiorFinal);
		puts("");
		printf("Menores notas da turma\t\t");
		printf("%5.0lf", notas.menorP1);
		printf("%5.0lf", notas.menorP2);
		printf("%5.0lf", notas.menorP3);
		printf("%5.0lf", notas.menorT1);
		printf("%5.0lf", notas.menorT2);
		printf("%8.0lf", notas.menorFinal);
		puts("");
		printf("Notas médias da turma\t\t");
		printf("%5.1lf", notas.mediaP1);
		printf("%5.1lf", notas.mediaP2);
		printf("%5.1lf", notas.mediaP3);
		printf("%5.1lf", notas.mediaT1);
		printf("%5.1lf", notas.mediaT2);
		printf("%8.1lf", notas.mediaTotal);
		puts("");
		printf("-----------------------------------------------------------------\n");
		puts("");
} // tabelaNotas

/**
 * @brief Lê a estrututa com as notas e faz um estatística destas. Também classifica a situação de cada estudante.
 *
 * @param estudante, estrutura com os dados do estudante: nome, matrícula, notas da provas e trabalhos, faltas e sua situação.
 * @param arquivo, estrutura com o nome do arquivo que será utilizado em alguma operação de manipulação de arquivo.
 * @param notas, estrutura com as todas as notas.
 * @param estat, para contabilizar os dados.
 * @return estrutura com as notas, após serem organizadas entre maiores, menores e as médias de notas da turma.
 */
Estatistica fazEstatistica(Estudante estudante[80], Arquivo arquivo, Notas notas, Estatistica estat){
	// colocando 0 nas variaveis, para que os contadores contem adequadamente	
	estat.zero10 = 0;
	estat.onze20 = 0;
	estat.vinteUm30 = 0;
	estat.trintaUm40 = 0;
	estat.quarentaUm50 = 0;
	estat.cinquentaUm60 = 0;
	estat.sessentaUm70 = 0;
	estat.setentaUm80 = 0;
	estat.oitentaUm90 = 0;
	estat.noventaUm100 = 0;
	estat.aprovados = 0;
	estat.reprovados = 0;
	estat.reprovadosFalta = 0;

	for(int i = 0; i < arquivo.qtdAlunos; i++){
			// filtragem para o histograma
			if(estudante[i].notaF <= 10)
				estat.zero10++;
			else if(estudante[i].notaF <= 20)
				estat.onze20++;
			else if(estudante[i].notaF <= 30)
				estat.vinteUm30++;
			else if(estudante[i].notaF <= 40)
				estat.trintaUm40++;
			else if(estudante[i].notaF <= 50)
				estat.quarentaUm50++;
			else if(estudante[i].notaF <= 60)
				estat.cinquentaUm60++;
			else if(estudante[i].notaF <= 70)
				estat.sessentaUm70++;
			else if(estudante[i].notaF <= 80)
				estat.setentaUm80++;
			else if(estudante[i].notaF <= 90)
				estat.oitentaUm90++;
			else if(estudante[i].notaF <= 100)
				estat.noventaUm100++;
			
			// 
			if(estudante[i].situacao == 'A')
				estat.aprovados++;
			if(estudante[i].situacao == 'R')
				estat.reprovados++;
			if(estudante[i].situacao == 'F')
				estat.reprovadosFalta++;
		} // for
		return estat;
} // fazEstatistica

/**
 * @brief Mostra estatística e histograma das notas.
 *
 * @param estudante, estrutura com os dados do estudante: nome, matrícula, notas da provas e trabalhos, faltas e sua situação.
 * @param arquivo, estrutura com o nome do arquivo que será utilizado em alguma operação de manipulação de arquivo.
 * @param notas, estrutura com as todas as notas.
 * @param estat, para contabilizar os dados.
 */
void mostraEstatistica(Estudante estudante[80], Arquivo arquivo, Notas notas, Estatistica estat){
	//Porcentagem
	printf("Número de estudantes aprovados:        \t\t");
	printf("%d (%.1lf %%)\n", estat.aprovados, (estat.aprovados*100.00)/arquivo.qtdAlunos);
	printf("Número de estudantes reprovados:       \t\t");
	printf("%d (%.1lf %%)\n", estat.reprovados, (estat.reprovados*100.00)/arquivo.qtdAlunos);
	printf("Número de estudantes reprovados por falta:  \t");
	printf("%d (%.1lf %%)\n", estat.reprovadosFalta, (estat.reprovadosFalta*100.00)/arquivo.qtdAlunos);
	puts("");

	// Hitograma
	printf("Histograma de notas finais em grupos de 10 pontos:\n");
	printf("  0 - 10   %2d ", estat.zero10);
	for(int k = 0; k < estat.zero10; k++)
		printf("*");
	puts("");
	printf(" 11 - 20   %2d ", estat.onze20);
	for(int k = 0; k < estat.onze20; k++)
		printf("*");		puts("");
	printf(" 21 - 30   %2d ", estat.vinteUm30);
	for(int k = 0; k < estat.vinteUm30; k++)
		printf("*");
	puts("");
	printf(" 31 - 40   %2d ", estat.trintaUm40);
	for(int k = 0; k < estat.trintaUm40; k++)
		printf("*");
	puts("");
	printf(" 41 - 50   %2d ", estat.quarentaUm50);
	for(int k = 0; k < estat.quarentaUm50; k++)
		printf("*");
	puts("");
	printf(" 51 - 60   %2d ", estat.cinquentaUm60);
	for(int k = 0; k < estat.cinquentaUm60; k++)
		printf("*");
	puts("");
	printf(" 61 - 70   %2d ", estat.sessentaUm70);
	for(int k = 0; k < estat.sessentaUm70; k++)
		printf("*");
	puts("");
	printf(" 71 - 80   %2d ", estat.setentaUm80);
	for(int k = 0; k < estat.setentaUm80; k++)
		printf("*");
	puts("");
	printf(" 81 - 90   %2d ", estat.oitentaUm90);
	for(int k = 0; k < estat.oitentaUm90; k++)
		printf("*");
	puts("");
	printf(" 91 - 100  %2d ", estat.noventaUm100);
	for(int k = 0; k < estat.noventaUm100; k++)
		printf("*");
	puts("");

	return;
} // mostraEstatistica

/**
 * @brief Função para validar se o usuário escolheu alguma opção existente.
 *
 * @param opcao número inteiro que indica a opção escolhida pelo usuário.
 * @param OPC_MAX número inteiro que indica a quantidade máximo de opções no menu em questão.
 * @return A opção validada.
 */
int validaOpcao (int opcao, int const OPC_MAX) {
	while (opcao < 1 || opcao > OPC_MAX){
        printf("Opção inválida. Tente novamente: ");
        scanf("%d", &opcao);
    } // while

    return opcao;
} // validaOpcao

int main(void){	
	// Declarações locais
	Estudante estudante[80]; // estrutura com o conjunto de dados dos estudantes
	Arquivo arquivo = {'\0'};  // nome do arquivo a ser aberto
	arquivo.qtdAlunos = 0; // Para contabilizar o número de alunos, usado para validações e cálculos

	menuPrincipal(estudante, arquivo);
	
	return 0;
} // main

/**
 * @brief Mostra o menu principal e com base na opção escolhida pelo usuário, essa função o redireciona para outros menus
 *
 * @param estudante, estrutura com os dados do estudante: nome, matrícula, notas da provas e trabalhos, faltas e sua situação.
 * @param arquivo, estrutura com o nome do arquivo que será utilizado em alguma operação de manipulação de arquivo.
 */
void menuPrincipal(Estudante estudante[80], Arquivo arquivo){
	// Declarações locais
	int const OPC_MAX = 7;
	int opcao = 0; // escolhe opção no menu

	system("cls");
	printf("=========================================\n");
	printf("\t    Menu Principal\n");	
	printf("=========================================\n");
	printf("Opções:\n\n");
	printf("1 - Ler turma de estudantes do arquivo\n2 - Imprimir turma de estudantes\n3 - Imprimir estatísticas da turma\n");
	printf("4 - Cadastrar novo estudante\n5 - Editar informações de um estudante\n6 - Reler turma de estudantes do arquivo\n");
	printf("7 - Salvar e Sair\n\n");
	printf("Digite uma opção: ");
	scanf("%d", &opcao);
	validaOpcao(opcao, OPC_MAX);
	system("cls");
	switch(opcao){
		case 1:
			lerTurma(estudante, arquivo);
			break;
		case 2:
			imprimirTurma(estudante, arquivo);
			break;
		case 3:
			imprimirEstatistica(estudante, arquivo);
			break;
		case 4:
			cadastrarEstudante(estudante, arquivo);
			break;
		case 5:
			editarInfoEstudante(estudante, arquivo);
			break;
		case 6:
			relerTurma(estudante, arquivo);
			break;
		case 7:
			salvaSair(estudante, arquivo);
			break;
		default:
			printf("Opção inválida\n");
	} // switch
	return;
} // menuPrincipal

/**
 * @brief Com base na opção escolhida pelo usuário, essa função o redireciona para outros menus
 *
 * @param estudante, estrutura com os dados do estudante: nome, matrícula, notas da provas e trabalhos, faltas e sua situação.
 * @param arquivo, estrutura com o nome do arquivo que será utilizado em alguma operação de manipulação de arquivo.
 */
void lerTurma(Estudante estudante[80], Arquivo arquivo){
	// Declarações locais
	int const OPC_MAX = 3;
	int opcao = 0; // escolhe opção no menu

	printf("=========================================\n");
	printf("\t    Ler Turma\n");
	printf("=========================================\n");
	printf("Opções:\n\n");
	printf("1 - Ler turma existente\n2 - Criar nova turma\n3 - Voltar ao menu principal\n\n");
	printf("Digite uma opção: ");
	scanf("%d", &opcao);
	validaOpcao(opcao, OPC_MAX);
	system("cls");
	switch(opcao){
		case 1:
			lerTurmaExistente(estudante, arquivo);
			break;
		case 2:
			criarTurmaNova(estudante, arquivo);
			break;
		case 3:
			menuPrincipal(estudante, arquivo);
			break;
	} // switch
	return;
} // lerTurma


/**
 * @brief Lê um arquivo indicado pelo usuário e o salva na memória. Feito isso é possivel 
 *	escolher outro menu como destino. 
 *
 * @param estudante, estrutura com os dados do estudante: nome, matrícula, notas da provas e trabalhos, faltas e sua situação.
 * @param arquivo, estrutura com o nome do arquivo que será utilizado em alguma operação de manipulação de arquivo.
 */
void lerTurmaExistente(Estudante estudante[80], Arquivo arquivo){
	getchar(); // consome o enter: '\n'
	
	printf("Digite o nome do arquivo com a turma: ");
	fgets(arquivo.nomeArquivo, sizeof(arquivo), stdin);
	arquivo.nomeArquivo[strcspn(arquivo.nomeArquivo, "\n")] = 0; // retira o \n no final da string
	system("cls");

	FILE *fp;
	fp = fopen(arquivo.nomeArquivo, "rb");
	if (fp == NULL){
		system("cls");
		fprintf(stderr, "Arquivo %s não existe!\n\n", arquivo.nomeArquivo);
		lerTurma(estudante, arquivo);
	} // if

	else{
		arquivo.qtdAlunos = 0;
		for(int i = 0; fp != NULL; i++){ // Se colocar 80 e não tive 80 vai dar erro, por isso EOF, a responsabilidade por
			if (fgets(estudante[i].nome, sizeof(estudante[i].nome), fp) == NULL){ //(continuacão): colocar até 80 é do usuário
				if(i != 0 && estudante[i-1].nome != NULL) // necessário porque senão ele escreve que tem erro ao ler nome do estudante direto
					break;
			} // if
			estudante[i].nome[strcspn(estudante[i].nome, "\n")] = 0; // necessário, pois senão ele pega \n também

			fgets(estudante[i].matricula, sizeof(estudante[i].matricula), fp);
					
			estudante[i].matricula[strcspn(estudante[i].matricula, "\n")] = 0;

			for(int j = 0; j < sizeof(estudante[i].provas)/sizeof(double); j++){
				fscanf(fp, "%lf", &estudante[i].provas[j]);
			} // for

			for(int j = 0; j < sizeof(estudante[i].trabalhos)/sizeof(double); j++){
				fscanf(fp, "%lf", &estudante[i].trabalhos[j]); 
			} // for
			fscanf(fp, "%d", &estudante[i].faltas); 

			fscanf(fp, "%lf", &estudante[i].notaF); 
		
			fscanf(fp, " %c ", &estudante[i].situacao);

			arquivo.qtdAlunos++;

		} // for
		printf("Arquivo %s lido com sucesso!\n\n", arquivo.nomeArquivo);
	} // else

	fclose(fp);
	lerTurma(estudante, arquivo);

	return;
} // lerTurmaExistente 

/**
 * @brief Cria um novo arquivo para uma nova turma de estudantes indicada pelo usuário e o 
 *	salva na memória. Feito isso é possivel escolher outro menu como destino. 
 *
 * @param estudante, estrutura com os dados do estudante: nome, matrícula, notas da provas e trabalhos, faltas e sua situação.
 * @param arquivo, estrutura com o nome do arquivo que será utilizado em alguma operação de manipulação de arquivo.
 */
void criarTurmaNova(Estudante estudante[80], Arquivo arquivo){ 
	//Declarações locais															
	char opcao = '\0'; // recebe a decisão sobre o destino do arquivo

	getchar(); // consome o enter: '\n'
	printf("Digite o nome do arquivo com a turma: ");
	if (fgets(arquivo.nomeArquivo, sizeof(arquivo), stdin) == NULL){ // Lê o nome do arquivo a ser aberto
    	fprintf(stderr, "Erro ao ler o nome do arquivo\n");
    	exit(1);
	} // if
	else{
		arquivo.nomeArquivo[strcspn(arquivo.nomeArquivo, "\n")] = 0; // retira o \n no final da string
	} // else

	FILE *fp;
	fp = fopen(arquivo.nomeArquivo, "rb"); // Abrir em modo leitura só para checar se o arquivo existe
								  
	if(fp == NULL){ // precisa fechar o arquivo de leitura, ele se fechou automaticamente quando recebu NULL
		fp = fopen(arquivo.nomeArquivo, "wb");
		fclose(fp);
		system("cls");
		printf("Arquivo %s criado com sucesso.\n\n", arquivo.nomeArquivo);
	} // if

	else{
		fclose(fp);
		system("cls");
		printf("Arquivo %s já existe. Deseja sobrescrevê-lo? (S/N)\n\n", arquivo.nomeArquivo);
		scanf("%s", &opcao);
		if(opcao == 'N'); // não faz nada com arquivo
		else if(opcao == 'S'){ // sobrescreve o arquivo
			fp = fopen(arquivo.nomeArquivo, "wb");
			if(fp == NULL){
				fprintf(stderr, "Erro ao abrir o arquivo %s.", arquivo.nomeArquivo);
				exit(1);
			}
			fclose(fp);
		} // else if
		system("cls");
	} // else
	lerTurma(estudante, arquivo);
} // criarTurmaNova

/**
 * @brief Imprime os dados da turma
 *
 * @param estudante, estrutura com os dados do estudante: nome, matrícula, notas da provas e trabalhos, faltas e sua situação.
 * @param arquivo, estrutura com o nome do arquivo que será utilizado em alguma operação de manipulação de arquivo.
 */
void imprimirTurma(Estudante estudante[80], Arquivo arquivo){
	getchar(); 
	if(arquivo.qtdAlunos == 0) 
		printf("Não há turma carregada!\n");
	else{
		imprimeCabecalho();
		for(int i=0; i < arquivo.qtdAlunos; i++){
			printf("%-50s", estudante[i].nome); 
			printf("%-13s", estudante[i].matricula);
			for(int j = 0; j < sizeof(estudante[i].provas)/sizeof(double); j++)
				printf("%-5.0lf", estudante[i].provas[j]);
			for(int j = 0; j < sizeof(estudante[i].trabalhos)/sizeof(double); j++)
				printf("%-5.0lf", estudante[i].trabalhos[j]);
			printf("%4d", estudante[i].faltas); 
			printf("%5.0lf", estudante[i].notaF);
			printf("%9c", estudante[i].situacao);
			puts("");
		} // for
		imprimeLinha();
	} // else

	getchar(); 
	menuPrincipal(estudante, arquivo);
	return;
} // imprimirTurma

/**
 * @brief Imprime a estatística das  notas da turma carregada
 *
 * @param estudante, estrutura com os dados do estudante: nome, matrícula, notas da provas e trabalhos, faltas e sua situação.
 * @param arquivo, estrutura com o nome do arquivo que será utilizado em alguma operação de manipulação de arquivo.
 */
void imprimirEstatistica(Estudante estudante[80], Arquivo arquivo){
	// Declarações locais
	Notas notas;
	Estatistica estat;

	getchar();
	if(arquivo.qtdAlunos == 0)
		printf("Não há turma carregada!\n");

	else{
		notas = filtraNotas(estudante, arquivo, notas);
		estat = fazEstatistica(estudante, arquivo, notas, estat);
		cabecalhoEstatisca();
		tabelaNotas(estudante, arquivo, notas);
		mostraEstatistica(estudante, arquivo, notas, estat);
	} // else
	getchar();
	menuPrincipal(estudante, arquivo);

	return;
} // imprimirEstatistica

/**
 * @brief Cadastra estudante
 *
 * @param estudante, estrutura com os dados do estudante: nome, matrícula, notas da provas e trabalhos, faltas e sua situação.
 * @param arquivo, estrutura com o nome do arquivo que será utilizado em alguma operação de manipulação de arquivo.
 */
void cadastrarEstudante(Estudante estudante[80], Arquivo arquivo){
	char nome[50], matricula[12] = {'\0'};
	double p1, p2, p3, t1, t2, final = 0.0;
	int faltas = 0;
	char situacao;
	char continuar;
	if (strlen(arquivo.nomeArquivo) == 0) {
		printf("Não há turma carregada!\n");
		getchar();
		menuPrincipal(estudante, arquivo);
	} else {
		printf("Digite o nome: ");
		getchar();
		if (fgets(nome, sizeof(nome), stdin) == NULL){
			fprintf(stderr, "Erro ao ler nome\n");
			exit(1);
		}

		printf("Digite a matrícula: ");
		if (fgets(matricula, sizeof(matricula), stdin) == NULL){
			fprintf(stderr, "Erro ao ler matricula\n");
			exit(1);
		}
		getchar(); // necessário para consumir o enter
		printf("Digite as três notas das provas: ");
		scanf("%lf %lf %lf", &p1, &p2, &p3);
		printf("Digite as duas notas dos trabalhos: ");
		scanf("%lf %lf", &t1, &t2);
		printf("Digite o número de faltas: ");
		scanf("%d", &faltas);
		final = p1 + p2 + p3 + t1 + t2;
		printf("\n\nNota final calculada: %.2lf\n", final);
		if (final < 60) {
			situacao = 'R';
			printf("Situação final: Reprovado\n");
		} else if (faltas > 18) {
			situacao = 'F';
			printf("Situação final: Reprovado por faltas\n");
		} else {
			situacao = 'A';
			printf("Situação final: Aprovado\n");
		}

		
		FILE *fpab;
		fpab = fopen(arquivo.nomeArquivo, "ab");

		fprintf(fpab, "%s%s\n%.2lf\n%.2lf\n%.2lf\n%.2lf\n%.2lf\n%d\n%.2lf\n%c\n", nome, matricula, p1, p2, p3, t1, t2, faltas, final, situacao);

		fclose(fpab);


		FILE *fp;
		fp = fopen(arquivo.nomeArquivo, "rb"); 
		if (fp == NULL){
			system("cls");
			fprintf(stderr, "Arquivo %s não existe!\n\n", arquivo.nomeArquivo);
			lerTurma(estudante, arquivo);
		} // if

		else{
			arquivo.qtdAlunos = 0;
			for(int i = 0; fp != NULL; i++){
				if (fgets(estudante[i].nome, sizeof(estudante[i].nome), fp) == NULL){ 
					if(i != 0 && estudante[i-1].nome != NULL)
						break;
				} // if
				estudante[i].nome[strcspn(estudante[i].nome, "\n")] = 0;

				fgets(estudante[i].matricula, sizeof(estudante[i].matricula), fp);
						
				estudante[i].matricula[strcspn(estudante[i].matricula, "\n")] = 0;

				for(int j = 0; j < sizeof(estudante[i].provas)/sizeof(double); j++){
					fscanf(fp, "%lf", &estudante[i].provas[j]);
				} // for

				for(int j = 0; j < sizeof(estudante[i].trabalhos)/sizeof(double); j++){
					fscanf(fp, "%lf", &estudante[i].trabalhos[j]); 
				} // for
				fscanf(fp, "%d", &estudante[i].faltas); 

				fscanf(fp, "%lf", &estudante[i].notaF); 
			
				fscanf(fp, " %c ", &estudante[i].situacao);

				arquivo.qtdAlunos++;

			} // for
		} // else

		fclose(fp);
		
		getchar();
		getchar();
		menuPrincipal(estudante, arquivo);
		return;
	}
} // cadastrarEstudante

/**
 * @brief Edita informações, procurando aluno por matrícula ou nome.
 *
 * @param estudante, estrutura com os dados do estudante: nome, matrícula, notas da provas e trabalhos, faltas e sua situação.
 * @param arquivo, estrutura com o nome do arquivo que será utilizado em alguma operação de manipulação de arquivo.
 */
void editarInfoEstudante(Estudante estudante[80], Arquivo arquivo){
	getchar();
	int const OPC_MAX = 3;
	int opcao = 0;
	system("cls");
	if (strlen(arquivo.nomeArquivo) == 0) {
		printf("Não há turma carregada!\n");
		getchar();
		menuPrincipal(estudante, arquivo);
	} else {
		printf("=========================================\n");
		printf("\t Editar Informações do Estudante\n");
		printf("=========================================\n");
		printf("Opções:\n\n");
		printf("1 - Selecionar por número de matrícula\n2 - Selecionar por nome\n3 - Voltar ao menu principal\n\n");
		printf("Digite uma opção: ");
		scanf("%d", &opcao);
		validaOpcao(opcao, OPC_MAX);
		system("cls");
		switch(opcao){
			case 1:
				validaMatricula(estudante, arquivo);
				break;
			case 2:
				validaNome(estudante, arquivo);
				break;
			case 3:
				menuPrincipal(estudante, arquivo);
				break;
		} // switch
		return;
	}
} // editarInfoEstudante

/**
 * @brief Valida matrícula
 *
 * @param estudante, estrutura com os dados do estudante: nome, matrícula, notas da provas e trabalhos, faltas e sua situação.
 * @param arquivo, estrutura com o nome do arquivo que será utilizado em alguma operação de manipulação de arquivo.
 */
void validaMatricula(Estudante estudante[80], Arquivo arquivo){
	int indice = -1;
	char matric[12] = {'\0'};
	char continuar;
	printf("Digite a matrícula: ");
	fgets(matric, sizeof(matric), stdin);
	matric[strcspn(matric, "\n")] = '\0';
	if (fgets(matric, sizeof(matric), stdin) == NULL){
		fprintf(stderr, "Erro ao ler matrícula\n");
		exit(1);
	}
	getchar(); // necessário para consumir o enter
	for(int i = 0; i < arquivo.qtdAlunos; i++) {
		if(!strcmp(matric, estudante[i].matricula)) indice = i; 
	}
	if(indice == -1){
		printf("Número de matrícula inválido!\n");
		//printf("Digite algo para voltar ao menu anterior...");
		//scanf("%s", &continuar);
		editarInfoEstudante(estudante, arquivo);
	} else editaInformacoes(indice, estudante, arquivo);
	return;
} // validaMatricula

/**
 * @brief Valida nome
 *
 * @param estudante, estrutura com os dados do estudante: nome, matrícula, notas da provas e trabalhos, faltas e sua situação.
 * @param arquivo, estrutura com o nome do arquivo que será utilizado em alguma operação de manipulação de arquivo.
 */
void validaNome(Estudante estudante[80], Arquivo arquivo){
	int indice = -1;
	char nome[51] = {'\0'};
	char continuar;
	printf("Digite o nome: ");
	//fgets(nome, sizeof(nome), stdin);
	//nome[strcspn(nome, "\n")] = '\0';
	getchar();

	if (fgets(nome, sizeof(nome), stdin) == NULL){
		fprintf(stderr, "Erro ao ler nome\n");
		exit(1);
	}
	nome[strcspn(nome, "\n")] = '\0';
	//getchar(); // necessário para consumir o enter
	//getchar();
	for(int i = 0; i < arquivo.qtdAlunos; i++) {
		if(!strcmp(nome, estudante[i].nome)) indice = i; 
	}
	if(indice == -1){
		printf("Nome inválido!\n");
		editarInfoEstudante(estudante, arquivo);
	} else editaInformacoes(indice, estudante, arquivo);
	return;
} // validaNome

/**
 * @brief Edita as informações que o usuário desejar
 *
 * @param estudante, estrutura com os dados do estudante: nome, matrícula, notas da provas e trabalhos, faltas e sua situação.
 * @param arquivo, estrutura com o nome do arquivo que será utilizado em alguma operação de manipulação de arquivo.
 */
void editaInformacoes(int indice, Estudante estudante[80], Arquivo arquivo){
	int const OPC_MAX = 9;	
	int opcao = 0;
	int i = indice;
	system("cls");
	printf("=========================================\n");
	printf("\t Editar Informações do Estudante\n");
	printf("=========================================\n");
	printf("Opções:\n\n");
	printf("1 - Alterar nome\n2 - Alterar matrícula\n3 - Alterar nota da prova 1\n4 - Alterar nota da prova 2\n5 - Alterar nota da prova 3\n");
	printf("6 - Alterar nota do trabalho 1\n7 - Alterar nota do trabalho 2\n8 - Alterar número de faltas\n9 - Voltar ao menu anterior\n\n");
	printf("Digite uma opção: ");
	scanf("%d", &opcao);
	validaOpcao(opcao, OPC_MAX);
	system("cls");
	switch(opcao){
		case 1:
			printf("Digite o nome: ");
			getchar();
			if (fgets(estudante[i].nome, sizeof(estudante[i].nome), stdin) == NULL){
				fprintf(stderr, "Erro ao ler nome\n");
				exit(1);
			}
			estudante[i].nome[strcspn(estudante[i].nome, "\n")] = '\0';
			break;
		case 2:
			printf("Digite a matrícula: ");
			getchar();
			if (fgets(estudante[i].matricula, sizeof(estudante[i].matricula), stdin) == NULL){
				fprintf(stderr, "Erro ao ler matrícula\n");
				exit(1);
			}
			estudante[i].matricula[strcspn(estudante[i].matricula, "\n")] = '\0';
			break;
		case 3:
			printf("Digite a nota da prova 1: ");
			fscanf(stdin, "%lf", &estudante[i].provas[0]);
			break;
		case 4:
			printf("Digite a nota da prova 2: ");
			fscanf(stdin, "%lf", &estudante[i].provas[1]);
			break;
		case 5:
			printf("Digite a nota da prova 3: ");
			fscanf(stdin, "%lf", &estudante[i].provas[2]);
			break;
		case 6:
			printf("Digite a nota do trabalho 1: ");
			fscanf(stdin, "%lf", &estudante[i].trabalhos[0]);
			break;
		case 7:
			printf("Digite a nota do trabalho 2: ");
			fscanf(stdin, "%lf", &estudante[i].trabalhos[1]);
			break;
		case 8:
			printf("Digite o número de faltas: ");
			fscanf(stdin, "%d", &estudante[i].faltas);
			break;
		case 9:
			editarInfoEstudante(estudante, arquivo);
			break;
	} // switch
	estudante[i].notaF = estudante[i].provas[0] + estudante[i].provas[1] + estudante[i].provas[2] + estudante[i].trabalhos[0] + estudante[i].trabalhos[1];
	if (estudante[i].notaF < 60) {
			estudante[i].situacao = 'R';
		} else if (estudante[i].faltas > 18) {
			estudante[i].situacao = 'F';
		} else {
			estudante[i].situacao = 'A';
		}
	editaInformacoes(indice, estudante, arquivo);
	return;
}

/**
 * @brief Dá um refresh no arquivo, lendo para a struct de novo
 *
 * @param estudante, estrutura com os dados do estudante: nome, matrícula, notas da provas e trabalhos, faltas e sua situação.
 * @param arquivo, estrutura com o nome do arquivo que será utilizado em alguma operação de manipulação de arquivo.
 */
void relerTurma(Estudante estudante[80], Arquivo arquivo){
	getchar();
	system("cls");
	if (strlen(arquivo.nomeArquivo) == 0) {
		printf("Não há turma carregada!\n");
		getchar();
		menuPrincipal(estudante, arquivo);
	} else{
		FILE *fp;
		fp = fopen(arquivo.nomeArquivo, "rb");
		arquivo.qtdAlunos = 0;
		for(int i = 0; fp != NULL; i++){ // Se colocar 80 e não tive 80 vai dar erro, por isso EOF, a responsabilidade por
			if (fgets(estudante[i].nome, sizeof(estudante[i].nome), fp) == NULL){ //(continuacão): colocar até 80 é do usuário
				if(i != 0 && estudante[i-1].nome != NULL) // necessário porque senão ele escreve que tem erro ao ler nome do estudante direto
					break;
			} // if
			estudante[i].nome[strcspn(estudante[i].nome, "\n")] = 0; // necessario, pois senão ele pega \n também

			fgets(estudante[i].matricula, sizeof(estudante[i].matricula), fp);
					
			estudante[i].matricula[strcspn(estudante[i].matricula, "\n")] = 0;

			for(int j = 0; j < sizeof(estudante[i].provas)/sizeof(double); j++){
				fscanf(fp, "%lf", &estudante[i].provas[j]); 
			} // for

			for(int j = 0; j < sizeof(estudante[i].trabalhos)/sizeof(double); j++){
				fscanf(fp, "%lf", &estudante[i].trabalhos[j]); 
			} // for
			fscanf(fp, "%d", &estudante[i].faltas); 

			fscanf(fp, "%lf", &estudante[i].notaF); 
		
			fscanf(fp, " %c ", &estudante[i].situacao);

			arquivo.qtdAlunos++;

		} // for
		fclose(fp);
		printf("Arquivo %s lido com sucesso!\n\n", arquivo.nomeArquivo);
		getchar();
		menuPrincipal(estudante, arquivo);
	}
} // relerTurma

/**
 * @brief Se o usuário quiser, salva tudo no arquivo e sai, se não quiser, só sai
 *
 * @param estudante, estrutura com os dados do estudante: nome, matrícula, notas da provas e trabalhos, faltas e sua situação.
 * @param arquivo, estrutura com o nome do arquivo que será utilizado em alguma operação de manipulação de arquivo.
 */
void salvaSair(Estudante estudante[80], Arquivo arquivo){
	char nome[50], matricula[12] = {'\0'};
	double p1, p2, p3, t1, t2, final = 0.0;
	int faltas = 0;
	char situacao = '\0';
	char opcao = '\0';

	system("cls");
	if (strlen(arquivo.nomeArquivo) == 0) {
		printf("Não há turma carregada!\n");
		getchar();
		getchar();
		menuPrincipal(estudante, arquivo);
	} // if
	else{
		printf("Arquivo %s já existe. Deseja sobrescrevê-lo? (S/N) ", arquivo.nomeArquivo);
		scanf(" %c", &opcao);
		if(opcao == 'S' || opcao == 's'){

			FILE *fp;
			fp = fopen(arquivo.nomeArquivo, "wb");
			for(int i = 0; i < arquivo.qtdAlunos; i++){
				//fgets(nome, sizeof(nome), estudante[i].nome);
				//fgets(matricula, sizeof(matricula), estudante[i].matricula);
				
				p1 = estudante[i].provas[0];
				p2 = estudante[i].provas[1];
				p3 = estudante[i].provas[2];
				t1 = estudante[i].trabalhos[0];
				t2 = estudante[i].trabalhos[0];
				faltas = estudante[i].faltas;
				final = estudante[i].notaF;
				situacao = estudante[i].situacao;

				fprintf(fp, "%s\n%s\n%.2lf\n%.2lf\n%.2lf\n%.2lf\n%.2lf\n%d\n%.2lf\n%c\n", estudante[i].nome, estudante[i].matricula, p1, p2, p3, t1, t2, faltas, final, situacao);
			} // for
			fclose(fp);
		} // if
		else if(opcao == 'N' || opcao == 'n'){} // else if
	} // else
	printf("Até a próxima.\n");
	exit(1);
} // salvaSair
