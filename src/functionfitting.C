// functionfitting.C: investigating functional form of the toy number loss distributions from trackloss.C
// Created by Isaac Mooney on 2/9/18

#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TStyle.h"
#include <iostream>
#include "TRandom3.h"
#include "TH1.h"
#include "TH2.h"

#include <vector>

using namespace std;

void functionfitting () {
//~~~~~~~~~~~~~~~~~~~~~~~~~~~setting up histograms and functions~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  TH1::SetDefaultSumw2();
  TH2::SetDefaultSumw2();
  TH3::SetDefaultSumw2();
  
  TFile *in = new TFile("../out/toyclosure.root", "READ");

  gStyle->SetOptTitle(kFALSE);
  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);

  TH1 *num_loss1 = (TH1D*)in->Get("num_loss1");
  TH1 *num_loss2 = (TH1D*)in->Get("num_loss2");
  TH1 *num_loss3 = (TH1D*)in->Get("num_loss3");
  TH1 *num_loss4 = (TH1D*)in->Get("num_loss4");
  TH1 *num_loss5 = (TH1D*)in->Get("num_loss5");

  TF1 *p = new TF1("p","ROOT::Math::poisson_pdf(x,[0])", 0, 6.5);
  TF1 *b = new TF1("b","ROOT::Math::binomial_pdf(x,[0],[1])", 0, 6.5);
  TF1 *nb = new TF1("nb","ROOT::Math::negative_binomial_pdf(x,[0],[1])", 0, 6.5);
  p->SetParameter(0, 1); b->SetParameter(0, 0.2); b->SetParameter(1, 5); nb->SetParameter(0, 0.8); nb->SetParameter(1, 1);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~drawing, fitting, and cosmetics~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

  num_loss1->Scale(1/num_loss1->Integral());
  num_loss2->Scale(1/num_loss2->Integral());
  num_loss3->Scale(1/num_loss3->Integral());
  num_loss4->Scale(1/num_loss4->Integral());
  num_loss5->Scale(1/num_loss5->Integral());
  
  num_loss1->GetYaxis()->SetRangeUser(0,0.45);
  num_loss2->GetYaxis()->SetRangeUser(0,0.45);
  num_loss3->GetYaxis()->SetRangeUser(0,0.45);
  num_loss4->GetYaxis()->SetRangeUser(0,0.45);
  num_loss5->GetYaxis()->SetRangeUser(0,0.45);
  
  num_loss1->GetXaxis()->SetTitle("tracks lost");
  num_loss2->GetXaxis()->SetTitle("tracks lost");
  num_loss3->GetXaxis()->SetTitle("tracks lost");
  num_loss4->GetXaxis()->SetTitle("tracks lost");
  num_loss5->GetXaxis()->SetTitle("tracks lost");

  //colors: kCherry/*1176*/, 987, 1050, 1113, 924, respectively (if you want to replace the GetColor()). 
  num_loss1->SetMarkerStyle(kFullCircle); num_loss1->SetLineColor(kCherry/*TColor::GetColor("#f9f611")*/); num_loss1->SetMarkerColor(kCherry/*TColor::GetColor("#f9f611")*/);
  num_loss2->SetMarkerStyle(kFullSquare); num_loss2->SetLineColor(TColor::GetColor("#1380d5")); num_loss2->SetMarkerColor(TColor::GetColor("#1380d5"));
  num_loss3->SetMarkerStyle(kFullTriangleUp); num_loss3->SetLineColor(TColor::GetColor("#2cb6a5")); num_loss3->SetMarkerColor(TColor::GetColor("#2cb6a5"));
  num_loss4->SetMarkerStyle(kFullTriangleDown); num_loss4->SetLineColor(TColor::GetColor("#ccbb5b")); num_loss4->SetMarkerColor(TColor::GetColor("#ccbb5b"));
  num_loss5->SetMarkerStyle(kOpenCircle); num_loss5->SetLineColor(TColor::GetColor("#352a86")); num_loss5->SetMarkerColor(TColor::GetColor("#352a86"));

  p->SetLineColor(kBlue); b->SetLineColor(kRed); nb->SetLineColor(kGreen);
  p->SetLineStyle(1);/*solid*/ b->SetLineStyle(4);/*dotted*/ nb->SetLineStyle(9);/*dashed*/

  TCanvas *m1 = new TCanvas("m1", "m1", 800, 800); m1->cd();

  num_loss1->DrawCopy(); //gPad->BuildLegend(); gPad->Update();

  TLegend *n1 = new TLegend(0.5,0.67,0.88,0.88,NULL,"brNDC");
  n1->AddEntry(num_loss1,"number loss from delta + flat","lp");

  n1->Draw("same");

  TCanvas *m2 = new TCanvas("m2", "m2", 800, 800); m2->cd();

  num_loss2->DrawCopy();

  TLegend *n2 = new TLegend(0.5,0.67,0.88,0.88,NULL,"brNDC");
  n2->AddEntry(num_loss2,"number loss from delta + real","lp");

  n2->Draw("same");

  TCanvas *m3 = new TCanvas("m3", "m3", 800, 800); m3->cd();

  num_loss3->DrawCopy();

  TLegend *n3 = new TLegend(0.5,0.67,0.88,0.88,NULL,"brNDC");
  n3->AddEntry(num_loss3,"number loss from poisson + flat","lp");

  n3->Draw("same");

  TCanvas *m4 = new TCanvas("m4", "m4", 800, 800); m4->cd();

  num_loss4->DrawCopy();

  TLegend *n4 = new TLegend(0.5,0.67,0.88,0.88,NULL,"brNDC");
  n4->AddEntry(num_loss4,"number loss from poisson + real","lp");

  n4->Draw("same");

  TCanvas *m5 = new TCanvas("m5", "m5", 800, 800); m5->cd();

  num_loss5->DrawCopy();

  TLegend *n5 = new TLegend(0.5,0.67,0.88,0.88,NULL,"brNDC");
  n5->AddEntry(num_loss5,"number loss from real multiplicity + real efficiency","lp");

  n5->Draw("same");

  TCanvas *c0 = new TCanvas("c0", "c0", 800, 800); c0->cd();

  num_loss1->DrawCopy();//"PLC PMC");
  num_loss2->DrawCopy("same");
  num_loss3->DrawCopy("same");
  num_loss4->DrawCopy("same");
  num_loss5->DrawCopy("same");

  //note to self: look into EColorPalette
  
  TLegend *all = new TLegend(0.5,0.67,0.88,0.88,NULL,"brNDC");
  all->AddEntry(num_loss1,"number loss from delta + flat","lp");
  all->AddEntry(num_loss2,"number loss from delta + real","lp");
  all->AddEntry(num_loss3,"number loss from poisson + flat","lp");
  all->AddEntry(num_loss4,"number loss from poisson + real","lp");
  all->AddEntry(num_loss5,"number loss from real multiplicity + real efficiency","lp");

  all->Draw("same");

  TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);

  cout << "\nFITTING delta+flat w/ poisson, binomial, and negative binomial, respectively!:\n\n";

  num_loss1->Fit(p, "ME+"); num_loss1->Fit(b, "ME+"); num_loss1->Fit(nb, "ME+");
  
  num_loss1->DrawCopy();

  TLegend *n1_fits = new TLegend(0.5,0.67,0.88,0.88,NULL,"brNDC");
  n1_fits->AddEntry(num_loss1,"number loss from delta + flat","lp");
  n1_fits->AddEntry(p,"poisson","lp");
  n1_fits->AddEntry(b,"binomial","lp");
  n1_fits->AddEntry(nb,"negative binomial","lp");

  n1_fits->Draw("same");  

   TCanvas *c2 = new TCanvas("c2", "c2", 800, 800); c2->cd();

   cout << "\nFITTING delta+real w/ poisson, binomial, and negative binomial, respectively!:\n\n";

   num_loss2->Fit(p,"ME+"); num_loss2->Fit(b,"ME+"); num_loss2->Fit(nb,"ME+"); 

   num_loss2->DrawCopy();

   TLegend *n2_fits = new TLegend(0.5,0.67,0.88,0.88,NULL,"brNDC");
   n2_fits->AddEntry(num_loss2,"number loss from delta + real","lp");
   n2_fits->AddEntry(p,"poisson","lp");
   n2_fits->AddEntry(b,"binomial","lp");
   n2_fits->AddEntry(nb,"negative binomial","lp");

   n2_fits->Draw("same");

   TCanvas *c3 = new TCanvas("c3", "c3", 800, 800); c3->cd();

   cout << "\nFITTING poisson+flat w/ poisson, binomial, and negative binomial, respectively!:\n\n";

   num_loss3->Fit(p, "ME+"); num_loss3->Fit(b, "ME+"); num_loss3->Fit(nb, "ME+"); 

   num_loss3->DrawCopy();

   TLegend *n3_fits = new TLegend(0.5,0.67,0.88,0.88,NULL,"brNDC");
   n3_fits->AddEntry(num_loss3,"number loss from poisson + flat","lp");
   n3_fits->AddEntry(p,"poisson","lp");
   n3_fits->AddEntry(b,"binomial","lp");
   n3_fits->AddEntry(nb,"negative binomial","lp");

   n3_fits->Draw("same");

   TCanvas *c4 = new TCanvas("c4", "c4", 800, 800); c4->cd();

   cout << "\nFITTING poisson+real w/ poisson, binomial, and negative binomial, respectively!:\n\n";

   num_loss4->Fit(p,"ME+"); num_loss4->Fit(b,"ME+"); num_loss4->Fit(nb,"ME+"); 

   num_loss4->DrawCopy();

   TLegend *n4_fits = new TLegend(0.5,0.67,0.88,0.88,NULL,"brNDC");
   n4_fits->AddEntry(num_loss4,"number loss from poisson + real","lp");
   n4_fits->AddEntry(p,"poisson","lp");
   n4_fits->AddEntry(b,"binomial","lp");
   n4_fits->AddEntry(nb,"negative binomial","lp");

   n4_fits->Draw("same");

   TCanvas *c5 = new TCanvas("c5", "c5", 800, 800); c5->cd();

   cout << "\nFITTING real+real w/ poisson, binomial, and negative binomial, respectively!:\n\n";

   num_loss5->Fit(p,"ME+"); num_loss5->Fit(b,"ME+"); num_loss5->Fit(nb,"ME+"); 

   num_loss5->DrawCopy();

   TLegend *n5_fits = new TLegend(0.5,0.67,0.88,0.88,NULL,"brNDC");
   n5_fits->AddEntry(num_loss5,"number loss from real multiplicity + real efficiency","lp");
   n5_fits->AddEntry(p,"poisson","lp");
   n5_fits->AddEntry(b,"binomial","lp");
   n5_fits->AddEntry(nb,"negative binomial","lp");

   n5_fits->Draw("same");

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~saving pdfs~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

  m1->SaveAs("../results/delta_flat.pdf");
  m2->SaveAs("../results/delta_real.pdf");
  m3->SaveAs("../results/pois_flat.pdf");
  m4->SaveAs("../results/pois_real.pdf");
  m5->SaveAs("../results/real_real.pdf");

  c0->SaveAs("../results/superimposed.pdf");

  c1->SaveAs("../results/delta_flat_w_fits.pdf");
  c2->SaveAs("../results/delta_real_w_fits.pdf");
  c3->SaveAs("../results/pois_flat_w_fits.pdf");
  c4->SaveAs("../results/pois_real_w_fits.pdf");
  c5->SaveAs("../results/real_real_w_fits.pdf");
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

}
