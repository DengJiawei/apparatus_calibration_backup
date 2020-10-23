// 263.574 *** 1.99126
// 268.293 *** 1.95911
// 273.481 *** 1.91287
// 278.665 *** 2.02553
// 282.905 *** 2.16614
// 287.143 *** 2.24395
// 292.319 *** 2.32595
// 297.962 *** 2.08559
// 303.132 *** 2.32464
// 307.829 *** 2.31204
// 312.992 *** 2.63262
// 318.152 *** 2.67406
// 323.31 *** 2.93029
// 327.995 *** 2.85681
// 333.146 *** 3.29329
// 337.826 *** 3.59556
// 343.438 *** 2.81962


// 349.981 *** 4.43684
// 354.651 *** 4.733
// 360.252 *** 5.10988
// 364.917 *** 5.27251
// 370.511 *** 5.47379
// 375.169 *** 6.06447
// 380.756 *** 6.60693
// 384.944 *** 6.55177
// 390.524 *** 7.00216
// 394.706 *** 6.71616
// 399.815 *** 7.18632
// 404.457 *** 7.61941
// 409.096 *** 7.72826
// 414.66 *** 8.16352
// 420.219 *** 8.72406
// 424.849 *** 9.01165
// 429.477 *** 9.49245
// 434.563 *** 9.69387
// 438.723 *** 9.76855
// 444.728 *** 10.2693
// 448.421 *** 10.4755
// 453.495 *** 11.0118
// 459.028 *** 11.6517
// 463.175 *** 12.1868
// 468.7 *** 13.3569
// 476.522 *** 13.3247
// 478.361 *** 10.0955



