


void draw_the_data()
{
	const int fileNum = 9;
	const int fileColumn = 3648;
	char fileName[50];
	char fileNameSave[50] = "380nm.pdf";
	char measure_condition[100] = "380nm power=8,9,10,12,14,20,30,40,60;wavelength;intensity";

	int powernumber[fileNum] = {8,9,10,12,14,20,30,40,50};
//	Double_t wavelength[fileColumn];
//	Double_t intensity[fileColumn];
	char douhao;

	ifstream inFile;

	TGraphErrors* tg[fileNum];
	auto tc = new TCanvas();

	for (int i = 0; i < fileNum; i++)
	{
		Double_t wavelength[fileColumn];
		Double_t intensity[fileColumn];
		sprintf(fileName,"%d.csv",powernumber[i]);
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

		if(i == 0)
		{

			tg[i]->SetLineColor(1+i%4);
			tg[i]->SetLineWidth(6);

			tg[i]->GetXaxis()->SetRangeUser(150,600);
			tg[i]->GetYaxis()->SetRangeUser(0,20000);
			tg[i]->SetTitle(measure_condition);
			tg[i]->Draw();
		}
		else if (i == fileNum-1)
		{
			tg[i]->SetLineColor(2);
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
