Int_t	colors[5] = {1, 2, 3, 4, 6};

void script() {
	
	TFile *file = new TFile("itog.root");
	TTree *t = (TTree*)file->Get("RawDataTree");
	TH1 *h;
	THStack *st = new THStack;
	TCanvas *c = new TCanvas("canva", "canva", 600, 1200);
	c->Divide(1,3);
	c->cd(1);
	t->Draw("main_MPPC4.time_begin-main_feuM.time_begin>>delta_t(200,-40,20)", "");
	h = (TH1*)gDirectory->Get("delta_t");
	TFitResultPtr fres_ptr = h->Fit("gaus", "S");
	Float_t deltaT_mean = fres_ptr->Parameter(1);
	Float_t deltaT_sigma = fres_ptr->Parameter(2);
	c->cd(2);
	t->Draw("main_MPPC4.Amplitude>>ampl(200, 0, 450)", TString::Format("main_MPPC4.time_begin-main_feuM.time_begin>%.2f"
									"&&main_MPPC4.time_begin-main_feuM.time_begin<%.2f", 										deltaT_mean-deltaT_sigma,deltaT_mean+deltaT_sigma).Data());
	TVirtualPad *pad = c->cd(3);
	for (int n=0; n<5; n++) {
		TString varexp, sel, histTitle;
		varexp = TString::Format("main_MPPC4.time_begin-main_feuM.time_begin>>h_%d(200)", n);
		sel = TString::Format("main_MPPC4.Amplitude>%d&&main_MPPC4.Amplitude<%d", n*70, (n+1)*70);
		histTitle = TString::Format("deltaT %dMeV<Ampl<%dMeV", n*70, (n+1)*70);
		t->Draw(varexp.Data(), sel.Data(), "goff");
		h = (TH1F*)gDirectory->Get(TString::Format("h_%d", n));
		h->SetTitle(histTitle.Data());
		h->SetName(histTitle.Data());
		h->SetLineColor(colors[n]);
		fres_ptr = h->Fit("gaus", "NSq");
		printf("%s. Sigma: %.2f ns\n", histTitle.Data(), fres_ptr->Parameter(2));
		st->Add(h);
	}
	st->Draw("nostack");
	pad->BuildLegend();
}
