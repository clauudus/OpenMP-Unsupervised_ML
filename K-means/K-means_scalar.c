/*
    This code is not part of my work, it was provided through the Virtual Campus from the Advanced Architectures course, from Computer Science bachelor degree.
    I am adding this code here only to illustrate the beginning of all the implementations.
    All rights reserved its respective the authors.
*/

#include <stdio.h>
#include <stdlib.h>

int minim_arg(double *V, int C)
{
  double m = V[0];
  int i, r = 0;
  for (i=1; i<C; i++)
    if (m > V[i])
    {
      r = i;
      m = V[i];
    }
  return r;
}

int recalcularCentro (int *Puntos, double *Centroides, int *PC, int *Sep, int N, int D, int C)
{
  // Array Auxiliar: D debe ser un valor pequeño para definir estructura dinámica en stack
  double v[D];

  int i, j, k, x, r=0;

  for (i=0; i<C; i++)
  {
    for (x=0; x<D; x++)
      v[x] = 0;

    for (j=0; j<Sep[i]; j++)
      for(k=0; k<D; k++) 
        v[k] += Puntos[ PC[i*N+j]*D + k];

    for(x=0; x<D; x++)
    {
      v[x] = v[x] / Sep[i];
      if (v[x] != Centroides[i*D+x]) 
        r = 1;
      Centroides[i*D+x] = v[x];
    }
  }
  return r;
}

void generarPuntos(int *Puntos, double *Centroides, int N, int D, int C, int DIMENSION)
{
  int i, j;

  for(i=0; i<N; i++)
    for(j=0; j<D; j++)
      Puntos[D*i+j] = random()%DIMENSION;

  for(i=0; i<C; i++)
  {
    int k = random()%N;
    for(j=0; j<D; j++)
      Centroides[i*D+j] = Puntos[k*D+j];
  }
}

void PuntosToCentroides(int *Puntos, double *Centroides, int *PC, int *Sep, int N, int D, int C)
{
  int i, j, k, m;

  // Array Auxiliar: C debe ser un valor pequeño para definir estructura dinámica en stack
  double dist[C];

  for (i=0; i<C; i++)
    Sep[i]=0;

  for(i=0; i<N; i++)
  {
    for(j=0; j<C; j++)
    {
      dist[j] = 0;
      for(k=0; k<D; k++)
	dist[j] += (Puntos[i*D+k] - Centroides[j*D+k]) * (Puntos[i*D+k] - Centroides[j*D+k]);
    }
    m = minim_arg(dist, C);
    PC[m*N+Sep[m]] = i;
    Sep[m] += 1;
  }
}


void main(int argc, char **argv)
{
  int N;	 // Numero de puntos
  int D;	 // Dimensiones de los puntos
  int C;	 // Numero de centroides
  int DIMENSION; // Dimension del espacio de puntos

  srand(7);

  // Parametros por defecto
  N = 1000; 
  D = 3;	
  C = 4;
  DIMENSION = 1000;
    
  // Parametros pasados por argumento
  if (argc>1) N = atoi(argv[1]); 
  if (argc>2) D = atoi(argv[2]); 
  if (argc>3) C = atoi(argv[3]);
  if (argc>4) DIMENSION = atoi(argv[4]);

  int    *Puntos     = (int*)    malloc( sizeof(int)*N*D );	// Puntos
  double *Centroides = (double*) malloc( sizeof(double)*C*D );	// Centroides
  int    *PC         = (int*)    malloc( sizeof(int)*N*C );	// Asignación punto a centroide
  int    *Sep        = (int*)    malloc( sizeof(int)*C );	// Num de puntos en centroide

  int cont = 0, final;

  printf("K-means, %d Puntos con %d Dimensiones y %d Centroides\n",N,D,C);
  printf("El espacio de puntos es de Dimensión: %d \n", DIMENSION);
	
  generarPuntos(Puntos, Centroides, N, D, C, DIMENSION);

  do {
    PuntosToCentroides(Puntos, Centroides, PC, Sep, N, D, C);
    cont +=1;
    final = recalcularCentro(Puntos, Centroides, PC, Sep, N, D, C);
  }
  while(final);

  printf("Numero de iteraciones necesarias: %d\n", cont);	
}
