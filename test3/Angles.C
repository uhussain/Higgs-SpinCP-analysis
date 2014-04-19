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
  // Spin_0m = new TTree("Spin_0m","Spin_0m Tree with variables for TMVA analysis");
   Spin_0m_B = new TTree("Spin_0m_B","Spin_0m_B Tree with variables for TMVA analysis");
 
   // Creating the branches
   // Replace "Spin_0m" with "Spin_0p" when writing the Spin_0p tree to TFile f
   Spin_0m_B->Branch("cth1",&cth1,"cth1/F");
   Spin_0m_B->Branch("cth2",&cth2,"cth2/F");
   Spin_0m_B->Branch("phi",&phi,"phi/F");
   Spin_0m_B->Branch("m12",&m12,"m12/F");   
   Spin_0m_B->Branch("m34",&m34,"m34/F");
    
    //..Histogram Definitions
   //htheta1 = new TH1F("htheta1", "",50,-1,1);
   //htheta2 = new TH1F("htheta2", "",50,-1,1);
   //hphi = new TH1F("hphi", "",50,-3.14,3.14);
   //hphi_1 = new TH1F("hphi_1", "",50,-3.14,3.14);
   //htheta = new TH1F("htheta", "",50,-1,1);
   //hmass = new TH1F ("hmass", "",50,-10,100);
   //htheta1->GetXaxis()->SetTitle("cos(#theta_{1})");
   //htheta1->SetLineColor(2);
   //htheta2->GetXaxis()->SetTitle("cos(#theta_{2})");
   //htheta2->SetLineColor(2);
   //hphi->GetXaxis()->SetTitle("#Phi");
   //hphi->SetLineColor(2);
   //hphi_1->GetXaxis()->SetTitle("#Phi_{1}");
   //hphi_1->SetLineColor(2);
   //htheta->GetXaxis()->SetTitle("cos(#theta*)");
   //htheta->SetLineColor(2);
   //hmass->GetXaxis()->SetTitle("Mass of Z-boson");
   //hmass->SetLineColor(2);
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
   
   //Calculating transverse energy of the leptons
   Float_t l1_Et = sqrt(pow(Particle_M[5],2.0)+pow(Particle_PT[5],2.0));
   Float_t l2_Et = sqrt(pow(Particle_M[6],2.0)+pow(Particle_PT[6],2.0));
   Float_t l3_Et = sqrt(pow(Particle_M[7],2.0)+pow(Particle_PT[7],2.0));
   Float_t l4_Et = sqrt(pow(Particle_M[8],2.0)+pow(Particle_PT[8],2.0));
   
   //Calculating dR for pairs of leptons where particle[5] is lepton#1 and so on until particle[8] is lepton#4
   Float_t dR_12 = sqrt(((Particle_Phi[5]-Particle_Phi[6])**2)+((Particle_Eta[5]-Particle_Eta[6])**2));
   Float_t dR_13 = sqrt(((Particle_Phi[5]-Particle_Phi[7])**2)+((Particle_Eta[5]-Particle_Eta[7])**2));
   Float_t dR_14 = sqrt(((Particle_Phi[5]-Particle_Phi[8])**2)+((Particle_Eta[5]-Particle_Eta[8])**2));
   Float_t dR_23 = sqrt(((Particle_Phi[6]-Particle_Phi[7])**2)+((Particle_Eta[6]-Particle_Eta[7])**2));
   Float_t dR_24 = sqrt(((Particle_Phi[6]-Particle_Phi[8])**2)+((Particle_Eta[6]-Particle_Eta[8])**2));
   Float_t dR_34 = sqrt(((Particle_Phi[7]-Particle_Phi[8])**2)+((Particle_Eta[7]-Particle_Eta[8])**2));
   
   //Define x,y to be used below
   int x;
   int y;
   //Defining local variables x and y depedning on the identity of each of the leptons
   if ((Particle_PID[5] = 11)||(Particle_PID[5] = -11)){x=0;}
   if ((Particle_PID[5] = 13)||(Particle_PID[5] = -13)){x=1;}//if l1 is Z1/Z2 particle/antiparticle then l2 is Z1/Z2 antiparticle/particle
   if ((Particle_PID[7] = 11)||(Particle_PID[7] = -11)){y=0;}
   if ((Particle_PID[7] = 13)||(Particle_PID[7] = -13)){y=1;}//if l3 is Z1/Z2 particle/antiparticle then l2 is Z1/Z2 antiparticle/particle
   
   //Declaring Variables to be used in methods below
   //Float_t cstheta;
   //Float_t cstheta_1;
   //Float_t cstheta_2;
   //Float_t Phi;
   //Float_t phi_1;
   //Float_t psi;
   Float_t s;
   Float_t m4l;
   Float_t m;
  // Float_t m1;
  // Float_t m2;  
   
   //4-Vectors of pair1(l1+l2) and pair2(l3+l4)
   TLorentzVector p_1 = (l1+l2);
   TLorentzVector p_2 = (l3+l4);
   Float_t mz  = 91.1876;
   //calculating the invariant mass of both pairs
   m12 = sqrt(((p_1.E())**2) - ((p_1.Px())**2)- ((p_1.Py())**2)- ((p_1.Pz())**2));
   m34 = sqrt(((p_2.E())**2) - ((p_2.Px())**2)- ((p_2.Py())**2)- ((p_2.Pz())**2));
   m4l = (m12 + m34) ;
   s =(m12-mz);
   //Define m which is used below://done in else statement below  m = 12 + 0.76*(m4l - 140)
   //'m' increases linearly from 12 GeV (for m4l<140 GeV) to 50 GeV (for m4l > 190 GeV).
   if (m4l < 140){m = 12;}
   else if (m4l > 190) {(m = 50);
   cout << "Linear m"<<endl;}
   //Write a linear equation for 'm' in this else statement.
   else {(m = 12 + 0.76*(m4l - 140));
   cout << "Linear m"<<endl ;
} 
   //Skip events which don't satisfy: m12 > 5 GeV & m34 > 5 GeV
   //And skip those which don't satisfy: dR > 0.1 for same flavor leptons. And lepton 1 & 2 are of the same flavor as are lepton 3 & 4
   if ((m12>5)&&(m34>5)&&(dR_12>0.1)&&(dR_34>0.1)){
   //Deciding if pair1 is closer to MZ = 91.6 or pair2
   if((m12-mz) < (m34-mz)){
   // 50 GeV < m12 < 106 GeV for leading di-lepton pair
   // m < m34 < 115 GeV where 'm' increases linearly 
   //from 12 GeV (for m4l<140 GeV) to 50 GeV (for m4l > 190 GeV).
   if ((50<m12<106)&&(m<m34<115)){
    GetHelicityAngles(l1,l2,l3,l4,&cthstr, &phi1,
                       &cth1, &cth2, &phi);}
   else{
}
   
   //TLorentzVector Z1 = (l1+l2);
   //TLorentzVector Z2 = (l3+l4);
}
   else{
   // m34 is the leading di-lepton pair here  since m34 is closer to mz
   if ((50<m34<106)&&(m<m12<115)){
    GetHelicityAngles(l3,l4,l1,l2,&cthstr, &phi1,
                       &cth1, &cth2, &phi);}
   else{
}

   // TLorentzVector Z1 = (l3+l4);
   // TLorentzVector Z2 = (l1+l2);

}}
   else{
}


      //Double_t scale; 
      //htheta->SetMinimum(0);
      //htheta1->SetMinimum(0);
      //htheta2->SetMinimum(0);
      //hphi->SetMinimum(0);
      //hphi_1->SetMinimum(0);
          
     //Filling the histograms with Kirill's calculated angles:
      //htheta->Fill(cthstr);
      //htheta1->Fill(cth1);
      //htheta2->Fill(cth2);
      //hphi->Fill(phi);
      //hphi_1->Fill(phi1);
     // cout <<"where is the error?"<<endl;
  
 //Cuts on lepton acceptance
    if (((Particle_Eta[5]>3) || (Particle_Eta[6]>3) || (Particle_Eta[7]>3) || (Particle_Eta[8]>3))||((Particle_PT[5]<=20)||(Particle_PT[6]<=15)||(Particle_PT[7]<=10))){
}   
    //Eta & Transverse energy cuts on leptons specifically electrons
    else if (((Particle_PID[5] = -11)||(Particle_PID[5] = 11))&&((Particle_Eta[5]>2.47)||(l1_Et<7))){}//condition for cuts on each electron
    else if (((Particle_PID[6] = -11)||(Particle_PID[6] = 11))&&((Particle_Eta[6]>2.47)||(l2_Et<7))){}
    else if (((Particle_PID[7] = -11)||(Particle_PID[7] = 11))&&((Particle_Eta[7]>2.47)||(l3_Et<7))){}
    else if (((Particle_PID[8] = -11)||(Particle_PID[8] = 11))&&((Particle_Eta[8]>2.47)||(l4_Et<7))){}

    //Eta & PT energy cuts on leptons specifically muons
    else if(((Particle_PID[5] = -13)||(Particle_PID[5] = 13))&&((Particle_Eta[5]>2.7)||(Particle_PT[5]<6))){} //condition for cuts on each muon
    else if(((Particle_PID[6] = -13)||(Particle_PID[6] = 13))&&((Particle_Eta[6]>2.7)||(Particle_PT[6]<6))){}
    else if(((Particle_PID[7] = -13)||(Particle_PID[7] = 13))&&((Particle_Eta[7]>2.7)||(Particle_PT[7]<6))){}
    else if(((Particle_PID[8] = -13)||(Particle_PID[8] = 13))&&((Particle_Eta[8]>2.7)||(Particle_PT[8]<6))){}
    
    //dR checks for same flavor and different flavor leptons
    else if (((x==0)&&(y==0))&&((dR_13<0.1)||(dR_23<0.1)||(dR_14<0.1)||(dR_24<0.1))){}//this is the 4e case so all leptons of same flavor
    else if (((x==0)&&(y==1))&&((dR_13<0.2)||(dR_23<0.2)||(dR_14<0.2)||(dR_24<0.2))){}//this is the 2e2mu case
    else if (((x==1)&&(y==0))&&((dR_13<0.2)||(dR_23<0.2)||(dR_14<0.2)||(dR_24<0.2))){}//this is the 2mu2e case
    else if (((x==1)&&(y==1))&&((dR_13<0.1)||(dR_23<0.1)||(dR_14<0.1)||(dR_24<0.1))){}//this is the 4mu case  	
    
    else{
       if (cth1>=0){	
       Spin_0m_B->Fill();}
       else{}
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
   Spin_0m_B->Write();
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}
