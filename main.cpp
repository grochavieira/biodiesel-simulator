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
float biodiesel = 0;
float secadorSolucao = 0;
float solucaoLavagem = 0;
bool lavagemAtiva[3] = {false, false, false};
float lavagem1 = 0;
float lavagem2 = 0;
float lavagem3 = 0;
bool decantadorAtivo = true;
float secadorEtOH = 0;
bool secadorEtohAtivo = true;
bool secadorBiodieselAtivo = false;
bool tanqueReatorEncheu = false;
int ciclos = 0;

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
void tanqueLavagem1();
void tanqueLavagem2();
void tanqueLavagem3();
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
            ciclos++;
            usleep(1000 * 2000);
            cout << "Ciclo " << ciclos << endl;
            cout << "Biodiesel => " << biodiesel << " | Glicerina => " << glicerina << " | Óleo => " << oleo << " | NaOH => " << naoh << " | EtOh => " << etoh << endl;
            cout << "***********************************************" << endl;
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
            // Secador
        }
        if (valorThread == 6)
        {
            // Tanque de NaOH e EtOH
            tanqueNaOH_EtOH();
        }
        if (valorThread == 7)
        {
            // Lavagem 1 (Emulsão)
            tanqueLavagem1();
        }
        if (valorThread == 8)
        {
            // Lavagem 2
            tanqueLavagem2();
        }
        if (valorThread == 9)
        {
            // Lavagem 3 (Água)
            tanqueLavagem3();
        }
        if (valorThread == 10)
        {
            // Secador Biodiesel
            secadorBiodiesel();
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
    //cout << "Enviando carga de óleo: " << cargaOleo << endl;
    //cout << "Óleo: " << oleo << endl;
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

    //cout << "Óleo do Reator: " << reator[0] << endl;
    //cout << "NaOH do Reator: " << reator[1] << endl;
    //cout << "EtOH do Reator: " << reator[2] << endl;

    if (reator[0] >= 1.25 && reator[1] >= 1.25 && reator[2] >= 2.50)
    {
        //cout << "Tanque do Reator ENCHEU" << endl;
        tanqueReatorEncheu = true;
    }

    usleep(1000 * 1000);
}

void tanqueDecantador()
{
    if (decantador < 10.0 && tanqueReatorEncheu)
    {
        float decantadorCopia = decantador;
        float restanteDecantador = 10.0 - decantador;

        decantador += 5.0;
        reator[0] = 0;
        reator[1] = 0;
        reator[2] = 0;

        //cout << "PAUSA DO DECANTADOR" << endl;

        usleep(1000 * 5000 * 1.66);

        //cout << "Enviou para o decantador" << endl;
        //cout << "VALOR DECANTADOR => " << decantador << endl;
        tanqueReatorEncheu = false;
    }

    if (decantador > 0 && decantadorAtivo == true)
    {
        //cout << endl << "DECANTADOR" << endl;
        glicerina += decantador * 0.02;
        secadorEtOH += decantador * 0.09;
        solucaoLavagem += decantador * 0.89;

        decantador = 0;
        lavagemAtiva[0] = true;
        secadorEtohAtivo = true;
        decantadorAtivo = false;
    }
}

void secadorDecantador()
{
    if (secadorEtohAtivo == true)
    {
        etoh += secadorEtOH - (secadorEtOH * 0.03);
        secadorEtOH = 0;
        secadorEtohAtivo = false;
        usleep(1000 * 5000);
    }
}

void tanqueNaOH_EtOH()
{
    usleep(1000 * 1000);
    naoh += 0.25;
    etoh += 0.125;

    //cout << "NaOH: " << naoh << endl;
    //cout << "EtOH: " << etoh << endl;

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

void tanqueLavagem1()
{
    if (lavagemAtiva[0] && solucaoLavagem > 0)
    {
        decantadorAtivo = false;
        lavagem1 += solucaoLavagem - (solucaoLavagem * 0.075);
        solucaoLavagem -= solucaoLavagem * 0.075;
        lavagemAtiva[0] = false;
        usleep(1000 * 1000);
        lavagemAtiva[1] = true;
        //cout << "LAVAGEM 1: " << lavagem1 << endl;
    }
}

void tanqueLavagem2()
{
    if (lavagemAtiva[1])
    {
        lavagem1 = 0;
        lavagem2 += solucaoLavagem - (solucaoLavagem * 0.075);
        solucaoLavagem -= solucaoLavagem * 0.075;
        lavagemAtiva[1] = false;
        usleep(1000 * 1000);
        lavagemAtiva[2] = true;
        //cout << "LAVAGEM 2: " << lavagem2 << endl;
    }
}

void tanqueLavagem3()
{
    if (lavagemAtiva[2])
    {
        lavagem2 = 0;
        lavagem3 += solucaoLavagem - (solucaoLavagem * 0.075);
        solucaoLavagem -= solucaoLavagem * 0.075;
        lavagemAtiva[2] = false;
        //cout << "LAVAGEM 3: " << lavagem3 << endl;
        usleep(1000 * 1000);
        secadorBiodieselAtivo = true;
        secadorSolucao = lavagem3;
        lavagem3 = 0;
    }
}

void secadorBiodiesel()
{
    if (secadorBiodieselAtivo)
    {
        biodiesel += secadorSolucao - (secadorSolucao * 0.03);
        //cout << "ENVIANDO SOLUÇÃO DO SECADOR..." << endl;
        usleep(1000 * secadorSolucao * 5000);
        //cout << endl << "************************" << endl;
        //cout << "ENVIOU PARA O BIODIESEL" << endl;
        //cout << "************************" << endl;
        secadorBiodieselAtivo = false;
        decantadorAtivo = true;
    }
}
// TO DO
// Apresentar uma mensagem quando os componentes mudarem do valor 0 para
// alguma coisa