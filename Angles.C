#define Angles_cxx
#include "Angles.h"
#include "GetHelicityAngles.h"
#include <TH2.h>
#include <TStyle.h>
#include <stdio.h>
#include "TMath.h"
#include "TMathBase.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <iomanip>

//Variables for Kirill's code
Float_t cthstr;
Float_t phi1;
Float_t cth1;
Float_t cth2;
Float_t phi;
Float_t m12;
Float_t m34;
   
void Angles::Begin(TTree * /*tree*/)
{
   TString option = GetOption();	
    //..Output file for the angles calculated
   f = new TFile("Higgs_Spin0p_Spin0m_8TeV.root","UPDATE");
   //Create the tree
   Spin_0m = new TTree("Spin_0m","Spin_0m Tree with variables for TMVA analysis");
   //Spin_0p = new TTree("Spin_0p","Spin_0p Tree with variables for TMVA analysis");
 
   // Creating the branches
   // Replace "Spin_0m" with "Spin_0p" when writing the Spin_0p tree to TFile f
   Spin_0m->Branch("cth1",&cth1,"cth1/F");
   Spin_0m->Branch("cth2",&cth2,"cth2/F");
   Spin_0m->Branch("phi",&phi,"phi/F");
   Spin_0m->Branch("m12",&m12,"m12/F");   
   Spin_0m->Branch("m34",&m34,"m34/F");
    
    //..Histogram Definitions
   htheta1 = new TH1F("htheta1", "",50,-1,1);
   htheta2 = new TH1F("htheta2", "",50,-1,1);
   hphi = new TH1F("hphi", "",50,-3.14,3.14);
   hphi_1 = new TH1F("hphi_1", "",50,-3.14,3.14);
   htheta = new TH1F("htheta", "",50,-1,1);
   hmass = new TH1F ("hmass", "",50,-10,100);
   htheta1->GetXaxis()->SetTitle("cos(#theta_{1})");
   htheta1->SetLineColor(2);
   htheta2->GetXaxis()->SetTitle("cos(#theta_{2})");
   htheta2->SetLineColor(2);
   hphi->GetXaxis()->SetTitle("#Phi");
   hphi->SetLineColor(2);
   hphi_1->GetXaxis()->SetTitle("#Phi_{1}");
   hphi_1->SetLineColor(2);
   htheta->GetXaxis()->SetTitle("cos(#theta*)");
   htheta->SetLineColor(2);
   hmass->GetXaxis()->SetTitle("Mass of Z-boson");
   hmass->SetLineColor(2);
}
//Kirill's method to calculate the angles
void Angles::GetHelicityAngles(const TLorentzVector& v1, const TLorentzVector& v2,
                       const TLorentzVector& v3, const TLorentzVector& v4,
                       Float_t* cthstr, Float_t* phi1,
                       Float_t* cth1, Float_t* cth2, Float_t* phi){
   // RFR means reference frame :)
   // definitions from http://arxiv.org/pdf/1208.4018.pdf page 3 [VI]
   TLorentzVector Z1 = ( v1 + v2 );
   TLorentzVector Z2 = ( v3 + v4 );
   const TLorentzVector H = Z1 + Z2;

   TLorentzVector q;
   q.SetPxPyPzE( 0, 0, ( H.E() + H.Pz() ) / 2., ( H.E() + H.Pz() ) / 2. );
   q.Boost( -( H.BoostVector() ) );

   const TVector3 parton = q.Vect().Unit();

   Z1.Boost( -( H.BoostVector() ) ); // go to Higgs RFR
   Z2.Boost( -( H.BoostVector() ) );

   const TVector3 z1 = Z1.Vect().Unit();
   const TVector3 z2 = Z2.Vect().Unit();

   //Costh*
   *cthstr = TMath::Cos( parton.Angle( z1 ) );

   // Boost the leptons into the Higgs rest frame:
   TLorentzVector vv1( v1 ), vv2( v2 ), vv3( v3 ), vv4( v4 );
   vv1.Boost( -( H.BoostVector() ) ); // go to Higgs RFR
   vv2.Boost( -( H.BoostVector() ) );
   vv3.Boost( -( H.BoostVector() ) );
   vv4.Boost( -( H.BoostVector() ) );
   
   const TVector3 v1p = vv1.Vect();
   const TVector3 v2p = vv2.Vect();
   const TVector3 v3p = vv3.Vect();
   const TVector3 v4p = vv4.Vect();
   const TVector3 nz( 0, 0, 1. );

   //Phi, Phi1
   const TVector3 n1p = v1p.Cross( v2p ).Unit();
   const TVector3 n2p = v3p.Cross( v4p ).Unit();
   const TVector3 nscp = nz.Cross( z1 ).Unit();
   *phi = ( z1.Dot( n1p.Cross( n2p ) ) /
            TMath::Abs( z1.Dot( n1p.Cross( n2p ) ) ) *
            TMath::ACos( -n1p.Dot( n2p ) ) );
   *phi1 = ( z1.Dot( n1p.Cross( nscp ) ) /
             TMath::Abs( z1.Dot( n1p.Cross( nscp ) ) ) *
             TMath::ACos( n1p.Dot( nscp ) ) );

   //Costh1,2
   TLorentzVector Z2_rfr_Z1 = Z2; // it's still in H RFR
   Z2_rfr_Z1.Boost( -( Z1.BoostVector() ) ); // now it's in Z1 RFR (both Z1 and Z2 are in H RFR)
   const TVector3 z2_rfr_Z1 = Z2_rfr_Z1.Vect();

   TLorentzVector Z1_rfr_Z2 = Z1; // it's still in H RFR
   Z1_rfr_Z2.Boost( -( Z2.BoostVector() ) ); // now it's in Z2 RFR (both Z1 and Z2 are still in H RFR)
   const TVector3 z1_rfr_Z2 = Z1_rfr_Z2.Vect();

   vv1.Boost( -( Z1.BoostVector() ) ); // Z1 and Z2 still in H RFR: put leptons in their Z's reference frame
   vv3.Boost( -( Z2.BoostVector() ) );

   *cth1 = - ( z2_rfr_Z1.Dot( vv1.Vect() ) /
               TMath::Abs( z2_rfr_Z1.Mag() * vv1.Vect().Mag() ) );
   *cth2 = - ( z1_rfr_Z2.Dot( vv3.Vect() ) /
               TMath::Abs( z1_rfr_Z2.Mag() * vv3.Vect().Mag() ) );

   return;	
   
}

