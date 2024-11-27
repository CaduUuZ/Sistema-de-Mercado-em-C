#include <stdio.h>
#include <string.h>
#include <ctype.h>
// Definição da cor das letras
#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// Definição dos estilos
#define BOLD    "\033[1m"
#define UNDERLINE "\033[4m"

// Cores de fundo
#define BG_BLACK   "\033[40m"
#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN    "\033[46m"
#define BG_WHITE   "\033[47m"

/*
nesta parte incluimos estúdios e definimos as cores, 
colocamos todas mesmo não usando para não precisar definir depois.
*/

/*=========================
        STRUCT's
=========================*/

struct cliente {
    char nomeCliente[30];
    char cpfCliente[12];
    char sexoCliente[2];
    int idCliente;
    char telefone[16];
};

//Aqui fica todas os vetores/strings ligados aos dados dos clientes e seus cadastros.

struct data {
    int dia;
    int mes;
    int ano;
};

//Aqui ficam as variáveis em INTEIRO para representar datas.

struct produto {
    int codigo_produto;  
    char nomeProduto[50];
    float precoProduto;
    struct data validade;
    char tipo[50];
};

//Aqui ficam as informações do produto para cadastro.

struct carrinho {
    struct cliente nomeCliente;
    struct produto produto[3];
    int quantidade[3];
    double subtotal;
    double desconto;
    double total;
};

//E aqui temos as inforações para o Carrinho de compras.

/*=========================
        FUNÇÕES
=========================*/

void menu(void);
struct cliente cadastroCliente(void);
struct produto cadastroProduto(void);
void listarProdutos(struct produto pro[], int numProdutos);
void listarCliente(struct cliente cli[], int numClientes);
void mascaraTelefone(const char *numero, char *resultado);
void inicializarCarrinho(struct carrinho *car, struct cliente cliente);
void adicionarAoCarrinho(struct carrinho *car, struct produto prod, int quantidade, int indice);
void aplicarDesconto(struct carrinho *car, double porcentagemDesconto);
void exibirCarrinho(struct carrinho *car);
int validarCPF(const char *cpf);
int validarTelefone(const char *telefone);
void gerenciarCarrinho(struct carrinho *car, struct produto produtos[], int numProdutos);
int encontrarProdutoPorCodigo(struct produto produtos[], int numProdutos, int codigo);

//Todas as funções, não tão necessárias, mas úteis para organizar o código.

/*=========================
    CÓDIGO PRINCIPAL
=========================*/

int main(void) {
    struct cliente cli[3];
    struct produto pro[10];
    struct carrinho car;
    int escolha;
    int numClientes = 0;
    int numProdutos = 0;
    /*========================================================
    Inicio do código, com as variáveis e passagem das structs.
    =========================================================*/
    do {
        menu();
        scanf("%d", &escolha);
        
        switch (escolha) {
            case 1:
                if (numClientes < 3) {
                    cli[numClientes++] = cadastroCliente();
                } else {
                    printf(YELLOW"⚠ Limite de clientes atingido!\n"RESET);
                }
                break;
                
            case 2:
                if (numProdutos < 10) {
                    pro[numProdutos++] = cadastroProduto();
                } else {
                    printf(YELLOW"⚠ Limite de produtos atingido!\n"RESET);
                }
                break;
                
            case 3:
                listarCliente(cli, numClientes);
                break;
                
            case 4:
                listarProdutos(pro, numProdutos);
                break;
                
            case 5:
                if (numClientes == 0 || numProdutos == 0) {
                printf("⚠ É necessário cadastrar pelo menos um cliente e um produto primeiro!\n");
                break;
    }
                
                printf("📋 Lista de produtos disponíveis:\n");
                listarProdutos(pro, numProdutos);
                
                printf("\n📋 Lista de clientes disponíveis:\n");
                listarCliente(cli, numClientes);
                
                printf("👤 Selecione o cliente para a compra (1-%d):\n", numClientes);
                int clienteSelecionado;
                scanf("%d", &clienteSelecionado);
                clienteSelecionado--; // Ajusta o número selecionado para o índice do array
                
                if (clienteSelecionado >= 0 && clienteSelecionado < numClientes) {
                inicializarCarrinho(&car, cli[clienteSelecionado]);
                gerenciarCarrinho(&car, pro, numProdutos);
                } else {
                 printf("❌ Cliente inválido!\n");
                }
                break;
                
            case 0:
                printf("👋 Programa finalizado!\n");
                break;
                
            default:
                printf("❌ Opção inválida!\n");
                break;
        }
    } while (escolha != 0);

    return 0;
}

