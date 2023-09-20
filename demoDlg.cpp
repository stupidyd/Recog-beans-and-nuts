
// demoDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "demo.h"
#include "demoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CdemoDlg::ProcessImage()
{
	int cols = temp_image.GetWidth();      //获取图片宽和高
	int rows = temp_image.GetHeight();
	int i = 0, j = 0;
	int HuangDou = 0, LvDou = 0, Lep = 0 ,LuoMu=0;    //初始化绿豆与黄豆数目
	int region[300] = { 0 };    //初始化面积数组，用于存放每个区域的面积
	int regionl[300] = { 0 };    //初始化面积数组，用于存放每个区域的面积
	int number = 0;
	unsigned char* p = (unsigned char*)temp_image.GetBits();
	unsigned char* show_p = (unsigned char*)show_image.GetBits();//调试参数用灰度图1
	unsigned char* show2_p = (unsigned char*)show2_image.GetBits();//调试参数用灰度图2
	unsigned char* pDst = (unsigned char*)image1.GetBits();//用来标记面积矩阵的！
	unsigned char* final = (unsigned char*)final_image.GetBits();//最终染色图

	unsigned char* test = (unsigned char*)test_image.GetBits();

	//图像预处理
	unsigned char** orig = new unsigned char* [rows];     //原始图像
	for (i = 0; i < rows; i++)
	{
		orig[i] = new unsigned char[cols];
	}
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			orig[i][j] = p[cols * i + j]; //像素赋值
		}
	}

	cols = cols - 200;

	unsigned char** orig0 = new unsigned char* [rows];
	for (i = 0; i < rows; i++)
	{
		orig0[i] = new unsigned char[cols];
	}

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			orig0[i][j] = orig[i][j + 100];
		}
	}
	unsigned char** orig1 = new unsigned char* [rows];
	for (i = 0; i < rows; i++)
	{
		orig1[i] = new unsigned char[cols];
	}
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			orig1[i][j] = orig0[i][j];
		}
	}
	binary(orig0, orig1, rows, cols, 127);
	////////////////////////////////////////////////////////////

	unsigned char** orig21 = new unsigned char* [rows];
	for (i = 0; i < rows; i++)
	{
		orig21[i] = new unsigned char[cols];
	}
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			orig21[i][j] = orig1[i][j];
		}
	}
	unsigned char** orig22 = new unsigned char* [rows];
	for (i = 0; i < rows; i++)
	{
		orig22[i] = new unsigned char[cols];
	}
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			orig22[i][j] = orig1[i][j];
		}
	}
	unsigned char** origA = new unsigned char* [rows];
	for (i = 0; i < rows; i++)
	{
		origA[i] = new unsigned char[cols];
	}
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			origA[i][j] = 0;
		}
	}
	//通过多次腐蚀将绿豆面积减小

	imerode(orig1, orig22, rows, cols);
	imerode(orig22, origA, rows, cols);
	/*	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			test[i * cols + j] = origA[i][j];
		}
	}*/


	//在这里创造一个存放边缘的图像
	/*unsigned char** imagedge = new unsigned char* [rows];
	for (i = 0; i < rows; i++)
	{
		imagedge[i] = new unsigned char[cols];
	}*/
	/*for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (orig1[i][j] != orig22[i][j])
			{
                   imagedge[i][j] = 255;  //获得边界
				   //test[i * cols + j] = imagedge[i][j];
			}
			else
			{
				imagedge[i][j] = 0;
				//test[i * cols + j] = imagedge[i][j];
			}
			
		}
	}*/
	

	unsigned char* yuanxinx = new unsigned char [rows+1];
	unsigned char* yuanxiny = new unsigned char [cols+1];
	//unsigned char* banjing = new unsigned char [200];
	// unsigned char* findnum = new unsigned char[2];
	//houghcircle(origA, yuanxinx,yuanxiny, banjing,findnum,rows,cols);
	int maxr = 45;                            //寻找半径范围
	int minr = 10;
	int stepr = 1;
	float percent = 0.6;                             //此参数为经过测试得出的效果最好的百分比
	float pi = 3.1415926535898;
	float stepa = pi / 180.0;
	int r = int(((maxr - minr) / stepr) + 0.5) + 1;      //半径变化范围
	int angle = int(2 * pi / stepa + 0.5);
	int z;
	//代替houghspace=zeros(m,n,r);
	unsigned char*** houghspace = new unsigned char**[rows];
	for ( i = 0; i < rows; i++) 
	{
		houghspace[i] = new unsigned char*[cols];
	}
	for ( i = 0; i < rows; i++)
	{
			for (j = 0; j < cols; j++)
			{
				houghspace[i][j] = new unsigned char[r];
			}
	}
	for ( i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			for (z= 0; z< r; z++)
			{
				houghspace[i][j][z] = 0;
			}
		}
	}

	int a, b, s;
	int af = 0, bf = 0;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (origA[i][j] >127)
			{
				for (s = 0; s < r; s++)
				{
					for (z = 0; z < angle; z++)
					{
						a = int((i - (minr + s * stepr) * cos((z + 1) * stepa)) + 0.5);
						b = int((j - (minr + s * stepr) * sin((z + 1) * stepa)) + 0.5);
						if (a == af && b == bf)
						{
							continue;
						}
						else
						{
							af = a;
							bf = b;						
							if (a >= 0 && a < rows && b >= 0 && b < cols) // 判断获得的圆心坐标是否在霍夫空间内且有效
							{
									houghspace[a][b][s] = houghspace[a][b][s] + 1;
							}
						}
						
					}
				}
			}
		}
	}

	int maxpower = 0;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			for (s = 0; s < r; s++)
			{
				if (houghspace[i][j][s] > maxpower)
				{
					maxpower = houghspace[i][j][s];
				}
			}
		}
	}

	//
	unsigned char* rmax = new unsigned char[r+1];

	for (s = 0; s < r; s++)
	{
		rmax[s] = 0;
	}

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			for (s = 0; s < r; s++)
			{
				if (houghspace[i][j][s] > rmax[s])
				{
					rmax[s] = houghspace[i][j][s];
				}
			}
		}
	}
	delete[] rmax;

	float par2 = float(maxpower) * percent;
	float par3 = float(maxpower) * 0.85;
	//
	int tim = 0;
	float rlonger = 3;
	int r2;
	int tim2 = 0;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			for (s = 0; s < r; s++)
			{
				if (orig22[i][j] == 255)
				{
					if (houghspace[i][j][s] > par2)
					{
						tim2 = tim2 + 1;
						if (houghspace[i][j][s] > par3)
						{
							origA[i][j] = 255;
							yuanxinx[tim2] = i;
							yuanxiny[tim2] = j;
							//banjing[tim2] = minr + s * stepr;
						}
						
						r2 = 1.0 * (minr + s * stepr); //覆盖圆半径
						if (1)
						{

							for (a = max(0, int(i - r2 + 0.5)); a < min(rows, int(i + r2 + 0.5)); a++)
							{
								for (b = max(0, int(j - r2 + 0.5)); b < min(cols, int(j + r2 + 0.5)); b++)
								{

									if ((i - a) * (i - a) + (j - b) * (j - b) <= r2 * r2)
									{
										orig22[a][b] = 255;

									}
								}
							}
						}
					}

				}

			}
		}
	}
	unsigned char* repe = new unsigned char[tim2+1];
	for (s = 0; s < tim2; s++)
	{
		repe[s] = 0;
	}

	for (i = 0; i < tim2; i++)
	{
		for (j = i+1; j < tim2; j++)
		{
			if (repe[j] == 0)
			{
				if ((yuanxinx[i] - yuanxinx[j]) *(yuanxinx[i] - yuanxinx[j]) + (yuanxiny[i] - yuanxiny[j]) * (yuanxiny[i] - yuanxiny[j]) <=100)
				{
					repe[j] = 1;
                 }
			}
		}
	}
	int timfinal = 0;
	for (i = 0; i < tim2; i++)
	{
		if (repe[i] == 0)
		{
			timfinal++;
		}
	}
	LuoMu = timfinal;
	/*	LuoMu = tim2;
	int tim = 0;
	float rlonger = 3;
	int r2;
	for (tim = 0; tim < LuoMu; tim++)
	{
		r2 = 300; //覆盖圆半径
		for (i = max(0, yuanxinx[tim] - r2); i <= min(rows, yuanxinx[tim] + r2); i++)
		{
			for (j = max(0, yuanxiny[tim] - r2); j <= min(cols, yuanxiny[tim] + r2); j++)
			{

				if ((i - yuanxinx[tim]) * (i - yuanxinx[tim]) + (j - yuanxiny[tim]) * (j - yuanxiny[tim]) <= r2 * r2)
				{
					orig22[i][j] = 255;
				}
			}
		}
	}*/


	//覆盖螺母目标
	
	/*for (i = 0; i < rows; i++)
	{
		delete[] houghspace[i];
	}
	delete[] houghspace;*/
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			delete[] houghspace[i][j];
		}
		delete[] houghspace[i];
	}
	delete[] houghspace;
	delete[] orig;
	
	delete[] repe;

	delete[] yuanxinx;
	delete[] yuanxiny;

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			test[i * cols + j] = origA[i][j];
		}
	}
	//完成螺母计数和消除螺母，回到分类黄豆绿豆问题
	//
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			orig1[i][j] = orig22[i][j];
		}
	}

	//这里需要加深腐蚀
	imerode(orig22, orig21, rows, cols);
	imerode(orig21, orig22, rows, cols);
	imerode(orig22, orig21, rows, cols);
	imerode(orig21, orig22, rows, cols);
	imerode(orig22, orig21, rows, cols);
	imerode(orig21, orig22, rows, cols);
	imerode(orig22, orig21, rows, cols);
	/////////////////////////////////////////////
		//将之前腐蚀后留下的边缘痕迹通过距离变换消除
	unsigned char** orig3 = new unsigned char* [rows];
	for (i = 0; i < rows; i++)
	{
		orig3[i] = new unsigned char[cols];
	}
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			orig3[i][j] = orig21[i][j];
		}
	}
	distancetransform(orig22, orig3, rows, cols);
	/////////////////////////////
	unsigned char** orig4 = new unsigned char* [rows];
	for (i = 0; i < rows; i++)
	{
		orig4[i] = new unsigned char[cols];
	}
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			orig4[i][j] = orig3[i][j];
		}
	}
	binary(orig3, orig4, rows, cols, 170);  //进行阈值分割
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			show_p[i * cols + j] = orig4[i][j];
		}
	}
	/////////////////////////////////////////////////////////////////////
		//面积统计
	unsigned char** orig6 = new unsigned char* [rows];
	for (i = 0; i < rows; i++)
	{
		orig6[i] = new unsigned char[cols];
	}
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			orig6[i][j] = 0;
		}
	}
	findRegion(orig4, orig6, rows, cols, region);    //区域生长，统计区域
	//进行黄豆分类计数
	for (i = 1; i < 100; i++)
	{
		if (region[i] != 0)     //如果还有区域
		{
			if (region[i] > 100)    //取出腐蚀操作产生的单个像素点的影响
				HuangDou++;
		}
		else
			break;
	}
	imdilate(orig4, orig3, rows, cols);    //膨胀
	imdilate(orig3, orig4, rows, cols);
	imdilate(orig4, orig3, rows, cols);
	imdilate(orig3, orig4, rows, cols);
	imdilate(orig4, orig3, rows, cols);
	findRegion(orig3, orig6, rows, cols, region);    //区域生长，统计连通区域
	//进行黄豆分类计数
	for (i = 1; i < 100; i++)
	{
		if (region[i] != 0)     //如果还有区域
		{
			if (region[i] > 50)    //取出腐蚀操作产生的单个像素点的影响
				Lep++;
		}
		else
			break;
	}

	imdilate(orig4, orig3, rows, cols);
	imdilate(orig3, orig4, rows, cols);
	/// 前置染色
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			final[(cols * i + j) * 3] = 0;
			final[(cols * i + j) * 3 + 1] = 100;
			final[(cols * i + j) * 3 + 2] = 255;//幕布为橙色
		}
	}
	//膨胀后的黄豆，呈棱形 显示为黄色
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (orig4[i][j] == 255)
			{
				final[(cols * i + j) * 3] = 0;
				final[(cols * i + j) * 3 + 1] = 255;
				final[(cols * i + j) * 3 + 2] = 255;//
			}
		}
	}
	///////////////////////////
	//膨胀后的图像与原始图做差，得到黄豆边缘和绿豆图
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			orig3[i][j] = orig1[i][j] + orig4[i][j];
		}
	}
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (orig3[i][j] == 0)
			{
				final[(cols * i + j) * 3] = 0;
				final[(cols * i + j) * 3 + 1] = 127;
				final[(cols * i + j) * 3 + 2] = 127;//棕色
			}
		}
	}
	//绿豆处理
	imdilate(orig3, orig4, rows, cols);
	imdilate(orig4, orig3, rows, cols);
	imdilate(orig3, orig4, rows, cols);
	imdilate(orig4, orig3, rows, cols);
	////////////////
		//对图像进行多次腐蚀，距离变换+阈值分割，将黄豆边缘消除	
	distancetransform(orig3, orig4, rows, cols);
	binary(orig4, orig3, rows, cols, 80);

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (orig3[i][j] == 255)
			{
				final[(cols * i + j) * 3] = 0;
				final[(cols * i + j) * 3 + 1] = 255;
				final[(cols * i + j) * 3 + 2] = 0;//绿色
			}
		}
	}
	//将被腐蚀的绿豆进行膨胀，方便计数以及染色
