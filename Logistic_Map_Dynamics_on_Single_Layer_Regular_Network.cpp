//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Time Dynamics Simulation of Coupled Logistic Map on a Single layer Regular Network
// Developed at Complex Systems Lab, IIT Indore India
// October, 31 2019. 
// Ref:
//Contact: sarikajalan9@gmail.com
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////Coupled Logistic Map////////////////////////////
// Li (t+1) = logs(Li(t)) +  eps/sum (aij) * Sum [ Gij * {logs(Lj(t)) - logs(Li(t))} ]
// logs(x)=mu * x * (1-x)
//// Description of variables
//Input:
//            logs        : Logistic map function 
//            mu         : Bifurcation Parameter; (mu=4.0; Chaotic Regime)
//             Nl           : Size of the regular Network
//             Kl           : Node degree or average degree of the Network       
//             G           : NxN Adjacency Matrix
//             eps        : overall coupling strength      
//              L          :  state variable 
//Output: 
//                ---  time x N matrix of the state variable data 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include<iostream>
#include<fstream>
#include<cstdlib>
#include <sys/stat.h>
#include <glob.h>
#include <dirent.h>
#include<random>
#include<ctime>
#include<algorithm>
#include<iterator>
#include<math.h>
#include<ctime>
#include<stdlib.h>
#include<iomanip>
#include<sstream>
#include<string>
#include<iomanip>
#include<sstream>
#include<string>
#include<iomanip>
#include<sstream>
#include<string>
#include<iomanip>
#include<cmath>
#include<cstdlib>
#include<sstream>
#include<random>
#include<vector>

using namespace::std;
using std::ios;
////////////////////Declarations///////////////////////////////////////////////////////////////////////////////////////
#define _USE_MATH_DEFINES
#define WHITE 1
#define GRAY 2
#define NIL 0
#define BLACK 3
///////////External Functions///////////////////////////////////////////////////////////////////////////////////////
void Network_Filei(vector<vector<int> > &g,int File_Num);
void Network_Filed(vector<vector<double> > &g,int File_Num);
void DFS_Visit(vector<vector<int> > &g,int u,int n,int *color,int *pi);
int dfs(vector<vector<int> > &g,int n);
void r_gen(vector<vector<int> > &G, int k, int Nl);
int create_directory(string path);
//////////////////////Logistic Map Function in Chaotic Regieme/////////////////////////////////////////////////////////////////////////////
inline double logs(double x){
  double mu=4.0;
  return mu*x*(1-x);}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(){

//////////////////////////////Inputs//////////////////////////////////////////////////////////
  int Titr=30000;        //Total Iteration
  int Obp=1000;        //Observation period after transient
  int Nl=100;            //Network Size
  int Kl =64;             //Node Degree
  double eps_start=0.35;       //Coupling strength range
  double eps_end=0.5;          //depending on which dynamical state to generate
//----------------Generating regular network-------------------------
  vector<vector<int> > G(Nl,vector<int>(Nl,0));
  r_gen(G,Kl,Nl);
//-----------------printing the Network
//Network_Filei(G,01);
//----------Creating Directories-------------------
string path="data//";
create_directory(path);
//----------setting the range of coupling parameters---------------------------------------
for(int ee=int(eps_start*Nl);ee<=int(eps_end*Nl);ee++){
        //-------------coupling strength parameters-------------------
          double eps=ee/(double)Nl;      //couping strength
        //declaring output file name
        string filename=path+"eps_" + to_string(ee) + ".dat";
        ofstream outFile(filename, std::ios::out | std::ofstream::binary);
        //--------Reading the special initial file-----------------
        ifstream init{"initial_logs.dat"};
          vector<double>initial{istream_iterator<long double>{init}, {}};
        //initializing the state variable array
        vector<vector<double> > L(Nl,vector<double>(2,0.0));
        //Inserting initial condition
        for(int b=0;b<Nl;b++)
                  L[b][0]=initial[b];
  //---------------Iterating------------------------
        for (int t=0;t<Titr;t++){
            for(int i=0;i<Nl;i++){
                  double sum=0.0;
                  int degree=0;
                  for (int j=0;j<Nl;j++){
                              sum+=G[i][j]* ( ( logs(L[j][0])) - ( logs(L[i][0])) );             //calculating the second term
                              degree+=G[i][j];                                                                    // calculating degree
                              }
	            L[i][1]=logs(L[i][0])+(eps/float (degree) ) * sum;
              }
  //--------------------Writing the Data---------------
        if (t>=(Titr-Obp)){
          for(int p=0;p<Nl;p++)
          	  outFile<<"\t"<<L[p][1]<<"\t";
          outFile<<endl;
          }
          //---------------------Writing the Final State-------------------------------
          if (t==Titr-1){
                string filename="eps_" + to_string(ee) + "_final_state.dat";
                ofstream outFile2(filename, std::ios::out | std::ofstream::binary);
                for(int p=0;p<Nl;p++) outFile2<<L[p][1]<<"\n";
                outFile2.close();
          }
//---------------rotating the array for next time step calculation-------------
  		      for (int j=0;j<Nl;j++)
        		    L[j][0]=L[j][1] ;
                }
outFile.close();
}
cout<<"Completes...."<<endl;
return 0;
}
//------------------------------------------------------Functions--------------------------------------------
//:::::::::::::::::::::::::For writing the network::::::::::::::::::::::::::::
void Network_Filei(vector<vector<int> > &g,int File_Num){
  string filename="net_" + to_string(File_Num) +".dat";
  ofstream outFile(filename);
  //ostream_iterator<int> output_iterator(outFile, "\t");
  for(const auto& row : g) {
      std::copy(row.cbegin(), row.cend(), ostream_iterator<int>(outFile, "\t"));
      outFile<< '\n';
    }
 outFile.close();
}
void Network_Filed(vector<vector<double> > &g,int File_Num){
  string filename="net_" + to_string(File_Num) +".dat";
  ofstream outFile(filename);
  //ostream_iterator<int> output_iterator(outFile, "\t");
  for(const auto& row : g) {
      std::copy(row.cbegin(), row.cend(), ostream_iterator<double>(outFile, "\t"));
      outFile<< '\n';
    }
 outFile.close();
}
//:::::::::::::::For connected ness checking in regular network::::::::::::::::::::
void DFS_Visit(vector<vector<int> > &g,int u,int n,int *color,int *pi){
  int *Adj;
  Adj = new int[n];
  int i,j,v;
  color[u]=GRAY;
  for(i=0,j=0;i<n;i++){
   if(g[u][i]==1)
    Adj[j++]=i;
  }
  for(i=0;i<j;i++){
    v=Adj[i];
    if(color[v]==WHITE) {
      pi[v]=u;
      DFS_Visit(g,v,n,color,pi);
    }
  }

  color[u]=BLACK;
  delete [] Adj;
}

