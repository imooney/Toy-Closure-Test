// trackloss.C: performing the track loss on the toy using distributions from dists.C
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

unsigned delta_mult_mean = 5, num_events = 1e6;
double flat_effic = 0.8;

void trackloss () {
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~setting up histograms and functions~~~~~~~~~~~~~~~~~~~~~~~~~~~//
   TH1::SetDefaultSumw2();
   TH2::SetDefaultSumw2();
   TH3::SetDefaultSumw2();
    
   TFile *in = new TFile("../out/dists.root", "READ");

   TFile *out = new TFile("../out/toyclosure.root", "RECREATE");

   //TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);

   TH1 *pois_mult = (TH1D*)in->Get("pois_mult");
   //these are Pythia8 particle-level matched jet track multiplicity and constituent pT to be used in the toy
   TH1 *real_mult = (TH1D*)in->Get("numpartmatchedperm");
   TH1 *pt_cons = (TH1D*)in->Get("ptconspartmatchedperm");
   TH1 *effic_eta_averaged = (TH1D*)in->Get("projy");
   TH1 *conspt_pminusd = (TH1D*)in->Get("mult"); //shouldn't actually need this


   TH1 *num_loss1 = new TH1D("num_loss1", "number loss from delta + flat", 7, -0.5, 6.5);
   TH1 *num_loss2 = new TH1D("num_loss2", "number loss from delta + real", 7, -0.5, 6.5);
   TH1 *num_loss3 = new TH1D("num_loss3", "number loss from poisson + flat", 7, -0.5, 6.5);
   TH1 *num_loss4 = new TH1D("num_loss4", "number loss from poisson + real", 7, -0.5, 6.5);
   TH1 *num_loss5 = new TH1D("num_loss5", "number loss from real multiplicity + real efficiency", 7, -0.5, 6.5);
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


   
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~analysis~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

   // 1) Delta function particle-level multiplicity + flat efficiency (0.8) = ?
   // 2) Delta function particle-level multiplicity + sampling from actual constituent pT distribution + real [eta-averaged] STAR efficiency (0.8) = ?
   // 3) Poisson particle-level multiplicity + sampling from actual constituent pT distribution + flat efficiency (0.8) = ?
   // 4) Same as 3) but with real STAR efficiency = ?
   // 5) Real matched jet particle-level multiplicity + actual constituent pT + real STAR efficiency = ?

   
   for (unsigned iEvent = 0; iEvent < num_events; ++ iEvent) { //begin event loop!
     // 1)
     unsigned num_loss = 0;
     for (unsigned iTrack = 0; iTrack < 5; ++ iTrack) {
       if (gRandom->Uniform(0.0, 1.0) >= flat_effic) { //this will happen on average 20% of the time - means we lose the current track
	 ++ num_loss;
       }
     }
     num_loss1->Fill(num_loss);
   
     // 2)
     num_loss = 0;
     for (unsigned iTrack = 0; iTrack < 5; ++ iTrack) {
       //getting random track from constituent spectrum, finding the corresponding efficiency, dividing by 10 to make it range from 0 to 1, and checking if we pass or not.
       if (gRandom->Uniform(0.0, 1.0) >= (effic_eta_averaged->GetBinContent((int)(pt_cons->GetRandom()) + 1)/(double) 10)) {
	 ++ num_loss;
       }
     }
     num_loss2->Fill(num_loss);
   

     // 3)
     //sampling from poisson for number of tracks first:
     unsigned num_tracks = pois_mult->GetRandom();
     num_loss = 0;
     for (unsigned iTrack = 0; iTrack < num_tracks; ++ iTrack) {
       if (gRandom->Uniform(0.0, 1.0) >= flat_effic) { //this will happen on average 20% of the time - means we lose the current track
	 ++ num_loss;
       }
     }
     num_loss3->Fill(num_loss);

     // 4)
     //sampling from poisson for number of tracks first:
     num_tracks = pois_mult->GetRandom();
     num_loss = 0;
     for (unsigned iTrack = 0; iTrack < num_tracks; ++ iTrack) {
       if (gRandom->Uniform(0.0, 1.0) >= (effic_eta_averaged->GetBinContent((int)(pt_cons->GetRandom()) + 1)/(double) 10)) {
	 ++ num_loss;
       }
     }
     num_loss4->Fill(num_loss);

     // 5)
     //sampling from real matched jet multiplicity for number of tracks first:
     num_tracks = real_mult->GetRandom();
     num_loss = 0;
     for (unsigned iTrack = 0; iTrack < num_tracks; ++ iTrack) {
       if (gRandom->Uniform(0.0, 1.0) >= (effic_eta_averaged->GetBinContent((int)(pt_cons->GetRandom()) + 1)/(double) 10)) {
	 ++ num_loss;
       }
     }
     num_loss5->Fill(num_loss);

   } //end event loop!

   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


   
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~writing to file~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
   out->Write();
   cout << "writing to toyclosure.root!" << '\n';
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
   return;
}