//////////////////////////对绿豆进行计数///////////////
	number = 0;
	findRegion(orig4, orig6, rows, cols, regionl);  //区域生长  
	for (i = 1; i < 100; i++)
	{
		if (regionl[i] > 150)
			LvDou++;

	}
	/// //////////////////////////////////////////////////
	//释放堆栈
	for (i = 0; i < rows; i++)
	{
		delete[] orig[i];
	}
	delete[] orig;
	for (i = 0; i < rows; i++)
	{
		delete[] orig0[i];
	}
	delete[] orig0;
	for (i = 0; i < rows; i++)
	{
		delete[] orig1[i];
	}
	delete[] orig1;
	for (i = 0; i < rows; i++)
	{
		delete[] orig21[i];
	}
	delete[] orig21;
	for (i = 0; i < rows; i++)
	{
		delete[] orig22[i];
	}
	delete[] orig22;
	for (i = 0; i < rows; i++)
	{
		delete[] orig3[i];
	}
	delete[] orig3;
	for (i = 0; i < rows; i++)
	{
		delete[] orig4[i];
	}
	delete[] orig4;
	for (i = 0; i < rows; i++)
	{
		delete[] orig6[i];
	}
	delete[] orig6;
	//释放霍夫圆算法所需空间
	/*for (i = 0; i < rows; i++)
	{
		delete[] houghspace[i];
	}*/
	

	LvDou += (Lep - HuangDou);
	LvDou--;
	//在界面上显示绿豆与黄豆数目
	HuangDou ++;
	CString str1;
	str1.Format(_T("%d"), HuangDou);
	edit1.SetWindowTextW(str1);

	CString str2;
	str2.Format(_T("%d"), LvDou);
	edit2.SetWindowTextW(str2);

	CString str3;
	str3.Format(_T("%d"), LuoMu);
	edit3.SetWindowTextW(str3);
}

