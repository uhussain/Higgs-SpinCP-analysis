#include "TFile.h"
#include "Event.h"
void cuttree() {
   Float_t BDT;
  // gSystem->Load("/export/home/uh234/home/HiggsResearch/test3");      
// Only selected entries are copied to the new Tree
   //Get old file, old tree and set top branch address
   TFile *oldfile = new TFile("Higgs_1.root");
   TTree *oldtree = (TTree*)oldfile->Get("Spin_0p;1");
   Long64_t nentries = oldtree->GetEntries();
   
   //Event *cth1 = new Event(0); //object must be created before
                             //setting the branch address
  // Float_t *cth1 = 0;
  // TBranch *branch = oldtree->GetBranch("id");
   oldtree->SetBranchAddress("BDT",&BDT);
   //Create a new file + a clone of old tree in new file
   TFile *newfile = new TFile("Higgs_safe.root","update");
   //TTree *Spin_0p_A = oldtree->CloneTree(0);
   TTree *Spin_0p_B = oldtree->CloneTree(0);

   for (Long64_t i=0;i<nentries; i++) {
      oldtree->GetEntry(i);
      if (BDT<=-0.08){Spin_0p_B->Fill();}
      else{}
     // if (cth1.GetNtrack() > 4203) newtree->Fill();
     // cth1->Clear();
   }
   Spin_0p_B->Print();
   Spin_0p_B->AutoSave();
  // delete oldfile;
  // delete newfile;
}