/*========================================================
Switch case, para seleções do Menu, com todas as 
opções necessárias para um bom funcionamento do programa 
=========================================================*/

/*=========================
       VALIDAÇÕES
=========================*/

int validarCPF(const char *cpf) {
    int len = strlen(cpf);
    if (len != 11) return 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit(cpf[i])) return 0;
    }
    return 1;
}

/*========================================================
Função para validar CPF, determinando o tamanho máximo de 
                    11 caracteres
=========================================================*/

int validarTelefone(const char *telefone) {
    int len = strlen(telefone);
    if (len != 11) return 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit(telefone[i])) return 0;
    }
    return 1;
}

/*========================================================
Função para validar CPF, determinando o tamanho máximo de 
                    11 caracteres
=========================================================*/

/*=========================
       CARRINHO
=========================*/

void inicializarCarrinho(struct carrinho *car, struct cliente cliente) {
    car->nomeCliente = cliente;
    car->subtotal = 0;
    car->desconto = 0;
    car->total = 0;
    memset(car->quantidade, 0, sizeof(car->quantidade));
}

/*========================================================
Função para iniciar carrinho, preparando o carrinho para um
                     novo cliente.
=========================================================*/

void adicionarAoCarrinho(struct carrinho *car, struct produto prod, int quantidade, int indice) {
    car->produto[indice] = prod;
    car->quantidade[indice] = quantidade;
    car->subtotal += prod.precoProduto * quantidade;
    car->total = car->subtotal - car->desconto;
}

/*========================================================
Função VOID, para adicionar produtos já antes adicionados/
                  existentes ao carrinho
=========================================================*/

void aplicarDesconto(struct carrinho *car, double porcentagemDesconto) {
    car->desconto = car->subtotal * (porcentagemDesconto / 100);
    car->total = car->subtotal - car->desconto;
}

/*========================================================
Aqui fizemos uma função Void onde se é possivel aplicar um 
cupom de desconto em porcentagem.
=========================================================*/

void exibirCarrinho(struct carrinho *car) {
    printf("\n🛒 === Detalhes do Carrinho === 🛒\n");
    printf("👤 Cliente: %s\n", car->nomeCliente.nomeCliente);
    printf("📝 CPF: %s\n", car->nomeCliente.cpfCliente);
    printf("\n📦 Produtos:\n");
    
    for (int i = 0; i < 3; i++) {
        if (car->quantidade[i] > 0) {
            printf("%d. %s\n", i+1, car->produto[i].nomeProduto);
            printf("   📊 Quantidade: %d\n", car->quantidade[i]);
            printf("   💰 Preço unitário: R$ %.2f\n", car->produto[i].precoProduto);
            printf("   💵 Subtotal item: R$ %.2f\n\n", 
                   car->produto[i].precoProduto * car->quantidade[i]);
        }
    }
    
    printf("📊 Subtotal: R$ %.2f\n", car->subtotal);
    printf("🏷 Desconto: R$ %.2f\n", car->desconto);
    printf("💳 Total a pagar: R$ %.2f\n", car->total);
}

/*========================================================
Uma parte bem complicada, mas que demos conta, foi o mostrar 
carrinho, utilizando alguns conceitos vistos em sala, para
iniciar o carinho sem itens/lixo e etc.
=========================================================*/

int encontrarProdutoPorCodigo(struct produto produtos[], int numProdutos, int codigo) {
    for (int i = 0; i < numProdutos; i++) {
        if (produtos[i].codigo_produto == codigo) {
            return i;
        }
    }
    return -1;
}