CdemoDlg::CdemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DEMO_DIALOG, pParent)
	, m_bRun(FALSE)
	, m_hCam(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT3, edit1);
	DDX_Control(pDX, IDC_EDIT4, edit2);
	DDX_Control(pDX, IDC_EDIT9, edit3);
	DDX_Control(pDX, IDC_EDIT1, name1);
	DDX_Control(pDX, IDC_EDIT2, name2);
	DDX_Control(pDX, IDC_EDIT5, name5);
	DDX_Control(pDX, IDC_EDIT6, name6);
	//DDX_Control(pDX, IDC_EDIT7, name7);
	DDX_Control(pDX, IDC_EDIT8, name8);
}

BEGIN_MESSAGE_MAP(CdemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OpenCam, &CdemoDlg::OnBnClickedOpencam)
	ON_BN_CLICKED(IDC_StartGrab, &CdemoDlg::OnBnClickedStartgrab)
	ON_BN_CLICKED(IDC_CloseCam, &CdemoDlg::OnBnClickedClosecam)

	//ON_BN_CLICKED(IDC_CloseCam2, &CdemoDlg::OnBnClickedClosecam2)
	//ON_BN_CLICKED(IDC_, &CdemoDlg::OnBnClickedStarttest)
	//ON_EN_CHANGE(IDC_EDIT5, &CdemoDlg::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT6, &CdemoDlg::OnEnChangeEdit6)
	//ON_STN_CLICKED(pic22, &CdemoDlg::OnStnClickedpic22)
