#include<iostream>
#include"TCanvas.h"
#include"TF1.h"
#include"TH1F.h"
#include"TH2F.h"
#include "TH1D.h"
#include"TFile.h"
#include"TTree.h"
#include "TLegend.h"
#include "TStyle.h"
#include <TROOT.h>
#include <TChain.h>
#include <string>
#include "TSystem.h"
#include "TSystemDirectory.h"
#include <TLorentzVector.h>

using namespace std;


int main(void){

    gROOT->Reset();

    //Ustawienie TStyle
	gStyle->SetOptStat(0);

    //Załadowanie danych z pliku ../Data/D2hhh.root
	TFile* LoadedData = new TFile("../Data/D2hhh.root");		
	TTree* DecayTree = (TTree*)LoadedData->Get("DecayTree");

    //Deklaracja wymaganych zmiennych

        Double_t h1_PX, h1_PY, h1_PZ, h1_M, h1_E; //h_1 symulowane
        Double_t h2_PX, h2_PY, h2_PZ, h2_M, h2_E; //h_2 symulowane
        Double_t h3_PX, h3_PY, h3_PZ, h3_M, h3_E; //h_3 symulowane

        //--->  Prawdopodobieństwo, że hadron jest kaonem
        Double_t h1_ProbNNk, h2_ProbNNk, h3_ProbNNk;

        //--->  Prawdopodobieństwo, że hadron jest pionem
        Double_t h1_ProbNNpi, h2_ProbNNpi, h3_ProbNNpi;

        //--->  Flight distance mezonu D
        Double_t D_FD;

    //Ustawienie zmiennej odcięcia
    Double_t odciecie = 0.9;
    Double_t rev_odciecie = 1 - odciecie;

    //Załadowaie zmiennych

        //Hadron h_1
        
        DecayTree->SetBranchAddress("h1_PX", &h1_PX);   //PX
        DecayTree->SetBranchAddress("h1_PY", &h1_PZ);   //PY
        DecayTree->SetBranchAddress("h1_PZ", &h1_PY);   //PZ
        DecayTree->SetBranchAddress("h1_E", &h1_E);     //E
        
        //Hadron h_2
        
        DecayTree->SetBranchAddress("h2_PX", &h2_PX);   //PX
        DecayTree->SetBranchAddress("h2_PY", &h2_PZ);   //PY
        DecayTree->SetBranchAddress("h2_PZ", &h2_PY);   //PZ
        DecayTree->SetBranchAddress("h2_E", &h2_E);     //E
        
        //Hadron h_3
        
        DecayTree->SetBranchAddress("h3_PX", &h3_PX);   //PX
        DecayTree->SetBranchAddress("h3_PY", &h3_PZ);   //PY
        DecayTree->SetBranchAddress("h3_PZ", &h3_PY);   //PZ
        DecayTree->SetBranchAddress("h3_E", &h3_E);     //E
        
        //Prawdopodobieństwo, że hadron jest kaonem
        
        DecayTree->SetBranchAddress("h1_ProbNNk", &h1_ProbNNk); //h1
        DecayTree->SetBranchAddress("h2_ProbNNk", &h2_ProbNNk); //h2
        DecayTree->SetBranchAddress("h3_ProbNNk", &h3_ProbNNk); //h3
        
        //Prawdopodobieństwo, że hadron jest pionem
        
        DecayTree->SetBranchAddress("h1_ProbNNpi", &h1_ProbNNpi);   //h1
        DecayTree->SetBranchAddress("h2_ProbNNpi", &h2_ProbNNpi);   //h2
        DecayTree->SetBranchAddress("h3_ProbNNpi", &h3_ProbNNpi);   //h3
        
        //Flight distance mezonu D
        DecayTree->SetBranchAddress("D_FD", &D_FD);


    //Utworzenie zmiennych TLorentzVector
    TLorentzVector Czteroped_h1;
    TLorentzVector Czteroped_h2;
    TLorentzVector Czteroped_h3;
    TLorentzVector Czteroped_D;

    //Pobranie z DecayTree liczby zdarzen
    Int_t LiczbaZdarzen = DecayTree->GetEntries();
    //Przelicznik z MeV na GeV
    Int_t toGeV = .001;
    //Liczba binów histogramow
    Int_t LiczbaBinow = 100;

    //Zadeklarowanie histogramow
    TH1D* RozkladMasyNiezmienniczej_h1 = new TH1D("RozkladMasyNiezmienniczej_h1", "Rozklad masy niezmienniczej;Masa [GeV/c^2];Liczba zdarzen", LiczbaBinow, 0, 0.6);
    TH1D* RozkladMasyNiezmienniczej_h2 = new TH1D("RozkladMasyNiezmienniczej_h2", "Rozklad masy niezmienniczej;Masa [GeV/c^2];Liczba zdarzen", LiczbaBinow, 0, 0.6);
    TH1D* RozkladMasyNiezmienniczej_h3 = new TH1D("RozkladMasyNiezmienniczej_h3", "Rozklad masy niezmienniczej;Masa [GeV/c^2];Liczba zdarzen", LiczbaBinow, 0, 0.6);
    TH1D* RozkladMasyNiezmienniczej_D = new TH1D("RozkladMasyNiezmienniczej_D", "Rozklad masy niezmienniczej - D;Masa [GeV/c^2];Liczba zdarzen", LiczbaBinow, 1.8, 2);
    TH1D* RozkladCzasuZycia_D = new TH1D("RozkladCzasuZycia_D", "Rozklad czasu zycia - D;Czas [s];Liczba zdarzen", LiczbaBinow, 0, 0.00000000005);
    TH1D* RozkladFlightDistance_D = new TH1D("RozkladFlightDistance_D", "Rozklad fligth distance - D;Dystans [mm];Liczba zdarzen", LiczbaBinow, 0, 40);

    TH1D* RozkladPrawdopodobienstwa_h1_kaon = new TH1D("RozkladPrawdopodobienstwa_h1_kaon", "Rozklad prawdopodobienstwa: h1 jest kaonem;Prawdopodobienstwo;Liczba zdarzen", LiczbaBinow, 0, 1);
    TH1D* RozkladPrawdopodobienstwa_h2_kaon = new TH1D("RozkladPrawdopodobienstwa_h2_kaon", "Rozklad prawdopodobienstwa: h2 jest kaonem;Prawdopodobienstwo;Liczba zdarzen", LiczbaBinow, 0, 1);
    TH1D* RozkladPrawdopodobienstwa_h3_kaon = new TH1D("RozkladPrawdopodobienstwa_h3_kaon", "Rozklad prawdopodobienstwa: h3 jest kaonem;Prawdopodobienstwo;Liczba zdarzen", LiczbaBinow, 0, 1);

    TH1D* RozkladPrawdopodobienstwa_h1_pion = new TH1D("RozkladPrawdopodobienstwa_h1_pion", "Rozklad prawdopodobienstwa: h1 jest pionem;Prawdopodobienstwo;Liczba zdarzen", LiczbaBinow, 0, 1);
    TH1D* RozkladPrawdopodobienstwa_h2_pion = new TH1D("RozkladPrawdopodobienstwa_h2_pion", "Rozklad prawdopodobienstwa: h2 jest pionem;Prawdopodobienstwo;Liczba zdarzen", LiczbaBinow, 0, 1);
    TH1D* RozkladPrawdopodobienstwa_h3_pion = new TH1D("RozkladPrawdopodobienstwa_h3_pion", "Rozklad prawdopodobienstwa: h3 jest pionem;Prawdopodobienstwo;Liczba zdarzen", LiczbaBinow, 0, 1);

    for (Int_t i = 0; i < LiczbaZdarzen; ++i)
    {
        DecayTree->GetEntry(i);

        RozkladPrawdopodobienstwa_h1_kaon->Fill(h1_ProbNNk);
        RozkladPrawdopodobienstwa_h2_kaon->Fill(h2_ProbNNk);
        RozkladPrawdopodobienstwa_h3_kaon->Fill(h3_ProbNNk);

        RozkladPrawdopodobienstwa_h1_pion->Fill(h1_ProbNNpi);
        RozkladPrawdopodobienstwa_h2_pion->Fill(h2_ProbNNpi);
        RozkladPrawdopodobienstwa_h3_pion->Fill(h3_ProbNNpi);

        Czteroped_h1.SetPxPyPzE(h1_PX, h1_PY, h1_PZ, h1_E);
        Czteroped_h2.SetPxPyPzE(h2_PX, h2_PY, h2_PZ, h2_E);
        Czteroped_h3.SetPxPyPzE(h3_PX, h3_PY, h3_PZ, h3_E);

        RozkladMasyNiezmienniczej_h1->Fill(Czteroped_h1.M());
        RozkladMasyNiezmienniczej_h2->Fill(Czteroped_h2.M());
        RozkladMasyNiezmienniczej_h3->Fill(Czteroped_h3.M());

        if
        (
            (h1_ProbNNk > odciecie && h1_ProbNNpi < rev_odciecie) &&
            (h2_ProbNNpi > odciecie && h2_ProbNNk < rev_odciecie) &&
            (h3_ProbNNpi > odciecie && h3_ProbNNk < rev_odciecie)
            ||
            (h1_ProbNNpi > odciecie && h1_ProbNNk < rev_odciecie) &&
            (h2_ProbNNk > odciecie && h2_ProbNNpi < rev_odciecie) &&
            (h3_ProbNNpi > odciecie && h3_ProbNNk < rev_odciecie)
            ||
            (h1_ProbNNpi > odciecie && h1_ProbNNk < rev_odciecie) &&
            (h2_ProbNNpi > odciecie && h2_ProbNNk < rev_odciecie) &&
            (h3_ProbNNk > odciecie && h3_ProbNNpi < rev_odciecie)
        ){
            Czteroped_D = Czteroped_h1 + Czteroped_h2 + Czteroped_h3;

            Double_t MasaD = Czteroped_D.M();   //GeV/c^2
            Double_t Ped_D = Czteroped_D.P();   //GeV/c
            Double_t CzasZycia = 1e-3 * D_FD * Ped_D /(3e8 * MasaD); //W sekundach

            RozkladMasyNiezmienniczej_D->Fill(MasaD);
            RozkladCzasuZycia_D->Fill(CzasZycia);
            RozkladFlightDistance_D->Fill(D_FD);
        }
    }


    //Utworzenie obeiktu TCanvas
	TCanvas* Okno = new TCanvas("Okno", "Okno", 1366, 768);
    //Zapis pliku
	std::string PlikZapisu = "mezon_d.pdf";
	Okno->SaveAs((PlikZapisu  + "[").c_str());

    //Masa niezmiennicza h1, h2 oraz h3
    RozkladMasyNiezmienniczej_h1->SetLineColorAlpha(1, 1);
    RozkladMasyNiezmienniczej_h2->SetLineColorAlpha(3, 1);
    RozkladMasyNiezmienniczej_h3->SetLineColorAlpha(6, 1);
    RozkladMasyNiezmienniczej_h1->Draw();
    RozkladMasyNiezmienniczej_h2->Draw("same");
    RozkladMasyNiezmienniczej_h3->Draw("same");

	TLegend Legenda(0.5, 0.75, 0.7, 0.85);

	Legenda.AddEntry(RozkladMasyNiezmienniczej_h1, "Hadron 1","l");
	Legenda.AddEntry(RozkladMasyNiezmienniczej_h2, "Hadron 2","l");
    Legenda.AddEntry(RozkladMasyNiezmienniczej_h3, "Hadron 3","l");
	Legenda.Draw();
	Okno->SaveAs(PlikZapisu.c_str());
	Okno->Clear();

    //Masa niezmiennicza
    TF1 *GaussMasaNiezmiennicza = new TF1("GaussMasaNiezmiennicza", "gaus", 1.82, 1.91);
    RozkladMasyNiezmienniczej_D->Fit(GaussMasaNiezmiennicza,"R");
    RozkladMasyNiezmienniczej_D->Draw();
	Okno->SaveAs(PlikZapisu.c_str());
	Okno->Clear();

    //Flight distance
    RozkladFlightDistance_D->Draw();
	Okno->SaveAs(PlikZapisu.c_str());
	Okno->Clear();

    //Sredni czas zycia
	TF1 *FunkcjaCzasZycia = new TF1("FunkcjaCzasZycia","[0]*exp([1]*x)", 1e-12*0, 1e-12*40);
    RozkladCzasuZycia_D->Fit(FunkcjaCzasZycia,"R");
	RozkladCzasuZycia_D->Draw();
	Okno->SaveAs(PlikZapisu.c_str());
	Okno->Clear();

    //Prawdopodobieństwo, że hadron h1 jest kaonem
    RozkladPrawdopodobienstwa_h1_kaon->Draw();
	Okno->SaveAs(PlikZapisu.c_str());
	Okno->Clear();

    //Prawdopodobieństwo, że hadron h2 jest kaonem
    RozkladPrawdopodobienstwa_h2_kaon->Draw();
	Okno->SaveAs(PlikZapisu.c_str());
	Okno->Clear();

    //Prawdopodobieństwo, że hadron h3 jest kaonem
    RozkladPrawdopodobienstwa_h3_kaon->Draw();
	Okno->SaveAs(PlikZapisu.c_str());
	Okno->Clear();

    //Prawdopodobieństwo, że hadron h1 jest pionem
    RozkladPrawdopodobienstwa_h1_pion->Draw();
	Okno->SaveAs(PlikZapisu.c_str());
	Okno->Clear();

    //Prawdopodobieństwo, że hadron h2 jest pionem
    RozkladPrawdopodobienstwa_h2_pion->Draw();
	Okno->SaveAs(PlikZapisu.c_str());
	Okno->Clear();

    //Prawdopodobieństwo, że hadron h3 jest pionem
    RozkladPrawdopodobienstwa_h3_pion->Draw();
	Okno->SaveAs(PlikZapisu.c_str());
	Okno->Clear();

	Okno->SaveAs((PlikZapisu  + "]").c_str());

	return 0;
}