void Main_laserCali()
{

    // const int StartWavelength = 265;
    // const int TotalFile = 17;

    const int TotalFile = 27;
    const int FileColumn = 2048;
    const int StartWavelength = 350;
    const int WaveStep = 5;
    // char FileName[200];
    TString FileName;
    TString FilePath;
    TGraphErrors *tg[TotalFile];
    TF1 *tf[TotalFile];


    // // ****************gaus
    Double_t peak[TotalFile];
    Double_t mu[TotalFile];
    Double_t mu_error[TotalFile];
    Double_t sigma[TotalFile];
    Double_t sigma_error[TotalFile];
    Double_t peak_error[TotalFile];


    // ************************FWHM
    double waveFWHM[TotalFile];
    double wavePeak[TotalFile];

    TCanvas *tc[TotalFile];

    ifstream inFile;
    for (int i = 0; i < TotalFile; i++)
    {
        Double_t wavelength[FileColumn];
        Double_t intensity[FileColumn];

        char FileNamefront[200];
        char GraphTitle[100];

        sprintf(FileNamefront, "%d", StartWavelength + WaveStep*i);
        // sprintf(FileName, "/Users/dengjiawei/UCAS/LS吸收重发射实验/实验数据/光谱仪/20181220/350nm-410nm_5ms_8.2\%/%s.csv", FileNamefront);
       
       if(StartWavelength == 265)
       {
           FileName = TString("./DUV265_345/DUV_HRD08281_") + Form("%05d.txt", 17-i);
       }
       else if (StartWavelength == 350)
       {
           FileName = TString("./UV350_480/UV_HRD08281_") + Form("%05d.txt", i+1);
       }
       else
       {
           cout << "please check the experiment data file " << endl;
           exit(EXIT_FAILURE);
       }
       
        sprintf(GraphTitle, "%d;wavelength(nm);intensity", StartWavelength + WaveStep*i);

        //以下是分析程序，对于两个拓展单元，不需要更改
        // ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V· ·V·
        inFile.open(FileName);
        if (!inFile.is_open())
        {
            cout << "cann't open " << FileName << endl;
            exit(EXIT_FAILURE);
        }
        char skipstring[500];
        for(int k = 0; k < 13; k ++)
        {
            inFile.getline(skipstring, 100);
        }
        for (int k = 0; k < FileColumn; k++)
        {
            inFile >> wavelength[k]  >> intensity[k];
        }
        inFile.close();

        cout << wavelength[0] << "   " << intensity[0] << endl;

        // *******************************************************************************************
        //单个文件中的信息
        double peakwave = wavelength[0];
        double peakintensity = intensity[0];
        int peakposi = 0;
        double FWHM;
        double HalfMax;
        double baseline = intensity[0];
        
        //寻找到信号的峰值
        for (int j = 0; j < FileColumn; j++)
        {
            if (intensity[j] > peakintensity)
            {
                peakintensity = intensity[j];
                peakwave = wavelength[j];
                peakposi = j;
            }
        }
        wavePeak[i] = wavelength[peakposi];

        //计算baseline：取除去峰位置的别的点的平均值
        Double_t countNum = 0;
        Double_t sum_A = 0;
        for (int j = 0; j < FileColumn; j++)
        {
            if(j < peakposi-40 || j > peakposi+40)
            {
                sum_A += intensity[j];
                countNum += 1;
            }
        }
        baseline = sum_A / countNum;
        cout << "baseline by  averaging all  = " << baseline << endl;

        // for (int j = 80; j < 500; j++)
        // {
        //     baseline += (intensity[peakposi - j] + intensity[peakposi + j]);
        // }
        // baseline /= 840.;
        HalfMax = baseline + 0.5 * (peakintensity - baseline);
        cout << "half max by counting = " << HalfMax << endl;

        double waveHM_l;
        double waveHM_r;

        for (int j = peakposi; j > peakposi - 100; j--)
        {
            if (intensity[j] < HalfMax)
            {
                waveHM_l = wavelength[j] + (HalfMax - intensity[j]) / (intensity[j + 1] - intensity[j]) * (wavelength[j + 1] - wavelength[j]);
                break;
            }
        }
        for (int j = peakposi; j < peakposi + 100; j++)
        {
            if (intensity[j] < HalfMax)
            {
                waveHM_r = wavelength[j] + (HalfMax - intensity[j]) / (intensity[j - 1] - intensity[j]) * (wavelength[j - 1] - wavelength[j]);
                break;
            }
        }
        FWHM = waveHM_r - waveHM_l;
        waveFWHM[i] = FWHM;
        cout << "FWHM by counting = " << FWHM << endl; 
        // *******************************************************************************************
        // gStyle->SetOptFit(111);
        gStyle->SetOptFit(011);

        tc[i] = new TCanvas(FileNamefront, FileNamefront, 1000, 1000);
        tg[i] = new TGraphErrors(FileColumn, wavelength, intensity, nullptr, nullptr);
        tf[i] = new TF1(FileNamefront, "gaus(0)+[3]", wavePeak[i]-30, wavePeak[i]+30);
        tf[i]->SetParameter(0, 10000);
        tf[i]->SetParameter(1, StartWavelength + WaveStep*i);
        tf[i]->SetParameter(2, 5);
        tf[i]->SetParameter(3, baseline);
        tf[i]->SetParNames("N","Peak","Sigma","Offset");

        tf[i]->SetParLimits(0,1,1e6);
        tf[i]->SetParLimits(1,250,500);
        tf[i]->SetParLimits(2,0,15);
        tf[i]->SetParLimits(3,-1000,3000);
        cout << "wavePeak[i]-30 =" << wavePeak[i]-30 << endl;
        cout << "StartWavelength + WaveStep*i =" << StartWavelength + WaveStep*i << endl;

        tg[i]->Fit(FileNamefront, "R");

        peak[i] = (tf[i]->GetParameter(0) - tf[i]->GetParameter(3));
        mu[i] = tf[i]->GetParameter(1);
        mu_error[i] = tf[i]->GetParError(1);
        sigma[i] = tf[i]->GetParameter(2);
        sigma_error[i] = tf[i]->GetParError(2);
        peak_error[i] = (tf[i]->GetParError(0) + tf[i]->GetParError(3));

        // tg[i]->SetTitle(GraphTitle);
        tg[i]->SetTitle(Form("wavelength@ %d nm",StartWavelength + WaveStep*i));


       if(StartWavelength == 265)
       {
           tg[i]->GetXaxis()->SetRangeUser(wavePeak[i]-10, wavePeak[i]+10);
       }
       else if (StartWavelength == 350)
       {
           tg[i]->GetXaxis()->SetRangeUser(wavePeak[i]-30, wavePeak[i]+30);
       }
       else
       {
           cout << "please check the experiment data file name " << endl;
           exit(EXIT_FAILURE);
       }


        tg[i]->SetMarkerStyle(8);
        tg[i]->Draw("APE");

        // TPaveStats *ptstats = new TPaveStats(0.7, 0.7, 0.9, 0.9, "brNDC");
        TPaveStats *ptstats = new TPaveStats(0.7, 0.7, 0.9, 0.9, "011");

        ptstats->SetName("stats");
        ptstats->SetBorderSize(1);
        ptstats->SetFillColor(0);
        ptstats->SetTextAlign(12);
        ptstats->SetTextFont(42);
        TText *ptstats_LaTex = ptstats->AddText("");
        ptstats->SetOptStat(10);
        ptstats->SetOptFit(111);
        tg[i]->GetListOfFunctions()->Add(ptstats);
        ptstats->Draw();

        // tf[i]->Draw("same");
        tc[i]->SaveAs(Form("%d.pdf",StartWavelength+WaveStep*i));

        // auto bsl = new TF1("bsl", "[0]", 100, 1000);
        // bsl->SetParameter(0, baseline);
        // bsl->Draw("same");
    }


    auto rc = new TCanvas();
    // for (int k = 0; k < TotalFile; k++)
    // {
    //     cout << mu[k] << " *** " << peak[k] << " *** " << sigma[k] << endl;
    // }
    // auto d = new TGraphErrors(TotalFile, mu, sigma, mu_error, sigma_error);
    // // //auto d = new TGraphErrors(TotalFile,mu,peak,sigma,peak_error);
    // // d->SetTitle("350~410nm5ms8.2%;wavelength(nm);sigma(nm)");
    // d->GetYaxis()->SetRangeUser(0, 4);
    //     d->GetXaxis()->SetRangeUser(350, 352);

    // d->SetMarkerStyle(8);
    // d->Draw();

    for (int k = 0; k < TotalFile; k++)
    {
        cout << wavePeak[k] << " *** " << waveFWHM[k] << endl;
    }
    auto tgFW = new TGraph(TotalFile, wavePeak, waveFWHM);
    // auto tgFW = new TGraphErrors(TotalFile, wavePeak, waveFWHM,nullptr,nullptr);
    //auto d = new TGraphErrors(TotalFile,mu,peak,sigma,peak_error);
    tgFW->SetTitle(";wavelength(nm);FWHM/(nm)");
    tgFW->GetYaxis()->SetRangeUser(0, 12);
    tgFW->SetMarkerStyle(8);
    // tgFW->SetMarkerColor(kRed);
    tgFW->Draw("AP");
    rc->Draw();
}