END_MESSAGE_MAP()


// CdemoDlg 消息处理程序

BOOL CdemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	MVSTATUS_CODES r;
	r = MVInitLib();
	if (r != MVST_SUCCESS)
	{
		MessageBox(_T("函数库初始化失败！"), _T("提示"), MB_ICONWARNING);
		return TRUE;
	}
	r = MVUpdateCameraList();
	if (r != MVST_SUCCESS)
	{
		MessageBox(_T("查找连接计算机的相机失败！"), _T("提示"), MB_ICONWARNING);
		return TRUE;
	}
	GetDlgItem(IDC_OpenCam)->EnableWindow(true);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CdemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CdemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CdemoDlg::OnBnClickedOpencam()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCams = 0;
	MVGetNumOfCameras(&nCams);
	if (nCams == 0)
	{
		MessageBox(_T(" 没 有 找 到 相 机 , 请确认连接和相机 IP 设 置 "), _T(" 提 示 "), MB_ICONWARNING);
		return;
	}
	MVSTATUS_CODES r = MVOpenCamByIndex(0, &m_hCam);
	if (m_hCam == NULL)
	{
		if (r == MVST_ACCESS_DENIED)
			MessageBox(_T(" 无 法 打 开 相 机 ， 可 能 正 被 别 的 软 件 控 制 "), _T(" 提 示 "),
				MB_ICONWARNING);
		else
			MessageBox(_T("无法打开相机"), _T("提示"), MB_ICONWARNING);
		return;
	}
	
	//获取图像的长、宽、像素格式信息，以此进行图像初始化操作
	int w, h;
	MVGetWidth(m_hCam, &w);
	MVGetHeight(m_hCam, &h);
	MVGetPixelFormat(m_hCam, &m_PixelFormat);     //获取像素格式
	m_image.CreateByPixelFormat(w, h, m_PixelFormat);     //生成摄像机拍摄的原始图像
	temp_image.CreateByPixelFormat(w, h, PixelFormat_Mono8);    //生成灰度图像
	image1.CreateByPixelFormat(w-200, h, PixelFormat_Mono8);    //生成裁剪后的灰度图像
	show_image.CreateByPixelFormat(w-200, h, PixelFormat_Mono8);    //生成观察用的灰度图像
	show2_image.CreateByPixelFormat(w-200, h, PixelFormat_Mono8);    //生成观察用的灰度图像
	final_image.CreateByPixelFormat(w-200, h, PixelFormat_BayerRG8);    //生成最终的灰度图像
	test_image.CreateByPixelFormat(w - 200, h, PixelFormat_Mono8);    //生成最终的灰度图像
	GetDlgItem(IDC_OpenCam)->EnableWindow(false);
	GetDlgItem(IDC_StartGrab)->EnableWindow(true);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);
}


