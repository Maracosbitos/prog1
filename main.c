#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Definição de constantes
#define MAX_LINE 100
#define PI 3.14159265

double tf, dt, S, b, m, rho, CD0, e, alpha, g; //tens de ler
double V, h, x, gamma = 0;
double D, L = 0;

void mostrar_menu()
{
    printf("\nLista de opções:\n");
    printf("0 – Terminar o programa\n");
    printf("1 – Simular o movimento da aeronave\n");
    printf("2 – Determinar os valores mínimos e máximos\n");
    printf("Selecione a opção: ");
}

int scan_parametros(void)
{
    FILE *fp = fopen("config_modelo.txt", "w");
    if (fp == NULL)
    {
        printf("Erro ao abrir arquivo config_modelo.txt\n");
        fclose(fp);
        return 0;
    }

    while (fgetc(fp) == '%') // Se encontrar '%', pula a linha
        while (fgetc(fp) != '\n');

    // Ler os valores do arquivo diretamente para as variáveis globais
    if (fscanf(fp, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
               &tf, &dt, &S, &b, &m, &rho, &CD0, &e, &alpha, &V, &gamma, &x, &h) != 13) {
        printf("Erro: Parâmetros insuficientes no arquivo!\n");
        fclose(fp);
        return 0;
               }

    fclose(fp);
    return 1;
}

// Funções para calcular as derivadas
void ddt(V, gamma, D, L, double *dV, double *dGamma) {
    *dV = (1.0 / m) * (-D - m * g * sin(gamma));
    *dGamma = (1.0 / m) * V * (L - m * g * cos(gamma));
}

// Método de Euler para atualizar os valores
void euler(double *V, double *gamma, double D, double L, double dt) {
    double dV, dGamma;
    ddt(*V, *gamma, D, L, &dV, &dGamma);
    *V += dV * dt;
    *gamma += dGamma * dt;
}

void VeGamma() {
    // Condições iniciais
    double V = 10.0;        // Velocidade inicial (m/s)
    double gamma = M_PI / 6; // Ângulo inicial (30 graus)
    double D = 0.5;         // Arrasto (exemplo)
    double L = 2.0;         // Sustentação (exemplo)
    double dt = 0.01;       // Passo de tempo
    int steps = 100;        // Número de iterações

    // Simulação
    for (int i = 0; i < steps; i++) {
        euler(&V, &gamma, D, L, dt);
        printf("t = %.2f, V = %.5f, gamma = %.5f rad (%.2f°)\n", i * dt, V, gamma, gamma * 180 / M_PI);
    }
}

void ddt(double V, double gamma, double *dx, double *dh) {
    *dx = V * cos(gamma);
    *dh = V * sin(gamma);
}

void euler2(double *x, double *h, double V, double gamma, double dt) {
    double dx, dh;
    ddt(V, gamma, &dx, &dh);
    *x += dx * dt;
    *h += dh * dt;
}

int main() {
    mostrar_menu();

    //Isto serve como default
    tf = 6.0;
    dt = 1e-5;
    S = 2.0e-2;
    b = 14e-2;
    m = 5e-3;
    rho = 1.225;
    CD0 = 0.02;
    e = 0.9;
    alpha = 0.1;

    double x = 0.0, h = 0.0;  // Condições iniciais
    double V = 1.0;           // Velocidade constante (exemplo)
    double gamma = M_PI / 4;  // Ângulo de 45 graus (exemplo)
    double dt = 0.01;         // Passo de tempo
    int steps = 100;          // Número de iterações

    for (int i = 0; i < steps; i++) {
        euler2(&x, &h, V, gamma, dt);
        printf("t = %.2f, x = %.5f, h = %.5f\n", i * dt, x, h);
    }

    return 0;
}

void Drift(double Cd, double Cl){
    Cl = (alpha*(pow(b, 2))/S*PI)/(1+sqrt((1+pow((((pow(b, 2))/S)/2), 2))));
    Cd = (CD0 + (1/(PI*e*AR))*Cl);
    D = Cd*0,5*rho*(pow(V, 2)*S);
    L = Cl*0,5*rho*(pow(V, 2)*S);
}


