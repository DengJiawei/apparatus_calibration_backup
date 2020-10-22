//2018.12.24邓佳维 重新修改
//不同文件夹需要改变的文件数量、命名的起始值、拟合参数的范围、标题等。
using namespace RooFit;

double doubledecay(double * x, double * par)
{
	return (par[2]*exp(-abs(x[0]-par[0])/par[1])+par[3]);
}





void measure_laser()
{
	const int TotalFile = 61;
	const int FileColumn = 3648;
	char FileName[50];
	TGraphErrors *tg[TotalFile];
	TF1 *tf[TotalFile];
	Double_t peak[TotalFile];
	Double_t mu[TotalFile];
	Double_t mu_error[TotalFile];
	Double_t sigma[TotalFile];
	Double_t sigma_error[TotalFile];
	Double_t peak_error[TotalFile];

	TCanvas *tc[TotalFile];

	ifstream inFile;
//	for (int i = 0; i < TotalFile; i++)
	 int i = 22;
	{
		Double_t wavelength[FileColumn];
		Double_t intensity[FileColumn];
		char douhao;

		char FileNamefront[50];
		char GraphTitle[100];

		sprintf(FileNamefront, "%d", 350 + i);
		sprintf(FileName, "%s.csv", FileNamefront);
		sprintf(GraphTitle, "%d;wavelength(nm);intensity", 350 + i);

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


tc[i] = new TCanvas();
		//		tc[i] = new TCanvas(FileNamefront,FileNamefront,1000,1000);
		tg[i] = new TGraphErrors(FileColumn, wavelength, intensity, nullptr, nullptr);
		tf[i] = new TF1(FileNamefront, "gaus(0)+[3]", 300, 600);
		tf[i]->SetParameter(0, 10000);
		tf[i]->SetParameter(1, 350 + i);
		tf[i]->SetParameter(2, 1);
		tg[i]->Fit(FileNamefront, "R");

// auto ttf = new TF1("ttf",doubledecay,300,600,4);
// ttf->SetParameters(350+i,1,10000,800);
// // tg[i]->Fit(ttf, "R");




		peak[i] = (tf[i]->GetParameter(0) - tf[i]->GetParameter(3));
		mu[i] = tf[i]->GetParameter(1);
		mu_error[i] = tf[i]->GetParError(1);
		sigma[i] = tf[i]->GetParameter(2);
		sigma_error[i] = tf[i]->GetParError(2);
		peak_error[i] = (tf[i]->GetParError(0) + tf[i]->GetParError(3));

		tg[i]->SetTitle(GraphTitle);
		tg[i]->GetXaxis()->SetRangeUser(345, 425);
		tg[i]->SetMarkerStyle(8);
		tg[i]->Draw("APE");
		// ttf->Draw("same");
		tf[i]->Draw("same");
// tc[i]->SaveAs(Form("%d.pdf",i));


		// RooExponential myexp("myexp", "myexp", t, tau);
	}

	// for (int k = 0; k < TotalFile; k ++)
	// {
	// 	cout << mu[k] << " *** " << peak[k] << " *** " << sigma[k] << endl;
	// }
	// auto d = new TGraphErrors(TotalFile,mu,sigma,mu_error,sigma_error);
	// //auto d = new TGraphErrors(TotalFile,mu,peak,sigma,peak_error);
	// d->SetTitle("350~410nm5ms8.2%;wavelength(nm);sigma(nm)");
	// d->GetYaxis()->SetRangeUser(0,2);
	// d->Draw("APE");
}
