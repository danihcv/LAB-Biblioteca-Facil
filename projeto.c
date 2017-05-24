#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define maxStrLen 10000
#define qntdPreposicoes 5
///##############################################Vari�veis globais:##################################################///
char opcao, digito, lixo[maxStrLen];
char espacamento[] = "\t    ";
int tomboGeral = 0;
const int qntdMaxResultadosPorPagina = 5;
char preposicoes[qntdPreposicoes][5] = {{"de"}, {"da"}, {"do"}, {"das"}, {"dos"}};
struct Obra{
    int numeroExemplar;
    char strTombo[maxStrLen], strDataCompra[maxStrLen];
};

struct Tombo { //Tombo (n�mero do tombo, nome da obra, nome do autor, nome da editora, c�digo da �rea)
    int codigoArea;
    char nomeObra[maxStrLen], nomeAutor[maxStrLen], nomeEditora[maxStrLen];
};

struct Consulta{
    int numeroExemplar, codigoArea, tombo, dd, mm, aa;
    char nomeObra[maxStrLen], nomeAutor[maxStrLen], nomeEditora[maxStrLen];
};

typedef struct Consulta consulta;

struct Tombo tombo;
struct Obra obra;
consulta consul[500];
//rela��o das FUN��ES dos SUBMENUS:
char opcoes[2][10][100] = {
        {{"Cadastrar Obra"}, {"Cadastrar Tombo"}},
        {{"Mostrar obras por area"}, {"Mostrar obras por autor"}, {"Mostrar obras por editora"}, {"Mostrar acervo"}, {"Mostrar tamanho do acervo"}},
};
int qntdOpcoes[2] = {2, 5}; //quantidade de op��es em cada submenu

///#############################################SUM�RIO DE FUN��ES###################################################///
int abrirArquivo(FILE *arquivo, char nomeArquivo[100], char tipoAbertura[2]);
void limparTela();
void animacaoFuncao(int posicao, int nFuncao, char strFuncao[100]);
void realizarFunc(int submenu, int funcao);
void menuPrincipal();
void imprimirSubmenu(int submenuSelecionado, char strSubmenuSelecionado[100], int nOpcoesSubmenu, char strOpcoesSubmenu[nOpcoesSubmenu][100]);
int verificarTombo(char nomeArquivo[1000], int numeroTombo);
int proximoNumeroExemplar(int tombo);
char* stringMinuscula(char *str);
char* stringFormatada(char str[]);
int ehPreposicao(char str[]);
int stringCompare(char str1[], char str2[]);
int stringDeInt(int strLen);
int avancaPagina(int resultados, int i, int j);
int encaminharParaOutroCadastro();
void encaminharParaOutraConsulta();
void cadastroObra();
void cadastroTombo();
void exibirConsulta(consulta consul[], int resultados, int nFuncao);
void consultaArea(consulta consul[], int nFuncao);
void consultaAutor(consulta consul[], int nFuncao);
void consultaEditora(consulta consul[], int nFuncao);
void consultaAcervo(consulta consul[], int nFuncao);
void consultaTamanhoAcervo();

///implementa��o da fun��o getch, para que funcione tanto em LINUX quanto em WINDOWS:
#ifdef __WIN32__
#include <conio.h>

#else
#include <termios.h>

    int getch(void)
    {
            struct termios velho, novo;
            int x;

            tcgetattr( STDIN_FILENO, &velho );
            novo = velho;
            novo.c_lflag &= ~( ICANON | ECHO );
            tcsetattr( STDIN_FILENO, TCSANOW, &novo );
            x= getchar();
            tcsetattr( STDIN_FILENO, TCSANOW, &velho );
            return x;
    }
#endif // __LINUX__

