#include <string>
#include "TFile.h"
void removeTree(){

  std::string file_name="Higgs_Spin0p_Spin0m_8TeV_weighted.root";
  TFile *file=new TFile((file_name).c_str(),"update");
  TTree *oldtree = (TTree*)file->Get("Spin_0p_A;1");
 // oldtree->Write("Spin_0m_A");//important line to rename trees
 // TTree *newtree=new TTree("Spin_0p_A");
 // newtree->AddFriend(oldtree);
 // file->Write();
  std::string object_to_remove="Spin_0p_A;1";
  //std::string object_to_remove_1="Spin_0m_A;1";
  //the object can be a tree, a histogram, etc, in this case "test1" is a TTree
  //notice the ";1" which means cycle 1; to remove all cycles do ";*"
  //if your object is not at the top directory, but in a directory in the .root file, called foo
  // you do first
  //file->cd("foo");
  //then continue with the Delete command which is only applied to the current gDirectory
  gDirectory->Delete(object_to_remove.c_str());
 // gDirectory->Delete(object_to_remove_1.c_str());
  file->Close();
}
