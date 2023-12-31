﻿
// demoDlg.h: 头文件
//

#pragma once
#include "MVGigE.h"
#include "MVImage.h"

// CdemoDlg 对话框
class CdemoDlg : public CDialogEx
{
	// 构造
public:
	CdemoDlg(CWnd* pParent = nullptr);	// 标准构造函数


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	//定义相机句柄对象
	HANDLE m_hCam;
	//定义图像处理对象
	MVImage m_image;

	MVImage temp_image;
	MVImage show_image;//观察用图像1
	MVImage show2_image;//观察用图像2
	MVImage image1;    //裁剪后的图像
	MVImage final_image;    //最终显示的图像

	MVImage test_image;



	//定义像素格式对象
	MV_PixelFormatEnums m_PixelFormat;
	//采集标识
	bool m_bRun;
	//通过回调函数获取图像数据信息
	int OnStreamCB(MV_IMAGE_INFO* pInfo);
	int OnStreamCB_test(MV_IMAGE_INFO* pInfo);
	//画图
	void DrawImage();
	void DrawImage_test();
	//关闭相机、释放资源
	void OnClose();
	void ProcessImage();

	afx_msg void OnBnClickedOpencam();
	afx_msg void OnBnClickedStartgrab();
	afx_msg void OnBnClickedClosecam();
	//afx_msg void OnEnChangeEdit3();
	CEdit edit1;
	CEdit edit2;
	CEdit edit3;
	//afx_msg void OnEnChangeEdit4();
	CEdit name1;
	CEdit name2;
	CEdit name5;
	CEdit name6;
	//CEdit name7;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit3();

	afx_msg void OnEnChangeEdit9();

	afx_msg void OnBnClickedClosecam2();
	afx_msg void OnBnClickedStarttest();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit6();

	CEdit name8;
	afx_msg void OnEnChangeEdit8();
	//afx_msg void OnStnClickedpic22();
};

//队列节点结构体定义
typedef struct QNode {
	int rows_data;
	int cols_data;
	struct QNode* next;
}QNode;

//队列结构体定义
typedef struct Queue {
	struct QNode* first;
	struct QNode* last;
}Queue;

void binary(unsigned char** grayimage, unsigned char** binaryimage, int rows, int cols, int th);
void imerode(unsigned char** image, unsigned char** result_image, int rows, int cols);
void imerode_w(unsigned char** image, unsigned char** result_image, int rows, int cols);
void imdilate(unsigned char** image, unsigned char** result_image, int rows, int cols);
void distancetransform(unsigned char** image, unsigned char** result_image, int rows, int cols);
void PushQueue(Queue* queue, int rows_data, int cols_data);
int PopQueue(Queue* queue, int* data);
void lh_SearchNeighbour(unsigned char** image, unsigned char** temp_image, int rows, int cols,
	int rows_data, int cols_data, Queue* queue, int* area);
void findRegion(unsigned char** image, unsigned char** temp_image, int rows, int cols, int* region);
void houghcircle(unsigned char** imagedge, unsigned char* yuanxinx, unsigned char* yuanxiny, unsigned char* banjing, unsigned char* findnum, int rows, int cols);
void bimerode(unsigned char** image, unsigned char** result_image, int rows, int cols);