

void measure_laser()
{
    const int TotalFile = 1;
    const int FileColumn = ;
	char FileName[50] = "405.csv";

    TGraphErrors * tg[TotalFile];
    TF1 * tf[TotalFile];

    TCanvas * tc = new TCanvas();

    ifstream inFile;
    for (int i = 0; i < TotalFile; i++)
    {
        Double_t wavelength[FileColumn];
        Double_t intensity[FileColumn];
        char douhao;

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

        tg[i] = new TGraphErrors(FileColumn,wavelength,intensity,nullptr,nullptr);
        tf[i] = new TF1("tf","gaus(0)+[3]",300,600);

        tg[i]->Fit("tf");
        tg[i]->Draw();
        tf[i]->Draw("same");



    }
}