int dfs(vector<vector<int> > &g,int n){
  int *color,*pi;
  color = new int[n];
  pi = new int[n];

  for(int i=0;i<n;i++){
     color[i]=WHITE;
     pi[i]=NIL;
  }
  //cout<<"Checking for connectedness..."<<endl;
  DFS_Visit(g,0,n,color,pi);
  for(int i=0;i<n;i++)
   if(color[i]!=BLACK){
     delete [] color;
     delete [] pi;
     return 0;
   }
 delete [] color;
 delete [] pi;
 return n;
}
//-----------------------Generating Regular Network --------------------------------------------
void r_gen(vector<vector<int> > &G, int k, int Nl){
  do{
    for( int i=0; i<Nl; i++){
      for( int j=0; j<Nl; j++){
          if(j==i){
            G[i][j]=0;
            G[j][i]=0;
          }
          else{
            if(abs(i-j)<=k/2){
              G[i][j]=1;
              G[j][i]=1;
            }

            if( abs(i-j)>k/2){
              if((Nl-abs(i-j))<= abs(k/2)){
                  G[i][j]=1;
                  G[j][i]=1;
              }
              else{
                  G[i][j]=0;
                  G[j][i]=0;
              }
          }
        }
  }
}
int nu=dfs(G,Nl);
if(nu=Nl) break;
}while(1);
}
//-------------------------Creating Directory-------------------------------------------------
int create_directory(string path)
{
    DIR *pDir;
    string a = "mkdir -p ";
    string in = a + path;

    pDir = opendir(path.c_str());
    if (pDir == NULL) {
      std::cout<<"Creating Directory..."<<std::endl;
      const int dir = system(in.c_str());
      if (dir< 0)
          return 0;
   }
   else{
     std::cout<<"Directory already exists..."<<std::endl;
     return 1;
   }
   closedir (pDir);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
