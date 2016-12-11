#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <bitset>

using namespace std;
#define NT 0 // not taken
#define T 1 // taken
//#define PredictTakenWithConfidence 11
//#define PredictTakenWithWeakConfidence 10
//#define PredictNotTakenWithConfidence 00
//#define PredictNotTakenWithWeakConfidence 0


struct config{
       int config_read;
       };

struct trace{
       string hexadd;
       int actualstatus;
       };


int main(int argc, char* argv[]){



    config branchpredictionconfig;
    ifstream branchprediction_params;
    branchprediction_params.open("config.txt");
    branchprediction_params>>branchpredictionconfig.config_read;
    int no = branchpredictionconfig.config_read;
    cout<<"no:"<<no<<endl;
    int rowsize=pow(2,no);
    cout<<"m in int:"<<rowsize<<endl;



    trace branchpredictionTrace;
    ifstream traces;
    ofstream tracesout;
    string outname;
    outname = string("tracetxt.out");

    traces.open("trace.txt");
    tracesout.open(outname.c_str());

    string line;
    string xaddr;       // the address from the memory trace store in hex;
    unsigned int addr;  // the address from the memory trace store in unsigned int;
    //bitset<32> accessaddr; // the address from the memory trace store in the bitset;
    if (traces.is_open()&&tracesout.is_open()){
      while (getline (traces,line)){
      traces>>branchpredictionTrace.hexadd;
      traces>>branchpredictionTrace.actualstatus;
      xaddr=branchpredictionTrace.hexadd;
      stringstream saddr(xaddr);
      saddr >> std::hex >> addr;
      //accessaddr = bitset<32> (addr);


      int TakenState =0;
      int NotTakenState=0;

      cout<<"hexadd:"<<branchpredictionTrace.hexadd<<" "<<"ActualStatus:"<<branchpredictionTrace.actualstatus<<endl;
      cout<<"addr:"<<xaddr<<endl;
      cout<<"accessaddr:"<<addr<<endl;

      const int coln=2;
      int rows=rowsize;
     //int PredictTakenWithConfidence[2]= {1,1};
     //cout<<"bitset:"<<PredictTakenWithConfidence[0]<<endl;





      tracesout<< TakenState << " " << NotTakenState << endl;

    }

    traces.close();
    tracesout.close();
    }


else cout<< "Unable to open trace or traceout file";

   return 0;
}
