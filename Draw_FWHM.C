void Draw_FWHM(TString DataName)
{
	const Int_t Num = 17;

	Double_t data_0[Num];
	Double_t data_1[Num];
	Double_t data_2[Num];
	Double_t data_3[Num];

	ifstream inFile;
	inFile.open(DataName);
	if(inFile.is_open())
	{
		cout << "open " << DataName << " successfully! " << endl; 
	}
	else
	{
		cout << "Fail to open " << DataName << endl;
		exit(EXIT_FAILURE);
	}

	auto rc = new TCanvas();
    for (int k = 0; k < Num; k++)
    {
		inFile >> data_0[k] >> data_1[k];
    }
    auto tgFW = new TGraph(Num, data_0, data_1);
    // tgFW->SetTitle("Bandwidth;Wavelength(nm);FWHM(nm)");
    tgFW->SetTitle(" ;Wavelength(nm);FWHM(nm)");
    tgFW->GetYaxis()->SetRangeUser(0, 5);
    tgFW->SetMarkerStyle(8);
    tgFW->Draw("AP");
    rc->Draw();
	rc->SaveAs("UVBandwidth.pdf");

}

void Draw_FWHM()
{
	TString DataName = "./UVData.txt";
	Draw_FWHM(DataName);
}
