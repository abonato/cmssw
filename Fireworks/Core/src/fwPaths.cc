#include "Fireworks/Core/interface/fwPaths.h"
#include "Fireworks/Core/interface/fwLog.h" 	 
#include "TString.h"
#include "TSystem.h"
#include "TPRegexp.h"
#include <iostream>
#include <fstream>

namespace fireworks  {

const TString datadir("/src/Fireworks/Core/") ; 	 
const TString p1 = gSystem->Getenv("CMSSW_BASE") + datadir; 	 
const TString p2 = gSystem->Getenv("CMSSW_RELEASE_BASE") + datadir;

void setPath( TString& v)
{
   if (gSystem->AccessPathName(p1 + v) == kFALSE)
   {
      v.Prepend(p1);
      return;
   }

   v.Prepend(p2);
   if (gSystem->AccessPathName(v))
      fwLog(fwlog::kError) << "Can't access path " << v << std::endl;
}



void getDecomposedVersion(const TString& s, int* out)
{
   TPMERegexp re("CMSSW_(\\d+)_(\\d+)_(\\d+)");
   re.Match(s);
   if (re.NMatches() > 3)
   {
      out[0] = re[1].Atoi();
      out[1] = re[2].Atoi();
      out[2] = re[3].Atoi();
   }
}



int* supportedDataFormatsVersion()
{
   static int mm[] = {0, 0, 0};

   if (!mm[0]) {
      TString v;
      if (gSystem->Getenv("CMSSW_VERSION"))
      {
         v = gSystem->Getenv("CMSSW_VERSION");

      }
      else
      {
         TString versionFileName = gSystem->Getenv("CMSSW_BASE");
         versionFileName += "/src/Fireworks/Core/data/version.txt";
         ifstream fs(versionFileName);
         TString infoText;
         infoText.ReadLine(fs); infoText.ReadLine(fs);
         fs.close();
         v = &infoText[13];
      }

      getDecomposedVersion(v, &mm[0]);
   }

   return &mm[0];
}

bool acceptDataFormatsVersion(TString& n)
{
   int v[] = {0, 0, 0};
   getDecomposedVersion(n, v);
   return v[0] == supportedDataFormatsVersion()[0];
}

}