///*############################################Fun��es Auxiliares##################################################*///
/*/////////////////////////fun��o para abrir arquivo e verificar se foi aberto com sucesso:///////////////////////////*/
int abrirArquivo(FILE *arquivo, char nomeArquivo[100], char tipoAbertura[2]){
    arquivo = fopen(nomeArquivo, tipoAbertura);
    if (arquivo == NULL)
    {
        printf("ERRO! O arquivo n�o pode ser aberto!\n");
        return 1;
    }else{
        fclose(arquivo);
        return 0;
    }
}
/*////////////////////////////////////////////fun��o para limpar a tela://////////////////////////////////////////////*/
void limparTela(){
#ifdef __WIN32__
    system("CLS");
#else
    //printf("\e[H\e[2J");
        system("clear");
#endif
}
/*////////////////////////////////////////////fun��o para limpar o buffer://////////////////////////////////////////////*/
void limparBuffer(){
#ifdef __WIN32__
    fflush(stdin);
#else
    __fpurge(stdin);
#endif
}
/*////////////////////////////////////fun��o de anima��o de SUBMENU selecionado://////////////////////////////////////*/
void animacaoFuncao(int posicao, int nFuncao, char strFuncao[100]){
    usleep(75000);
    limparTela();
    int i, c;
    //imprimir cabe�alho:
    printf("\n%s..................................\n", espacamento);
    printf("%s:        Biblioteca F�cil        :\n", espacamento);
    printf("%s --------------------------------\n", espacamento);
    for(i = posicao; i > 2; i--){
        printf("\n");
    }
    if(posicao > 1)
        printf("%s --------------------------------\n", espacamento);
    printf("%s|%d. %s", espacamento, nFuncao, strFuncao);
    for(c = strlen(strFuncao) + 4; c <= 32; c++){
        printf(" ");
    }
    printf("|\n");
    printf("%s --------------------------------\n", espacamento);
    if(posicao > 1){
        animacaoFuncao(posicao-1, nFuncao, strFuncao);
    }
}
/*/////////////////////////////fun��o usada para chamar a FUNCAO selecionada pelo usu�rio:////////////////////////////*/
void realizarFunc(int submenu, int funcao){
    if(submenu == 1){ //se o SUBMENU selecionado foi CADASTROS
        if(funcao == 1){ //se a FUN��O selecionada foi CADASTRAR OBRA
            cadastroObra();
        }
        else if(funcao == 2){ //se a FUN��O selecionada foi CADASTRAR TOMBO
            cadastroTombo();
        }
    }
    else if(submenu == 2){ //se o SUBMENU selecionado foi CONSULTAS
        if(funcao == 1){
            consultaArea(consul, 1);
        }
        else if(funcao == 2){
            consultaAutor(consul, 2);
        }
        else if(funcao == 3){
            consultaEditora(consul, 3);
        }
        else if(funcao == 4){
            consultaAcervo(consul, 4);
        }
        else if (funcao == 5) {
            consultaTamanhoAcervo();
        }
    }
}

/*//////////////////////////////////////fun��o para imprimir o MENU PRINCIPAL:////////////////////////////////////////*/
void menuPrincipal(){
    printf("\n%s..................................\n", espacamento);
    printf("%s:        Biblioteca F�cil        :\n", espacamento);
    printf("%s --------------------------------\n", espacamento);
    printf("%s|  1. Cadastros                  |\n", espacamento);
    printf("%s|  2. Consultas                  |\n", espacamento);
    printf("%s|  0. Sair                       |\n", espacamento);
    printf("%s --------------------------------\n", espacamento);

    opcao = (char)getch();
    printf("%c\n", opcao);
    if(opcao == '0'){
        while(printf("\n -Tem certeza que deseja sair? S ou N?\n > "), scanf(" %c", &opcao)){

            fgets(lixo, maxStrLen, stdin);
            if(toupper(opcao) == 'N'){
                limparTela();
                menuPrincipal();
                break;
            }
            else if(toupper(opcao) == 'S')
                break;
            else if(opcao != '\n')
                printf("\n%c n�o � uma op��o v�lida.", opcao);
        }
    }
    else if (opcao == '1'){
        limparTela();
        imprimirSubmenu((int)opcao - 48, "Cadastros", qntdOpcoes[1-1], opcoes[1-1]);
    }
    else if (opcao == '2'){
        limparTela();
        imprimirSubmenu((int)opcao - 48, "Consultas", qntdOpcoes[(int)opcao - 49], opcoes[(int)opcao - 49]);
    }
    else {
        limparTela();
        if(opcao != '\n')
            printf("%c => Op��o inv�lida! Por favor selecione uma op��o v�lida:", opcao);
        menuPrincipal();
    }
}
/*//////////////////////////////////////fun��o para imprimir os SUBMENUS://///////////////////////////////////////////*/
void imprimirSubmenu(int submenuSelecionado, char strSubmenuSelecionado[100], int nOpcoesSubmenu, char strOpcoesSubmenu[nOpcoesSubmenu][100]){
    int c, i;
    //imprimir cabe�alho:
    printf("\n%s..................................\n", espacamento);
    printf("%s:        Biblioteca F�cil        :\n", espacamento);
    printf("%s --------------------------------\n", espacamento);
    //imprimir SUBMENU selecionada:
    printf("%s|%d. %s", espacamento, submenuSelecionado, strSubmenuSelecionado);
    for(i = (strlen(strSubmenuSelecionado) + 3); i < 32; i++){
        printf(" ");
    }
    printf("|\n");
    printf("%s --------------------------------\n", espacamento);
    //imprimir op��es do submenu selecionado:
    for(i = 0; i < nOpcoesSubmenu; i++){
        printf("%s|  %d. %s", espacamento, i+1, strOpcoesSubmenu[i]);
        for(c = strlen(strOpcoesSubmenu[i]) + 6; c <= 32; c++){
            printf(" ");
        }
        printf("|\n");
    }
    printf("%s|  0. Voltar                     |\n", espacamento);
    printf("%s --------------------------------\n", espacamento);
    //leitura da proxima escolha:
    opcao = (char)getch();
    if(opcao == '0'){
        limparTela();
        menuPrincipal();
    }
    else if((int)opcao - 48 <= qntdOpcoes[submenuSelecionado-1] && (int)opcao - 48 >= 0){
        limparTela();
        animacaoFuncao((int)opcao - 46, (int)opcao - 48, opcoes[submenuSelecionado-1][(int)opcao - 49]);
        realizarFunc(submenuSelecionado, (int)opcao - 48);
    }
    else {
        limparTela();
        if(opcao != '\n')
            printf("%c => Op��o inv�lida! Por favor selecione uma op��o v�lida:", opcao);
        imprimirSubmenu(submenuSelecionado, strSubmenuSelecionado, nOpcoesSubmenu, strOpcoesSubmenu);
    }
}
/*////////////////////////fun��o para verificar se o tombo referido � novo ou existente://////////////////////////////*/
int verificarTombo(char nomeArquivo[1000], int numeroTombo){
    int numeroTomboTemp;
    FILE *arquivoLeitura, *arquivo2Leitura;
    arquivoLeitura = fopen(nomeArquivo, "r");
    while(fscanf(arquivoLeitura, "%d | %[^\n]\n", &numeroTomboTemp, lixo) != EOF){
        if(numeroTomboTemp == numeroTombo){
            fclose(arquivoLeitura);
            return 1; //retorna 1 caso o tombo j� esteja cadastrado
        }
    }
    arquivo2Leitura = fopen("tombos.txt", "r");
    while(fscanf(arquivo2Leitura, "%d | %[^\n]\n", &numeroTomboTemp, lixo) != EOF) {
        if(numeroTomboTemp == numeroTombo){
            fclose(arquivoLeitura);
            fclose(arquivo2Leitura);
            return 0; //retorna 0 caso o tombo esteja cadastrado no outro arquivo, logo possa ser selecionado
        }
    }
    fclose(arquivoLeitura);
    fclose(arquivo2Leitura);
    return 2;//retorna 2 caso o tombo seja inv�lido/inexistente
}
/*////////////////////////fun��o para definir o numero do prox exemplar a ser cadastrado://///////////////////////////*/
int proximoNumeroExemplar(int tombo){
    char confNomeObra[maxStrLen], confNomeAutor[maxStrLen], buscaNomeAutor[maxStrLen], buscaNomeObra[maxStrLen];
    FILE *arquivo;
    arquivo = fopen("tombos.txt", "r");
    int confTombo, exemplar = 0;
    while(fscanf(arquivo, "%d | %[^|]| %[^|]|%[^\n]\n", &confTombo, confNomeObra, confNomeAutor, lixo) != EOF){
        if(tombo == confTombo) {
            strcpy(buscaNomeObra, confNomeObra);
            strcpy(buscaNomeAutor, confNomeAutor);
            break;
        }
    }
    fclose(arquivo);
    arquivo = fopen("tombos.txt", "r");
    while(fscanf(arquivo, "%d | %[^|]| %[^|]|%[^\n]\n", &confTombo, confNomeObra, confNomeAutor, lixo) != EOF && confTombo <= tombo){
        if((strcmp(buscaNomeObra, confNomeObra) == 0) && (strcmp(buscaNomeAutor, confNomeAutor) == 0))
            exemplar++;
    }
    fclose(arquivo);
    return exemplar;
}

