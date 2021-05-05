#include <stdio.h>
#include <chrono>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <queue>
#include <time.h>

using namespace std;

#define Componentes 14

int Componente[Componentes];
float etoh;
float naoh;
float oleo;
float reator[3] = {0, 0, 0};
float decantador = 0;
float glicerina = 0;
float solucaoLavagem = 0;

pthread_cond_t condicao[Componentes];

void *threadComponente(void *arg);
void oleoResidualFritura();
void tanqueOleo();
void tanqueReator();
void tanqueDecantador();
void tanqueGlicerina();
void secadorDecantador();
void tanqueEtOH();
void tanqueNaOH_EtOH();
void lavagem1();
void lavagem2();
void lavagem3();
void secadorBiodiesel();
void tanqueBiodiesel();

int main(int argc, char *argv[])
{ //main

    pthread_t ph_thread[Componentes];

    for (int i = 0; i < Componentes; i++)
    {
        pthread_cond_init(&condicao[i], NULL);
        Componente[i] = i;
    }

    for (int i = 0; i < Componentes; i++)
    {
        printf("%4d - Componente #%d foi inicializado.\n", i, i);
        pthread_create(&ph_thread[i], NULL, threadComponente, &Componente[i]);
        sleep(0);
    }

    for (int i = 0; i < Componentes; i++)
        pthread_join(ph_thread[i], NULL);

    for (int i = 0; i < Componentes; i++)
        pthread_cond_destroy(&condicao[i]);

    return 0;
}

void *threadComponente(void *arg)
{
    int valorThread = *(int *)arg;

    while (1)
    {
        if (valorThread == 0)
        {
            // Orquestrador
        }
        if (valorThread == 1)
        {
            // Óleo residual de fritura
            oleoResidualFritura();
        }
        if (valorThread == 2)
        {
            // Tanque de Óleo
            tanqueOleo();
        }
        if (valorThread == 3)
        {
            // Reator
            tanqueReator();
        }
        if (valorThread == 4)
        {
            // Decantador
            tanqueDecantador();
        }
        if (valorThread == 5)
        {
            // Tanque de Glicerina
        }
        if (valorThread == 6)
        {
            // Secador
        }
        if (valorThread == 7)
        {
            // Tanque de EtOH
        }
        if (valorThread == 8)
        {
            // Tanque de NaOH e EtOH
            tanqueNaOH_EtOH();
        }
        if (valorThread == 9)
        {
            // Lavagem 1 (Emulsão)
        }
        if (valorThread == 10)
        {
            // Lavagem 2
        }
        if (valorThread == 11)
        {
            // Lavagem 3 (Água)
        }
        if (valorThread == 12)
        {
            // Secador Final
        }
        if (valorThread == 13)
        {
            // Tanque de Biodiesel
        }
    }

    return 0;
}

void oleoResidualFritura()
{
    int tempo = (rand() % (1000 * 1000 * 10));
    usleep(tempo);
    float aux = rand() % 1000;
    float cargaOleo = 1.0 + (aux / 1000);
    oleo += cargaOleo;
    cout << "Enviando carga de óleo: " << cargaOleo << endl;
    cout << "Óleo: " << oleo << endl;
    usleep((1000 * 10000) - tempo);
}

void tanqueOleo()
{
    if (reator[0] < 1.25 && oleo >= 1.25)
    {
        float reatorOleo = reator[0];
        float oleoRestante = 1.25 - reatorOleo;
        reator[0] += oleoRestante;
        oleo -= oleoRestante;
    }
}

void tanqueReator()
{
    bool transferiuConteudo = false;
    if (reator[0] >= 1.25 && reator[1] >= 1.25 && reator[2] >= 2.50)
    {
        cout << "ENCHEU" << endl;

        if (decantador < 10.0)
        {
            float decantadorCopia = decantador;
            float restanteDecantador = 10.0 - decantador;
            if (restanteDecantador >= 3.0)
            {
                decantador += 3.0;
                reator[0] = 1.25 / (3 / 5);
                reator[1] = 1.25 / (3 / 5);
                reator[2] = 2.50 / (3 / 5);
            }
            else
            {
                decantador += restanteDecantador;
                reator[0] = 1.25 / (restanteDecantador / 5);
                reator[1] = 1.25 / (restanteDecantador / 5);
                reator[2] = 2.50 / (restanteDecantador / 5);
            }

            cout << "Enviou para o decantador" << endl;
            transferiuConteudo = true;
        }
    }
    cout << "Óleo do Reator: " << reator[0] << endl;
    cout << "NaOH do Reator: " << reator[1] << endl;
    cout << "EtOH do Reator: " << reator[2] << endl;

    if (!transferiuConteudo)
    {
        usleep(1000 * 1000);
    }
    else
    {
        usleep(1000 * 5000);
        transferiuConteudo = false;
    }
}

void tanqueDecantador()
{
    if (decantador == 10.0)
    {
        glicerina += decantador * 0.02;
        etoh += decantador * 0.09;
        solucaoLavagem += decantador * 0.89;

        decantador = 0;
    }
}

void tanqueGlicerina()
{
}

void secadorDecantador()
{
}

void tanqueEtOH()
{
}

void tanqueNaOH_EtOH()
{
    usleep(1000 * 1000);
    naoh += 0.25;
    etoh += 0.125;

    cout << "NaOH: " << naoh << endl;
    cout << "EtOH: " << etoh << endl;

    if (reator[1] < 1.25 && naoh >= 1.25)
    {
        float reatorNAOH = reator[1];
        float naohRestante = 1.25 - reatorNAOH;
        reator[1] += naohRestante;
        naoh -= naohRestante;
    }
    if (reator[2] < 2.50 && etoh >= 2.50)
    {
        float reatorETOH = reator[2];
        float etohRestante = 2.50 - reatorETOH;
        reator[2] += etohRestante;
        etoh -= etohRestante;
    }
}

void lavagem1()
{
}

void lavagem2()
{
}

void lavagem3()
{
}

void secadorBiodiesel()
{
}

void tanqueBiodiesel()
{
}