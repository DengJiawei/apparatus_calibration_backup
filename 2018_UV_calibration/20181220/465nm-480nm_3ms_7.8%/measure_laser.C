

void measure_laser()
{
    const int TotalFile = 16;
    const int FileColumn = 3648 ;
	char FileName[50] ;
    TGraphErrors * tg[TotalFile];
    TF1 * tf[TotalFile];
	Double_t peak[TotalFile];
	Double_t mu[TotalFile];
	Double_t mu_error[TotalFile];
	Double_t sigma[TotalFile];
	Double_t sigma_error[TotalFile];
	Double_t peak_error[TotalFile];

    TCanvas * tc[TotalFile];

    ifstream inFile;
    for (int i = 0; i < TotalFile; i++)
    {
        Double_t wavelength[FileColumn];
        Double_t intensity[FileColumn];
        char douhao;

		char FileNamefront[50];
		char GraphTitle[100];
        
		sprintf(FileNamefront,"%d",465+i);
		sprintf(FileName,"%s.csv",FileNamefront);
		sprintf(GraphTitle,"%d;wavelength(nm);intensity",465+i);

		inFile.open(FileName);
        if (!inFile.is_open())
        {
            cout << "cann't open " << FileName << endl;
            exit(EXIT_FAILURE);
        }
        for (int k = 0; k < FileColumn; k++)
        {
            inFile >> wavelength[k] >> douhao >> intensity[k];
        }
        inFile.close();

//		tc[i] = new TCanvas(FileNamefront,FileNamefront,1000,1000);
        tg[i] = new TGraphErrors(FileColumn,wavelength,intensity,nullptr,nullptr);
        tf[i] = new TF1(FileNamefront,"gaus(0)+[3]",300,600);
		tf[i]->SetParameter(0,10000);
		tf[i]->SetParameter(1,465+i);
		tf[i]->SetParameter(2,1);
        tg[i]->Fit(FileNamefront,"R");

		peak[i] = (tf[i]->GetParameter(0) - tf[i]->GetParameter(3));
		mu[i] = tf[i]->GetParameter(1);
		mu_error[i] = tf[i]->GetParError(1);
		sigma[i] = tf[i]->GetParameter(2);
		sigma_error[i] = tf[i]->GetParError(2);
		peak_error[i] = (tf[i]->GetParError(0) + tf[i]->GetParError(3));


		tg[i]->SetTitle(GraphTitle);
		tg[i]->GetXaxis()->SetRangeUser(345,425);
//		tg[i]->Draw();
//		tf[i]->Draw("same");

    }

	for (int k = 0; k < TotalFile; k ++)
	{
		cout << mu[k] << " *** " << peak[k] << " *** " << sigma[k] << endl;
	}
    auto d = new TGraphErrors(TotalFile,mu,sigma,mu_error,sigma_error);
	//auto d = new TGraphErrors(TotalFile,mu,peak,sigma,peak_error);
    //d->SetTitle("465~480nm5ms7.8%;wavelength(nm);intensity");
    d->SetTitle("465~480nm5ms7.8%;wavelength(nm);sigma(nm)");
	d->GetYaxis()->SetRangeUser(0,2);
    d->Draw("APE");

}
