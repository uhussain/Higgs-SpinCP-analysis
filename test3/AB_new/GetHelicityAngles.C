// $Id: GetElicityAngles.cxx 516537 2012-09-06 16:04:14Z krasznaa $

// ROOT include(s):
#include <TLorentzVector.h>
#include <TMath.h>

// Local include(s):
#include "GetHelicityAngles.h"
#include "Angles.h"
void GetHelicityAngles( const TLorentzVector& v1, const TLorentzVector& v2,
                       const TLorentzVector& v3, const TLorentzVector& v4,
                       Float_t* cthstr, Float_t* phi1,
                       Float_t* cth1, Float_t* cth2, Float_t* phi ) {

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