/*//////////////////////////////fun��o que retorna a string toda em maiusculo:////////////////////////////////////////*/
char* stringMinuscula(char *str){
    int i;
    for(i = 0; i < strlen(str); i++)
        str[i] = (char)tolower(str[i]);
    return str;
}
/*//////////////////////////////fun��o que formata a string de forma correta://///////////////////////////////////////*/
char* stringFormatada(char str[]){
    int offset;
    char temp[1000], dest[1000];
    dest[0] = '\0';
    while(sscanf(str, " %s%n", temp, &offset) == 1){
        if(!ehPreposicao(temp))
            temp[0] = (char)toupper(temp[0]);
        strcat(dest, temp);
        strcat(dest, " ");
        str += offset;
    }
    strcpy(str, dest);
    str[0] = (char)toupper(str[0]);
    return str;
}
/*//////////////////////////////fun��o que checa se a string � uma preposi��o:////////////////////////////////////////*/
int ehPreposicao(char str[]){
    int i;
    for(i = 0; i < qntdPreposicoes; i++)
        if(strcmp(str, preposicoes[i]) == 0)
            return 1;
    return 0;
}
/*/////////////////////fun��o que compara 2 strings para saber se o inicio delas � igual://///////////////////////////*/
int stringCompare(char str1[], char str2[]){
    int i;
    for(i = 0; i < strlen(str1)-1 && i < strlen(str2)-1; i++) //esse "-1" tem que estar aqui, pois a str1 vem com um caractere " " ao final dela
        if(str1[i] != str2[i])
            return 1; //retorna 1 se as strings forem direntes
    return 0; //retorna 0 se as strings forem iguais
}
/*////////////////fun��o que s� aceita teclas de inteiros e retorna a string final dos intieiros://///////////////////*/
int stringDeInt(int strLen){
    int idx = 0, valor = 0;
    char str[maxStrLen];
    do {
        digito = (char) getch();
        if (0 <= digito - '0' && digito - '0' < 10 && idx < maxStrLen) {
            putchar(digito);
            str[idx++] = digito;
        } else if (((int) digito == 8 || (int) digito == 127) && idx > 0) { //"8" � o inteiro usado pra representar a tecla "backspace" e "127" � o inteirousado para representar a tecla "delete"
            putchar('\b');
            putchar(' ');
            putchar('\b');
            str[idx] = '\0';
            str[--idx] = '\0';
        } else if ((int) digito == 27) { //"27" � o inteiro usado pra representar a tecla "esc"
            memset(str, '\0', (size_t) idx);
            valor = -1;
            return valor;
        }
    } while (((int) digito != 13 && (int) digito != 10) || idx < strLen); //"13" � o inteiro usado pra representar a tecla "enter"
    valor = atoi(str);
    return valor;
}
/*//////////////////////fun��o que controla a navega��o nas paginas de resultados:////////////////////////////////////*/
int avancaPagina(int resultados, int i, int j){
    int op, ok;

#ifdef __WIN32__
    do {
        op = getch();
        ok = 1;
        if (op == 224) { //224 � o inteiro que indica que uma seta foi apertada
            op = getch();
            if (op == 75 && i >= qntdMaxResultadosPorPagina){
                return 0;
            }
            else if(op == 75) //75 indica que a seta apertada foi a "<-"
                ok = 0;
            else if(i+j == resultados)
                ok = 0;
            else if(op != 77) //77 indica que a seta apertada foi a "->"
                ok = 0;
            limparBuffer();
        }
        else if (op == 27) { //27 � o inteiro correspondente a tecla ESC
            limparTela();
            return -1;
        }
        else
            ok = 0;
    }while(!ok);
    return 1;
#else
    do {
        op = getch();
        ok = 1;
        if (op == 'a' || op == 'd') {
            if (op == 'a' && i >= qntdMaxResultadosPorPagina){
                return 0;
            }
            else if(op == 'a')
                ok = 0;
            else if(i+j == resultados)
                ok = 0;
            else if(op != 'd')
                ok = 0;
            limparBuffer();
        }
        else if (op == 27) { //27 � o inteiro correspondente a tecla ESC
            limparTela();
            return -1;
        }
        else
            ok = 0;
    }while(!ok);
    return 1;
#endif
}

