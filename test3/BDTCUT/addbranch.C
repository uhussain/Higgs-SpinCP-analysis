#include "TFile.h"
#include "Event.h"
void addbranch() {

   //Get old file, old tree and set top branch address
   TFile *oldfile = new TFile("Higgs_Spin0p_Spin0m_8TeV_weighted.root");
   TTree *oldtree = (TTree*)oldfile->Get("Spin_0m;1");
   Long64_t nentries = oldtree->GetEntries();

   //Event *cth1 = new Event(0); //object must be created before
                             //setting the branch address
  // Float_t *cth1 = 0;
  // TBranch *branch = oldtree->GetBranch("cth1");
  // oldtree->SetBranchAddress("cth1",&cth1);
   Float_t id;
   oldtree->SetBranchAddress("id", &id);
   //Create a new file + a clone of old tree in new file
   TFile *newfile = new TFile("Higgs_final.root","update");
   TTree *Spin_0m = oldtree->CloneTree(0);
   //add the branch to the new tree and try to fill it
   Spin_0m.Branch("id", &id,"Id for sig_bkg/F" );  
   for (Long64_t i=0;i<nentries; i++) {
      oldtree->GetEntry(i);
      id = 0;
      Spin_0m->Fill();
     // if (cth1.GetNtrack() > 4203) newtree->Fill();
     // cth1->Clear();
   }
   Spin_0m->Print();
   Spin_0m->AutoSave();
}
