#include "TFile.h"
#include "Event.h"
void copytree3() {
   
  // gSystem->Load("/export/home/uh234/home/HiggsResearch/test3");	
// Only selected entries are copied to the new Tree
   //Get old file, old tree and set top branch address
   TFile *oldfile = new TFile("Higgs_Spin0p_Spin0m_8TeV.root");
   TTree *oldtree = (TTree*)oldfile->Get("Spin_0m_A;1");
   Long64_t nentries = oldtree->GetEntries();
   
   //Event *cth1 = new Event(0); //object must be created before
                             //setting the branch address
  // Float_t *cth1 = 0;
  // TBranch *branch = oldtree->GetBranch("cth1");
  // oldtree->SetBranchAddress("cth1",&cth1);
   //Create a new file + a clone of old tree in new file
   TFile *newfile = new TFile("safe.root","update");
   TTree *Spin_0m_A = oldtree->CloneTree(0);

   for (Long64_t i=2600;i<nentries; i++) {
      oldtree->GetEntry(i);
      Spin_0m_A->Fill();
     // if (cth1.GetNtrack() > 4203) newtree->Fill();
     // cth1->Clear();
   }
   Spin_0m_A->Print();
   Spin_0m_A->AutoSave();
  // delete oldfile;
  // delete newfile;
}