///*#######################################Fun��es de "Deseja continuar?"###########################################*///
/*////////////////////////fun��o que pergunta se o usu�rio deseja fazer outro cadastro:///////////////////////////////*/
int encaminharParaOutroCadastro(){
    while(printf("\n -Deseja continuar cadastrando? S ou N?\n > "), scanf(" %c", &opcao)){
        fgets(lixo, maxStrLen, stdin);
        if(toupper(opcao) == 'N'){
            return 0;
        }
        else if(toupper(opcao) == 'S'){
            return 1;
        }
        else
            printf("\n%c n�o � uma op��o v�lida.", opcao);
    }
    return 0; //return de seguran�a, para o caso de o loop dar algum problema inesperado
}
/*//////////////////////////fun��o que pergunta se o usu�rio deseja fazer outra consulta://///////////////////////////*/
void encaminharParaOutraConsulta(){
    limparTela();
    while(printf("\n -Deseja fazer outra consulta? S ou N?\n > "), scanf(" %c", &opcao)) {
        fgets(lixo, maxStrLen, stdin);
        if (toupper(opcao) == 'N') {
            limparTela();
            menuPrincipal();
            break;
        } else if (toupper(opcao) == 'S') {
            limparTela();
            imprimirSubmenu(2, "Consultas", qntdOpcoes[1], opcoes[1]);
            break;
        } else
            printf("\n%c n�o � uma op��o v�lida.", opcao);
    }
}

