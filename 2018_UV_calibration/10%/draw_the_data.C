


void draw_the_data()
{
	const int fileNum = 14;
	const int fileColumn = 3648;
	char fileName[50];
	char fileNameSave[50] = "10.pdf";
	char measure_condition[100] = "power = 10 %;wavelength;intensity";

//	Double_t wavelength[fileColumn];
//	Double_t intensity[fileColumn];
	char douhao;

	ifstream inFile;

	TGraphErrors* tg[fileNum];
	auto tc = new TCanvas();

	// for (int i = 0; i < fileNum; i++)
int i = 13;
	{
		Double_t wavelength[fileColumn];
		Double_t intensity[fileColumn];
		sprintf(fileName,"%dnm.csv",350+10*i);
		inFile.open(fileName);
		cout << "open file " << fileName << endl;
		if(!inFile.is_open())
		{
			cout << "can't open file " << fileName << endl;
			exit(EXIT_FAILURE);
		}
		for (int k = 0; k < fileColumn; k++)
		{
			inFile >> wavelength[k] >> douhao >> intensity[k];
		}
		inFile.close();
		tg[i] = new TGraphErrors(fileColumn,wavelength,intensity,nullptr,nullptr);

		if(i == 13)
		{

			// tg[i]->SetLineColor(1+i%4);
			tg[i]->SetLineWidth(6);

			tg[i]->GetXaxis()->SetRangeUser(0,600);
			tg[i]->GetYaxis()->SetRangeUser(0,20000);
			tg[i]->SetTitle(measure_condition);
			tg[i]->Draw();
		}
		else if (i == fileNum-1)
		{
			tg[i]->SetLineColor(1);
			tg[i]->SetLineWidth(6);
			tg[i]->Draw("same");
			
		}
		else
		{
			tg[i]->SetLineColor(1+i%8);
			tg[i]->SetLineWidth(2);
			tg[i]->Draw("same");
		}
	}

	tc->SaveAs(fileNameSave);




}