void gerenciarCarrinho(struct carrinho *car, struct produto produtos[], int numProdutos) {
    int codigo, quantidade, opcao;
    int indiceCarrinho = 0;

    do {
        printf("\n🛍 === Gerenciamento do Carrinho === 🛍\n");
        printf("1 - ➕ Adicionar Produto\n");
        printf("2 - 📋 Exibir Carrinho\n");
        printf("3 - 🏷 Aplicar Desconto\n");
        printf("0 - 💳 Finalizar Compra\n");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                if (indiceCarrinho >= 3) {
                    printf("⚠ Carrinho cheio!\n");
                    break;
                }
                
                printf("\n📦 Produtos disponíveis:\n");
                listarProdutos(produtos, numProdutos);
                
                printf("🔢 Digite o código do produto: ");
                scanf("%d", &codigo);
                
                int indiceProduto = encontrarProdutoPorCodigo(produtos, numProdutos, codigo);
                if (indiceProduto == -1) {
                    printf("❌ Produto não encontrado!\n");
                    break;
                }

                printf("📊 Digite a quantidade: ");
                scanf("%d", &quantidade);
                if (quantidade <= 0) {
                    printf("❌ Quantidade inválida!\n");
                    break;
                }

                adicionarAoCarrinho(car, produtos[indiceProduto], quantidade, indiceCarrinho++);
                printf("✅ Produto adicionado com sucesso!\n");
                break;

            case 2:
                exibirCarrinho(car);
                break;

            case 3:
                double porcentagem;
                printf("🏷 Digite a porcentagem de desconto: ");
                scanf("%lf", &porcentagem);
                if (porcentagem >= 0 && porcentagem <= 100) {
                    aplicarDesconto(car, porcentagem);
                    printf("✅ Desconto aplicado com sucesso!\n");
                } else {
                    printf("❌ Porcentagem inválida!\n");
                }
                break;
        }
    } while (opcao != 0);
}

/*========================================================
Aqui temos o menu do carrinho, onde fizemos tudo ficar
do jeito mais harmonico possivel, para que o cliente não
se confunda.
=========================================================*/

/*=========================
    MÁSCARA TELEFONE
=========================*/

void mascaraTelefone(const char *numero, char *resultado) {
    snprintf(resultado, 16, "(%c%c) %c%c%c%c%c-%c%c%c%c", 
             numero[0], numero[1],
             numero[2], numero[3], numero[4], numero[5],
             numero[6], numero[7], numero[8], numero[9], numero[10]);
}

/*========================================================
Máscara do telefone, feita para que possamos mostrar-no sem
                        erros
=========================================================*/

/*=========================
        MENU
=========================*/

void menu() {
    printf("\n");
    printf("╔═════════════════════════════════════════════╗\n");
    printf("║              MENU PRINCIPAL                 ║\n");
    printf("╠════════════════════════════════════════════╣\n");
    printf("║ 1 - 👤 Cadastrar Clientes                    ║\n");
    printf("║ 2 - 📦 Cadastrar Produtos                    ║\n");
    printf("║ 3 - 📋 Listar Clientes                       ║\n");
    printf("║ 4 - 📋 Listar Produtos                       ║\n");
    printf("║ 5 - 🛒 Iniciar Compra                        ║\n");
    printf("║ 0 - 🚪 Sair                                  ║\n");
    printf("╚═════════════════════════════════════════════╝\n");
    printf("\n");
}

/*========================================================
Menu principal, feito com tabela ASCII e alguns emojis para
                    deixar mais claro
=========================================================*/

/*=========================
    FUNÇÕES DE REGISTRO
=========================*/


int gerarIdCliente(const char* cpf) {
    return (cpf[0] - '0') * 1000 + (cpf[1] - '0') * 100 + (cpf[2] - '0') * 10 + (cpf[3] - '0');
}

/*========================================================
Função inteira que serve para criar a id do cliente com base
                        no seu CPF.
=========================================================*/

struct cliente cadastroCliente() {
    struct cliente cli;
    char telefoneSemMascara[12];
    
    do {
        printf("👤 Nome Cliente:\n");
        getchar();
        fgets(cli.nomeCliente, sizeof(cli.nomeCliente), stdin);
        cli.nomeCliente[strcspn(cli.nomeCliente, "\n")] = 0;
    } while (strlen(cli.nomeCliente) == 0);
    
    do {
        printf("📝 CPF Cliente (apenas números):\n");
        scanf("%s", cli.cpfCliente);
        if (!validarCPF(cli.cpfCliente)) {
            printf("❌ CPF inválido! Digite exatamente 11 dígitos numéricos.\n");
        }
    } while (!validarCPF(cli.cpfCliente));

cli.idCliente = gerarIdCliente(cli.cpfCliente);
    printf("✅ ID gerado para o cliente: %d\n", cli.idCliente);
    