///*########################################Fun��es de Cadastros####################################################*///
/*///////////////////////////////////////////fun��o para cadastrar OBRAS://///////////////////////////////////////////*/
void cadastroObra(){
    FILE *arquivo = NULL;
    int verificador, verificacaoTombo, localTombo = 0, idx = 0;
    verificador = abrirArquivo(arquivo, "obras.txt", "a");
    if(!verificador){
        arquivo = fopen("obras.txt", "a");
        printf("-Insira o n�mero do tombo:\n  > ");
        localTombo = stringDeInt(1);
        if(localTombo == -1) {
            limparTela();
            printf("\n   Cadastro cancelado!");
            if (encaminharParaOutroCadastro() == 1) {
                animacaoFuncao(1, 1, opcoes[0][0]);
                cadastroObra();
            } else {
                limparTela();
                menuPrincipal();
            }
        }
        else{
            verificacaoTombo = verificarTombo("obras.txt", localTombo);
            if (verificacaoTombo != 0) {
                do {
                    if(verificacaoTombo == 1)
                        printf("\t **tombo j� cadastrado**\n");
                    else
                        printf("\t **tombo inv�lido**\n");
                    printf("*Insira outro n�mero de tombo:\n  > ");
                    localTombo = stringDeInt(1);

                    if(localTombo == -1){
                        limparTela();
                        printf("\n   Cadastro cancelado!");
                        if(encaminharParaOutroCadastro() == 1){
                            animacaoFuncao(1, 1, opcoes[0][0]);
                            cadastroObra();
                        }
                        else {
                            limparTela();
                            menuPrincipal();
                        }
                    }
                    verificacaoTombo = verificarTombo("obras.txt", localTombo);
                } while (verificacaoTombo != 0 && localTombo != -1);
            }
            if(localTombo != -1) {
                printf("\t **tombo selecionado**\n");

                printf("\n-Este exemplar ser� cadastrado como n�:\n  > ");
                obra.numeroExemplar = proximoNumeroExemplar(localTombo);
                printf("%d\n", obra.numeroExemplar);
                printf("\n-Insira a data da compra do exemplar no formato dd/mm/aaaa:\n  > ");
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                idx = 0;
                int dataValida = 0;
                do {
                    digito = (char) getch();
                    if (0 <= digito - '0' && digito - '0' < 10 && idx < 10) {
                        if (idx == 2 || idx == 5) {
                            putchar('/');
                            obra.strDataCompra[idx++] = '/';
                        }
                        putchar(digito);
                        obra.strDataCompra[idx++] = digito;
                    } else if (((int) digito == 8 || (int) digito == 127) && idx > 0) { //"8" � o inteiro usado pra representar a tecla "backspace" e "127" para representar a tecla "delete"
                        putchar('\b');
                        putchar(' ');
                        putchar('\b');
                        obra.strDataCompra[idx] = '\0';
                        idx--;
                    }
                    if(idx == 10){
                        int confAno = ((int)obra.strDataCompra[6]-'0')*1000+((int)obra.strDataCompra[7]-'0')*100+((int)obra.strDataCompra[8]-'0')*10+(int)obra.strDataCompra[9]-'0';
                        int confMes = ((int)obra.strDataCompra[3]-'0')*10+(int)obra.strDataCompra[4]-'0';
                        int confDia = ((int)obra.strDataCompra[0]-'0')*10+(int)obra.strDataCompra[1]-'0';
                        if(confAno < tm.tm_year+1900) {
                            dataValida = 1;
                        }
                        else if (confAno == tm.tm_year + 1900){
                            if (confMes < tm.tm_mon + 1) {
                                dataValida = 1;
                            }
                            else if (confMes == tm.tm_mon + 1){
                                if (confDia <= tm.tm_mday)
                                    dataValida = 1;
                            }
                        }
                        if(confMes>12 || confMes<1 ){
                            dataValida = 0;
                        }
                        if(confMes == 2){
                            if((confAno % 400 == 0) || (confAno%4 == 0 && confAno%100!=0) ){
                                if(confDia>29 || confDia < 1){
                                    dataValida = 0;
                                }
                            }else if(confDia > 28 || confDia <1){
                                dataValida = 0;
                            }
                        }
                        else if(confMes == 1 || confMes == 3 || confMes == 5 || confMes == 7 || confMes == 8 || confMes == 10 || confMes == 12){
                            if(confDia > 31 || confDia < 1)
                                dataValida = 0;
                        }else if(confMes == 4 || confMes == 6 || confMes == 9 || confMes == 11){
                            if(confDia>30 || confDia <1)
                                dataValida = 0;
                        }
                        getch();
                        if(!dataValida){
                            printf("\t **data inv�lida**\n");
                            printf("*Insira uma data v�lida e no formato dd/mm/aaaa:\n  > ");
                            idx = 0;
                        }
                    }
                } while (((int) digito != 13 && (int) digito != 10 && !dataValida) || idx < 10); //"13" � o inteiro usado pra representar a tecla "enter"
                fprintf(arquivo, "%d | %d | %s\n", localTombo, obra.numeroExemplar, obra.strDataCompra);
                fclose(arquivo);
            }
        }
    }
    else{
        printf("ERRO AO CADASTRAR OBRA!\n");
    }
    if(localTombo != -1){
        limparTela();
        printf("\n   Cadastro feito com sucesso.");
        if(encaminharParaOutroCadastro() == 1){
            animacaoFuncao(1, 1, opcoes[0][0]);
            cadastroObra();
        }
        else {
            limparTela();
            menuPrincipal();
        }
    }
}
/*////////////////////////////////////////////fun��o para cadastrar TOMBOS:///////////////////////////////////////////*/
void cadastroTombo(){
    FILE *arquivo = NULL;
    int verificador, localCodigoArea = 0;
    verificador = abrirArquivo(arquivo, "tombos.txt", "a");
    if(!verificador){
        arquivo = fopen("tombos.txt", "a");
        printf("-O tombo ser� cadastro como:\n  > ");
        printf("%d\n", tomboGeral+1);
        printf("\n-Insira o nome da obra:\n  > ");
        scanf(" %[^\n]s", tombo.nomeObra);
        printf("\n-Insira o nome do autor:\n  > ");
        scanf(" %[^\n]s", tombo.nomeAutor);
        printf("\n-Insira o nome da editora:\n  > ");
        scanf(" %[^\n]s", tombo.nomeEditora);
        printf("\n-Digite o c�digo de �rea da obra:\n  > ");
        localCodigoArea = stringDeInt(1);

        if(localCodigoArea != -1) {
            fprintf(arquivo, "%d | %s | %s | %s | %d\n", ++tomboGeral, stringMinuscula(tombo.nomeObra), stringMinuscula(tombo.nomeAutor), stringMinuscula(tombo.nomeEditora), localCodigoArea);
            fclose(arquivo);
        }
        else{
            limparTela();
            printf("\n   Cadastro cancelado!");
            if(encaminharParaOutroCadastro() == 1){
                animacaoFuncao(2, 2, opcoes[0][1]);
                cadastroTombo();
            }
            else {
                limparTela();
                menuPrincipal();
            }
        }
    }
    if(localCodigoArea != -1){
        limparTela();
        printf("\n   Cadastro feito com sucesso.");
        if (encaminharParaOutroCadastro() == 1) {
            animacaoFuncao(2, 2, opcoes[0][1]);
            cadastroTombo();
        } else {
            limparTela();
            menuPrincipal();
        }
    }
}

