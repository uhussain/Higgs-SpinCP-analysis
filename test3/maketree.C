#include "TFile.h"
#include "Event.h"
void maketree()
{
   TFile hfile("Event.root","RECREATE");
 
 
   // Create a ROOT Tree
 
   TTree *tree = new TTree("T","An example of a ROOT tree");
 
   Int_t split = 0;
   Int_t bsize = 64000;
   Event  *event = 0;
 
    // Create one branch. If splitlevel is set, event is a superbranch
    // creating a sub branch for each data member of the Event object.
 
   tree->Branch("event", "Event", &event, bsize,split);
 
   // Fill event, header and tracks with some random numbers
   char keyname[16];
   for (Int_t ev = 0; ev < 1000; ev++) {
 
      Float_t sigmat, sigmas;
      gRandom->Rannor(sigmat,sigmas);
      Int_t ntrack   = Int_t(600 +5*sigmat);
      Float_t random = gRandom->Rndm(1);
 
      event = new Event();    // Create one event
 
      event->SetHeader(ev, 200, 960312, random);
      event->SetNseg(Int_t(10*ntrack+20*sigmas));
      event->SetNvertex(1);
      event->SetFlag(UInt_t(random+0.5));
      event->SetTemperature(random+20.);
 
      //  Create and Fill the Track objects
      for (Int_t t = 0; t < ntrack; t++) event->AddTrack(random);
 
      //  Serialize single branch of event into one single basket
      tree->Fill();
 
      delete event;
   }
   hfile.Close();
   return 0;
}
