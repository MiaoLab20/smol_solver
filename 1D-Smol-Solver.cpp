#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>

// M total numbers of discrete point for this 1D space
#define M 120
#define X 2

using namespace std;

int main(int argc, char** argv) 
{
  if(argc < 5)
  {
    cout << "Usage: exec inputfile startpoint endpoint N(number of itr) " << endl;
    return 1;
  }

  double u[M];
  double p[M][X];

  double tmps, tmph;
  double x;
  int xmin, xts, CTL;

  // dt=0.1;
  // dx=1.0;

  double starta = atof(argv[2]);
  double stopa = atof(argv[3]);
  int bc = atoi(argv[4]); // 1: both absorbing; 2: both reflective; 3: left reflective/right absorbing; 4: left absorbing/right reflective
  double dx = atof(argv[5]); // bin size
  double dt = atof(argv[6]); // time step
  int N = atoi(argv[7]);

  // printf("%f %f %d %f %f %d\n", starta, stopa, bc, dx, dt, N);

  int i, j, m, k;
  double key1;
  double key2;

  //read in scaled potential u
  ifstream in(argv[1]);  

  if(in.fail())
  {
    cout << "File not found!! " << endl;
    return 2;
  }

  for(m=0; m < M; m++)
  {

    in >> key2 >> key1;
    u[m]= key1;

    if(key2 == starta)
      CTL = m + 1;

    if(key2 == stopa)
      xts = m - 1;

    p[m][0]=p[m][1]=0.0;
  }

  
  // find min and ts from potential u
  tmps=1000000.0;
  tmph=-1000000.0;
  
  for(m = CTL; m <= xts; m++)
  {
      if(u[m] < tmps)
      {
        tmps = u[m];
        xmin = m;
      }
  }

  // printf(" min and ts are %d %f and %d %f\n", xmin,tmps, xts-1, tmph);
  
  tmps=0.0;

  //setup init boltzmann conditions
  for(m = CTL; m <= xts; m++)
  {
    p[m][0]=p[m][1]=exp(-(u[m]-u[xmin]));     
    tmps+=p[m][0];
    // printf(" m,tmps are %d %f\n", m,tmps);
  }

  cout << "t(s)	Ln(S(t))" << endl;

  k = 0;
  for(i = 1; i <= N; i++)
  {
    tmph=0.0;
    // dynamic update

    if ( bc == 1 ) {
      // both absorbing
      for(m = 0; m < CTL; m++)
      {
        p[m][0] = 0.0;
      }
      for(m = xts + 1; m < M; m++)
      {
        p[m][0] = 0.0;
      }
    } else if ( bc == 2 ) {
      // both reflective
      for(m = 0; m < CTL; m++)
      {
        p[m-1][0] = p[m][0] * (1.0-(u[m]-u[m-1])) ;
      }
      for(m = xts + 1; m < M; m++)
      {
        p[m][0] = p[m-1][0] / (1.0-(u[m]-u[m-1])) ;
      }
    } else if ( bc == 3 ) {
      // left reflective/right absorbing
      for(m = 0; m < CTL; m++)
      {
        p[m-1][0] = p[m][0] * (1.0-(u[m]-u[m-1])) ;
      }
      for(m = xts + 1; m < M; m++)
      {
        p[m][0] = 0.0;
      }
    } else if ( bc == 4 ) {
      // left absorbing/right reflective
      for(m = 0; m < CTL; m++)
      {
        p[m][0] = 0.0;
      }
      for(m = xts + 1; m < M; m++)
      {
        p[m][0] = p[m-1][0] / (1.0-(u[m]-u[m-1])) ;
      }
    }

    for(m = CTL; m <= xts; m++)
    {
      p[m][1] = p[m][0] + ( p[m+1][0]*exp(-(u[m]-u[m+1])/2.0) + p[m-1][0]*exp(-(u[m]-u[m-1])/2.0)
				    - p[m][0]*(exp(-(u[m+1]-u[m])/2.0) + exp(-(u[m-1]-u[m])/2.0))) * dt / (dx*dx);

      if(m == CTL)
      {
        p[m][1]=p[m][0] + ( p[m+1][0]*exp(-(u[m]-u[m+1])/2.0)
                                    - p[m][0]*(exp(-(u[m+1]-u[m])/2.0) + exp(-(u[m-1]-u[m])/2.0))) * dt / (dx*dx);
      }
	 
      if(m == xts)
      {
        p[m][1]=p[m][0] + ( p[m-1][0]*exp(-(u[m]-u[m-1])/2.0)
                                    - p[m][0]*(exp(-(u[m-1]-u[m])/2.0) + exp(-(u[m+1]-u[m])/2.0))) * dt / (dx*dx);
      }


      if(p[m][1]<0)
      { 
        p[m][1] = 0;
      }

      tmph+=p[m][1];      
    }	 
      
    k++;
    //output and exchange p[m][0] to p[m][1]
    if(k==10000)
    {  
      // cout << i*dt << " tmps " << tmps << endl;
      // cout << i*dt << " tmph/tmps " << tmph/tmps << endl;
      cout << i*dt << " " << log(tmph/tmps) << endl;
      k=0;
    }

    for(m=0;m<M;m++)
    {
      // printf(" %d %f\n",m,p[m][0]);
      p[m][0]=p[m][1];
    }
  }

  return 0;
}