///*########################################Fun��es de Consultas####################################################*///
/*//////////////////////////////////////fun��o para exibir as consultas feitas:///////////////////////////////////////*/
void exibirConsulta(consulta consul[], int resultados, int nFuncao){
    int i, j;
    for(i = 0 ; i < resultados ; i += qntdMaxResultadosPorPagina){
        limparTela();
        animacaoFuncao(0, nFuncao, opcoes[1][nFuncao-1]);
        if(i == 0){
            printf("\t\t  %d %s\n", resultados, (resultados > 1 ? "obras encontradas." : "obra encontrada."));
            printf("\n  ------------------------------------------------------\n");
        }
        for(j = 0; j < qntdMaxResultadosPorPagina && i+j < resultados; j++) {
            printf(" Tombo: %d\n", consul[i+j].tombo);
            printf(" Obra: %s\n", consul[i+j].nomeObra);
            printf(" Autor: %s\n", consul[i+j].nomeAutor);
            printf(" Editora: %s\n", consul[i+j].nomeEditora);
            printf(" �rea: %d\n", consul[i+j].codigoArea);
            if (consul[i+j].numeroExemplar == -1) {
                printf(" N�mero do exemplar: N�O CADASTRADO\n");
                printf(" Data da compra do exemplar: N�O CADASTRADO\n");
            } else {
                printf(" N�mero do exemplar: %d\n", consul[i+j].numeroExemplar);
                printf(" Data da compra do exemplar: %02d/%02d/%04d\n", consul[i+j].dd, consul[i+j].mm, consul[i+j].aa);
            }
            printf("  ------------------------------------------------------\n");
        }
        printf("\n\n\t         P�gina %d de %d\n", i/qntdMaxResultadosPorPagina+1, resultados%qntdMaxResultadosPorPagina == 0 ? resultados/qntdMaxResultadosPorPagina : resultados/qntdMaxResultadosPorPagina+1);
        if(i > 0 && i+j < resultados)
            printf("\t   <- anterior | seguinte ->\n");
        else if(i > 0)
            printf("\t\t  <- anterior\n");
        else if(i+j < resultados)
            printf("\t\t  seguinte ->\n");
        printf("\t\t   Esc. Sair");

        int op = avancaPagina(resultados, i, j);
        if(op == 0){
            i -= qntdMaxResultadosPorPagina * 2;
        }
        else if(op == -1)
            break;
    }
    limparTela();
}
/*//////////////////////////////////////fun��o para consultar obras por �rea://///////////////////////////////////////*/
void consultaArea(consulta consul[], int nFuncao){
    int buscaArea, confTombo, conf2Tombo, conf2Exemplar, conf2dd, conf2mm, conf2aa, confArea, resultados = 0, resultadosObras;
    printf("Digite o c�digo de �rea: ");
    buscaArea = stringDeInt(1);
    if(buscaArea != -1) {
        FILE *tombos, *obras;
        tombos = fopen("tombos.txt", "r");
        char confObra[maxStrLen], confAutor[maxStrLen], confEditora[maxStrLen];
        printf("\n  ------------------------------------------------------\n");
        while (fscanf(tombos, "%d | %[^|] | %[^|] | %[^|] | %d\n", &confTombo, confObra, confAutor, confEditora,
                      &confArea) != EOF) {
            if (confArea == buscaArea) {
                consul[resultados].tombo = confTombo;
                strcpy(consul[resultados].nomeObra, stringFormatada(confObra));
                strcpy(consul[resultados].nomeAutor, stringFormatada(confAutor));
                strcpy(consul[resultados].nomeEditora, stringFormatada(confEditora));
                consul[resultados].codigoArea = confArea;

                resultadosObras = 0;
                obras = fopen("obras.txt", "r");
                while (fscanf(obras, "%d | %d | %d/%d/%d\n", &conf2Tombo, &conf2Exemplar, &conf2dd, &conf2mm,
                              &conf2aa) != EOF) {
                    if (conf2Tombo == confTombo) {
                        consul[resultados].numeroExemplar = conf2Exemplar;
                        consul[resultados].dd = conf2dd;
                        consul[resultados].mm = conf2mm;
                        consul[resultados].aa = conf2aa;
                        resultadosObras++;
                    }
                }
                fclose(obras);
                if (resultadosObras == 0) {
                    consul[resultados].numeroExemplar = -1;
                    consul[resultados].dd = -1;
                    consul[resultados].mm = -1;
                    consul[resultados].aa = -1;
                }
                resultados++;
            }
        }
        fclose(tombos);

        exibirConsulta(consul, resultados, nFuncao);
        printf("\n  Busca finalizada: ");
        if (resultados == 0) {
            printf("c�digo de �rea \"%d\" n�o encontrado!\n", buscaArea);
        } else {
            printf("%d %s\n", resultados, (resultados > 1 ? "obras encontradas." : "obra encontrada."));
        }
        limparBuffer();
        printf("\nPressione qualquer tecla para continuar...");
        getch();
        encaminharParaOutraConsulta();
    }
    else{
        limparTela();
        printf("\n   Consulta cancelada!");
        encaminharParaOutraConsulta();
    }
}
/*//////////////////////////////////////fun��o para consultar obras por autor:////////////////////////////////////////*/
void consultaAutor(consulta consul[], int nFuncao){
    char buscaAutor[maxStrLen];
    printf("Digite o nome do autor: ");
    scanf(" %[^\n]", buscaAutor);
    stringMinuscula(buscaAutor);
    strcat(buscaAutor, " ");
    FILE *tombos, *obras;
    tombos = fopen("tombos.txt","r");
    int confTombo, confArea, resultados = 0, conf2Tombo, conf2Exemplar, conf2dd, conf2mm, conf2aa, resultadosObras = 0;
    char confObra[maxStrLen], confAutor[maxStrLen], confEditora[maxStrLen];
    printf("  ------------------------------------------------------\n");
    while(fscanf(tombos, "%d | %[^|] | %[^|] | %[^|] | %d\n", &confTombo, confObra, confAutor, confEditora, &confArea) != EOF){
        if(stringCompare(buscaAutor, confAutor) == 0){
            consul[resultados].tombo = confTombo;
            strcpy(consul[resultados].nomeObra, stringFormatada(confObra));
            strcpy(consul[resultados].nomeAutor, stringFormatada(confAutor));
            strcpy(consul[resultados].nomeEditora, stringFormatada(confEditora));
            consul[resultados].codigoArea = confArea;

            resultadosObras = 0;
            obras = fopen("obras.txt", "r");
            while(fscanf(obras, "%d | %d | %d/%d/%d\n", &conf2Tombo, &conf2Exemplar, &conf2dd, &conf2mm, &conf2aa) != EOF){
                if(conf2Tombo == confTombo){
                    consul[resultados].numeroExemplar = conf2Exemplar;
                    consul[resultados].dd = conf2dd;
                    consul[resultados].mm = conf2mm;
                    consul[resultados].aa = conf2aa;
                    resultadosObras++;
                }
            }
            fclose(obras);
            if(resultadosObras == 0){
                consul[resultados].numeroExemplar = -1;
                consul[resultados].dd = -1;
                consul[resultados].mm = -1;
                consul[resultados].aa = -1;
            }
            resultados++;
        }
    }
    fclose(tombos);

    exibirConsulta(consul, resultados, nFuncao);
    printf("\n  Busca finalizada: ");
    if(resultados == 0){
        printf("autor \"%s\b\" n�o encontrado!\n", buscaAutor);
    }
    else {
        printf("%d %s\n", resultados, (resultados > 1 ? "obras encontradas." : "obra encontrada."));
    }
    limparBuffer();
    printf("\nPressione qualquer tecla para continuar...");
    getch();
    encaminharParaOutraConsulta();
}
/*////////////////////////////////////fun��o para consultar obras por editora:////////////////////////////////////////*/
void consultaEditora(consulta consul[], int nFuncao){
    char buscaEditora[maxStrLen];
    printf("Digite o nome da editora: ");
    scanf(" %[^\n]", buscaEditora);
    stringMinuscula(buscaEditora);
    strcat(buscaEditora, " ");
    FILE *tombos, *obras;
    tombos = fopen("tombos.txt", "r");
    int confTombo, confArea, resultados = 0, conf2Tombo, conf2Exemplar, conf2dd, conf2mm, conf2aa, resultadosObras = 0;
    char confObra[maxStrLen], confAutor[maxStrLen], confEditora[maxStrLen];
    printf("  ------------------------------------------------------\n");
    while(fscanf(tombos, "%d | %[^|] | %[^|] | %[^|] | %d\n", &confTombo, confObra, confAutor, confEditora, &confArea) != EOF){
        if(stringCompare(buscaEditora, confEditora) == 0){
            consul[resultados].tombo = confTombo;
            strcpy(consul[resultados].nomeObra, stringFormatada(confObra));
            strcpy(consul[resultados].nomeAutor, stringFormatada(confAutor));
            strcpy(consul[resultados].nomeEditora, stringFormatada(confEditora));
            consul[resultados].codigoArea = confArea;

            resultadosObras = 0;
            obras = fopen("obras.txt", "r");
            while(fscanf(obras, "%d | %d | %d/%d/%d\n", &conf2Tombo, &conf2Exemplar, &conf2dd, &conf2mm, &conf2aa) != EOF){
                if(conf2Tombo == confTombo){
                    consul[resultados].numeroExemplar = conf2Exemplar;
                    consul[resultados].dd = conf2dd;
                    consul[resultados].mm = conf2mm;
                    consul[resultados].aa = conf2aa;
                    resultadosObras++;
                }
            }
            fclose(obras);
            if(resultadosObras == 0){
                consul[resultados].numeroExemplar = -1;
                consul[resultados].dd = -1;
                consul[resultados].mm = -1;
                consul[resultados].aa = -1;
            }
            resultados++;
        }
    }
    fclose(tombos);

    exibirConsulta(consul, resultados, nFuncao);
    printf("\n  Busca finalizada: ");
    if(resultados == 0){
        printf("editora \"%s\b\" n�o encontrada!\n", buscaEditora);
    }
    else {
        printf("%d %s\n", resultados, (resultados > 1 ? "obras encontradas." : "obra encontrada."));
    }
    limparBuffer();
    printf("\nPressione qualquer tecla para continuar...");
    getch();
    encaminharParaOutraConsulta();
}
/*//////////////////////////////////////fun��o para consultar o Acervo existente://///////////////////////////////////////*/
void consultaAcervo(consulta consul[], int nFuncao){
    FILE *tombos, *obras;
    tombos = fopen("tombos.txt","r");
    int resultados = 0, resultadosObras, confTombo, confExemplar, confdd, confmm, confaa;
    while(fscanf(tombos, "%d | %[^|] | %[^|] | %[^|] | %d\n", &consul[resultados].tombo, consul[resultados].nomeObra, consul[resultados].nomeAutor, consul[resultados].nomeEditora, &consul[resultados].codigoArea) != EOF){
        strcpy(consul[resultados].nomeObra, stringFormatada(consul[resultados].nomeObra));
        strcpy(consul[resultados].nomeAutor, stringFormatada(consul[resultados].nomeAutor));
        strcpy(consul[resultados].nomeEditora, stringFormatada(consul[resultados].nomeEditora));

        resultadosObras = 0;
        obras = fopen("obras.txt", "r");
        while(fscanf(obras, "%d | %d | %d/%d/%d\n", &confTombo, &confExemplar, &confdd, &confmm, &confaa) != EOF){
            if(confTombo == consul[resultados].tombo){
                consul[resultados].numeroExemplar = confExemplar;
                consul[resultados].dd = confdd;
                consul[resultados].mm = confmm;
                consul[resultados].aa = confaa;
                resultadosObras++;
            }
        }
        fclose(obras);
        if(resultadosObras == 0){
            consul[resultados].numeroExemplar = -1;
            consul[resultados].dd = -1;
            consul[resultados].mm = -1;
            consul[resultados].aa = -1;
        }
        resultados++;
    }
    fclose(tombos);

    exibirConsulta(consul, resultados, nFuncao);
    printf("\n  Busca finalizada: ");
    if(resultados == 0){
        printf("Seu acervo no momento � zero!\n");
    }else {
        printf("%d %s\n", resultados, (resultados > 1 ? "obras encontradas." : "obra encontrada."));
        printf("  ------------------------------------------------------\n");
    }
    limparBuffer();
    printf("\nPressione qualquer tecla para continuar...");
    getch();
    encaminharParaOutraConsulta();//a gente vai mudar isso aqui para o gerente neh, depois,
}
/*////////////////////////////////////fun��o para consultar o tamanho do acervo://////////////////////////////////////*/
void consultaTamanhoAcervo(){
    printf("  ------------------------------------------------------\n");
    if(tomboGeral > 0)
        printf("  Seu acervo � composto atualmente por %d %s.\n", tomboGeral, (tomboGeral > 1 ? "livros cadastrados" : "livro cadastrado"));
    else
        printf("  Seu acervo n�o possui livros cadastrados atualmente.\n");
    printf("  ------------------------------------------------------\n");
    printf("\nPressione qualquer tecla para continuar...");
    getch();
    encaminharParaOutraConsulta();
}

///*###########################################Fun��o Principal#####################################################*///
int main(){
    limparTela();
    int tomboTemp;
    setlocale(LC_ALL, "");
    FILE *arquivoObras = NULL, *arquivoTombos = NULL;
    abrirArquivo(arquivoObras, "obras.txt", "a");
    abrirArquivo(arquivoTombos, "tombos.txt", "a");
    arquivoTombos =  fopen("tombos.txt", "r");
    while(fscanf(arquivoTombos, "%d | %[^\n]\n", &tomboTemp, lixo) != EOF){
        tomboGeral = tomboTemp;
    }
    fclose(arquivoTombos);
    menuPrincipal();
    return 0;
}