void CdemoDlg::DrawImage()
{
	CRect rct;
	GetDlgItem(pic)->GetClientRect(&rct);
	int dstW = rct.Width();
	int dstH = rct.Height();
	CDC* pDC = GetDlgItem(pic)->GetDC();
	{
		pDC->SetStretchBltMode(COLORONCOLOR);
		final_image.Draw(pDC->GetSafeHdc(), 0, 0, 800, 600);

	}
	ReleaseDC(pDC);
	GetDlgItem(pic11)->GetClientRect(&rct);
	dstW = rct.Width();
	dstH = rct.Height();
	pDC = GetDlgItem(pic11)->GetDC();
	{
		pDC->SetStretchBltMode(COLORONCOLOR);
		test_image.Draw(pDC->GetSafeHdc(), 0, 0, 800, 600);

	}
	ReleaseDC(pDC);
	/*
	GetDlgItem(pic22)->GetClientRect(&rct);
	dstW = rct.Width();
	dstH = rct.Height();
	pDC = GetDlgItem(pic22)->GetDC();
	{
		pDC->SetStretchBltMode(COLORONCOLOR);
		show_image.Draw(pDC->GetSafeHdc(), 0, 0, 550, 450);
	}
	ReleaseDC(pDC);
	*/

}


void CdemoDlg::DrawImage_test()
{
	CRect rct;
	GetDlgItem(pic)->GetClientRect(&rct);
	int dstW = rct.Width();
	int dstH = rct.Height();
	CDC* pDC = GetDlgItem(pic)->GetDC();
	{
		pDC->SetStretchBltMode(COLORONCOLOR);
		//final_image.Draw(pDC->GetSafeHdc(), 0, 0, 800, 600);
		show_image.Draw(pDC->GetSafeHdc(), 0, 0, 800, 600);
		//temp_image.Draw(pDC->GetSafeHdc(), 0, 0, 800, 600);
	}
	ReleaseDC(pDC);
}



//用于控制实时操作的函数，它表示每一步（即每一次采集）需要处理的内容
int CdemoDlg::OnStreamCB(MV_IMAGE_INFO* pInfo)
{
	//根据图像初始化信息得到图像
	MVInfo2Image(m_hCam, pInfo, &m_image);
	MVInfo2Image(m_hCam, pInfo, &temp_image);
	
	//在屏幕上显示“黄豆”、“绿豆”
	name1.SetWindowTextW(_T("黄豆"));
	name2.SetWindowTextW(_T("绿豆"));
	name5.SetWindowTextW(_T("最终染色图"));
	name6.SetWindowTextW(_T("中间测试图"));
	//name7.SetWindowTextW(_T("截取绿豆效果图"));
	name8.SetWindowTextW(_T("螺母"));

	ProcessImage();     //进行图像处理
	DrawImage();      //显示处理后图像
	return 0;
}

