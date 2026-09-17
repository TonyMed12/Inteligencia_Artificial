#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define epoca 500000
#define K 0.03f

/* Función de entrenamiento de la red completa */
float EntNt(float, float, float);

/* Funciones de entrenamiento de la capa oculta */
float EntNtAND(float, float, float);
float EntNtOR(float, float, float);

/* Función para usar la red después del entrenamiento */
float InitNt(float, float);

/* Función sigmoide */
float sigmoide(float);

/* Inicializar pesos aleatorios */
void pesos_initNt(void);

/*
    Pesos originales.
*/
float Pesos[2];
float bias = 0.5f;

/* Pesos adicionales de la neurona AND */
float PesosAND[2];
float biasAND = 0.5f;

/* Pesos adicionales de la neurona OR */
float PesosOR[2];
float biasOR = 0.5f;

/* Último error de la neurona XOR */
float Error;

float EntNt(float x0, float x1, float target)
{
  float net = 0;
  float out = 0;
  float delta[2];

  float salidaAND;
  float salidaOR;

  float targetAND;
  float targetOR;

  /*
      Obtener automáticamente las respuestas
      correctas de AND y OR.
  */
  targetAND = x0 * x1;

  if (x0 == 1 || x1 == 1)
  {
    targetOR = 1;
  }
  else
  {
    targetOR = 0;
  }

  /*
      Primera capa:
      entrenar AND y OR.
  */
  salidaAND = EntNtAND(x0, x1, targetAND);
  salidaOR = EntNtOR(x0, x1, targetOR);

  /*
      Segunda capa:
      las entradas ya no son x0 y x1.        Ahora son las salidas de AND y OR.
  */
  net = Pesos[0] * salidaAND + Pesos[1] * salidaOR - bias;

  net = sigmoide(net);

  Error = target - net;

  /*
      Ajustar el bias de la neurona XOR.
  */
  bias -= K * Error;

  /*
      Ajustar los pesos de la neurona XOR.
  */
  delta[0] = K * Error * salidaAND;
  delta[1] = K * Error * salidaOR;

  Pesos[0] += delta[0];
  Pesos[1] += delta[1];

  out = net;

  return out;
}

/*
    Entrenamiento de la neurona AND.
*/
float EntNtAND(float x0, float x1, float target)
{
  float net = 0;
  float out = 0;
  float ErrorAND;
  float delta[2];

  net = PesosAND[0] * x0 + PesosAND[1] * x1 - biasAND;

  net = sigmoide(net);

  ErrorAND = target - net;

  biasAND -= K * ErrorAND;

  delta[0] = K * ErrorAND * x0;
  delta[1] = K * ErrorAND * x1;

  PesosAND[0] += delta[0];
  PesosAND[1] += delta[1];

  out = net;

  return out;
}

/*
    Entrenamiento de la neurona OR.
*/
float EntNtOR(float x0, float x1, float target)
{
  float net = 0;
  float out = 0;
  float ErrorOR;
  float delta[2];

  net = PesosOR[0] * x0 + PesosOR[1] * x1 - biasOR;

  net = sigmoide(net);

  ErrorOR = target - net;

  biasOR -= K * ErrorOR;

  delta[0] = K * ErrorOR * x0;
  delta[1] = K * ErrorOR * x1;

  PesosOR[0] += delta[0];
  PesosOR[1] += delta[1];

  out = net;

  return out;
}

/*
    Ejecutar la red sin modificar pesos.
*/
float InitNt(float x0, float x1)
{
  float netAND = 0;
  float netOR = 0;
  float netXOR = 0;
  float out = 0;

  /*
      Primera capa: obtener AND.
  */
  netAND = PesosAND[0] * x0 + PesosAND[1] * x1 - biasAND;
  netAND = sigmoide(netAND);

  /*
      Primera capa: obtener OR.
  */
  netOR = PesosOR[0] * x0 + PesosOR[1] * x1 - biasOR;
  netOR = sigmoide(netOR);

  /*
      Segunda capa: obtener XOR.
  */
  netXOR = Pesos[0] * netAND + Pesos[1] * netOR - bias;
  netXOR = sigmoide(netXOR);

  out = netXOR;

  return out;
}

/*
    Inicializar los pesos de las tres neuronas.
*/
void pesos_initNt(void)
{
  int i;

  for (i = 0; i < 2; i++)
  {
    Pesos[i] = (float)rand() / RAND_MAX;
    PesosAND[i] = (float)rand() / RAND_MAX;
    PesosOR[i] = (float)rand() / RAND_MAX;
  }
}

float sigmoide(float s)
{
  return 1.0f / (1.0f + expf(-s));
}

int main(void)
{
  int i = 0;
  float apr;

  srand(time(NULL));

  pesos_initNt();

  while (i < epoca)
  {
    /*
        Dataset XOR:

        1 XOR 1 = 0
        1 XOR 0 = 1
        0 XOR 1 = 1
        0 XOR 0 = 0
    */

    apr = EntNt(1, 1, 0);
    apr = EntNt(1, 0, 1);
    apr = EntNt(0, 1, 1);
    apr = EntNt(0, 0, 0);

    /*
        Mostrar resultados cada 10,000 épocas.
    */
    if (i % 10000 == 0 || i == epoca - 1)
    {
      printf("------------------------\n");
      printf("Salida Entrenamiento Epoco %d\n", i);

      /*
          InitNt solamente consulta la red.
          No vuelve a modificar sus pesos.
      */
      apr = InitNt(1, 1);
      printf("1,1=%f\n", apr);

      apr = InitNt(1, 0);
      printf("1,0=%f\n", apr);

      apr = InitNt(0, 1);
      printf("0,1=%f\n", apr);

      apr = InitNt(0, 0);
      printf("0,0=%f\n", apr);

      printf("\n");

      printf("Pesos de cada epoca\n");
      printf("Peso 0 = %f\n", Pesos[0]);
      printf("Peso 1 = %f\n", Pesos[1]);
      printf("Bias = %f\n", bias);
      printf("Error %f\n", Error);

      printf("------------------------\n");
    }

    i++;
  }

  return 0;
}