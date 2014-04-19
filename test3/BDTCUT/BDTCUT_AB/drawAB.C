#include <string>
#include "TFile.h"
void drawAB(){

        TFile f("TMVA_0p_0m_A.root");
        f.cd("Method_BDT/BDT");
        MVA_BDT_B->SetLineColor(2);
        MVA_BDT_S->SetLineColor(1);
        MVA_BDT_S->SetName("g1");
        MVA_BDT_B->SetName("g2");
        MVA_BDT_B->Draw("SAME");
        MVA_BDT_S->Draw("SAME");
        TFile g("TMVA_0p_0m_B.root");
        g.cd("Method_BDT/BDT");
        MVA_BDT_B->SetLineColor(8);
        MVA_BDT_S->SetLineColor(4);
        MVA_BDT_S->SetName("g3");
        MVA_BDT_B->SetName("g4");
        MVA_BDT_B->Draw();
        MVA_BDT_S->Draw("SAME");
        leg_hist= new TLegend(0.5,0.5,0.75,0.75);
        leg_hist->SetHeader("Key");
        leg_hist->AddEntry(g1,"Sig_unweighted","l");
        leg_hist->AddEntry(g2,"Bkg_unweighted","l");
        leg_hist->AddEntry(g3,"Sig_weighted","l");
        leg_hist->AddEntry(g4,"Bkg_weighted","l");
        leg_hist->Draw();
}
