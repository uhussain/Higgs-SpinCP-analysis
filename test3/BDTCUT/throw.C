#include "TFile.h"
#include "Event.h"
void throw() {      
// Only selected entries are copied to the new Tree
   //Get old file, old tree and set top branch address
   TFile *oldfile = new TFile("Higgs_safe.root");
   TTree *oldtree = (TTree*)oldfile->Get("Spin_0m_A;1");
   Long64_t nentries = oldtree->GetEntries();

   //Event *cth1 = new Event(0); //object must be created before
                             //setting the branch address
  // Float_t *cth1 = 0;
  // TBranch *branch = oldtree->GetBranch("id");
   //oldtree->SetBranchAddress("id",&id);
   //Create a new file + a clone of old tree in new file
   TFile *newfile = new TFile("HiggsAB_weighted.root","update");
   TTree *Spin_0m_A = oldtree->CloneTree(0);

   for (Long64_t i=3000;i<nentries; i++) {
      oldtree->GetEntry(i);
      Spin_0m_A->Fill();
}

   Spin_0m_A->Print();
   Spin_0m_A->AutoSave();
}
