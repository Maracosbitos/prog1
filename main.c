#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Definição de constantes
#define MAX_LINE 100
#define PI 3.14159265

double tf, dt, S, b, m, rho, CD0, e, alpha, g; // Variáveis Lidas
double V, h, x, gamma_v = 0;
double D, L = 0;


void mostrar_menu()
{
    printf("\nLista de opções:\n");
    printf("0 – Terminar o programa\n");
    printf("1 – Simular o movimento da aeronave\n");
    printf("2 – Determinar os valores mínimos e máximos\n");
    printf("Selecione a opção: ");
}

int scan_parametros(void) // Leitura dos parâmetros
{
    FILE *fp = fopen("config_modelo.txt", "r");
    if (fp == NULL)
    {
        printf("Erro ao abrir arquivo config_modelo.txt\n"); //Caso não consiga abrir retorna 0
        return 0;
    }

    double arr[13] = {0};
    char var[20];


    for (int i = 0; i < 13; i++) {
        if (fscanf(fp, "%[^=]=%lf", var, &arr[i]) != 2) { // Quando a leitura é feita corretamente a função retorna o valor do  número de valores de leitura certa
            printf("Error reading value at index %d\n", i);
            arr[i] = 0.0;
        }
        printf("%lf\n", arr[i]);
    }

    tf=arr[0];
    dt=arr[1];
    S=arr[2];
    b=arr[3];
    m=arr[4];
    rho=arr[5];
    CD0=arr[6];
    e=arr[7];
    alpha=arr[8];
    V=arr[9];
    gamma_v=arr[10];
    x=arr[11];
    h=arr[12];


    fclose(fp);
    return 1;
}

void simular_voo() { // Simulação (1)
    FILE *arquivo = fopen("voo_sim.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo voo_sim.txt!\n"); //Verifica se o ficheiro existe
    }

    fprintf(arquivo, "%lf %lf %lf %lf %lf %lf %lf %lf %lf\n", tf, dt, S, b, m, rho, CD0, e, alpha);
    printf("\nSimulação iniciada...\n");                            //Escreve os parâmetros no ficheiro

    double t = 0.0;

    while (t < tf && h > 0) {

        // Equações do Drift (Drag * Lift)
        double Cl = (alpha*(pow(b, 2))/S*PI)/(1+sqrt((1+pow((((pow(b, 2))/S)/2), 2))));
        double Cd = (CD0 + (1/(PI*e*(pow(b, 2))/S))*Cl);
        D = Cd*0.5*rho*(pow(V, 2)*S);
        L = Cl*0.5*rho*(pow(V, 2)*S);

        // Cálculo das equações diferenciais
        double dV = (-D - m * g * sin(gamma_v)) / m;
        double dGamma = (L - m * g * cos(gamma_v)) / (m * V);
        double dx = V * cos(gamma_v);
        double dh = V * sin(gamma_v);

        // Resolução das equações diferenciais
        V += dV * dt;
        gamma_v += dGamma * dt;
        x += dx * dt;
        h += dh * dt;
        t += dt;

        // Documentação no arquivo
        fprintf(arquivo, "%lf %lf %lf %lf %lf\n", t, V, gamma_v, x, h);
    }

    fclose(arquivo);
    printf("Simulação concluída! Resultados salvos em voo_sim.txt\n");
}

int main() {
    int in;

    do {
        mostrar_menu();
        scanf("%d", &in);  // Atualiza a escolha do usuário a cada iteração

        switch (in) {
            case 0:
                printf("\nVoo terminado!\n");
                break;
            case 1:
                if (scan_parametros()) {
                    printf("\nParâmetros carregados com sucesso!\n");
                }
                break;
            case 2:
                    simular_voo();
                    printf("\nSimulação terminada\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }
    } while (in != 0);

    return 0;
}