#include <stdio.h>
#include <locale.h>
#include <string.h>
//vari�veis globais:
char opcao;
	//rela��o das FUN��ES dos SUBMENUS:
char opcoes[2][10][100] = {
		{{"Cadastrar Obra"}, {"Cadastrar Tombo"}},
		{{"Mostrar obra por area"}, {"Mostrar obras por autor"}, {"Mostrar obra por editora"}, {"Mostrar tamanho do acervo"}},
	};
int qntdOpcoes[2] = {2,	4};
//implementa��o da fun��o getch, para que funcione tanto em LINUS quanto em WINDOWS:
char Fgetch(){
    #ifdef __WIN32__
        #include <conio.h>
        return getch();
    #endif // __WIN32__

    #ifdef __LINUX__
        #include <termios.h>
        #include <unistd.h>

        int getch(void)
        {
            struct termios oldattr, newattr;
            int ch;
            tcgetattr( STDIN_FILENO, &oldattr );
            newattr = oldattr;
            newattr.c_lflag &= ~( ICANON | ECHO );
            tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
            ch = getchar();
            tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
            return ch;
        }
    #endif // __LINUX__
}
//fun��o usada para chamar a FUNCAO selecionada pelo usu�rio:
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

        }
        else if(funcao == 2){

        }
        else if(funcao == 3){

        }
        else if(funcao == 4){

        }
    }
}

//fun��o para imprimir os SUBMENUS:
int imprimirSubmenu(int submenuSelecionado, char strSubmenuSelecionado[100], int nOpcoesSubmenu, char strOpcoesSubmenu[nOpcoesSubmenu][100]){
	int c, i;
	//imprimir cabe�alho:
	printf("\n ..................................\n");
    printf(" :        Biblioteca F�cil        :\n");
    printf("  --------------------------------\n");
    //imprimir SUBMENU selecionada:
    printf(" |%d. %s", submenuSelecionado, strSubmenuSelecionado);
    for(i = (strlen(strSubmenuSelecionado) + 3); i < 32; i++){
    	printf(" ");
    }
    printf("|\n");
    printf("  --------------------------------\n");
    //imprimir op��es do submenu selecionado:
    for(i = 0; i < nOpcoesSubmenu; i++){
    	printf(" |  %d. %s", i+1, strOpcoesSubmenu[i]);
    	for(c = strlen(strOpcoesSubmenu[i]) + 6; c <= 32; c++){
    		printf(" ");
    	}
    	printf("|\n");
    }
    printf(" |  0. Voltar                     |\n");
    printf("  --------------------------------\n");
    //leitura da proxima escolha:
    opcao = Fgetch();
    if(opcao == '0'){
        system("CLS");
        menuPrincipal();
    }
    else if((int)opcao - 48 <= qntdOpcoes[submenuSelecionado-1] && (int)opcao - 48 >= 0){
        system("CLS");
        realizarFunc(submenuSelecionado, (int)opcao - 48);
    }
    else {
        system("CLS");
        printf("%c => Op��o inv�lida! Por favor selecione uma op��o v�lida:", opcao);
        imprimirSubmenu(submenuSelecionado, strSubmenuSelecionado, nOpcoesSubmenu, strOpcoesSubmenu);
    }
}

//fun��o para imprimir o MENU PRINCIPAL:
void menuPrincipal(){
    printf("\n ..................................\n");
    printf(" :        Biblioteca F�cil        :\n");
    printf("  --------------------------------\n");
    printf(" |  1. Cadastros                  |\n");
    printf(" |  2. Consultas                  |\n");
    printf(" |  0. Sair                       |\n");
    printf("  --------------------------------\n");

    opcao = Fgetch();
    printf("%c\n", opcao);
    if(opcao == '0'){
        return 0;
    }
    else if (opcao == '1'){
        system("CLS");
        imprimirSubmenu((int)opcao - 48, "Cadastros", qntdOpcoes[1-1], opcoes[1-1]);
    }
    else if (opcao == '2'){
        system("CLS");
        imprimirSubmenu((int)opcao - 48, "Consultas", qntdOpcoes[(int)opcao - 49], opcoes[(int)opcao - 49]);
    }
    else {
        system("CLS");
        printf("%c => Op��o inv�lida! Por favor selecione uma op��o v�lida:", opcao);
        menuPrincipal();
    }
}

//fun��es relacionadas �s FUN��ES do programa:
void cadastroObra(){
    printf("N�o tem nada aqui ainda.\n");
}

void cadastroTombo(){
    printf("N�o tem nada aqui ainda.\n");
}

//fun��o principal:
int main(){
    setlocale(LC_ALL, "");
    menuPrincipal();
    return 0;
}
