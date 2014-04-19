nclude <string>
#include "TFile.h"
void drawAB_train(){

        TFile f("TMVA_0p_0m_A.root");
        f.cd("Method_BDT/BDT");
        MVA_BDT_Train_B->SetLineColor(2);
        MVA_BDT_Train_S->SetLineColor(1);
        MVA_BDT_Train_S->SetName("g1");
        MVA_BDT_Train_B->SetName("g2");
        MVA_BDT_Train_S->Draw("SAME");
        MVA_BDT_Train_B->Draw("SAME");
        TFile g("TMVA_0p_0m_B.root");
        g.cd("Method_BDT/BDT");
        MVA_BDT_Train_B->SetLineColor(8);
        MVA_BDT_Train_S->SetLineColor(4);
        MVA_BDT_Train_S->SetName("g3");
        MVA_BDT_Train_B->SetName("g4");
        MVA_BDT_Train_S->Draw();
        MVA_BDT_Train_B->Draw("SAME");
        leg_hist= new TLegend(0.5,0.5,0.75,0.75);
        leg_hist->SetHeader("Key");
        leg_hist->AddEntry(g1,"SigA","l");
        leg_hist->AddEntry(g2,"BkgA","l");
        leg_hist->AddEntry(g3,"SigB","l");
        leg_hist->AddEntry(g4,"BkgB","l");
        leg_hist->Draw();
}

