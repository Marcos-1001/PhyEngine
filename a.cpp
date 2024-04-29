double matriz(double X[M][N])
{
    int i, j;
    float suma;

    X_temp  = X

    #pragma omp parallel for private(X_temp, j) shared(X) 
    for (i = 0; i < M - 1; i++)
    {
        for (j = 0; j < N; j++)
        {
            X[i][j] = 2.0 * X_temp[i + 1][j];
        }
    }
    suma = 0.0;
    #pragma omp parallel for private(suma, j) reduction(+:suma)
    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++)
        {
            suma = suma + X[i][j];
        }
    }
    return suma;
}