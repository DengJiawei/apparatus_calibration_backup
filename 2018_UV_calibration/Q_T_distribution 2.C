//2018.12.5
//此程序首先是读取示波器中三列数据，第一列为时间，第二三列为pmt和激光的信号
//其次找出激光信号中第二个峰值的时间位置，作出相应直方图
//然后再计算pmt信号的积分值，作出响应直方图
//2018.12.6
//改变了峰积分时候的阈值
//增加了寻找两个峰的程序，但是这个文件中没有找到另外一个峰


void Q_T_distribution()
{

	const int FileQuantity = 1000;//总共的文件数量
	const int FileColumn = 8000;//每个文件的总列数
	char FileName[50] ;//文件的名称，用于读取文件
	char FileNamefront[50] = "ls_410nm_lvbo430nm";
	ifstream inFile;
	Double_t Q[FileQuantity] = {0};//pmt信号积分值
	Double_t T[FileQuantity]; //时间差
	int Q_peakNum[FileQuantity];//pmt信号峰的个数
	int Q_peak1Position[FileQuantity];
	int Q_peak2Position[FileQuantity];
	Double_t Q_peak1Value[FileQuantity];
	Double_t Q_peak2Value[FileQuantity];

	for (int i = 0; i < FileQuantity; i++)
	{
		Double_t t[FileColumn];
		Double_t pmt[FileColumn];
		Double_t laser[FileColumn];
		char douhao;

		sprintf(FileName,"%s%05d.csv",FileNamefront,i);

		inFile.open(FileName);
		if(!inFile.is_open())
		{
			cout << "can't open the file " << FileName << endl;
			exit(EXIT_FAILURE);
		}

		cout << "open " << FileName << endl;
		for (int k = 0; k < FileColumn; k++)
		{
			inFile >> t[k] >> douhao >> pmt[k] >> douhao >> laser[k];
		}
		inFile.close();



		//找出峰的位置
		int tistart[5] = {0};
		int tiend[5] = {0};
		int tipeak[5] = {0};
		int ti = 0;
		int twidth[5] = {0};
		int ttest = 0;
		Double_t _threshold_t = 0.4;

		for (int k = 0; k < FileColumn; k++)
		{
			if(laser[k] > _threshold_t)
			{
				tistart[ti] = k;
				tipeak[ti] = k;
				while (laser[tistart[ti]+twidth[ti]] > _threshold_t )
				{
					if (laser[tipeak[ti]] < laser[tistart[ti]+twidth[ti]])
					{
						tipeak[ti] = tistart[ti] + twidth[ti];
					}
					twidth[ti] ++;
				}
				tiend[ti] = tistart[ti] + twidth[ti] - 1;

				k = tiend[ti] + 1;
				ti ++;
			}
			ttest ++;
			if (ttest > FileColumn)
			{
				cout << " bug bug bug *************************************/n" << endl;
				exit(EXIT_FAILURE);
			}
		}

		cout << "peaks are " << tipeak[0] << " " << tipeak[1] << " " << tipeak[2] << " " << tipeak[3] << " " << tipeak[4] << endl; 

		T[i] = t[tipeak[1]];
	
	//求每个文件中信号的积分值
		//每个峰大小绝对值得大于某个阈值
		//我们对最高峰位置和大小应该加上某些限定
		//位置（跟示波器采数位置有关）。
		//峰值大小（最高峰我们都挑出来，但是若太小我们把这个事例给跳出来）
		//第二个峰绝对值得大于某个值，我们才认为它有第二个峰
		//如果信号有多个峰，我们排除掉第一个峰的位置
		Double_t _threshold_q = -0.006;
		Double_t _threshold_qfortwo = -0.012;
		int qistart = 0;
		int qiend = 0;
		int qpeak = 0;
		int qmove_l;
		int qmove_r;

		int qpeak_two = 0;
		int qistart_two = 0;
		int qiend_two = 0;

		for ( int k = 0; k < FileColumn; k++)
		{
			if(pmt[k] < pmt[qpeak])
			{
				qpeak = k;
			}
		}

		Q_peak1Position[i] = qpeak;

		qistart = qpeak;
		while (pmt[qistart] < _threshold_q)
		{
			qistart -= 1;
			if (qistart < 1)
			{
				cout << "qistart is wrong " << endl;
				exit(EXIT_FAILURE);
			}
		}
		qiend = qpeak;
		while (pmt[qiend] < _threshold_q)
		{
			qiend += 1;
			if (qiend < 1)
			{
				cout << "qiend is wrong " << endl;
				exit(EXIT_FAILURE);
			}
		}

		for(int k = qistart; k < qiend; k++ )
		{
			Q[i] += pmt[k]*(t[k+1]-t[k]);
		}
	
		//找第二个峰
		for (int k = 0; k < FileColumn; k++)
		{
			if( k > qistart && k < qiend)
			{
				continue;
			}
			if(laser[k] < _threshold_qfortwo)
			{
				cout << "********************************************there are some points > threshold_qfor two " << endl;
				qpeak_two = k;
				if (laser[k] < laser[qpeak_two])
				{
					qpeak_two = k;
					k++;
				}

			}
		}
		Q_peak2Position[i] = qpeak_two;

		Q_peak1Value[i] = laser[Q_peak1Position[i]];

		if (Q_peak2Position[i] != 0)
		{
			Q_peak2Value[i] = laser[Q_peak2Position[i]];
		}

	}



	TCanvas* tc_t = new TCanvas();
	int tbintNum = 100;
	double tbintstart = -10E-9;
	double tbintend = -10E-9;
	char t_title[80];//这个是直方图的标题注释
	sprintf(t_title,"%s;t;number",FileNamefront);

	char title_t[80];//这是用来保存直方图的文件名
	sprintf(title_t,"%s_t.pdf",FileNamefront);
	TH1F* t_distribution = new TH1F("Time",t_title,tbintNum,tbintstart,tbintend);

	for (int i = 0; i < FileQuantity; i++)
	{
		t_distribution->Fill(T[i]);
	}
	t_distribution->Draw();

	tc_t->SaveAs(title_t);

	//画出电荷的积分分布
	auto tc_Q = new TCanvas();
	int q_binNum = 100;
	double q_binstart = 0.1*Q[0];
	double q_binend = 10*Q[0];
	char q_title[80];
	sprintf(q_title,"%s;-Q;number",FileNamefront);
	TH1F* q_distribution = new TH1F("Q",q_title,q_binNum,q_binstart,q_binend);
	for (int i = 0; i < FileQuantity; i++)
	{
		q_distribution->Fill(-Q[i]);
	}
	q_distribution->Draw();

	char q_title_forsave[80];
	sprintf(q_title_forsave,"%s.pdf",FileNamefront);
	tc_Q->SaveAs(q_title_forsave);

	for (int k = 0; k < FileQuantity; k++)
	{
		cout << "the position of peaks are " << Q_peak1Position[k] << " ** " << Q_peak2Position[k] << endl;
	}
}