void CdemoDlg::OnBnClickedStarttest()
{
	// TODO: 在此添加控件通知处理程序代码

}

int CdemoDlg::OnStreamCB_test(MV_IMAGE_INFO* pInfo)
{
	//根据图像初始化信息得到图像
	//MVInfo2Image(m_hCam, pInfo, &m_image);
	//MVInfo2Image(m_hCam, pInfo, &temp_image);

	//在屏幕上显示“黄豆”、“绿豆”
	//name1.SetWindowTextW(_T("黄豆"));
	//name2.SetWindowTextW(_T("绿豆"));
	//ProcessImage();     //进行图像处理
	//DrawImage_test();      //显示处理后图像
	return 0;
}



int __stdcall StreamCB(MV_IMAGE_INFO* pInfo, ULONG_PTR nUserVal)
{
	CdemoDlg* pDlg = (CdemoDlg*)nUserVal;
	return (pDlg->OnStreamCB(pInfo));
}
int __stdcall StreamCB_test(MV_IMAGE_INFO* pInfo, ULONG_PTR nUserVal)
{
	CdemoDlg* pDlg = (CdemoDlg*)nUserVal;
	return (pDlg->OnStreamCB_test(pInfo));
}

void CdemoDlg::OnBnClickedStartgrab()
{
	// TODO: 在此添加控件通知处理程序代码
	TriggerModeEnums enumMode;
	MVGetTriggerMode(m_hCam, &enumMode);
	if (enumMode != TriggerMode_Off)
	{
		MVSetTriggerMode(m_hCam, TriggerMode_Off);
		Sleep(100);
	}
	MVStartGrab(m_hCam, StreamCB, (ULONG_PTR)this);
	m_bRun = true;
	GetDlgItem(IDC_OpenCam)->EnableWindow(false);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(true);
}


void CdemoDlg::OnClose()
{
	if (m_bRun != false)
	{
		MVStopGrab(m_hCam);
	}
	MVTerminateLib();
	CDialog::OnClose();
}

void CdemoDlg::OnBnClickedClosecam()
{
	// TODO: 在此添加控件通知处理程序代码
	MVStopGrab(m_hCam);
	MVCloseCam(m_hCam);
	m_bRun = false;
	GetDlgItem(IDC_OpenCam)->EnableWindow(true);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);
}






void binary(unsigned char** grayimage, unsigned char** binaryimage, int rows, int cols, int th)
{
	int i = 0, j = 0;

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (grayimage[i][j] >= th)
			{
				binaryimage[i][j] = 255;      //如果灰度值大于阈值，置255
			}
			else
			{
				binaryimage[i][j] = 0;      //如果灰度值小于阈值，置0
			}
		}
	}
}



void bimerode(unsigned char** image, unsigned char** result_image, int rows, int cols)
{
	int i = 0, j = 0;

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (i - 2 >= 0 && i + 2 < rows && j - 2 >= 0 && j + 2 < cols)
			{
				if (image[i][j] == 0)
				{
					if (image[i + 1][j] == 0  && image[i][j + 1] == 0 &&
					      image[i - 1][j] == 0  &&image[i][j - 1] == 0 )
						result_image[i][j] = 0;
					else
						result_image[i][j] = 255;
				}
			}
		}
	}
}


void imerode(unsigned char** image, unsigned char** result_image, int rows, int cols)
{
	int i = 0, j = 0;

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (i - 2 >= 0 && i + 2 < rows && j - 2 >= 0 && j + 2 < cols)    
			{
				if (image[i][j] == 0)
				{
					if (image[i + 1][j] == 0 && image[i + 2][j] == 0 && image[i][j + 1] == 0 &&
						image[i][j + 2] == 0 && image[i - 1][j] == 0 && image[i - 2][j] == 0 &&
						image[i][j - 1] == 0 && image[i][j - 2] == 0 && image[i + 1][j + 1] == 0 &&
						image[i + 1][j - 1] == 0 && image[i - 1][j + 1] == 0 && image[i - 1][j - 1] == 0)
						result_image[i][j] = 0;
					else
						result_image[i][j] = 255;      
				}
			}
		}
	}
}