    do {
        printf("👥 Sexo Cliente (M/F):\n");
        getchar();
        fgets(cli.sexoCliente, sizeof(cli.sexoCliente), stdin);
        cli.sexoCliente[strcspn(cli.sexoCliente, "\n")] = 0;
    } while (cli.sexoCliente[0] != 'M' && cli.sexoCliente[0] != 'F');
    
    do {
        printf("📱 Telefone Cliente (digite apenas os números, 11 dígitos):\n");
        scanf("%s", telefoneSemMascara);
        if (!validarTelefone(telefoneSemMascara)) {
            printf("❌ Telefone inválido! Digite exatamente 11 dígitos numéricos.\n");
        }
    } while (!validarTelefone(telefoneSemMascara));
    
    mascaraTelefone(telefoneSemMascara, cli.telefone);
    return cli;
}

/*========================================================
Função para cadastrar clientes sempre verificando se os 
                    dados são válidos
=========================================================*/

void listarCliente(struct cliente cli[], int numClientes) {
    printf("\n📋 === Lista de Clientes === 📋\n");
    for(int i = 0; i < numClientes; i++) {
        printf("\n👤 Cliente %d:\n", i + 1);
        printf("📝 Nome: %s\n", cli[i].nomeCliente);
        printf("🔢 CPF: %s\n", cli[i].cpfCliente);
        printf("🏷 ID: %d\n", cli[i].idCliente);
        printf("👥 Sexo: %s\n", cli[i].sexoCliente);
        printf("📱 Telefone: %s\n", cli[i].telefone);
    }
    printf("\n");
}

// Função para gerar o código do produto
int gerarCodigoProduto(const char* nome) {
    if (strlen(nome) < 2) {
        return 0; // Retorna 0 se o nome for muito curto
    }
    // Pega os valores ASCII das duas primeiras letras e combina em um número
    int codigo = ((int)nome[0] << 8) | (int)nome[1];
    return codigo;
}

// Função de cadastro de produto com código automático
struct produto cadastroProduto() {
    struct produto pro;
    printf("📦 Nome Produto:\n");
    getchar();
    fgets(pro.nomeProduto, sizeof(pro.nomeProduto), stdin);
    pro.nomeProduto[strcspn(pro.nomeProduto, "\n")] = 0;
    
    pro.codigo_produto = gerarCodigoProduto(pro.nomeProduto);
    
    printf("💰 Preço do Produto:\n");
    scanf("%f", &pro.precoProduto);
    
    printf("📝 Tipo de Produto:\n");
    getchar();
    fgets(pro.tipo, sizeof(pro.tipo), stdin);
    pro.tipo[strcspn(pro.tipo, "\n")] = 0;
    
    printf("📅 Data de Validade (DD MM AAAA):\n");
    scanf("%d %d %d", &pro.validade.dia, &pro.validade.mes, &pro.validade.ano);
    
    printf("\n✅ Código gerado para o produto: %d (ASCII: %c%c)\n", 
           pro.codigo_produto, pro.nomeProduto[0], pro.nomeProduto[1]);
    
    return pro;
}

/*========================================================
Função para cadastrar o produto, tambem buscando manter
as informações validas.
=========================================================*/

// Função para listar todos os produtos
void listarProdutos(struct produto pro[], int numProdutos) {
    printf("\n📦 === Lista de Produtos === 📦\n");
    for (int i = 0; i < numProdutos; i++) {
        printf("\n📦 Produto %d:\n", i + 1);
        printf("🔢 Código: %d\n", pro[i].codigo_produto);
        printf("📝 Nome: %s\n", pro[i].nomeProduto);
        printf("💰 Preço: R$ %.2f\n", pro[i].precoProduto);
        printf("📝 Tipo: %s\n", pro[i].tipo);
        printf("📅 Validade: %02d/%02d/%04d\n", 
               pro[i].validade.dia, pro[i].validade.mes, pro[i].validade.ano);
    }
   printf("\n");
}

/*========================================================
Função para listar produtos, utilizando printf's
=========================================================*/
