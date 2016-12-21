#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <bitset>
#include <array>

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
    long int rows=pow(2,no);
    cout<<"2 raise to m:"<<no<<"is"<<rows<<endl;


    trace branchpredictionTrace;
    ifstream traces;
    ofstream tracesout;
    string outname;
    outname = string("tracetxt.out");

    traces.open("trace.txt");
    tracesout.open(outname.c_str());

    array<bool,2> strongly_taken = {true, true};
    array<bool,2> weakly_taken = {true, false};
    array<bool,2> strongly_not_taken = {false, false};
    array<bool,2> weakly_not_taken = {false, true};

    string line;
    string xaddr;       // the address from the memory trace store in hex;
    int actualstatus;
    unsigned int addr;  // the address from the memory trace store in unsigned int;
    bitset<32> accessaddr; // the address from the memory trace store in the bitset;
    const int coln=2;
    cout<<"reached here declare array"<<endl;
    bool saturatingcounter[rows][coln]={};
    cout<<"reached here for loop"<<endl;
      for (long int i=0; i<rows; i++){
        for (int j=0; j<coln; j++){
            saturatingcounter[i][j] = true;
        }
      }

    int lineCounter = 0;
    int misPredictCounter = 0;
    cout<<"reached here"<<endl;
    if (traces.is_open()&&tracesout.is_open()){
      while (getline (traces,line)){
//      traces>>branchpredictionTrace.hexadd;
//      traces>>branchpredictionTrace.actualstatus;
//      xaddr=branchpredictionTrace.hexadd;
//      actualstatus=branchpredictionTrace.actualstatus;

        ++lineCounter;

        istringstream iss(line);
        if (!(iss >> xaddr >> actualstatus)) {break;}
      stringstream saddr(xaddr);
      saddr >> std::hex >> addr;
      accessaddr = bitset<32> (addr);

      int TakenState = 1;
      int NotTakenState= 0;

      string str_addr = accessaddr.to_string();
      string counter_index = str_addr.substr(32-no, no);
      array<bool,2> tempArr = {saturatingcounter[bitset<32>(counter_index).to_ulong()][0],
                            saturatingcounter[bitset<32>(counter_index).to_ulong()][1]};
      int result = 0;
      //array<bool,2> tempArr1 = tempArr;

      switch(actualstatus){
        case 0:
            if(tempArr == strongly_taken){
                result = TakenState;
                tempArr = weakly_taken;
                cout<<"actual NT predict T satcounter WT ##" << endl;
                cout<<"---------------------------------------------"<<endl;
            } else if(tempArr == weakly_taken){
                result = TakenState;
                tempArr = strongly_not_taken;
                cout<<"actual NT predict NT satcounter SNT ##" << endl;
                cout<<"---------------------------------------------"<<endl;
            } else if((tempArr == weakly_not_taken) || (tempArr == strongly_not_taken)){
                result = NotTakenState;
                tempArr = strongly_not_taken;
                cout<<"actual NT predict NT satcounter SNT ##" << endl;
                cout<<"---------------------------------------------"<<endl;
            }
            break;

        case 1:
            if(tempArr == strongly_not_taken){
                result = NotTakenState;
                tempArr = weakly_not_taken;
                cout<<"actual T predict NT satcounter WNT ##" << endl;
                cout<<"---------------------------------------------"<<endl;
            } else if (tempArr == weakly_not_taken) {
                result = NotTakenState;
                tempArr = strongly_taken;
                cout<<"actual T predict T satcounter ST ##" << endl;
                cout<<"---------------------------------------------"<<endl;
            } else if ((tempArr == weakly_taken) || (tempArr == strongly_taken)) {
                result = TakenState;
                tempArr = strongly_taken;
                cout<<"actual T predict T satcounter ST ##" << endl;
                cout<<"---------------------------------------------"<<endl;
            }
            break;
        default:
            break;
      }

      saturatingcounter[bitset<32>(counter_index).to_ulong()][0] = tempArr[0];
      saturatingcounter[bitset<32>(counter_index).to_ulong()][1] = tempArr[1];

//      cout<<"hexadd:"<<branchpredictionTrace.hexadd<<" "<<"ActualStatus:"<<actualstatus<<endl;
//      cout<<"addr:"<<xaddr<<endl;
//      cout<<"accessaddr:"<<accessaddr<<endl;


     //int PredictTakenWithConfidence[2]= {1,1};
     //cout<<"bitset:"<<PredictTakenWithConfidence[0]<<endl;
     //cout<<"array 1st element at 4095:"<<saturatingcounter[0][0]<<endl;//" array 2nd element:"<<saturatingcounter[0][1]<<endl;
     //tracesout<<actualstatus<<" " <<tempArr1[0]<<tempArr1[1]<< ">>>>" <<saturatingcounter[bitset<32>(counter_index).to_ulong()][0]<<saturatingcounter[bitset<32>(counter_index).to_ulong()][1]<<" "<<counter_index<< " "<< bitset<32>(counter_index).to_ulong() << " " << result << endl;
     //tracesout<<actualstatus<< " " << result << endl;
     tracesout<< result << endl;

     if (actualstatus != result){
        ++misPredictCounter;
     }

    }
        cout<<"misprediction: "<<misPredictCounter<<" lineCounter:"<<lineCounter<<endl;
        double mispredictionRate = (double)misPredictCounter/lineCounter * 100;
        cout<<"mispredictionRate: "<<mispredictionRate<<endl;

        traces.close();
        tracesout.close();
    }

else cout<< "Unable to open trace or traceout file";

   return 0;
}