void imerode_w(unsigned char** image, unsigned char** result_image, int rows, int cols)
{
	int i = 0, j = 0;

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (i - 2 >= 0 && i + 2 < rows && j - 2 >= 0 && j + 2 < cols)   
			{
				if (image[i][j] == 0)
				{
					if (image[i + 1][j] == 255 && image[i + 2][j] == 255 && image[i][j + 1] == 255 &&
						image[i][j + 2] == 255 && image[i - 1][j] == 255 && image[i - 2][j] == 255 &&
						image[i][j - 1] == 255 && image[i][j - 2] == 255 && image[i + 1][j + 1] == 255 &&
						image[i + 1][j - 1] == 255 && image[i - 1][j + 1] == 255 && image[i - 1][j - 1] == 255)
						result_image[i][j] = 255;
					else
						result_image[i][j] = 0;      
				}
			}
		}
	}
}

void distancetransform(unsigned char** image, unsigned char** result_image, int rows, int cols)
{
	int i = 0, j = 0;
	int d[5] = { 0 };
	int min = 0;


	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (image[i][j] == 0)
				result_image[i][j] = 255;
			if (image[i][j] == 255)
				result_image[i][j] = 0;
		}
	}


	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (i - 1 >= 0 && j - 1 >= 0 && j + 1 < cols)
			{
				if (result_image[i][j] != 0)
				{
					d[0] = result_image[i][j];
					d[1] = 17 + result_image[i][j - 1];
					d[2] = 25 + result_image[i - 1][j - 1];
					d[3] = 17 + result_image[i - 1][j];
					d[4] = 25 + result_image[i - 1][j + 1];   
					min = d[0];
					for (int k = 1; k <= 4; k++)
					{
						if (d[k] < min)
							min = d[k];     
					}
					result_image[i][j] = min;     
				}
			}
		}
	}


	for (i = rows - 1; i >= 0; i--)
	{
		for (j = cols - 1; j >= 0; j--)
		{
			if (i + 1 < rows && j - 1 >= 0 && j + 1 < cols)
			{
				if (result_image[i][j] != 0)
				{
					d[0] = result_image[i][j];
					d[1] = 17 + result_image[i][j + 1];
					d[2] = 25 + result_image[i + 1][j + 1];
					d[3] = 17 + result_image[i + 1][j];
					d[4] = 25 + result_image[i + 1][j - 1];
					min = d[0];
					for (int k = 1; k <= 4; k++)
					{
						if (d[k] < min)
							min = d[k];
					}
					result_image[i][j] = min;
				}
			}
		}
	}
}


void imdilate(unsigned char** image, unsigned char** result_image, int rows, int cols)
{
	int i = 0, j = 0;

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (i - 2 >= 0 && i + 2 < rows && j - 2 >= 0 && j + 2 < cols)    
			{
				if (image[i + 1][j] == 255 || image[i + 2][j] == 255 || image[i][j + 1] == 255 ||
					image[i][j + 2] == 255 || image[i - 1][j] == 255 || image[i - 2][j] == 255 ||
					image[i][j - 1] == 255 || image[i][j - 2] == 255 || image[i + 1][j + 1] == 255 ||
					image[i + 1][j - 1] == 255 || image[i - 1][j + 1] == 255 || image[i - 1][j - 1] == 255)
					result_image[i][j] = 255;
				else
					result_image[i][j] = 0;     
			}
		}
	}
}


void PushQueue(Queue* queue, int rows_data, int cols_data)
{
	QNode* p = NULL;
	p = (QNode*)malloc(sizeof(QNode));  
	p->rows_data = rows_data;
	p->cols_data = cols_data;      

	if (queue->first == NULL)
	{
		queue->first = p;
		queue->last = p;
		p->next = NULL;
	}
	else
	{
		p->next = NULL;
		queue->last->next = p;
		queue->last = p;
	}
}


