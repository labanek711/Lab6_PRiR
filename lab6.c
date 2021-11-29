#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>


double leibniz (float N) {
    double pi=1.0;
    int i;
    int n;
    for (i=3, n=2*N+1; i<=n; i+=2)
        pi += ((i&2) ? -1.0 : 1.0) / i;
    return 4*pi;
    
}
    double wzorfunkcji(double x)
    {
    return pow(x, 2);
    }
    double wys(double a, double b, double N){
        return (b - a) / N;
    }

int main(int argc,char **argv){

    int rank;
    int num_proc;
    double leib;
    int tag = 1999;
    MPI_Status status;

    double a = 5;
    double b = 10;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0)
    {
       leib = leibniz	(num_proc);
        printf("%d rank leibniz = %f \n",rank, leib);
        MPI_Send(&leib,1,MPI_DOUBLE,rank+1,tag,MPI_COMM_WORLD);
    }else if(rank+1 == num_proc) {
        MPI_Recv(&leib,1,MPI_DOUBLE,rank-1,tag,MPI_COMM_WORLD,&status);
        printf("%d rank leibniz = %f \n",rank+1, leib);
    }else{
        MPI_Recv(&leib,1,MPI_DOUBLE,rank-1,tag,MPI_COMM_WORLD,&status);
        printf("%d rank leibniz = %f \n",rank, leib);
        MPI_Send(&leib,1,MPI_DOUBLE,rank+1,tag,MPI_COMM_WORLD);
    }

    // Metoda prostokątów
     double suma = 0;
     double dx = ( b - a) / num_proc;
        if(rank == num_proc - 1 ){ 
            
        suma += wzorfunkcji(a + rank * dx );
        
        MPI_Send(&suma,1,MPI_DOUBLE,rank-1,tag,MPI_COMM_WORLD);
        }
        else if(rank != 0 && rank < num_proc - 1)  { 
            MPI_Recv(&suma,1,MPI_DOUBLE,rank+1,tag,MPI_COMM_WORLD,&status);
            suma += wzorfunkcji(a + rank * dx );
            
            MPI_Send(&suma,1,MPI_DOUBLE,rank-1,tag,MPI_COMM_WORLD);
        }else{
            MPI_Recv(&suma,1,MPI_DOUBLE,rank+1,tag,MPI_COMM_WORLD,&status);
            suma *= dx;
            printf("suma prostokatow = %f\n", suma);     
        }

    //Metoda Trapezów
        double xi = 0;

        if(rank == num_proc - 1 ){ 
            xi += (wzorfunkcji(a + (((double) rank / (double) num_proc) * (b - a))));
        MPI_Send(&xi,1,MPI_DOUBLE,rank-1,tag,MPI_COMM_WORLD);

        }
        else if(rank != 0 && rank < num_proc - 1)  { 
            MPI_Recv(&xi,1,MPI_DOUBLE,rank+1,tag,MPI_COMM_WORLD,&status);
            xi += (wzorfunkcji(a + (((double) rank / (double) num_proc) * (b - a))));           
            MPI_Send(&xi,1,MPI_DOUBLE,rank-1,tag,MPI_COMM_WORLD);
        }else{
            MPI_Recv(&xi,1,MPI_DOUBLE,rank+1,tag,MPI_COMM_WORLD,&status);
            suma = wys(a,b,num_proc) * (wzorfunkcji(a) / 2 + xi + wzorfunkcji(b)/2);
            printf("rank %d, suma trapezów = %f \n", rank, suma);
        }
    MPI_Finalize();
    return 0;
}
