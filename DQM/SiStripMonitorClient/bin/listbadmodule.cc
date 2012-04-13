#include <iostream>
#include <stdio.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TKey.h>
#include <TH1.h>
#include <Riostream.h>
#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <stdint.h>
#include <cstdlib>
#include <cstdio>

#include "listbadmodule.h"

//using namespace std;

int main(int argc , char *argv[]) {

  if(argc==2) {
    char* filename = argv[1];

    std::cout << "ready to prepare list of bad modules " << filename << std::endl;

    listbadmodule(filename);

  }
  else {std::cout << "Too few arguments: " << argc << std::endl; return -1; }
  return 0;

}


void listbadmodule(std::string filename) {

  int debug = 1;

  cout << " here " << endl;
  std::vector<std::string> subdet;
  subdet.push_back("TIB");
  subdet.push_back("TID/side_1"); 
  subdet.push_back("TID/side_2");
  subdet.push_back("TOB");
  subdet.push_back("TEC/side_1");
  subdet.push_back("TEC/side_2");

  std::string nrun = filename.substr(filename.find("_R000")+5, 6);
  int fileNum = atoi(nrun.c_str()); 
  cout << " ------   Run " << fileNum << endl;
  
  ofstream outfile;
  std::string namefile;
  namefile = "QualityTest_run" + nrun + ".txt";
  outfile.open(namefile.c_str());
 
  TFile *myfile = TFile::Open(filename.c_str());
  if (debug == 1){	  
    std::cout <<" Opened "<< filename << std::endl; 
  }
  std::string topdir = "DQMData/Run " + nrun + "/SiStrip/Run summary/MechanicalView";
  gDirectory->cd(topdir.c_str());
  TDirectory* mec1 = gDirectory;
  
  for (unsigned int i=0; i < subdet.size(); i++){
    std::string badmodule_dir = subdet[i] + "/BadModuleList";
    outfile << " " << endl;
    outfile << "SubDetector " << subdet[i] << endl;
    outfile << " " << endl;
    cout << badmodule_dir.c_str() << endl;
    if (gDirectory->cd(badmodule_dir.c_str())){
    //
    // Loop to find bad module for each partition
    //
      TIter next(gDirectory->GetListOfKeys());
      TKey *key;
      
      while  ( (key = dynamic_cast<TKey*>(next())) ) {
	std::string sflag = key->GetName();
	if (sflag.size() == 0) continue;
	std::string detid = sflag.substr(sflag.find("<")+1,9); 
	size_t pos1 = sflag.find("/");
	sflag = sflag.substr(sflag.find("<")+13,pos1-2);
	int flag = atoi(sflag.c_str());	
	std::string message;
	message = "Module " + detid;
	if (((flag >> 0) & 0x1) > 0) message += " Fed BadChannel : ";
	if (((flag >> 1) & 0x1) > 0) message += " # of Digi : ";  
	if (((flag >> 2) & 0x1) > 0) message += " # of Clusters :";
	if (((flag >> 3) & 0x1) > 0) message += " Excluded FED Channel ";
	if (((flag >> 4) & 0x1) > 0) message += " DCSError "; 
	outfile << message.c_str() << std::endl;
	
      }
    }
    mec1->cd();
  }
  myfile->Close();
  outfile.close();
}


