// dists.C: creating distributions to be read by the toy model closure test
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

unsigned desired_stats = 1e6, delt_mult_mean = 5;
double pois_mult_mean = 5;

void dists () {
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~setting up histograms and functions~~~~~~~~~~~~~~~~~~~~~~~~~~~//
   TH1::SetDefaultSumw2();
   TH2::SetDefaultSumw2();
   TH3::SetDefaultSumw2();

   TFile *effic = new TFile("../in/matchedout_realeffic.root","READ");

   TH1 * conspt_pminusd = (TH1D*)effic->Get("mult");
   TH1 * effic_eta_averaged = (TH1D*)effic->Get("projy");

   TFile *in = new TFile("../in/ddc_matched_skinny_real.root", "READ");
   
   TFile *out = new TFile("../out/dists.root", "RECREATE");

   //TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);

   TH1 * pois_mult = new TH1D("pois_mult", "multiplicity ~ poisson", 20, -0.5, 19.5);
   TH1 * real_mult = (TH1D*)in->Get("numpartmatchedperm");
   TH1 * pt_cons = (TH1D*)in->Get("ptconspartmatchedperm");

   TF1 * pois = new TF1("pois", "ROOT::Math::poisson_pdf(x,5)", -0.5, 19.5);
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
   
   
   //~~~~~~~~~~~~~~~~~~~~~~~~filling multiplicity distribution!~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
   for (unsigned i = 0; i < desired_stats; ++ i) {
     pois_mult->Fill(pois->GetRandom() - 0.5); //Because I can't change how the "bins" of the function are centered for some reason - systematic shift to the left by 0.5.
   }
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
   

   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~writing to file!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
   conspt_pminusd->Write();
   effic_eta_averaged->Write();
   real_mult->Write();
   pt_cons->Write();
   out->Write();
   cout << "Writing distributions to dists.root!" << '\n';
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
   return;
}
