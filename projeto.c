#include <stdio.h>
#include <locale.h>
#include <string.h>
//variáveis globais:
char opcao;
	//relação das FUNÇÕES dos SUBMENUS:
char opcoes[2][10][100] = {
		{{"Cadastrar Obra"}, {"Cadastrar Tombo"}},
		{{"Mostrar obra por area"}, {"Mostrar obras por autor"}, {"Mostrar obra por editora"}, {"Mostrar tamanho do acervo"}},
	};
int qntdOpcoes[2] = {2,	4};
//implementação da função getch, para que funcione tanto em LINUS quanto em WINDOWS:
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
//função usada para chamar a FUNCAO selecionada pelo usuário:
void realizarFunc(int submenu, int funcao){
    if(submenu == 1){ //se o SUBMENU selecionado foi CADASTROS
        if(funcao == 1){ //se a FUNÇÃO selecionada foi CADASTRAR OBRA
            cadastroObra();
        }
        else if(funcao == 2){ //se a FUNÇÃO selecionada foi CADASTRAR TOMBO
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

//função para imprimir os SUBMENUS:
int imprimirSubmenu(int submenuSelecionado, char strSubmenuSelecionado[100], int nOpcoesSubmenu, char strOpcoesSubmenu[nOpcoesSubmenu][100]){
	int c, i;
	//imprimir cabeçalho:
	printf("\n ..................................\n");
    printf(" :        Biblioteca Fácil        :\n");
    printf("  --------------------------------\n");
    //imprimir SUBMENU selecionada:
    printf(" |%d. %s", submenuSelecionado, strSubmenuSelecionado);
    for(i = (strlen(strSubmenuSelecionado) + 3); i < 32; i++){
    	printf(" ");
    }
    printf("|\n");
    printf("  --------------------------------\n");
    //imprimir opções do submenu selecionado:
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
        printf("%c => Opção inválida! Por favor selecione uma opção válida:", opcao);
        imprimirSubmenu(submenuSelecionado, strSubmenuSelecionado, nOpcoesSubmenu, strOpcoesSubmenu);
    }
}

//função para imprimir o MENU PRINCIPAL:
void menuPrincipal(){
    printf("\n ..................................\n");
    printf(" :        Biblioteca Fácil        :\n");
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
        printf("%c => Opção inválida! Por favor selecione uma opção válida:", opcao);
        menuPrincipal();
    }
}

//funções relacionadas às FUNÇÕES do programa:
void cadastroObra(){
    printf("Não tem nada aqui ainda.\n");
}

void cadastroTombo(){
    printf("Não tem nada aqui ainda.\n");
}

//função principal:
int main(){
    setlocale(LC_ALL, "");
    menuPrincipal();
    return 0;
}