void houghcircle(unsigned char** imagedge, unsigned char* yuanxinx, unsigned char* yuanxiny, unsigned char* banjing, unsigned char* findnum, int rows, int cols)
{
	int maxr = 30;                            //寻找半径范围
	int minr = 3;
	int stepr = 1;
	float percent = 0.8;                             //此参数为经过测试得出的效果最好的百分比
	float pi = 3.1415926535898;
	float stepa = pi / 180.0;
	int r = int(((maxr - minr) / stepr) + 0.5) + 1;      //半径变化范围
	int angle = int(2 * pi / stepa + 0.5);
	int i, j, z;
	//代替houghspace=zeros(m,n,r);
	char*** houghspace = (char***)malloc(sizeof(char**) * rows);
	for (int i = 0; i < rows; i++) {
		houghspace[i] = (char**)malloc(sizeof(char*) * cols);
		for (int j = 0; j < cols; j++) {
			houghspace[i][j] = (char*)malloc(sizeof(char) * r);
			memset(houghspace[i][j], 0, sizeof(char) * r);
		}
		
	}

	int a,b,s;
	int zhongjianzhi;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (imagedge[i][j] == 255)
			{
				for (s = 0; s < r; s++)
				{
					for (z = 0; z < angle; z++)
					{
						a = int((i - (minr + s * stepr) * cos((z + 1) * stepa)) + 0.5);
						b = int((j - (minr + s * stepr) * sin((z + 1) * stepa)) + 0.5);
						if (a >= 0 && a < rows && b >= 0 && b < cols) // 判断获得的圆心坐标是否在霍夫空间内且有效
						{
							zhongjianzhi=houghspace[a][b][s];
							zhongjianzhi = zhongjianzhi + 1;
							houghspace[a][b][s] = zhongjianzhi;
						}
					}
				}
			}
		}
	}

	//替代par=max(max(max(houghspace)))；
	int maxpower=0;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
				for (s = 0; s < r; s++)
				{
					 if (houghspace[i][j][s]>maxpower)
					 {
						 maxpower = houghspace[i][j][s];
					 }
				}
		}
	}
	float par2 = float(maxpower) * percent;

	//
	int tim2 = 0;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			for (s = 0; s < r; s++)
			{
				if (houghspace[i][j][s] > par2)
				{
					yuanxinx[tim2] = i;
					yuanxiny[tim2] = j;
					banjing[tim2] = minr + s * stepr;
					tim2 = tim2 + 1;
				}
			}
		}
	}
	findnum[0] = tim2;
}


int PopQueue(Queue* queue, int* data)
{
	QNode* p = NULL;
	if (queue->first == NULL)
	{
		return -1;   
	}
	p = queue->first;
	data[0] = p->rows_data;
	data[1] = p->cols_data;     

	if (queue->first->next == NULL)
	{
		queue->first = NULL;
		queue->last = NULL;
	}
	else
	{
		queue->first = p->next;
	}
	free(p);
	return 1;
}



void lh_SearchNeighbour(unsigned char** image, unsigned char** temp_image, int rows, int cols,
	int rows_data, int cols_data, Queue* queue, int* area)
{
	if (rows_data - 1 >= 0)    
	{
		if (image[rows_data - 1][cols_data] != 0 && temp_image[rows_data - 1][cols_data] == 0)
		{
			PushQueue(queue, rows_data - 1, cols_data);     
			temp_image[rows_data - 1][cols_data] = 1;    
			(*area)++;      
		}
	}
	if (rows_data + 1 < rows)      
	{
		if (image[rows_data + 1][cols_data] != 0 && temp_image[rows_data + 1][cols_data] == 0)
		{
			PushQueue(queue, rows_data + 1, cols_data);
			temp_image[rows_data + 1][cols_data] = 1;
			(*area)++;
		}
	}
	if (cols_data - 1 >= 0)     
	{
		if (image[rows_data][cols_data - 1] != 0 && temp_image[rows_data][cols_data - 1] == 0)
		{
			PushQueue(queue, rows_data, cols_data - 1);
			temp_image[rows_data][cols_data - 1] = 1;
			(*area)++;
		}
	}
	if (cols_data + 1 < cols)    
	{
		if (image[rows_data][cols_data + 1] != 0 && temp_image[rows_data][cols_data + 1] == 0)
		{
			PushQueue(queue, rows_data, cols_data + 1);
			temp_image[rows_data][cols_data + 1] = 1;
			(*area)++;
		}
	}
}


void findRegion(unsigned char** image, unsigned char** temp_image, int rows, int cols, int* region)
{
	Queue* queue = NULL;
	queue = (Queue*)malloc(sizeof(Queue));     
	queue->first = NULL;
	queue->last = NULL;    

	int i = 0, j = 0;
	int area = 0;
	int number = 0;
	int* p = &area;
	int data[2] = { 0 };
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (image[i][j] != 0 && temp_image[i][j] == 0)   
			{
				area = 0;
				PushQueue(queue, i, j);
				temp_image[i][j] = 1;
				area++;
				while (queue->first != NULL)    
				{
					PopQueue(queue, data);
					lh_SearchNeighbour(image, temp_image, rows, cols, data[0], data[1], queue, p);
				}
				if (area > 10)
				{
					region[number] = area;    
					number++;
				}
			}
		}
	}
	free(queue);    
}


void CdemoDlg::OnEnChangeEdit6()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
