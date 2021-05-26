#include <stdio.h>
#include <chrono>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <queue>
#include <time.h>

using namespace std;

#define Componentes 11

int temp = 100;

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
bool reatorAtivo = false;
bool lavagemAtivo = false;

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

  while (ciclos <= 3600)
  {
    if (valorThread == 0)
    {
      // Orquestrador
      ciclos++;
      usleep(temp * 1000);
      cout << "Ciclo " << ciclos << endl;
      cout << "Biodiesel => " << biodiesel << " | Glicerina => " << glicerina << " | " << endl
           << "Óleo => " << oleo << " | NaOH => " << naoh << " | EtOH => " << etoh << endl;
      cout << "Óleo do Reator => " << reator[0] << " | NaOH do Reator => " << reator[1] << " | EtOH do Reator => " << reator[2] << endl
           << "Valor do Decantador => " << decantador << endl;
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
      // Tanque de NaOH e EtOH
      tanqueNaOH_EtOH();
    }
    if (valorThread == 6)
    {
      // Lavagem 1 (Emulsão)
      tanqueLavagem1();
    }
    if (valorThread == 7)
    {
      // Lavagem 2
      tanqueLavagem2();
    }
    if (valorThread == 8)
    {
      // Lavagem 3 (Água)
      tanqueLavagem3();
    }
    if (valorThread == 9)
    {
      // Secador Biodiesel
      secadorBiodiesel();
    }
    if (valorThread == 10)
    {
      // Secador Decantador
      secadorDecantador();
    }
  }

  return 0;
}

void oleoResidualFritura()
{
  int tempo = (rand() % (temp * 1000 * 10));
  usleep(tempo);
  cout << endl
       << "++++++++++++++++++++"
       << "\nENVIANDO ÓLEO\n"
       << "++++++++++++++++++++" << endl;
  float aux = rand() % 1000;
  float cargaOleo = 1.0 + (aux / 1000);
  oleo += cargaOleo;
  usleep((temp * 10000) - tempo);
  cout << endl
       << "++++++++++++++++++++"
       << "\nÓLEO ENVIADO\n"
       << "++++++++++++++++++++" << endl;
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
  if (reator[0] >= 1.25 && reator[1] >= 1.25 && reator[2] >= 2.50)
  {
    tanqueReatorEncheu = true;
    if (!reatorAtivo)
      cout << "O REATOR lançou os produtos quimicos para o DECANTADOR." << endl;
    reatorAtivo = true;
  }

  usleep(temp * 1000);
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

    usleep(temp * 5000 * 1.66);

    tanqueReatorEncheu = false;
  }

  if (decantador > 0 && decantadorAtivo == true)
  {
    glicerina += decantador * 0.02;
    secadorEtOH += decantador * 0.09;
    solucaoLavagem += decantador * 0.89;

    decantador = 0;
    lavagemAtiva[0] = true;
    secadorEtohAtivo = true;
    decantadorAtivo = false;

    cout << "O DECANTADOR lançou os produtos quimicos." << endl;
  }
}

void secadorDecantador()
{
  if (secadorEtohAtivo == true)
  {
    cout << "Enviou do secador EtOH para o EtOH" << endl;
    etoh += secadorEtOH - (secadorEtOH * 0.03);
    secadorEtOH = 0;
    secadorEtohAtivo = false;
    usleep(temp * 5000);
  }
}

void tanqueNaOH_EtOH()
{
  usleep(temp * 1000);
  naoh += 0.25;
  etoh += 0.125;

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
    cout << "Solução para Lavagem: " << solucaoLavagem << endl;
    decantadorAtivo = false;
    lavagem1 += solucaoLavagem - (solucaoLavagem * 0.075);
    solucaoLavagem -= solucaoLavagem * 0.075;
    lavagemAtiva[0] = false;
    usleep(temp * 1000);
    lavagemAtiva[1] = true;

    if (!lavagemAtivo)
    {
      cout << "A lavagem de solução ativou" << endl;
      lavagemAtivo = true;
    }
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
    usleep(temp * 1000);
    lavagemAtiva[2] = true;
    cout << "\nLAVAGEM 2: " << lavagem2 << endl;
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
    cout << "\nLAVAGEM 3: " << lavagem3 << endl;
    usleep(temp * 1000);
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
    cout << "\nENVIANDO SOLUÇÃO DO SECADOR...\n";
    usleep(temp * secadorSolucao * 5000);
    cout << "\n\n+++++++++++++++++++++++++\n";
    cout << "ENVIOU PARA O BIODIESEL";
    cout << "\n+++++++++++++++++++++++++\n\n";
    secadorBiodieselAtivo = false;
    decantadorAtivo = true;
  }
}
