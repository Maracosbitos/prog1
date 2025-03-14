#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Definição de constantes
#define MAX_LINE 100
#define PI 3.14159265
#define MAX_ANGLE 100

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

int scan_parametros(void) {
    FILE *fp = fopen("config_modelo.txt", "r");
    if (fp == NULL) {
        printf("Error: Cannot open config_modelo.txt\n");
        return 0;
    }

    char line[MAX_LINE];
    int values_read = 0;

    while (fgets(line, sizeof(line), fp)) {
        // Ignore comments and empty lines
        if (line[0] == '%' || line[0] == '\n') continue;

        // Use sscanf to read from the buffer, not from the file
        switch (values_read) {
            case 0:  sscanf(line, "%lf", &tf); break;
            case 1:  sscanf(line, "%lf", &dt); break;
            case 2:  sscanf(line, "%lf", &S); break;
            case 3:  sscanf(line, "%lf", &b); break;
            case 4:  sscanf(line, "%lf", &m); break;
            case 5:  sscanf(line, "%lf", &rho); break;
            case 6:  sscanf(line, "%lf", &CD0); break;
            case 7:  sscanf(line, "%lf", &e); break;
            case 8:  sscanf(line, "%lf", &alpha); break;
            case 9:  sscanf(line, "%lf", &V); break;
            case 10: sscanf(line, "%lf", &gamma_v); break;
            case 11: sscanf(line, "%lf", &x); break;
            case 12: sscanf(line, "%lf", &h); break;
            default: break;
        }

        values_read++;
    }

    printf("%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n%lf\n",
        tf, dt, S, b, m, rho, CD0, e, alpha, V, gamma_v, x, h);

    fclose(fp);
    return 1;
}



void simular_voo() { // Simulação (1)

    FILE *arquivo = fopen("voo_sim.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo voo_sim.txt!\n"); //Verifica se o ficheiro existe
    }

    printf("\nSimulação iniciada...\n");                            //Iniciar a simulação

    double t = 0.0;                             //Reset do tempo

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