void Angles::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t Angles::Process(Long64_t entry)
{
    
   //..Just read the full event
   fChain->GetTree()->GetEntry(entry);

   //..Defining the 4-momentum vectors for the 4 leptons
   TLorentzVector l1(Particle_Px[5],Particle_Py[5],Particle_Pz[5],Particle_E[5]);
   TLorentzVector l2(Particle_Px[6],Particle_Py[6],Particle_Pz[6],Particle_E[6]);
   TLorentzVector l3(Particle_Px[7],Particle_Py[7],Particle_Pz[7],Particle_E[7]);
   TLorentzVector l4(Particle_Px[8],Particle_Py[8],Particle_Pz[8],Particle_E[8]);
  
   //Declaring Variables to be used in methods below
   //Float_t cstheta;
   //Float_t cstheta_1;
   //Float_t cstheta_2;
   //Float_t Phi;
   //Float_t phi_1;
   //Float_t psi;
   Float_t s;
  // Float_t m1;
  // Float_t m2;  
   
   //4-Vectors of pair1(l1+l2) and pair2(l3+l4)
   TLorentzVector p_1 = (l1+l2);
   TLorentzVector p_2 = (l3+l4);
   Float_t mz  = 91.1876;
   //calculating the invariant mass of both pairs
   m12 = sqrt(((p_1.E())**2) - ((p_1.Px())**2)- ((p_1.Py())**2)- ((p_1.Pz())**2));
   m34 = sqrt(((p_2.E())**2) - ((p_2.Px())**2)- ((p_2.Py())**2)- ((p_2.Pz())**2));
   s =(m12-mz);
   //Deciding if pair1 is closer to MZ = 91.6 or pair2
    if((m12-mz) < (m34-mz)){
    GetHelicityAngles(l1,l2,l3,l4,&cthstr, &phi1,
                       &cth1, &cth2, &phi);
   
   //TLorentzVector Z1 = (l1+l2);
   //TLorentzVector Z2 = (l3+l4);
}
    else{
    GetHelicityAngles(l3,l4,l1,l2,&cthstr, &phi1,
                       &cth1, &cth2, &phi);

   // TLorentzVector Z1 = (l3+l4);
   // TLorentzVector Z2 = (l1+l2);

} 


      Double_t scale; 
      htheta->SetMinimum(0);
      htheta1->SetMinimum(0);
      htheta2->SetMinimum(0);
      hphi->SetMinimum(0);
      hphi_1->SetMinimum(0);
          
     //Filling the histograms with Kirill's calculated angles:
      //htheta->Fill(cthstr);
      //htheta1->Fill(cth1);
      //htheta2->Fill(cth2);
      //hphi->Fill(phi);
      //hphi_1->Fill(phi1);
     // cout <<"where is the error?"<<endl;
  
 //Cuts on lepton acceptance
    if ((Particle_Eta[5]>3) || (Particle_Eta[6]>3) || (Particle_Eta[7]>3) || (Particle_Eta[8]>3)){

}	
    else{
       Spin_0m->Fill();
}
   //Filling the histograms with my calculations;
  // htheta->Fill(cstheta);
  // htheta1->Fill(cstheta_1);
  // htheta2->Fill(cstheta_2);
  // hphi->Fill(Phi);
  // hphi_1->Fill(phi_1);
  // hmass->Fill(s);
   return kTRUE;
}

void Angles::SlaveTerminate()
{
// cout <<"where is the error?"<<endl;
}

void Angles::Terminate()
{
  
   cout <<"Output file written"<<endl;
   Spin_0m->Write();
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}
