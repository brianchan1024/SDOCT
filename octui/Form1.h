#pragma once


#using "..\x64\Release\ControllerWrapper.dll"
#include "imgform.h"
#include "gallery.h"
#include "fft_util.h"
#include <time.h>
#include <iostream>
#include <string>
#include <vector>
#include <cliext/adapter>
#include <cliext/vector>

#include "fftw3.h"
#include <math.h>
#include <assert.h>
#include <random>

using namespace std;
using namespace System;
using namespace System::IO;

namespace octui {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Reflection;

	using namespace System::Collections::Generic;

	typedef unsigned int uInt_32;

	enum WorkingModel_e
	{
		IDEL = 0,
		BACKGROUND,
		ACQUIRE,
		PREVIEWXZ,
		PREVIEWYZ,
		FUNDUS_VIEW,
		GALLERY
	};

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			this->previewUpdateWorker = gcnew System::ComponentModel::BackgroundWorker;
			this->fundusUpdateWorker = gcnew System::ComponentModel::BackgroundWorker;
			this->acquireWorker = gcnew System::ComponentModel::BackgroundWorker;
			initBackgoundWorker();

			center1 =  200;
			center2 = 500;
			centerAreaIdx = 0;
			centerMoveIdx = 0;

			fundusXoff_tb->Text = Convert::ToString(0);
			fundusYoff_tb->Text = Convert::ToString(0);

			//initData();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TabPage^  tabPage_Parameter;
	private: System::Windows::Forms::TabPage^  tabPage_main;
	protected: 

	protected: 


	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::GroupBox^  groupBox4;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::PictureBox^  imageXZ;
	private: System::Windows::Forms::PictureBox^  imageYZ;



	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label10;

	private: System::Windows::Forms::Label^  label9;


	private: System::Windows::Forms::Button^  UserApplyBtn;

	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  yoffsettb;

	private: System::Windows::Forms::TextBox^  xoffsettb;

	private: System::Windows::Forms::TextBox^  yrangetb;

	private: System::Windows::Forms::TextBox^  xrangetb;

	private: System::Windows::Forms::TextBox^  NoBcgrstb;

	private: System::Windows::Forms::TextBox^  NoBstb;
private: System::Windows::Forms::TextBox^  NoAstb;




	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Button^  EndBtn;

	private: System::Windows::Forms::Button^  PreviewBtn;
	private: System::Windows::Forms::Button^  AcquireBtn;



	private: System::Windows::Forms::Button^  RefreshBcgrBtn;



	private: System::Windows::Forms::GroupBox^  groupBox9;


	private: System::Windows::Forms::GroupBox^  groupBox6;
	private: System::Windows::Forms::GroupBox^  groupBox5;
	private: System::Windows::Forms::Button^  FundusRestBtn;

	private: System::Windows::Forms::Button^  FundusStartBtn;
	private: System::Windows::Forms::TextBox^  fundusYoff_tb;








	private: System::Windows::Forms::Label^  label14;
private: System::Windows::Forms::TextBox^  fundusXoff_tb;


	private: System::Windows::Forms::Label^  label13;
private: System::Windows::Forms::PictureBox^  imagefundus;


	private: System::Windows::Forms::Button^  DataProcApplyBtn;

	private: System::Windows::Forms::TextBox^  gaussiantb;
	private: System::Windows::Forms::TextBox^  lambdaktb;
	private: System::Windows::Forms::TextBox^  lambda0tb;

	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::Label^  label21;
	private: System::Windows::Forms::Label^  label20;
	private: System::Windows::Forms::Label^  label19;
	private: System::Windows::Forms::Label^  label18;
	private: System::Windows::Forms::TextBox^  yfactortb;
	private: System::Windows::Forms::TextBox^  xfactortb;
	private: System::Windows::Forms::TextBox^  yvoltagetb;
	private: System::Windows::Forms::TextBox^  xvoltagetb;
	private: System::Windows::Forms::Label^  label29;

	private: System::Windows::Forms::TextBox^  preExposuretb;

	private: System::Windows::Forms::Button^  ScannerApplyBtn;
	private: System::Windows::Forms::TabPage^  tabPage_dispersion;

	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  modeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveFileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  rawToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  processedToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  tiffToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  pngToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  bothToolStripMenuItem;

	private: System::Windows::Forms::Label^  label32;
	private: System::Windows::Forms::Label^  label31;
	private: System::Windows::Forms::Label^  label30;
	private: System::Windows::Forms::Label^  label28;
	private: System::Windows::Forms::Label^  label27;
	private: System::Windows::Forms::Label^  label26;
	private: System::Windows::Forms::TextBox^  grayLeveltb;
	private: System::Windows::Forms::TextBox^  alphatb;
	private: System::Windows::Forms::TextBox^  cropEndtb;
	private: System::Windows::Forms::TextBox^  cropStarttb;
	private: System::Windows::Forms::TextBox^  Bbatchtb;
	private: System::Windows::Forms::TextBox^  Abatchtb;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::Button^  ReduceYZBtn;

	private: System::Windows::Forms::Button^  ZoomYZBtn;

	private: System::Windows::Forms::Panel^  panel3;
	private: System::Windows::Forms::Button^  ReduceXZBtn;

	private: System::Windows::Forms::Button^  ZoomXZBtn;

	private: System::Windows::Forms::ToolStripMenuItem^  userToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  debugToolStripMenuItem;
	private: System::Windows::Forms::TextBox^  PathSavetb;
	private: System::Windows::Forms::TextBox^  IDtb;
	private: System::Windows::Forms::TextBox^  ForderPathtb;
	private: System::Windows::Forms::Timer^  PreviewUpdateTimer;

	private: System::Windows::Forms::TextBox^  NoPixeltb;

	private: System::Windows::Forms::Label^  label33;

	private: System::Windows::Forms::Label^  label22;
	private: System::Windows::Forms::TextBox^  waveformtb;
	private: System::Windows::Forms::Label^  label23;
	private: System::Windows::Forms::Label^  label24;
	private: System::Windows::Forms::TextBox^  dacratetb;
	private: System::Windows::Forms::Label^  label25;

	private: System::Windows::Forms::Timer^  SpectrumUpdateTimer;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  sp_chart;

	private: System::Windows::Forms::TextBox^  channeltb;

	private: System::Windows::Forms::TextBox^  grabertb;

	private: System::ComponentModel::BackgroundWorker^ previewUpdateWorker;
	private: System::ComponentModel::BackgroundWorker^ fundusUpdateWorker;
	private: System::ComponentModel::BackgroundWorker^ acquireWorker;

	private: System::Windows::Forms::Button^  resetDispersion;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  dp_chart;
	private: System::Windows::Forms::Button^  center1_m;
	private: System::Windows::Forms::Label^  label34;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  center1_p;
	private: System::Windows::Forms::Button^  center2_p;
	private: System::Windows::Forms::Button^  center2_m;
	private: System::Windows::Forms::Button^  center2_p_click;
	private: System::Windows::Forms::TextBox^  center2_tb;
	private: System::Windows::Forms::TextBox^  center1_tb;
	private: System::Windows::Forms::TextBox^  delta_x_tb;
	private: System::Windows::Forms::Label^  label37;
	private: System::Windows::Forms::TextBox^  dp_max2_tb;
	private: System::Windows::Forms::TextBox^  dp_max1_tb;
	private: System::Windows::Forms::Label^  label36;
	private: System::Windows::Forms::Label^  label35;
	private: System::ComponentModel::IContainer^  components;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


		//user-defined variable

		// variable used to active controller @brian params
		WorkingModel_e m_eWorkingMode;
		bool initParamsDone;
		bool initControllerDone;
		bool initBackgroundDone;

		bool m_bPreview;
		bool m_bAcquire;
		ControllerWrapper::ControllerAccess^ controller;
		
		int returnValue;
		float fXMaxV;
		float fYMaxV;
		float fXScaleVM; // [v/mm]
		float fYScaleVM; // [v/mm]

		uInt_32 uiUpdateRate; //[points/sec]
		float fWaveEffi;
		System::String^ sPhyChannel;

		float fExposureTime;

		int iPreviewAScan;
		int iBackgroundAScan;
		int iPreviewAScanBatch;
		int iFundusViewAScan;

		// scanpattern params...
		float fXRangeV;
		float fYRangeV;
		float fXOffsetV;
		float fYOffsetV;

		uInt_32 uiNumPixel;
		uInt_32 uiNumAscan;
		uInt_32 uiNumBscan;
		uInt_32 uiNumBcgrFrame; // background frames
		uInt_32 uiNumBscanRepeat;

		// fundus view calibration
		float fFundusXOffsetV;
		float fFundusYOffsetV;

		//dac calinration
		System::String^ sXChanName;
		System::String^ sYChanName;
		System::String^ sSwitchChanName;
		bool bExterClked;
		System::String^ sExterClkChanName;
		unsigned long ulTrajGenRate;


		// data process calibration
		// data process calibration
		int batchFrames;					// batch frames. Lowers preview frequency. Lower numbers are fine ~2.
		int batchAscans;					// batch A-scans to increase occupancy on GPU. 10 or 20 gets decent results.
		int prevFreq;						// 1 means show every frame, 2 means show every other frame, 4 means show 1 in 4 frames, etc. Higher is better, has an decent impact on speed.
		bool reslice;						// reslice into enface view								

		int startPixel;
		int endPixel;
		float alpha;
		int grayLevel;
		bool interpLinear;

		bool dispModeGS; // not used as switch any more since hard coded in Contoller::resetDispersionParams, but should be kept to init GPU code
		int dispFrame;
		int dispTests;
		float dispA2;
		float dispA3;

		System::String^ fnamePhase;

		System::String^ fAcquireDataSet;
		System::String^ fFundusDataSet;
		System::String^ fbackground;
		System::String^ fpreviewxz;
		System::String^ fpreviewyz;
		System::String^ fpreviewxzImage;
		System::String^	fpreviewyzImage;
		System::String^	ffudnusImage;
		System::String^ fparams_conf;
		System::String^ fimg_dir;
		// for xz preview zoom and drag
		Bitmap^ xz_img;
		float xz_zoom_ratio;
		bool xz_mouse_down;
		Point xz_new_offset, xz_old_offset, xz_mouse_down_p;

		// for yz preview zoom and drag
		Bitmap^ yz_img;
		float yz_zoom_ratio;
		bool yz_mouse_down;
		Point yz_new_offset, yz_old_offset, yz_mouse_down_p;
		
		Bitmap^ fundus_img;

		// for background worker lock
		 bool acquireWorker_busy;
		 bool fundusUpdateWorker_busy;
		 bool previewUpdateWorker_busy;
		 // for spectrum dispersion display
		 int center1;
		 int center2;
		 int centerAreaIdx;
		 int centerMoveIdx;
private: System::Windows::Forms::Label^  label38;
private: System::Windows::Forms::TextBox^  bscanrepeat_tb;
private: System::Windows::Forms::Button^  fundusoffupdate_btn;

		 cliext::vector<int>^ spectrum_rawdata;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea2 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series5 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series6 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series7 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->tabPage_Parameter = (gcnew System::Windows::Forms::TabPage());
			this->groupBox9 = (gcnew System::Windows::Forms::GroupBox());
			this->label32 = (gcnew System::Windows::Forms::Label());
			this->label31 = (gcnew System::Windows::Forms::Label());
			this->label30 = (gcnew System::Windows::Forms::Label());
			this->label28 = (gcnew System::Windows::Forms::Label());
			this->label27 = (gcnew System::Windows::Forms::Label());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->grayLeveltb = (gcnew System::Windows::Forms::TextBox());
			this->alphatb = (gcnew System::Windows::Forms::TextBox());
			this->cropEndtb = (gcnew System::Windows::Forms::TextBox());
			this->cropStarttb = (gcnew System::Windows::Forms::TextBox());
			this->Bbatchtb = (gcnew System::Windows::Forms::TextBox());
			this->Abatchtb = (gcnew System::Windows::Forms::TextBox());
			this->DataProcApplyBtn = (gcnew System::Windows::Forms::Button());
			this->gaussiantb = (gcnew System::Windows::Forms::TextBox());
			this->lambdaktb = (gcnew System::Windows::Forms::TextBox());
			this->lambda0tb = (gcnew System::Windows::Forms::TextBox());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
			this->channeltb = (gcnew System::Windows::Forms::TextBox());
			this->preExposuretb = (gcnew System::Windows::Forms::TextBox());
			this->label29 = (gcnew System::Windows::Forms::Label());
			this->ScannerApplyBtn = (gcnew System::Windows::Forms::Button());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->waveformtb = (gcnew System::Windows::Forms::TextBox());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->dacratetb = (gcnew System::Windows::Forms::TextBox());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->yfactortb = (gcnew System::Windows::Forms::TextBox());
			this->grabertb = (gcnew System::Windows::Forms::TextBox());
			this->xfactortb = (gcnew System::Windows::Forms::TextBox());
			this->yvoltagetb = (gcnew System::Windows::Forms::TextBox());
			this->xvoltagetb = (gcnew System::Windows::Forms::TextBox());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->FundusRestBtn = (gcnew System::Windows::Forms::Button());
			this->FundusStartBtn = (gcnew System::Windows::Forms::Button());
			this->fundusYoff_tb = (gcnew System::Windows::Forms::TextBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->fundusXoff_tb = (gcnew System::Windows::Forms::TextBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->imagefundus = (gcnew System::Windows::Forms::PictureBox());
			this->tabPage_main = (gcnew System::Windows::Forms::TabPage());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->PathSavetb = (gcnew System::Windows::Forms::TextBox());
			this->IDtb = (gcnew System::Windows::Forms::TextBox());
			this->ForderPathtb = (gcnew System::Windows::Forms::TextBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->EndBtn = (gcnew System::Windows::Forms::Button());
			this->PreviewBtn = (gcnew System::Windows::Forms::Button());
			this->AcquireBtn = (gcnew System::Windows::Forms::Button());
			this->RefreshBcgrBtn = (gcnew System::Windows::Forms::Button());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->bscanrepeat_tb = (gcnew System::Windows::Forms::TextBox());
			this->label38 = (gcnew System::Windows::Forms::Label());
			this->NoPixeltb = (gcnew System::Windows::Forms::TextBox());
			this->label33 = (gcnew System::Windows::Forms::Label());
			this->UserApplyBtn = (gcnew System::Windows::Forms::Button());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->xrangetb = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->yoffsettb = (gcnew System::Windows::Forms::TextBox());
			this->xoffsettb = (gcnew System::Windows::Forms::TextBox());
			this->yrangetb = (gcnew System::Windows::Forms::TextBox());
			this->NoBcgrstb = (gcnew System::Windows::Forms::TextBox());
			this->NoBstb = (gcnew System::Windows::Forms::TextBox());
			this->NoAstb = (gcnew System::Windows::Forms::TextBox());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->ReduceYZBtn = (gcnew System::Windows::Forms::Button());
			this->ZoomYZBtn = (gcnew System::Windows::Forms::Button());
			this->imageYZ = (gcnew System::Windows::Forms::PictureBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->ReduceXZBtn = (gcnew System::Windows::Forms::Button());
			this->ZoomXZBtn = (gcnew System::Windows::Forms::Button());
			this->imageXZ = (gcnew System::Windows::Forms::PictureBox());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage_dispersion = (gcnew System::Windows::Forms::TabPage());
			this->delta_x_tb = (gcnew System::Windows::Forms::TextBox());
			this->label37 = (gcnew System::Windows::Forms::Label());
			this->dp_max2_tb = (gcnew System::Windows::Forms::TextBox());
			this->dp_max1_tb = (gcnew System::Windows::Forms::TextBox());
			this->label36 = (gcnew System::Windows::Forms::Label());
			this->label35 = (gcnew System::Windows::Forms::Label());
			this->center2_tb = (gcnew System::Windows::Forms::TextBox());
			this->center1_tb = (gcnew System::Windows::Forms::TextBox());
			this->center2_p = (gcnew System::Windows::Forms::Button());
			this->center2_m = (gcnew System::Windows::Forms::Button());
			this->center1_p = (gcnew System::Windows::Forms::Button());
			this->center1_m = (gcnew System::Windows::Forms::Button());
			this->label34 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->dp_chart = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->resetDispersion = (gcnew System::Windows::Forms::Button());
			this->sp_chart = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->modeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->userToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->debugToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->rawToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->processedToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tiffToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pngToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->bothToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->PreviewUpdateTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->SpectrumUpdateTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->fundusoffupdate_btn = (gcnew System::Windows::Forms::Button());
			this->tabPage_Parameter->SuspendLayout();
			this->groupBox9->SuspendLayout();
			this->groupBox6->SuspendLayout();
			this->groupBox5->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->imagefundus))->BeginInit();
			this->tabPage_main->SuspendLayout();
			this->panel1->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->panel2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->imageYZ))->BeginInit();
			this->groupBox2->SuspendLayout();
			this->panel3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->imageXZ))->BeginInit();
			this->tabControl1->SuspendLayout();
			this->tabPage_dispersion->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dp_chart))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->sp_chart))->BeginInit();
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// tabPage_Parameter
			// 
			this->tabPage_Parameter->BackColor = System::Drawing::Color::WhiteSmoke;
			this->tabPage_Parameter->Controls->Add(this->groupBox9);
			this->tabPage_Parameter->Controls->Add(this->groupBox6);
			this->tabPage_Parameter->Controls->Add(this->groupBox5);
			this->tabPage_Parameter->ForeColor = System::Drawing::SystemColors::ControlText;
			this->tabPage_Parameter->Location = System::Drawing::Point(4, 25);
			this->tabPage_Parameter->Name = L"tabPage_Parameter";
			this->tabPage_Parameter->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_Parameter->Size = System::Drawing::Size(1081, 737);
			this->tabPage_Parameter->TabIndex = 1;
			this->tabPage_Parameter->Text = L"Parameter";
			// 
			// groupBox9
			// 
			this->groupBox9->Controls->Add(this->label32);
			this->groupBox9->Controls->Add(this->label31);
			this->groupBox9->Controls->Add(this->label30);
			this->groupBox9->Controls->Add(this->label28);
			this->groupBox9->Controls->Add(this->label27);
			this->groupBox9->Controls->Add(this->label26);
			this->groupBox9->Controls->Add(this->grayLeveltb);
			this->groupBox9->Controls->Add(this->alphatb);
			this->groupBox9->Controls->Add(this->cropEndtb);
			this->groupBox9->Controls->Add(this->cropStarttb);
			this->groupBox9->Controls->Add(this->Bbatchtb);
			this->groupBox9->Controls->Add(this->Abatchtb);
			this->groupBox9->Controls->Add(this->DataProcApplyBtn);
			this->groupBox9->Controls->Add(this->gaussiantb);
			this->groupBox9->Controls->Add(this->lambdaktb);
			this->groupBox9->Controls->Add(this->lambda0tb);
			this->groupBox9->Controls->Add(this->label17);
			this->groupBox9->Controls->Add(this->label16);
			this->groupBox9->Controls->Add(this->label15);
			this->groupBox9->Location = System::Drawing::Point(91, 495);
			this->groupBox9->Name = L"groupBox9";
			this->groupBox9->Size = System::Drawing::Size(877, 157);
			this->groupBox9->TabIndex = 4;
			this->groupBox9->TabStop = false;
			this->groupBox9->Text = L"Data processing setting";
			// 
			// label32
			// 
			this->label32->AutoSize = true;
			this->label32->Location = System::Drawing::Point(64, 74);
			this->label32->Name = L"label32";
			this->label32->Size = System::Drawing::Size(73, 16);
			this->label32->TabIndex = 20;
			this->label32->Text = L"Gray Level:";
			// 
			// label31
			// 
			this->label31->AutoSize = true;
			this->label31->Location = System::Drawing::Point(92, 99);
			this->label31->Name = L"label31";
			this->label31->Size = System::Drawing::Size(45, 16);
			this->label31->TabIndex = 19;
			this->label31->Text = L"Alpha:";
			// 
			// label30
			// 
			this->label30->AutoSize = true;
			this->label30->Location = System::Drawing::Point(389, 104);
			this->label30->Name = L"label30";
			this->label30->Size = System::Drawing::Size(131, 16);
			this->label30->TabIndex = 18;
			this->label30->Text = L"Cropping - End Pixel:";
			// 
			// label28
			// 
			this->label28->AutoSize = true;
			this->label28->Location = System::Drawing::Point(384, 77);
			this->label28->Name = L"label28";
			this->label28->Size = System::Drawing::Size(136, 16);
			this->label28->TabIndex = 17;
			this->label28->Text = L"Cropping - Start Pixel:";
			// 
			// label27
			// 
			this->label27->AutoSize = true;
			this->label27->Location = System::Drawing::Point(405, 50);
			this->label27->Name = L"label27";
			this->label27->Size = System::Drawing::Size(115, 16);
			this->label27->TabIndex = 16;
			this->label27->Text = L"B-Scans to Batch:";
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->Location = System::Drawing::Point(404, 23);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(115, 16);
			this->label26->TabIndex = 15;
			this->label26->Text = L"A-Scans to Batch:";
			// 
			// grayLeveltb
			// 
			this->grayLeveltb->Location = System::Drawing::Point(143, 74);
			this->grayLeveltb->Name = L"grayLeveltb";
			this->grayLeveltb->Size = System::Drawing::Size(184, 22);
			this->grayLeveltb->TabIndex = 12;
			// 
			// alphatb
			// 
			this->alphatb->Location = System::Drawing::Point(143, 101);
			this->alphatb->Name = L"alphatb";
			this->alphatb->Size = System::Drawing::Size(184, 22);
			this->alphatb->TabIndex = 11;
			// 
			// cropEndtb
			// 
			this->cropEndtb->Location = System::Drawing::Point(524, 101);
			this->cropEndtb->Name = L"cropEndtb";
			this->cropEndtb->Size = System::Drawing::Size(200, 22);
			this->cropEndtb->TabIndex = 10;
			// 
			// cropStarttb
			// 
			this->cropStarttb->Location = System::Drawing::Point(524, 74);
			this->cropStarttb->Name = L"cropStarttb";
			this->cropStarttb->Size = System::Drawing::Size(200, 22);
			this->cropStarttb->TabIndex = 9;
			// 
			// Bbatchtb
			// 
			this->Bbatchtb->Location = System::Drawing::Point(524, 47);
			this->Bbatchtb->Name = L"Bbatchtb";
			this->Bbatchtb->Size = System::Drawing::Size(200, 22);
			this->Bbatchtb->TabIndex = 8;
			// 
			// Abatchtb
			// 
			this->Abatchtb->Location = System::Drawing::Point(524, 20);
			this->Abatchtb->Name = L"Abatchtb";
			this->Abatchtb->Size = System::Drawing::Size(200, 22);
			this->Abatchtb->TabIndex = 7;
			// 
			// DataProcApplyBtn
			// 
			this->DataProcApplyBtn->Location = System::Drawing::Point(774, 123);
			this->DataProcApplyBtn->Name = L"DataProcApplyBtn";
			this->DataProcApplyBtn->Size = System::Drawing::Size(86, 24);
			this->DataProcApplyBtn->TabIndex = 6;
			this->DataProcApplyBtn->Text = L"Apply";
			this->DataProcApplyBtn->UseVisualStyleBackColor = true;
			this->DataProcApplyBtn->Click += gcnew System::EventHandler(this, &Form1::DataProcApplyBtn_Click);
			// 
			// gaussiantb
			// 
			this->gaussiantb->Location = System::Drawing::Point(525, 128);
			this->gaussiantb->Name = L"gaussiantb";
			this->gaussiantb->Size = System::Drawing::Size(199, 22);
			this->gaussiantb->TabIndex = 5;
			// 
			// lambdaktb
			// 
			this->lambdaktb->Location = System::Drawing::Point(143, 47);
			this->lambdaktb->Name = L"lambdaktb";
			this->lambdaktb->Size = System::Drawing::Size(184, 22);
			this->lambdaktb->TabIndex = 4;
			// 
			// lambda0tb
			// 
			this->lambda0tb->Location = System::Drawing::Point(143, 21);
			this->lambda0tb->Name = L"lambda0tb";
			this->lambda0tb->Size = System::Drawing::Size(184, 22);
			this->lambda0tb->TabIndex = 3;
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(373, 131);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(147, 16);
			this->label17->TabIndex = 2;
			this->label17->Text = L"Gaussian window width:";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(68, 49);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(69, 16);
			this->label16->TabIndex = 1;
			this->label16->Text = L"Lambda k:";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(68, 24);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(69, 16);
			this->label15->TabIndex = 0;
			this->label15->Text = L"Lambda 0:";
			// 
			// groupBox6
			// 
			this->groupBox6->Controls->Add(this->channeltb);
			this->groupBox6->Controls->Add(this->preExposuretb);
			this->groupBox6->Controls->Add(this->label29);
			this->groupBox6->Controls->Add(this->ScannerApplyBtn);
			this->groupBox6->Controls->Add(this->label21);
			this->groupBox6->Controls->Add(this->label22);
			this->groupBox6->Controls->Add(this->label20);
			this->groupBox6->Controls->Add(this->waveformtb);
			this->groupBox6->Controls->Add(this->label23);
			this->groupBox6->Controls->Add(this->label19);
			this->groupBox6->Controls->Add(this->label24);
			this->groupBox6->Controls->Add(this->dacratetb);
			this->groupBox6->Controls->Add(this->label18);
			this->groupBox6->Controls->Add(this->label25);
			this->groupBox6->Controls->Add(this->yfactortb);
			this->groupBox6->Controls->Add(this->grabertb);
			this->groupBox6->Controls->Add(this->xfactortb);
			this->groupBox6->Controls->Add(this->yvoltagetb);
			this->groupBox6->Controls->Add(this->xvoltagetb);
			this->groupBox6->Location = System::Drawing::Point(424, 101);
			this->groupBox6->Name = L"groupBox6";
			this->groupBox6->Size = System::Drawing::Size(544, 330);
			this->groupBox6->TabIndex = 1;
			this->groupBox6->TabStop = false;
			this->groupBox6->Text = L"Galvo and Camera setting";
			// 
			// channeltb
			// 
			this->channeltb->Location = System::Drawing::Point(191, 253);
			this->channeltb->Name = L"channeltb";
			this->channeltb->Size = System::Drawing::Size(201, 22);
			this->channeltb->TabIndex = 17;
			// 
			// preExposuretb
			// 
			this->preExposuretb->Location = System::Drawing::Point(190, 292);
			this->preExposuretb->Name = L"preExposuretb";
			this->preExposuretb->Size = System::Drawing::Size(200, 22);
			this->preExposuretb->TabIndex = 9;
			// 
			// label29
			// 
			this->label29->AutoSize = true;
			this->label29->Location = System::Drawing::Point(68, 291);
			this->label29->Name = L"label29";
			this->label29->Size = System::Drawing::Size(100, 16);
			this->label29->TabIndex = 13;
			this->label29->Text = L"Exposure time :";
			// 
			// ScannerApplyBtn
			// 
			this->ScannerApplyBtn->Location = System::Drawing::Point(434, 291);
			this->ScannerApplyBtn->Name = L"ScannerApplyBtn";
			this->ScannerApplyBtn->Size = System::Drawing::Size(86, 24);
			this->ScannerApplyBtn->TabIndex = 8;
			this->ScannerApplyBtn->Text = L"Apply";
			this->ScannerApplyBtn->UseVisualStyleBackColor = true;
			this->ScannerApplyBtn->Click += gcnew System::EventHandler(this, &Form1::ScannerApplyBtn_Click);
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Location = System::Drawing::Point(42, 121);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(143, 16);
			this->label21->TabIndex = 7;
			this->label21->Text = L"Y scale factor [V/mm] :";
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Location = System::Drawing::Point(57, 255);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(111, 16);
			this->label22->TabIndex = 16;
			this->label22->Text = L"Physical channel:";
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Location = System::Drawing::Point(44, 89);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(141, 16);
			this->label20->TabIndex = 6;
			this->label20->Text = L"X scale factor [V/mm] :";
			// 
			// waveformtb
			// 
			this->waveformtb->Location = System::Drawing::Point(192, 220);
			this->waveformtb->Name = L"waveformtb";
			this->waveformtb->Size = System::Drawing::Size(200, 22);
			this->waveformtb->TabIndex = 11;
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->Location = System::Drawing::Point(18, 222);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(161, 16);
			this->label23->TabIndex = 15;
			this->label23->Text = L"Efficiency of the waveform:";
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(68, 57);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(119, 16);
			this->label19->TabIndex = 5;
			this->label19->Text = L"Y max voltage [V] :";
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Location = System::Drawing::Point(7, 189);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(180, 16);
			this->label24->TabIndex = 14;
			this->label24->Text = L"DAC update rate [points/sec]:";
			// 
			// dacratetb
			// 
			this->dacratetb->Location = System::Drawing::Point(193, 186);
			this->dacratetb->Name = L"dacratetb";
			this->dacratetb->Size = System::Drawing::Size(200, 22);
			this->dacratetb->TabIndex = 10;
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Location = System::Drawing::Point(68, 25);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(117, 16);
			this->label18->TabIndex = 4;
			this->label18->Text = L"X max voltage [V] :";
			// 
			// label25
			// 
			this->label25->AutoSize = true;
			this->label25->Location = System::Drawing::Point(44, 153);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(105, 16);
			this->label25->TabIndex = 13;
			this->label25->Text = L"Graber delay [s]:";
			// 
			// yfactortb
			// 
			this->yfactortb->Location = System::Drawing::Point(192, 115);
			this->yfactortb->Name = L"yfactortb";
			this->yfactortb->Size = System::Drawing::Size(200, 22);
			this->yfactortb->TabIndex = 3;
			// 
			// grabertb
			// 
			this->grabertb->Location = System::Drawing::Point(193, 150);
			this->grabertb->Name = L"grabertb";
			this->grabertb->Size = System::Drawing::Size(200, 22);
			this->grabertb->TabIndex = 9;
			// 
			// xfactortb
			// 
			this->xfactortb->Location = System::Drawing::Point(192, 83);
			this->xfactortb->Name = L"xfactortb";
			this->xfactortb->Size = System::Drawing::Size(200, 22);
			this->xfactortb->TabIndex = 2;
			// 
			// yvoltagetb
			// 
			this->yvoltagetb->Location = System::Drawing::Point(192, 51);
			this->yvoltagetb->Name = L"yvoltagetb";
			this->yvoltagetb->Size = System::Drawing::Size(200, 22);
			this->yvoltagetb->TabIndex = 1;
			// 
			// xvoltagetb
			// 
			this->xvoltagetb->Location = System::Drawing::Point(192, 19);
			this->xvoltagetb->Name = L"xvoltagetb";
			this->xvoltagetb->Size = System::Drawing::Size(200, 22);
			this->xvoltagetb->TabIndex = 0;
			// 
			// groupBox5
			// 
			this->groupBox5->Controls->Add(this->fundusoffupdate_btn);
			this->groupBox5->Controls->Add(this->FundusRestBtn);
			this->groupBox5->Controls->Add(this->FundusStartBtn);
			this->groupBox5->Controls->Add(this->fundusYoff_tb);
			this->groupBox5->Controls->Add(this->label14);
			this->groupBox5->Controls->Add(this->fundusXoff_tb);
			this->groupBox5->Controls->Add(this->label13);
			this->groupBox5->Controls->Add(this->imagefundus);
			this->groupBox5->Location = System::Drawing::Point(47, 68);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(328, 385);
			this->groupBox5->TabIndex = 0;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L"Fundus View";
			// 
			// FundusRestBtn
			// 
			this->FundusRestBtn->Location = System::Drawing::Point(188, 348);
			this->FundusRestBtn->Name = L"FundusRestBtn";
			this->FundusRestBtn->Size = System::Drawing::Size(120, 24);
			this->FundusRestBtn->TabIndex = 6;
			this->FundusRestBtn->Text = L"Set Offset to 0";
			this->FundusRestBtn->UseVisualStyleBackColor = true;
			this->FundusRestBtn->Click += gcnew System::EventHandler(this, &Form1::FundusRestBtn_Click);
			// 
			// FundusStartBtn
			// 
			this->FundusStartBtn->Location = System::Drawing::Point(18, 348);
			this->FundusStartBtn->Name = L"FundusStartBtn";
			this->FundusStartBtn->Size = System::Drawing::Size(120, 24);
			this->FundusStartBtn->TabIndex = 5;
			this->FundusStartBtn->Text = L"Start";
			this->FundusStartBtn->UseVisualStyleBackColor = true;
			this->FundusStartBtn->Click += gcnew System::EventHandler(this, &Form1::FundusStartBtn_Click);
			// 
			// fundusYoff_tb
			// 
			this->fundusYoff_tb->Location = System::Drawing::Point(188, 318);
			this->fundusYoff_tb->Name = L"fundusYoff_tb";
			this->fundusYoff_tb->Size = System::Drawing::Size(24, 22);
			this->fundusYoff_tb->TabIndex = 4;
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(165, 324);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(20, 16);
			this->label14->TabIndex = 3;
			this->label14->Text = L"Y:";
			// 
			// fundusXoff_tb
			// 
			this->fundusXoff_tb->Location = System::Drawing::Point(132, 318);
			this->fundusXoff_tb->Name = L"fundusXoff_tb";
			this->fundusXoff_tb->Size = System::Drawing::Size(24, 22);
			this->fundusXoff_tb->TabIndex = 2;
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(17, 324);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(117, 16);
			this->label13->TabIndex = 1;
			this->label13->Text = L"Scanner Offset:  X:";
			// 
			// imagefundus
			// 
			this->imagefundus->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"imagefundus.Image")));
			this->imagefundus->Location = System::Drawing::Point(18, 23);
			this->imagefundus->Name = L"imagefundus";
			this->imagefundus->Size = System::Drawing::Size(290, 290);
			this->imagefundus->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->imagefundus->TabIndex = 0;
			this->imagefundus->TabStop = false;
			this->imagefundus->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::fundus_Paint);
			// 
			// tabPage_main
			// 
			this->tabPage_main->BackColor = System::Drawing::Color::WhiteSmoke;
			this->tabPage_main->Controls->Add(this->panel1);
			this->tabPage_main->Controls->Add(this->groupBox4);
			this->tabPage_main->Controls->Add(this->groupBox3);
			this->tabPage_main->Controls->Add(this->groupBox2);
			this->tabPage_main->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->tabPage_main->Location = System::Drawing::Point(4, 25);
			this->tabPage_main->Name = L"tabPage_main";
			this->tabPage_main->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_main->Size = System::Drawing::Size(1081, 737);
			this->tabPage_main->TabIndex = 0;
			this->tabPage_main->Text = L"main";
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::Color::WhiteSmoke;
			this->panel1->Controls->Add(this->PathSavetb);
			this->panel1->Controls->Add(this->IDtb);
			this->panel1->Controls->Add(this->ForderPathtb);
			this->panel1->Controls->Add(this->label12);
			this->panel1->Controls->Add(this->EndBtn);
			this->panel1->Controls->Add(this->PreviewBtn);
			this->panel1->Controls->Add(this->AcquireBtn);
			this->panel1->Controls->Add(this->RefreshBcgrBtn);
			this->panel1->Controls->Add(this->label11);
			this->panel1->Controls->Add(this->label10);
			this->panel1->Controls->Add(this->label9);
			this->panel1->ForeColor = System::Drawing::SystemColors::ControlText;
			this->panel1->Location = System::Drawing::Point(627, 560);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(437, 156);
			this->panel1->TabIndex = 4;
			// 
			// PathSavetb
			// 
			this->PathSavetb->BackColor = System::Drawing::SystemColors::Window;
			this->PathSavetb->Font = (gcnew System::Drawing::Font(L"Sitka Banner", 9.749999F));
			this->PathSavetb->Location = System::Drawing::Point(118, 79);
			this->PathSavetb->Name = L"PathSavetb";
			this->PathSavetb->Size = System::Drawing::Size(256, 24);
			this->PathSavetb->TabIndex = 13;
			this->PathSavetb->TextChanged += gcnew System::EventHandler(this, &Form1::savePath_Changed);
			// 
			// IDtb
			// 
			this->IDtb->Font = (gcnew System::Drawing::Font(L"Sitka Banner", 9.749999F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->IDtb->Location = System::Drawing::Point(118, 10);
			this->IDtb->Name = L"IDtb";
			this->IDtb->Size = System::Drawing::Size(152, 24);
			this->IDtb->TabIndex = 12;
			this->IDtb->TextChanged += gcnew System::EventHandler(this, &Form1::ForderPath_Changed);
			// 
			// ForderPathtb
			// 
			this->ForderPathtb->BackColor = System::Drawing::SystemColors::Window;
			this->ForderPathtb->Font = (gcnew System::Drawing::Font(L"Sitka Banner", 9.749999F));
			this->ForderPathtb->Location = System::Drawing::Point(117, 45);
			this->ForderPathtb->Name = L"ForderPathtb";
			this->ForderPathtb->ReadOnly = true;
			this->ForderPathtb->Size = System::Drawing::Size(257, 24);
			this->ForderPathtb->TabIndex = 11;
			this->ForderPathtb->TextChanged += gcnew System::EventHandler(this, &Form1::ForderPath_Changed);
			this->ForderPathtb->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::FolderPathtb_MouseDown);
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Font = (gcnew System::Drawing::Font(L"Arial Black", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label12->Location = System::Drawing::Point(289, 10);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(46, 18);
			this->label12->TabIndex = 10;
			this->label12->Text = L"idel...";
			// 
			// EndBtn
			// 
			this->EndBtn->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->EndBtn->Location = System::Drawing::Point(318, 117);
			this->EndBtn->Name = L"EndBtn";
			this->EndBtn->Size = System::Drawing::Size(95, 26);
			this->EndBtn->TabIndex = 9;
			this->EndBtn->Text = L"End";
			this->EndBtn->UseVisualStyleBackColor = true;
			this->EndBtn->Click += gcnew System::EventHandler(this, &Form1::EndBtn_Click);
			// 
			// PreviewBtn
			// 
			this->PreviewBtn->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->PreviewBtn->Location = System::Drawing::Point(116, 117);
			this->PreviewBtn->Name = L"PreviewBtn";
			this->PreviewBtn->Size = System::Drawing::Size(95, 26);
			this->PreviewBtn->TabIndex = 8;
			this->PreviewBtn->Text = L"Preview";
			this->PreviewBtn->UseVisualStyleBackColor = true;
			this->PreviewBtn->Click += gcnew System::EventHandler(this, &Form1::PreviewBtn_Click);
			// 
			// AcquireBtn
			// 
			this->AcquireBtn->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->AcquireBtn->Location = System::Drawing::Point(217, 117);
			this->AcquireBtn->Name = L"AcquireBtn";
			this->AcquireBtn->Size = System::Drawing::Size(95, 26);
			this->AcquireBtn->TabIndex = 7;
			this->AcquireBtn->Text = L"Acquire Start";
			this->AcquireBtn->UseVisualStyleBackColor = true;
			this->AcquireBtn->Click += gcnew System::EventHandler(this, &Form1::AcquireBtn_Click);
			// 
			// RefreshBcgrBtn
			// 
			this->RefreshBcgrBtn->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->RefreshBcgrBtn->Location = System::Drawing::Point(17, 117);
			this->RefreshBcgrBtn->Name = L"RefreshBcgrBtn";
			this->RefreshBcgrBtn->Size = System::Drawing::Size(95, 26);
			this->RefreshBcgrBtn->TabIndex = 6;
			this->RefreshBcgrBtn->Text = L"Refresh bcgr";
			this->RefreshBcgrBtn->UseVisualStyleBackColor = true;
			this->RefreshBcgrBtn->Click += gcnew System::EventHandler(this, &Form1::RefreshBcgrBtn_Click);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label11->Location = System::Drawing::Point(14, 82);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(104, 16);
			this->label11->TabIndex = 5;
			this->label11->Text = L"Path to save file:";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label10->Location = System::Drawing::Point(14, 18);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(24, 16);
			this->label10->TabIndex = 4;
			this->label10->Text = L"ID:";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label9->Location = System::Drawing::Point(14, 50);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(89, 16);
			this->label9->TabIndex = 1;
			this->label9->Text = L"Path to folder:";
			// 
			// groupBox4
			// 
			this->groupBox4->BackColor = System::Drawing::Color::WhiteSmoke;
			this->groupBox4->Controls->Add(this->bscanrepeat_tb);
			this->groupBox4->Controls->Add(this->label38);
			this->groupBox4->Controls->Add(this->NoPixeltb);
			this->groupBox4->Controls->Add(this->label33);
			this->groupBox4->Controls->Add(this->UserApplyBtn);
			this->groupBox4->Controls->Add(this->label8);
			this->groupBox4->Controls->Add(this->label5);
			this->groupBox4->Controls->Add(this->xrangetb);
			this->groupBox4->Controls->Add(this->label7);
			this->groupBox4->Controls->Add(this->label6);
			this->groupBox4->Controls->Add(this->label4);
			this->groupBox4->Controls->Add(this->label3);
			this->groupBox4->Controls->Add(this->label2);
			this->groupBox4->Controls->Add(this->yoffsettb);
			this->groupBox4->Controls->Add(this->xoffsettb);
			this->groupBox4->Controls->Add(this->yrangetb);
			this->groupBox4->Controls->Add(this->NoBcgrstb);
			this->groupBox4->Controls->Add(this->NoBstb);
			this->groupBox4->Controls->Add(this->NoAstb);
			this->groupBox4->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->groupBox4->ForeColor = System::Drawing::SystemColors::ControlText;
			this->groupBox4->Location = System::Drawing::Point(6, 560);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(615, 156);
			this->groupBox4->TabIndex = 3;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"User define raster scan";
			// 
			// bscanrepeat_tb
			// 
			this->bscanrepeat_tb->Font = (gcnew System::Drawing::Font(L"SimSun", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->bscanrepeat_tb->Location = System::Drawing::Point(133, 130);
			this->bscanrepeat_tb->Name = L"bscanrepeat_tb";
			this->bscanrepeat_tb->Size = System::Drawing::Size(154, 22);
			this->bscanrepeat_tb->TabIndex = 18;
			// 
			// label38
			// 
			this->label38->AutoSize = true;
			this->label38->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label38->Location = System::Drawing::Point(5, 132);
			this->label38->Name = L"label38";
			this->label38->Size = System::Drawing::Size(127, 16);
			this->label38->TabIndex = 17;
			this->label38->Text = L"No. of  Bscan repeat";
			// 
			// NoPixeltb
			// 
			this->NoPixeltb->Font = (gcnew System::Drawing::Font(L"SimSun", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->NoPixeltb->Location = System::Drawing::Point(134, 104);
			this->NoPixeltb->Name = L"NoPixeltb";
			this->NoPixeltb->Size = System::Drawing::Size(154, 22);
			this->NoPixeltb->TabIndex = 16;
			// 
			// label33
			// 
			this->label33->AutoSize = true;
			this->label33->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label33->Location = System::Drawing::Point(16, 104);
			this->label33->Name = L"label33";
			this->label33->Size = System::Drawing::Size(80, 16);
			this->label33->TabIndex = 15;
			this->label33->Text = L"No. of pixels";
			// 
			// UserApplyBtn
			// 
			this->UserApplyBtn->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->UserApplyBtn->Location = System::Drawing::Point(508, 130);
			this->UserApplyBtn->Name = L"UserApplyBtn";
			this->UserApplyBtn->Size = System::Drawing::Size(95, 26);
			this->UserApplyBtn->TabIndex = 14;
			this->UserApplyBtn->Text = L"Apply";
			this->UserApplyBtn->UseVisualStyleBackColor = true;
			this->UserApplyBtn->Click += gcnew System::EventHandler(this, &Form1::UserApplyBtn_Click);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label8->Location = System::Drawing::Point(329, 104);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(90, 16);
			this->label8->TabIndex = 13;
			this->label8->Text = L"Y offset [mm]:";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label5->Location = System::Drawing::Point(331, 26);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(89, 16);
			this->label5->TabIndex = 10;
			this->label5->Text = L"X range [mm]:";
			// 
			// xrangetb
			// 
			this->xrangetb->Font = (gcnew System::Drawing::Font(L"SimSun", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->xrangetb->Location = System::Drawing::Point(448, 22);
			this->xrangetb->Name = L"xrangetb";
			this->xrangetb->Size = System::Drawing::Size(154, 22);
			this->xrangetb->TabIndex = 3;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label7->Location = System::Drawing::Point(331, 79);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(88, 16);
			this->label7->TabIndex = 12;
			this->label7->Text = L"X offset [mm]:";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label6->Location = System::Drawing::Point(328, 54);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(91, 16);
			this->label6->TabIndex = 11;
			this->label6->Text = L"Y range [mm]:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(16, 78);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(109, 16);
			this->label4->TabIndex = 9;
			this->label4->Text = L"No. of bcgr Scan:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(16, 50);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(93, 16);
			this->label3->TabIndex = 8;
			this->label3->Text = L"No. of B Scan:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(16, 22);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(91, 16);
			this->label2->TabIndex = 7;
			this->label2->Text = L"No. of A Scan:";
			// 
			// yoffsettb
			// 
			this->yoffsettb->Font = (gcnew System::Drawing::Font(L"SimSun", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->yoffsettb->Location = System::Drawing::Point(447, 103);
			this->yoffsettb->Name = L"yoffsettb";
			this->yoffsettb->Size = System::Drawing::Size(154, 22);
			this->yoffsettb->TabIndex = 6;
			// 
			// xoffsettb
			// 
			this->xoffsettb->Font = (gcnew System::Drawing::Font(L"SimSun", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->xoffsettb->Location = System::Drawing::Point(448, 76);
			this->xoffsettb->Name = L"xoffsettb";
			this->xoffsettb->Size = System::Drawing::Size(154, 22);
			this->xoffsettb->TabIndex = 5;
			// 
			// yrangetb
			// 
			this->yrangetb->Font = (gcnew System::Drawing::Font(L"SimSun", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->yrangetb->Location = System::Drawing::Point(448, 49);
			this->yrangetb->Name = L"yrangetb";
			this->yrangetb->Size = System::Drawing::Size(154, 22);
			this->yrangetb->TabIndex = 4;
			// 
			// NoBcgrstb
			// 
			this->NoBcgrstb->Font = (gcnew System::Drawing::Font(L"SimSun", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->NoBcgrstb->Location = System::Drawing::Point(134, 76);
			this->NoBcgrstb->Name = L"NoBcgrstb";
			this->NoBcgrstb->Size = System::Drawing::Size(154, 22);
			this->NoBcgrstb->TabIndex = 2;
			// 
			// NoBstb
			// 
			this->NoBstb->Font = (gcnew System::Drawing::Font(L"SimSun", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->NoBstb->Location = System::Drawing::Point(134, 48);
			this->NoBstb->Name = L"NoBstb";
			this->NoBstb->Size = System::Drawing::Size(154, 22);
			this->NoBstb->TabIndex = 1;
			// 
			// NoAstb
			// 
			this->NoAstb->Font = (gcnew System::Drawing::Font(L"SimSun", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->NoAstb->Location = System::Drawing::Point(134, 20);
			this->NoAstb->Name = L"NoAstb";
			this->NoAstb->Size = System::Drawing::Size(154, 22);
			this->NoAstb->TabIndex = 0;
			// 
			// groupBox3
			// 
			this->groupBox3->BackColor = System::Drawing::Color::WhiteSmoke;
			this->groupBox3->Controls->Add(this->panel2);
			this->groupBox3->Controls->Add(this->imageYZ);
			this->groupBox3->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->groupBox3->Location = System::Drawing::Point(554, 6);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(510, 535);
			this->groupBox3->TabIndex = 2;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Cross-sectional image YZ";
			this->groupBox3->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::groupBox3_Paint);
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->ReduceYZBtn);
			this->panel2->Controls->Add(this->ZoomYZBtn);
			this->panel2->Location = System::Drawing::Point(333, 11);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(155, 28);
			this->panel2->TabIndex = 2;
			// 
			// ReduceYZBtn
			// 
			this->ReduceYZBtn->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"ReduceYZBtn.BackgroundImage")));
			this->ReduceYZBtn->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->ReduceYZBtn->FlatAppearance->BorderSize = 0;
			this->ReduceYZBtn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->ReduceYZBtn->Location = System::Drawing::Point(118, 2);
			this->ReduceYZBtn->Name = L"ReduceYZBtn";
			this->ReduceYZBtn->Size = System::Drawing::Size(35, 23);
			this->ReduceYZBtn->TabIndex = 1;
			this->ReduceYZBtn->UseVisualStyleBackColor = true;
			this->ReduceYZBtn->Click += gcnew System::EventHandler(this, &Form1::ReduceYZBtn_Click);
			// 
			// ZoomYZBtn
			// 
			this->ZoomYZBtn->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"ZoomYZBtn.BackgroundImage")));
			this->ZoomYZBtn->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->ZoomYZBtn->FlatAppearance->BorderSize = 0;
			this->ZoomYZBtn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->ZoomYZBtn->Location = System::Drawing::Point(79, 2);
			this->ZoomYZBtn->Name = L"ZoomYZBtn";
			this->ZoomYZBtn->Size = System::Drawing::Size(35, 23);
			this->ZoomYZBtn->TabIndex = 0;
			this->ZoomYZBtn->UseVisualStyleBackColor = true;
			this->ZoomYZBtn->Click += gcnew System::EventHandler(this, &Form1::ZoomYZBtn_Click);
			// 
			// imageYZ
			// 
			this->imageYZ->InitialImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"imageYZ.InitialImage")));
			this->imageYZ->Location = System::Drawing::Point(17, 39);
			this->imageYZ->Name = L"imageYZ";
			this->imageYZ->Size = System::Drawing::Size(471, 471);
			this->imageYZ->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->imageYZ->TabIndex = 1;
			this->imageYZ->TabStop = false;
			this->imageYZ->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::imageYZ_Paint);
			this->imageYZ->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::imageYZ_MouseDown);
			this->imageYZ->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::imageYZ_MouseMove);
			this->imageYZ->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::imageYZ_MouseUp);
			// 
			// groupBox2
			// 
			this->groupBox2->BackColor = System::Drawing::Color::WhiteSmoke;
			this->groupBox2->Controls->Add(this->panel3);
			this->groupBox2->Controls->Add(this->imageXZ);
			this->groupBox2->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->groupBox2->Location = System::Drawing::Point(6, 6);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(510, 535);
			this->groupBox2->TabIndex = 1;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Cross-sectional image XZ";
			this->groupBox2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::groupBox2_Paint);
			// 
			// panel3
			// 
			this->panel3->Controls->Add(this->ReduceXZBtn);
			this->panel3->Controls->Add(this->ZoomXZBtn);
			this->panel3->Location = System::Drawing::Point(324, 11);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(155, 28);
			this->panel3->TabIndex = 3;
			// 
			// ReduceXZBtn
			// 
			this->ReduceXZBtn->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"ReduceXZBtn.BackgroundImage")));
			this->ReduceXZBtn->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->ReduceXZBtn->FlatAppearance->BorderSize = 0;
			this->ReduceXZBtn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->ReduceXZBtn->Location = System::Drawing::Point(118, 2);
			this->ReduceXZBtn->Name = L"ReduceXZBtn";
			this->ReduceXZBtn->Size = System::Drawing::Size(35, 23);
			this->ReduceXZBtn->TabIndex = 1;
			this->ReduceXZBtn->UseVisualStyleBackColor = true;
			this->ReduceXZBtn->Click += gcnew System::EventHandler(this, &Form1::ReduceXZBtn_Click);
			// 
			// ZoomXZBtn
			// 
			this->ZoomXZBtn->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"ZoomXZBtn.BackgroundImage")));
			this->ZoomXZBtn->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->ZoomXZBtn->FlatAppearance->BorderSize = 0;
			this->ZoomXZBtn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->ZoomXZBtn->Location = System::Drawing::Point(79, 2);
			this->ZoomXZBtn->Name = L"ZoomXZBtn";
			this->ZoomXZBtn->Size = System::Drawing::Size(35, 23);
			this->ZoomXZBtn->TabIndex = 0;
			this->ZoomXZBtn->UseVisualStyleBackColor = true;
			this->ZoomXZBtn->Click += gcnew System::EventHandler(this, &Form1::ZoomXZBtn_Click);
			// 
			// imageXZ
			// 
			this->imageXZ->InitialImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"imageXZ.InitialImage")));
			this->imageXZ->Location = System::Drawing::Point(8, 39);
			this->imageXZ->Name = L"imageXZ";
			this->imageXZ->Size = System::Drawing::Size(471, 471);
			this->imageXZ->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->imageXZ->TabIndex = 0;
			this->imageXZ->TabStop = false;
			this->imageXZ->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::imageXZ_Paint);
			this->imageXZ->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::imageXZ_MouseDown);
			this->imageXZ->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::imageXZ_MouseMove);
			this->imageXZ->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::imageXZ_MouseUp);
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage_main);
			this->tabControl1->Controls->Add(this->tabPage_Parameter);
			this->tabControl1->Controls->Add(this->tabPage_dispersion);
			this->tabControl1->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->tabControl1->Location = System::Drawing::Point(9, 27);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(1089, 766);
			this->tabControl1->TabIndex = 0;
			this->tabControl1->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::tabControl1_SelectedIndexChanged);
			// 
			// tabPage_dispersion
			// 
			this->tabPage_dispersion->BackColor = System::Drawing::Color::WhiteSmoke;
			this->tabPage_dispersion->Controls->Add(this->delta_x_tb);
			this->tabPage_dispersion->Controls->Add(this->label37);
			this->tabPage_dispersion->Controls->Add(this->dp_max2_tb);
			this->tabPage_dispersion->Controls->Add(this->dp_max1_tb);
			this->tabPage_dispersion->Controls->Add(this->label36);
			this->tabPage_dispersion->Controls->Add(this->label35);
			this->tabPage_dispersion->Controls->Add(this->center2_tb);
			this->tabPage_dispersion->Controls->Add(this->center1_tb);
			this->tabPage_dispersion->Controls->Add(this->center2_p);
			this->tabPage_dispersion->Controls->Add(this->center2_m);
			this->tabPage_dispersion->Controls->Add(this->center1_p);
			this->tabPage_dispersion->Controls->Add(this->center1_m);
			this->tabPage_dispersion->Controls->Add(this->label34);
			this->tabPage_dispersion->Controls->Add(this->label1);
			this->tabPage_dispersion->Controls->Add(this->dp_chart);
			this->tabPage_dispersion->Controls->Add(this->resetDispersion);
			this->tabPage_dispersion->Controls->Add(this->sp_chart);
			this->tabPage_dispersion->Location = System::Drawing::Point(4, 25);
			this->tabPage_dispersion->Name = L"tabPage_dispersion";
			this->tabPage_dispersion->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_dispersion->Size = System::Drawing::Size(1081, 737);
			this->tabPage_dispersion->TabIndex = 2;
			this->tabPage_dispersion->Text = L"Dispersion";
			// 
			// delta_x_tb
			// 
			this->delta_x_tb->Location = System::Drawing::Point(1002, 507);
			this->delta_x_tb->Name = L"delta_x_tb";
			this->delta_x_tb->Size = System::Drawing::Size(51, 22);
			this->delta_x_tb->TabIndex = 16;
			// 
			// label37
			// 
			this->label37->AutoSize = true;
			this->label37->Location = System::Drawing::Point(932, 509);
			this->label37->Name = L"label37";
			this->label37->Size = System::Drawing::Size(54, 16);
			this->label37->TabIndex = 15;
			this->label37->Text = L"delta_x:";
			// 
			// dp_max2_tb
			// 
			this->dp_max2_tb->Location = System::Drawing::Point(1002, 473);
			this->dp_max2_tb->Name = L"dp_max2_tb";
			this->dp_max2_tb->Size = System::Drawing::Size(51, 22);
			this->dp_max2_tb->TabIndex = 14;
			// 
			// dp_max1_tb
			// 
			this->dp_max1_tb->Location = System::Drawing::Point(1002, 442);
			this->dp_max1_tb->Name = L"dp_max1_tb";
			this->dp_max1_tb->Size = System::Drawing::Size(51, 22);
			this->dp_max1_tb->TabIndex = 13;
			// 
			// label36
			// 
			this->label36->AutoSize = true;
			this->label36->Location = System::Drawing::Point(928, 476);
			this->label36->Name = L"label36";
			this->label36->Size = System::Drawing::Size(72, 16);
			this->label36->TabIndex = 12;
			this->label36->Text = L"dp_max_2:";
			// 
			// label35
			// 
			this->label35->AutoSize = true;
			this->label35->Location = System::Drawing::Point(927, 446);
			this->label35->Name = L"label35";
			this->label35->Size = System::Drawing::Size(72, 16);
			this->label35->TabIndex = 11;
			this->label35->Text = L"dp_max_1:";
			// 
			// center2_tb
			// 
			this->center2_tb->Location = System::Drawing::Point(990, 149);
			this->center2_tb->Name = L"center2_tb";
			this->center2_tb->Size = System::Drawing::Size(44, 22);
			this->center2_tb->TabIndex = 10;
			this->center2_tb->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::center2_keyenter);
			// 
			// center1_tb
			// 
			this->center1_tb->Location = System::Drawing::Point(990, 115);
			this->center1_tb->Name = L"center1_tb";
			this->center1_tb->Size = System::Drawing::Size(44, 22);
			this->center1_tb->TabIndex = 9;
			this->center1_tb->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::center1_keyenter);
			// 
			// center2_p
			// 
			this->center2_p->Location = System::Drawing::Point(1041, 149);
			this->center2_p->Name = L"center2_p";
			this->center2_p->Size = System::Drawing::Size(14, 22);
			this->center2_p->TabIndex = 8;
			this->center2_p->Text = L">";
			this->center2_p->UseVisualStyleBackColor = true;
			this->center2_p->Click += gcnew System::EventHandler(this, &Form1::center2_p_Click);
			// 
			// center2_m
			// 
			this->center2_m->Location = System::Drawing::Point(970, 149);
			this->center2_m->Name = L"center2_m";
			this->center2_m->Size = System::Drawing::Size(14, 22);
			this->center2_m->TabIndex = 7;
			this->center2_m->Text = L"<";
			this->center2_m->UseVisualStyleBackColor = true;
			this->center2_m->Click += gcnew System::EventHandler(this, &Form1::center2_m_Click);
			// 
			// center1_p
			// 
			this->center1_p->Location = System::Drawing::Point(1041, 115);
			this->center1_p->Name = L"center1_p";
			this->center1_p->Size = System::Drawing::Size(14, 22);
			this->center1_p->TabIndex = 6;
			this->center1_p->Text = L">";
			this->center1_p->UseVisualStyleBackColor = true;
			this->center1_p->Click += gcnew System::EventHandler(this, &Form1::center1_p_Click);
			// 
			// center1_m
			// 
			this->center1_m->Location = System::Drawing::Point(970, 115);
			this->center1_m->Name = L"center1_m";
			this->center1_m->Size = System::Drawing::Size(14, 22);
			this->center1_m->TabIndex = 5;
			this->center1_m->Text = L"<";
			this->center1_m->UseVisualStyleBackColor = true;
			this->center1_m->Click += gcnew System::EventHandler(this, &Form1::center1_m_Click);
			// 
			// label34
			// 
			this->label34->AutoSize = true;
			this->label34->Location = System::Drawing::Point(912, 149);
			this->label34->Name = L"label34";
			this->label34->Size = System::Drawing::Size(55, 16);
			this->label34->TabIndex = 4;
			this->label34->Text = L"center2:";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(912, 118);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(55, 16);
			this->label1->TabIndex = 3;
			this->label1->Text = L"center1:";
			// 
			// dp_chart
			// 
			this->dp_chart->BackColor = System::Drawing::Color::WhiteSmoke;
			chartArea1->Name = L"ChartArea1";
			this->dp_chart->ChartAreas->Add(chartArea1);
			legend1->Name = L"Legend1";
			this->dp_chart->Legends->Add(legend1);
			this->dp_chart->Location = System::Drawing::Point(3, 371);
			this->dp_chart->Name = L"dp_chart";
			series1->ChartArea = L"ChartArea1";
			series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
			series1->Legend = L"Legend1";
			series1->Name = L"dp1";
			series2->ChartArea = L"ChartArea1";
			series2->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
			series2->Legend = L"Legend1";
			series2->Name = L"dp2";
			this->dp_chart->Series->Add(series1);
			this->dp_chart->Series->Add(series2);
			this->dp_chart->Size = System::Drawing::Size(1030, 360);
			this->dp_chart->TabIndex = 2;
			this->dp_chart->Text = L"dispersion_chart";
			// 
			// resetDispersion
			// 
			this->resetDispersion->Location = System::Drawing::Point(935, 298);
			this->resetDispersion->Name = L"resetDispersion";
			this->resetDispersion->Size = System::Drawing::Size(117, 23);
			this->resetDispersion->TabIndex = 1;
			this->resetDispersion->Text = L"ResetDispersion";
			this->resetDispersion->UseVisualStyleBackColor = true;
			this->resetDispersion->Click += gcnew System::EventHandler(this, &Form1::resetDispersion_Click);
			// 
			// sp_chart
			// 
			this->sp_chart->BackColor = System::Drawing::Color::WhiteSmoke;
			chartArea2->AxisY->Maximum = 2000;
			chartArea2->AxisY->Minimum = -10;
			chartArea2->Name = L"ChartArea1";
			this->sp_chart->ChartAreas->Add(chartArea2);
			legend2->Name = L"Legend1";
			this->sp_chart->Legends->Add(legend2);
			this->sp_chart->Location = System::Drawing::Point(3, 0);
			this->sp_chart->Name = L"sp_chart";
			series3->ChartArea = L"ChartArea1";
			series3->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
			series3->Legend = L"Legend1";
			series3->Name = L"sp";
			series4->ChartArea = L"ChartArea1";
			series4->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
			series4->Legend = L"Legend1";
			series4->Name = L"center1";
			series5->ChartArea = L"ChartArea1";
			series5->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
			series5->Legend = L"Legend1";
			series5->Name = L"center2";
			series6->ChartArea = L"ChartArea1";
			series6->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
			series6->Legend = L"Legend1";
			series6->Name = L"gaussian1";
			series7->ChartArea = L"ChartArea1";
			series7->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
			series7->Legend = L"Legend1";
			series7->Name = L"gaussian2";
			this->sp_chart->Series->Add(series3);
			this->sp_chart->Series->Add(series4);
			this->sp_chart->Series->Add(series5);
			this->sp_chart->Series->Add(series6);
			this->sp_chart->Series->Add(series7);
			this->sp_chart->Size = System::Drawing::Size(1060, 365);
			this->sp_chart->TabIndex = 0;
			this->sp_chart->Text = L"chart1";
			this->sp_chart->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::spchart_mousedown);
			this->sp_chart->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::spchart_mousemove);
			this->sp_chart->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::spchart_mouseup);
			// 
			// menuStrip1
			// 
			this->menuStrip1->BackColor = System::Drawing::Color::Snow;
			this->menuStrip1->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 9));
			this->menuStrip1->GripStyle = System::Windows::Forms::ToolStripGripStyle::Visible;
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->modeToolStripMenuItem, 
				this->saveFileToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1115, 25);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// modeToolStripMenuItem
			// 
			this->modeToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->userToolStripMenuItem, 
				this->debugToolStripMenuItem});
			this->modeToolStripMenuItem->ForeColor = System::Drawing::SystemColors::ControlText;
			this->modeToolStripMenuItem->Name = L"modeToolStripMenuItem";
			this->modeToolStripMenuItem->Size = System::Drawing::Size(55, 21);
			this->modeToolStripMenuItem->Text = L"Mode";
			// 
			// userToolStripMenuItem
			// 
			this->userToolStripMenuItem->Name = L"userToolStripMenuItem";
			this->userToolStripMenuItem->Size = System::Drawing::Size(115, 22);
			this->userToolStripMenuItem->Text = L"User";
			this->userToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::userToolStripMenuItem_Click);
			// 
			// debugToolStripMenuItem
			// 
			this->debugToolStripMenuItem->Name = L"debugToolStripMenuItem";
			this->debugToolStripMenuItem->Size = System::Drawing::Size(115, 22);
			this->debugToolStripMenuItem->Text = L"Debug";
			this->debugToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::debugToolStripMenuItem_Click);
			// 
			// saveFileToolStripMenuItem
			// 
			this->saveFileToolStripMenuItem->BackColor = System::Drawing::Color::Transparent;
			this->saveFileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->rawToolStripMenuItem, 
				this->processedToolStripMenuItem});
			this->saveFileToolStripMenuItem->ForeColor = System::Drawing::SystemColors::ControlText;
			this->saveFileToolStripMenuItem->Name = L"saveFileToolStripMenuItem";
			this->saveFileToolStripMenuItem->Size = System::Drawing::Size(66, 21);
			this->saveFileToolStripMenuItem->Text = L"SaveFile";
			// 
			// rawToolStripMenuItem
			// 
			this->rawToolStripMenuItem->Name = L"rawToolStripMenuItem";
			this->rawToolStripMenuItem->Size = System::Drawing::Size(136, 22);
			this->rawToolStripMenuItem->Text = L"Raw";
			this->rawToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::rawToolStripMenuItem_Click);
			// 
			// processedToolStripMenuItem
			// 
			this->processedToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->tiffToolStripMenuItem, 
				this->pngToolStripMenuItem, this->bothToolStripMenuItem});
			this->processedToolStripMenuItem->Name = L"processedToolStripMenuItem";
			this->processedToolStripMenuItem->Size = System::Drawing::Size(136, 22);
			this->processedToolStripMenuItem->Text = L"Processed";
			// 
			// tiffToolStripMenuItem
			// 
			this->tiffToolStripMenuItem->Name = L"tiffToolStripMenuItem";
			this->tiffToolStripMenuItem->Size = System::Drawing::Size(103, 22);
			this->tiffToolStripMenuItem->Text = L"tiff";
			this->tiffToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::tiffToolStripMenuItem_Click);
			// 
			// pngToolStripMenuItem
			// 
			this->pngToolStripMenuItem->Name = L"pngToolStripMenuItem";
			this->pngToolStripMenuItem->Size = System::Drawing::Size(103, 22);
			this->pngToolStripMenuItem->Text = L"png";
			this->pngToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::pngToolStripMenuItem_Click);
			// 
			// bothToolStripMenuItem
			// 
			this->bothToolStripMenuItem->Name = L"bothToolStripMenuItem";
			this->bothToolStripMenuItem->Size = System::Drawing::Size(103, 22);
			this->bothToolStripMenuItem->Text = L"both";
			this->bothToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::bothToolStripMenuItem_Click);
			// 
			// PreviewUpdateTimer
			// 
			this->PreviewUpdateTimer->Enabled = true;
			this->PreviewUpdateTimer->Tick += gcnew System::EventHandler(this, &Form1::peview_update_timer_Tick);
			// 
			// SpectrumUpdateTimer
			// 
			this->SpectrumUpdateTimer->Enabled = true;
			this->SpectrumUpdateTimer->Interval = 1000;
			// 
			// fundusoffupdate_btn
			// 
			this->fundusoffupdate_btn->Location = System::Drawing::Point(233, 317);
			this->fundusoffupdate_btn->Name = L"fundusoffupdate_btn";
			this->fundusoffupdate_btn->Size = System::Drawing::Size(75, 23);
			this->fundusoffupdate_btn->TabIndex = 7;
			this->fundusoffupdate_btn->Text = L"apply";
			this->fundusoffupdate_btn->UseVisualStyleBackColor = true;
			this->fundusoffupdate_btn->Click += gcnew System::EventHandler(this, &Form1::fundusoffupdate_btn_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Control;
			this->ClientSize = System::Drawing::Size(1115, 805);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->menuStrip1);
			this->DoubleBuffered = true;
			this->Font = (gcnew System::Drawing::Font(L"Arial", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->ForeColor = System::Drawing::SystemColors::ControlText;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MainMenuStrip = this->menuStrip1;
			this->MaximizeBox = false;
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"SDOCT Controll System";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
			this->Shown += gcnew System::EventHandler(this, &Form1::Form1_Shown);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::center2_keyenter);
			this->tabPage_Parameter->ResumeLayout(false);
			this->groupBox9->ResumeLayout(false);
			this->groupBox9->PerformLayout();
			this->groupBox6->ResumeLayout(false);
			this->groupBox6->PerformLayout();
			this->groupBox5->ResumeLayout(false);
			this->groupBox5->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->imagefundus))->EndInit();
			this->tabPage_main->ResumeLayout(false);
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->imageYZ))->EndInit();
			this->groupBox2->ResumeLayout(false);
			this->panel3->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->imageXZ))->EndInit();
			this->tabControl1->ResumeLayout(false);
			this->tabPage_dispersion->ResumeLayout(false);
			this->tabPage_dispersion->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dp_chart))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->sp_chart))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void userToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->tabPage_Parameter->Parent = nullptr;
				 this->tabPage_dispersion->Parent = nullptr;

			 }
	private: System::Void debugToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->tabPage_Parameter->Parent = this->tabControl1;
				 this->tabPage_dispersion->Parent = this->tabControl1;
			 }

			 //The tabPage named "Dispersion":Event function of reset button
	private: System::Void resetBtn_Click(System::Object^  sender, System::EventArgs^  e) {
			 }

			 //Menu bar item named "Save File":Event function of Raw sub-item
	private: System::Void rawToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 }

			 //Menu bar item named "Save File":Event function of of the sub-item of "Processed",named "tiff" 
	private: System::Void tiffToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 }

			 //Menu bar item named "Save File":Event function of of the sub-item of "Processed",named "png" 
	private: System::Void pngToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 }

			 //Menu bar item named "Save File":Event function of of the sub-item of "Processed",named "both" 
	private: System::Void bothToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 }

			 //The tabPage named "main":Event function of RefreshBcgrBtn button
	private: System::Void RefreshBcgrBtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 m_bPreview = false;
				 getBackground();
				 m_bPreview = true;
			 }

			 //The tabPage named "main":Event function of Preview button
	private: System::Void PreviewBtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (!initParamsDone)
				 {
					 initController();
					 getBackground();
				 }
				 m_eWorkingMode = PREVIEWXZ;
				 updateBtnStatus();
			 }

			 //The tabPage named "main":Event function of Acquire start button
	private: System::Void AcquireBtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 m_eWorkingMode = ACQUIRE;
				 updateBtnStatus();
				 acquireWorker->RunWorkerAsync();
				 this->label12->Text = L"Scanning sample...";
			 }

			 //The tabPage named "main":Event function of End button
	private: System::Void EndBtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 m_eWorkingMode = IDEL;
				 updateBtnStatus();
			 }

			 //The tabPage named "main":Event function of User apply button
	private: System::Void UserApplyBtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 m_eWorkingMode = IDEL;
				 //m_eWorkingMode = PREVIEWXZ;
				 updateBtnStatus();
				 initParams_ScanPattern();
			 }


	private: System::Void FundusStartBtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->EndBtn->PerformClick(); 
				 if (!initParamsDone)
				 {
					 initController();
					 getBackground();
				 }
				 m_eWorkingMode = FUNDUS_VIEW;
				 updateBtnStatus();
			}

	private: System::Void FundusRestBtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 float xoffest = 0;
				 float yoffest = 0;
				 fundusXoff_tb->Text = Convert::ToString(xoffest);
				 fundusYoff_tb->Text = Convert::ToString(yoffest);
				 if (fFundusXOffsetV != xoffest ||  fFundusYOffsetV != yoffest)
				 {
					 fFundusXOffsetV = xoffest;
					 fFundusYOffsetV = yoffest;
					 initParamsDone = false;
				 }
			 }
			 //The tabPage named "Parameter":Event function of Set Offset to Zero button
	private: System::Void fundusoffupdate_btn_Click(System::Object^  sender, System::EventArgs^  e) {
				 float xoffest = Convert::ToDouble(fundusXoff_tb->Text) * fXScaleVM;
				 float yoffest = Convert::ToDouble(fundusYoff_tb->Text) * fYScaleVM;

				 if (xoffest+fXRangeV/2 > fXMaxV || xoffest - fXRangeV/2 < -fXMaxV)
				 {
					 messageBoxInfo("fundus x offset error");
					 return;
				 }
				 if (yoffest+fYRangeV/2 > fYMaxV || yoffest - fYRangeV/2 < -fYMaxV)
				 {
					 messageBoxInfo("fundus y offset error");
					 return;
				 }
				 if (fFundusXOffsetV != xoffest ||  fFundusYOffsetV != yoffest)
				 {
					 fFundusXOffsetV = xoffest;
					 fFundusYOffsetV = yoffest;
					 initParamsDone = false;
				 }
			 }

			 //The tabPage named "Parameter":Event function of Scanner Apply button
	private: System::Void ScannerApplyBtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 m_eWorkingMode = IDEL;
				 updateBtnStatus();
				 initParams_GolveCamera();
			 }

			 //The tabPage named "Parameter":Event function of Data process Apply button
	private: System::Void DataProcApplyBtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 m_eWorkingMode = IDEL;
				 updateBtnStatus();
				 initParams_DataProcessing();
			 }

			 //Adjust the Size of Form and tabControl
	private: System::Void tabControl1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 if(this->tabControl1->SelectedTab == this->tabPage_main)
				 {
					 this->Size = System::Drawing::Size(1115,826);
					 this->tabControl1->Size = System::Drawing::Size(1082,752);
				 }
				 else if(this->tabControl1->SelectedTab == tabPage_Parameter){
					 //this->Size = System::Drawing::Size(930,663);
					 //this->tabControl1->Size = System::Drawing::Size(899,592);
					 this->Size = System::Drawing::Size(1115,826);
					 this->tabControl1->Size = System::Drawing::Size(1082,752);
				 }else{
					 //this->Size = System::Drawing::Size(930,663);
					 //this->tabControl1->Size = System::Drawing::Size(899,592);
					 this->Size = System::Drawing::Size(1115,826);
					 this->tabControl1->Size = System::Drawing::Size(1082,752);
				 }
			 }

	private: System::Void fundus_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
				 this->OnPaint(e);
				 if (this->fundus_img == nullptr) return;
				 e->Graphics->Clear( Color::WhiteSmoke );
				 e->Graphics->DrawImage(this->fundus_img, 0, 0, 100, 100);
			 }
			 
			 //OnPaint override
	private: System::Void imageXZ_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
				 this->OnPaint(e);
				 if (this->xz_img == nullptr) return;

				 int img_x = this->imageXZ->Width / 2 - this->xz_img->Width * xz_zoom_ratio / 2 + xz_new_offset.X;
				 int img_y = this->imageXZ->Height / 2 - this->xz_img->Height * xz_zoom_ratio / 2 + xz_new_offset.Y;
				 int img_width = this->xz_img->Width * xz_zoom_ratio;
				 int img_height = this->xz_img->Height * xz_zoom_ratio;

				 e->Graphics->Clear( Color::WhiteSmoke );
				 e->Graphics->DrawImage(this->xz_img, img_x, img_y, img_width, img_height);
			 }
			 
	private: System::Void imageXZ_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 this->xz_mouse_down = true;
				 this->xz_mouse_down_p = Point(e->X, e->Y);
			 }
			
	private: System::Void imageXZ_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 if (this->xz_mouse_down)
				 {
					 this->xz_new_offset.X = e->X - this->xz_mouse_down_p.X + this->xz_old_offset.X;
					 this->xz_new_offset.Y = e->Y - this->xz_mouse_down_p.Y + this->xz_old_offset.Y;
					 this->imageXZ->Invalidate();
				 }
			 }

	private: System::Void imageXZ_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 this->xz_mouse_down = false;
				 this->xz_old_offset = this->xz_new_offset;
			 }
			 //Reduce Image XZ
	private: System::Void ReduceXZBtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (xz_zoom_ratio > 1.0)
				 {
					 xz_zoom_ratio -= 0.1;
				 }
				 this->imageXZ->Invalidate();
			 }

	private: System::Void ZoomXZBtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (xz_zoom_ratio < 2.5)
				 {
					 xz_zoom_ratio += 0.1;
				 }
				 this->imageXZ->Invalidate();
			}


	private: System::Void imageYZ_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
				 this->OnPaint(e);
				 
				 if (this->yz_img == nullptr) return;

				 int img_x = this->imageYZ->Width / 2 - this->yz_img->Width * yz_zoom_ratio / 2 + yz_new_offset.X;
				 int img_y = this->imageYZ->Height / 2 - this->yz_img->Height * yz_zoom_ratio / 2 + yz_new_offset.Y;
				 int img_width = this->yz_img->Width * yz_zoom_ratio;
				 int img_height = this->yz_img->Height * yz_zoom_ratio;
				 
				 e->Graphics->Clear( Color::WhiteSmoke );
				 e->Graphics->DrawImage(this->yz_img, img_x, img_y, img_width, img_height);
				 //e->Graphics->DrawImage(this->yz_img, 0, 0, 100, 100);
			 }

	private: System::Void imageYZ_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 this->yz_mouse_down = true;
				 this->yz_mouse_down_p = Point(e->X, e->Y);
			 }
			 
	private: System::Void imageYZ_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 this->yz_mouse_down = false;
				 this->yz_old_offset = this->yz_new_offset;
			 }
			
	private: System::Void imageYZ_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 if (this->yz_mouse_down)
				 {
					 this->yz_new_offset.X = e->X - this->yz_mouse_down_p.X + this->yz_old_offset.X;
					 this->yz_new_offset.Y = e->Y - this->yz_mouse_down_p.Y + this->yz_old_offset.Y;
					 this->imageYZ->Invalidate();
				 }
			 }

	private: System::Void ReduceYZBtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (yz_zoom_ratio > 1.0)
				 {
					 yz_zoom_ratio -= 0.1;
				 }
				 this->imageYZ->Invalidate();
			 }

	private: System::Void ZoomYZBtn_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (yz_zoom_ratio < 2.5)
				 {
					 yz_zoom_ratio += 0.1;
				 }
				 this->imageYZ->Invalidate();
			 }

	private: System::Void groupBox2_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
				 this->OnPaint(e);
				 int height = this->groupBox2->Height;
				 int width = this->groupBox2->Width;
				 Pen^ blackPen = gcnew Pen(Color::Black);

				 //draw the X-rule
				 Point point1 = *(new Point(width-60,height-15));
				 Point point2 = *(new Point(width-60,height-10));
				 Point point3 = *(new Point(width-30,height-10));
				 Point point4 = *(new Point(width-30,height-15));
				 array<Point>^point_array1 = {point1,point2,point3,point4}; 

				 //draw the Y-rule
				 Point point5 = *(new Point(width-15,height-25));
				 Point point6 = *(new Point(width-10,height-25));
				 Point point7 = *(new Point(width-10,height-55));
				 Point point8 = *(new Point(width-15,height-55));
				 array<Point>^point_array2 = {point5,point6,point7,point8}; 

				 e->Graphics->DrawLines(blackPen,point_array1);
				 e->Graphics->DrawLines(blackPen,point_array2);
			 }
	private: System::Void groupBox3_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
				 this->OnPaint(e);
				 int height = this->groupBox2->Height;
				 int width = this->groupBox2->Width;
				 Pen^ blackPen = gcnew Pen(Color::Black);

				 //draw the X-rule
				 Point point1 = *(new Point(width-54,height-15));
				 Point point2 = *(new Point(width-54,height-10));
				 Point point3 = *(new Point(width-24,height-10));
				 Point point4 = *(new Point(width-24,height-15));
				 array<Point>^point_array1 = {point1,point2,point3,point4}; 

				 //draw the Y-rule
				 Point point5 = *(new Point(width-15,height-25));
				 Point point6 = *(new Point(width-10,height-25));
				 Point point7 = *(new Point(width-10,height-55));
				 Point point8 = *(new Point(width-15,height-55));
				 array<Point>^point_array2 = {point5,point6,point7,point8}; 

				 e->Graphics->DrawLines(blackPen,point_array1);
				 e->Graphics->DrawLines(blackPen,point_array2);
			 }
			 //open the explorer to choose the image file
	private: System::Void FolderPathtb_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 if(e->Button == System::Windows::Forms::MouseButtons::Left)
				 {
					 FolderBrowserDialog^ folderBrowserDialog = gcnew FolderBrowserDialog;
					 if(folderBrowserDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					 {
						 this->ForderPathtb->Text = folderBrowserDialog->SelectedPath;
						 
					 }
				 }
			 }

	private: System::Void ForderPath_Changed( Object^ /*sender*/, EventArgs^ /*e*/ )
			 {
				 this->PathSavetb->Text = this->ForderPathtb->Text + "\\" + this->IDtb->Text;
			 }

	private: System::Void savePath_Changed( Object^ /*sender*/, EventArgs^ /*e*/ )
			 {
				 this->PathSavetb->Text = this->ForderPathtb->Text + "\\" + this->IDtb->Text;
				 fAcquireDataSet = this->PathSavetb->Text;
				 if (initControllerDone)
				 {
					 //messageBoxInfo("save path changed");
					 controller->resetAcquireDataSet(fAcquireDataSet);
				 }
				 
			 }

	private: System::Void peview_update_timer_Tick(System::Object^  sender, System::EventArgs^  e) {
				 
				 // update preview
				 if (m_eWorkingMode == PREVIEWXZ && !previewUpdateWorker_busy && !fundusUpdateWorker_busy)
				 {
					 previewUpdateWorker->RunWorkerAsync();
				 }

				 // update fundus view
				 if (m_eWorkingMode == FUNDUS_VIEW && !previewUpdateWorker_busy && !fundusUpdateWorker_busy)
				 {
					 fundusUpdateWorker->RunWorkerAsync();
				 }
		 }
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			 getBackground();
		 }

	// used defined function @brian *************************************************************************//

	private: void formWorkingPrepare() {
				// init params and load background after form shown
			initParamsDone = false;
			initControllerDone = false;
			initBackgroundDone = false;
			controller = gcnew ControllerWrapper::ControllerAccess();

			std::remove("../log/main.log");
			freopen("../log/main.log", "w", stdout);
			std::remove("../log/sdoct_0.log");

			// need load first since load params will need default parmas file dir
			initParams_DataFile();

			bool params_load_succ = loadParams_conf();
			if (params_load_succ)
			{
				initParams();
				initController();
				getBackground();
			}

			this->spectrum_rawdata = gcnew cliext::vector<int>;
			this->spectrum_rawdata->resize(uiNumPixel);
		}

	private:

		bool initParams_ScanPattern()
		{
			// scan pattern params: 8
			try {
				float value = Convert::ToDouble(xrangetb->Text);
				if (value != fXRangeV) {
					fXRangeV = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			try {
				float value = Convert::ToDouble(yrangetb->Text);
				if (value != fYRangeV) {
					fYRangeV = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			try {
				float value = Convert::ToDouble(xoffsettb->Text);
				if (value != fXOffsetV) {
					fXOffsetV = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			try {
				float value = Convert::ToDouble(yoffsettb->Text);
				if (value != fYOffsetV) {
					fYOffsetV = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			try {
				float value = Convert::ToDouble(NoPixeltb->Text);
				if (value != uiNumPixel) {
					uiNumPixel = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			try {
				float value = Convert::ToDouble(NoAstb->Text);
				if (value != uiNumAscan) {
					uiNumAscan = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			try {
				float value = Convert::ToDouble(NoBstb->Text);
				if (value != uiNumBscan) {
					uiNumBscan = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			try {
				float value = Convert::ToDouble(NoBcgrstb->Text);
				if (value != uiNumBcgrFrame) {
					uiNumBcgrFrame = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			try {
				float value = Convert::ToDouble(bscanrepeat_tb->Text);
				if (value != uiNumBscanRepeat) {
					uiNumBscanRepeat = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			if (((fXRangeV + fXOffsetV) * fXScaleVM > 2 * fXMaxV)
				|| (( - fXRangeV + fXOffsetV) * fXScaleVM <  - 2 * fXMaxV))
			{
				messageBoxInfo("please set a small x range in scan pattern");
				return false;
			}

			if (((fYRangeV + fYOffsetV) * fYScaleVM > 2 * fXMaxV)
				|| (( - fYRangeV + fYOffsetV) * fYScaleVM <  - 2 * fXMaxV))
			{
				messageBoxInfo("please set a small x range in scan pattern");
				return false;
			}

			return true;
		}

		bool initParams_GolveCamera()
		{
			// init fundus view here
			try {
				float value = Convert::ToDouble(fundusXoff_tb->Text);
				if (fFundusXOffsetV != value) {
					fFundusXOffsetV = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params fundus view x offset error");
				return false;
			}

			try {
				float value = Convert::ToDouble(fundusYoff_tb->Text);
				if (fFundusYOffsetV != value) {
					fFundusYOffsetV = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params fundus view y offset error");
				return false;
			}

			try {
				float value = Convert::ToDouble(xvoltagetb->Text);
				if (fXMaxV != value) {
					fXMaxV = value * 0.9;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			// galvo and camera setting: 9-1 = 8	**delete:grabber_delay
			try {
				float value = Convert::ToDouble(xvoltagetb->Text);
				if (fXMaxV != value) {
					fXMaxV = value * 0.9;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			try {
				float value = Convert::ToDouble(yvoltagetb->Text);
				if (fYMaxV != value) {
					fYMaxV = value * 0.9;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			try {
				float value = Convert::ToDouble(xfactortb->Text);
				if (fXScaleVM != value) {
					fXScaleVM = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			try {
				float value = Convert::ToDouble(yfactortb->Text);
				if (fYScaleVM != value) {
					fYScaleVM = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			try {
				float value = Convert::ToDouble(dacratetb->Text);
				if (uiUpdateRate != value) {
					uiUpdateRate = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			try {
				float value = Convert::ToDouble(waveformtb->Text);
				if (fWaveEffi != value) {
					fWaveEffi = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			try {
				System::String^ value = Convert::ToString(channeltb->Text);
				if (sPhyChannel != value) {
					sPhyChannel = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			try {
				float value = Convert::ToDouble(preExposuretb->Text);
				if (fExposureTime != value) {
					fExposureTime = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			return true;
		}

		bool initParams_DataProcessing()
		{
			try {
				float value = Convert::ToDouble(grayLeveltb->Text);
				if (grayLevel != value) {
					grayLevel = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			try {
				float value = Convert::ToDouble(alphatb->Text);
				if (alpha != value) {
					alpha = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			try {
				float value = Convert::ToDouble(Abatchtb->Text);
				if (batchAscans != value) {
					batchAscans = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}

			try {
				float value = Convert::ToDouble(Bbatchtb->Text);
				if (batchFrames != value) {
					batchFrames = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}
			
			try {
				float value = Convert::ToDouble(cropStarttb->Text);
				if (startPixel != value) {
					startPixel = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}
			
			try {
				float value = Convert::ToDouble(cropEndtb->Text);
				if (endPixel != value) {
					endPixel = value;
					initParamsDone = false;
				}
			} catch ( Exception^ e ) {
				messageBoxInfo("Init params XRange error");
				return false;
			}
			
			return true;
		}

		bool initParams_DataPath()
		{
			//fAcquireDataSet = "../data/acquireDataSet";
			fAcquireDataSet = ForderPathtb->Text;
			return true;
		}

		void initParams_DataFile()
		{
			fnamePhase = "../calibration/1024_calibration_file.txt";
			//fAcquireDataSet = "../data/acquireDataSet";
			fbackground = "../data/background";
			fpreviewxz = "../data/previewxz";
			fpreviewyz = "../data/previewyz";
			fFundusDataSet = "../data/fundusview";
			fpreviewxzImage = "../data/GPU_processed/previewxz/0.png";
			fpreviewyzImage = "../data/GPU_processed/previewyz/0.png";
			ffudnusImage = "../data/GPU_processed/fundus/0.png";
			fparams_conf = "../conf/OCT_params_default.conf";
		}

		void initParmas_previewUI()
		{
			// parmas for xz zoom in_out and drag
			xz_zoom_ratio = 1.0;
			xz_mouse_down = false;
			xz_new_offset = Point(0,0);
			xz_old_offset = Point(0,0);
			xz_mouse_down_p = Point(0,0);

			// parmas for xz zoom in_out and drag
			yz_zoom_ratio = 1.0;
			yz_mouse_down = false;
			yz_new_offset = Point(0,0);
			yz_old_offset = Point(0,0);
			yz_mouse_down_p = Point(0,0);

			updatePrevFromFile(fpreviewxzImage, this->xz_img);
			updatePrevFromFile(fpreviewyzImage, this->yz_img);
		}

		bool initParams()
		{
			m_eWorkingMode = IDEL;
			updateBtnStatus();
			
			returnValue = 0;
			previewUpdateWorker_busy = false;
			fundusUpdateWorker_busy = false;
			// could not change order of init parmas
			
			initParmas_previewUI();
			

			// need to read file
			bool re;
			re = initParams_ScanPattern();
			if (!re) return false;

			re = initParams_GolveCamera();
			if (!re) return false;

			re = initParams_DataProcessing();
			if (!re) return false;

			re = initParams_DataPath();
			if (!re) return false;

			// data processing setting

			// gaussiantb not used
			iPreviewAScan = 400;
			iBackgroundAScan = 400;
			iPreviewAScanBatch = 10;
			iFundusViewAScan = 100;

			//dac calibration
			
			//sXChanName = "Dev1/ao1";
			//sYChanName = "Dev1/ao2";
			//sSwitchChanName = "Dev1/ao0";

			sXChanName = channeltb->Text + "/ao1";
			sYChanName = channeltb->Text + "/ao2";
			sSwitchChanName = channeltb->Text + "/ao0";

			bExterClked = false;
			sExterClkChanName = "PFI5"; // not used, use internal clock since bExterClked is false
			ulTrajGenRate = 4000;

			// data process calibration
			prevFreq = 1;						// 1 means show every frame, 2 means show every other frame, 4 means show 1 in 4 frames, etc. Higher is better, has an decent impact on speed.
			reslice = false;						// reslice into enface view								

			interpLinear = true;

			dispModeGS = false; // not used as switch any more since hard coded in Contoller::resetDispersionParams, but should be kept to init GPU code
			dispFrame = 1;
			dispTests = 100; // iteration
			//dispA2 = 0.00909076;
			//dispA3 = 2.44613e-7;
			
			initParamsDone = true;
			std::cout<<"DEBUG: "<<__FUNCTION__<<"\t\tinit params done"<<std::endl;
			return true;
		}



private: bool initController()
		 {
			 if (controller->isActive())
			 {
				 controller->clear();
			 }
			 std::cout<<"TRACE: "<<__FUNCTION__<<std::endl;
			 if (! initParamsDone)
			 {
				 std::cout<<"DEBUG: "<<__FUNCTION__<<"\t\tinit initParamsDone is false"<<std::endl;
				 bool re = initParams();
				 if (! re)
				 {
					 initControllerDone = false;
					 return false;
				 }
			 }
			 m_eWorkingMode = BACKGROUND;
			 updateBtnStatus();
			 controller->initiate();

			 //controller = gcnew ControllerWrapper::ControllerAccess();
			 controller->setScanPattern(fXRangeV, fYRangeV, fXOffsetV, fYOffsetV, uiNumPixel, uiNumAscan, uiNumBscan, uiNumBcgrFrame, uiNumBscanRepeat);
		
			 controller->setTrajectoryCalibration(fXMaxV, fYMaxV, fXScaleVM, fYScaleVM, uiUpdateRate, fWaveEffi, sPhyChannel, fExposureTime, iPreviewAScan, iBackgroundAScan, iFundusViewAScan, fFundusXOffsetV, fFundusYOffsetV);

			 controller->setControllerCalibration(uiNumPixel, uiNumAscan, uiNumBscan, uiNumBcgrFrame, uiNumBscanRepeat, iPreviewAScan, iBackgroundAScan, iFundusViewAScan, fbackground, fpreviewxz, fpreviewyz, fAcquireDataSet, fFundusDataSet);
			 controller->setDacCalibration(fXMaxV, fYMaxV, fXScaleVM, fYScaleVM, sXChanName, sYChanName, sSwitchChanName, bExterClked, sExterClkChanName, ulTrajGenRate);
			 controller->setPreviewPrcssrCalibration(1, iPreviewAScanBatch, prevFreq, reslice, iPreviewAScan, 1, uiNumPixel, uiNumBcgrFrame,
				 startPixel, endPixel, alpha, grayLevel, interpLinear, dispModeGS, dispFrame, dispTests, dispA2, dispA3,
				 fnamePhase, fbackground+".bin", fpreviewxz+".bin");
			 controller->setFundusPrcssrCalibration(batchFrames, batchAscans, prevFreq, reslice, iFundusViewAScan, /*numBScans*/ iFundusViewAScan, uiNumPixel, uiNumBcgrFrame,
				 startPixel, endPixel, alpha, grayLevel, interpLinear, dispModeGS, dispFrame, dispTests, dispA2, dispA3,
				 fnamePhase, fbackground+".bin", fpreviewxz+".bin");
			 controller->setAcquirePrcssrCalibration(batchFrames, batchAscans, prevFreq, reslice, uiNumAscan, uiNumBscan, uiNumPixel, uiNumBcgrFrame, uiNumBscanRepeat,
				 startPixel, endPixel, alpha, grayLevel, interpLinear, dispModeGS, dispFrame, dispTests, dispA2, dispA3,
				 fnamePhase, fbackground+".bin", fpreviewxz+".bin");

			 
			 returnValue = controller->initTrajectory();
			 if (0 != returnValue)
			 {
				 MessageBox::Show("initTrajectory error", "My Application", MessageBoxButtons::OKCancel, MessageBoxIcon::Asterisk);
				 return false;
			 }

			 controller->initNIDacCard();
			 
			 m_eWorkingMode = IDEL;
			 updateBtnStatus();
			 initControllerDone = true;
			 std::cout<<"DEBUG: "<<__FUNCTION__<<"\t\tinit init controller done"<<std::endl;
			 //MessageBox::Show("init controller done", "My Application", MessageBoxButtons::OKCancel, MessageBoxIcon::Asterisk);
			 return true;
		 }

private: bool getBackground()
		 {
			 std::cout<<"TRACE: "<<__FUNCTION__<<std::endl;
			 if (! initControllerDone)
			 {
				 std::cout<<"DEBUG: "<<__FUNCTION__<<"\t\tinit initControllerDone is false"<<std::endl;
				 bool re = initController();
				 if (! re)
				 {
					 initBackgroundDone = false;
					 return false;
				 }
			 }

			 m_eWorkingMode = BACKGROUND;
			 updateBtnStatus();
			 
			 returnValue = controller->getBackground();
			 if (0 != returnValue)
			 {
				 MessageBox::Show("getBackground error", "My Application", MessageBoxButtons::OKCancel, MessageBoxIcon::Asterisk);
				 m_eWorkingMode = IDEL;
				 updateBtnStatus();
				 return false;
			 }
			 //returnValue = controller->processPreviewBcgr(true, dispA2, dispA3);
			 if (!dispModeGS)
			 {
				 returnValue = controller->processPreviewBcgr();
			 } else {
				 float a2, a3;
				 returnValue = controller->processPreviewBcgr(true, a2, a3);
				 dispA2 = a2;
				 dispA3 = a3;
				 controller->resetDispersion(dispModeGS, dispFrame, dispTests, dispA2, dispA3); // update the value of a2 and a3
			 }
			 
			 if (0 != returnValue)
			 {
				 MessageBox::Show("processPreviewBcgr error", "My Application", MessageBoxButtons::OKCancel, MessageBoxIcon::Asterisk);
				 initBackgroundDone = false;
				 m_eWorkingMode = IDEL;
				 updateBtnStatus();
				 return false;
			 }

			 initBackgroundDone = true;
			 m_eWorkingMode = IDEL;
			 updateBtnStatus();
			 std::cout<<"DEBUG: "<<__FUNCTION__<<"\t\tinit init background done"<<std::endl;
			 return true;
		 }

private: System::Void updateBtnStatus()
		 {
			 int btn_cnt = 5;
			 bool btnStatus[5] = {false};
			 if (m_eWorkingMode == IDEL)
			 {
				 this->Cursor = Cursors::Default;
				 for (int i = 0; i < btn_cnt; i++)
					 btnStatus[i] = true;	
				 this->label12->Text = L"Idel...";
			 } 
			 else if (m_eWorkingMode == BACKGROUND)
			 {
				 for (int i = 0; i < btn_cnt; i++)
					 btnStatus[i] = false;	
				 this->label12->Text = L"Getting Background...";
			 } 
			 else if (m_eWorkingMode == ACQUIRE)
			 {
				 this->Cursor = Cursors::WaitCursor;
				 for (int i = 0; i < btn_cnt; i++)
					 btnStatus[i] = false;
				 this->label12->Text = L"Scanning sample...";
			 }
			 else if (m_eWorkingMode == PREVIEWXZ)
			 {
				 this->Cursor = Cursors::Default;
				 btnStatus[3] = true; // this->EndBtn
				 this->label12->Text = L"Getting preview...";
			 }
			 else if (m_eWorkingMode == FUNDUS_VIEW)
			 {
				 this->Cursor = Cursors::WaitCursor;
				 btnStatus[3] = true; // this->EndBtn
				 this->label12->Text = L"Getting fundus view...";
			 }
			 else if (m_eWorkingMode == GALLERY)
			 {
				 coutTime();
				 std::cout<<__FUNCTION__<<"\t\t before load gallery"<<std::endl;

				 this->Cursor = Cursors::WaitCursor;
				 gallery^ gallery_form = gcnew gallery(fimg_dir, uiNumBscan * (1 + uiNumBscanRepeat));
				 gallery_form->Show();

				 coutTime();
				 std::cout<<__FUNCTION__<<"\t\t after load gallery"<<std::endl;

				 m_eWorkingMode = IDEL;
				 for (int i = 0; i < btn_cnt; i++)
					 btnStatus[i] = true;	
				 this->label12->Text = L"Idel...";
			 }

			this->RefreshBcgrBtn->Enabled = btnStatus[0];
			this->AcquireBtn->Enabled = btnStatus[1];
			this->PreviewBtn->Enabled = btnStatus[2];
			this->EndBtn->Enabled = btnStatus[3];
		 }

private: System::Void coutTime()
		 {
			 time_t now = time(0);
			 tm *ltm = localtime(&now);
			 cout<<"DEBUG: "<<1+ltm->tm_hour<<":"<<1+ltm->tm_min<<":"<<1 + ltm->tm_sec<<" "<<endl;
		 }

private: System::Void writeParams()
		 {
			 StreamWriter^ outFile = gcnew StreamWriter(fparams_conf);

			 // scan pattern params: 8
			 outFile->WriteLine(xrangetb->Text);
			 outFile->WriteLine(yrangetb->Text);
			 outFile->WriteLine(xoffsettb->Text);
			 outFile->WriteLine(yoffsettb->Text);

			 outFile->WriteLine(NoAstb->Text);
			 outFile->WriteLine(NoBstb->Text);
			 outFile->WriteLine(NoBcgrstb->Text);
			 outFile->WriteLine(NoPixeltb->Text);

			 outFile->WriteLine(bscanrepeat_tb->Text);

			 // galvo and camera setting: 9
			 outFile->WriteLine(xvoltagetb->Text);
			 outFile->WriteLine(yvoltagetb->Text);
			 outFile->WriteLine(xfactortb->Text);
			 outFile->WriteLine(yfactortb->Text);

			 outFile->WriteLine(grabertb->Text);
			 outFile->WriteLine(dacratetb->Text);
			 outFile->WriteLine(waveformtb->Text);
			 outFile->WriteLine(channeltb->Text);
			 outFile->WriteLine(preExposuretb->Text);

			 // data processing setting
			 outFile->WriteLine(lambda0tb->Text);
			 outFile->WriteLine(lambdaktb->Text);
			 outFile->WriteLine(grayLeveltb->Text);
			 outFile->WriteLine(alphatb->Text);

			 outFile->WriteLine(Abatchtb->Text);
			 outFile->WriteLine(Bbatchtb->Text);
			 outFile->WriteLine(cropStarttb->Text);
			 outFile->WriteLine(cropEndtb->Text);
			 outFile->WriteLine(gaussiantb->Text);
			 
			 outFile->WriteLine(ForderPathtb->Text);

			 // dispersion a2, a3
			 outFile->WriteLine(Convert::ToString(dispA2));
			 outFile->WriteLine(Convert::ToString(dispA3));

			 outFile->Close();
			 if ( outFile ) delete (IDisposable^)outFile; // necessary?
			 std::cout<<"DEBUG: "<<__FUNCTION__<<"\t\twrite params done"<<std::endl;
		 }

private: bool loadParams_conf()
		 {
			 try
			 {
				 StreamReader^ inFile = gcnew StreamReader(fparams_conf);
				 try
				 {
					 // scan pattern params: 8
					 xrangetb->Text = inFile->ReadLine();
					 yrangetb->Text = inFile->ReadLine();
					 xoffsettb->Text = inFile->ReadLine();
					 yoffsettb->Text = inFile->ReadLine();

					 NoAstb->Text = inFile->ReadLine();
					 NoBstb->Text = inFile->ReadLine();
					 NoBcgrstb->Text = inFile->ReadLine();
					 NoPixeltb->Text = inFile->ReadLine();

					 bscanrepeat_tb->Text = inFile->ReadLine();

					 // galvo and camera setting: 9
					 xvoltagetb->Text = inFile->ReadLine();
					 yvoltagetb->Text = inFile->ReadLine();
					 xfactortb->Text = inFile->ReadLine();
					 yfactortb->Text = inFile->ReadLine();

					 grabertb->Text = inFile->ReadLine();
					 dacratetb->Text = inFile->ReadLine();
					 waveformtb->Text = inFile->ReadLine();
					 channeltb->Text = inFile->ReadLine();
					 preExposuretb->Text = inFile->ReadLine();
					 
					 // data processing setting
					 lambda0tb->Text = inFile->ReadLine();
					 lambdaktb->Text = inFile->ReadLine();
					 grayLeveltb->Text = inFile->ReadLine();
					 alphatb->Text = inFile->ReadLine();

					 Abatchtb->Text = inFile->ReadLine();
					 Bbatchtb->Text = inFile->ReadLine();
					 cropStarttb->Text = inFile->ReadLine();
					 cropEndtb->Text = inFile->ReadLine();
					 gaussiantb->Text = inFile->ReadLine();

					 ForderPathtb->Text = inFile->ReadLine();

					 dispA2 = Convert::ToDouble(inFile->ReadLine());
					 dispA3 = Convert::ToDouble(inFile->ReadLine());
				 }
				 catch ( Exception^ e ) 
				 {
					 messageBoxInfo("Read " + fparams_conf + "for default params error");
					 return false;
				 }
				 if ( inFile ) delete (IDisposable^)inFile;
			 }
			 catch ( Exception^ e ) 
			 {
				 messageBoxInfo("Could not find " + fparams_conf + "for default params. Please input params.");
				 return false;
			 }
			 std::cout<<"DEBUG: "<<__FUNCTION__<<"\t\tload params done"<<std::endl;
			 return true;
		 }
		 
private: System::Void Form1_Shown(System::Object^  sender, System::EventArgs^  e) {
		formWorkingPrepare();	 
	}
System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
			 writeParams();
		 }

System::Void messageBoxInfo(System::String^ msg) {
		MessageBox::Show(msg, "My Application", MessageBoxButtons::OKCancel, MessageBoxIcon::Asterisk);
	}
		 
System::Void updatePrevFromFile(System::String^ fpreviewImage, Bitmap^% prevImg) {
		try
		{
			FileStream^ fs = gcnew FileStream(fpreviewImage, FileMode::Open);
			prevImg = (Bitmap^)Image::FromStream(fs);
			fs->Close();
		} catch ( Exception^ e ){
			prevImg = nullptr;
		}
	}
private: bool checkPrevReturn(int returnValue, System::String^ errorMsg) {
		if (returnValue!=0)
		{
			m_eWorkingMode = IDEL;
			messageBoxInfo(errorMsg);
			return true;
		} else {
			return false;
		}
	}
private: System::Void initBackgoundWorker()
		 {
			 previewUpdateWorker->DoWork += gcnew DoWorkEventHandler( this, &Form1::previewUpdateWorker_DoWork );
			 previewUpdateWorker->RunWorkerCompleted += gcnew RunWorkerCompletedEventHandler( this, &Form1::previewUpdateWorker_RunWorkerCompleted );
			 //previewUpdateWorker->ProgressChanged += gcnew ProgressChangedEventHandler( this, &Form1::previewUpdateWorker_ProgressChanged );

			 fundusUpdateWorker->DoWork += gcnew DoWorkEventHandler( this, &Form1::fundusUpdateWorker_DoWork );
			 fundusUpdateWorker->RunWorkerCompleted += gcnew RunWorkerCompletedEventHandler( this, &Form1::fundusUpdateWorker_RunWorkerCompleted );
			 //fundusUpdateWorker->ProgressChanged += gcnew ProgressChangedEventHandler( this, &Form1::fundusUpdateWorker_ProgressChanged );

			 acquireWorker->DoWork += gcnew DoWorkEventHandler( this, &Form1::acquireWorker_DoWork );
			 acquireWorker->RunWorkerCompleted += gcnew RunWorkerCompletedEventHandler( this, &Form1::acquireWorker_RunWorkerCompleted );
			 //acquireWorker->ProgressChanged += gcnew ProgressChangedEventHandler( this, &Form1::previewUpdateWorker_ProgressChanged );
		 }


private: System::Void previewUpdateWorker_DoWork( Object^ sender, DoWorkEventArgs^ e )
		 {
			// update xz preview
			bool retValCheck = false;
			previewUpdateWorker_busy = true;
			
			returnValue = controller->getPreviewXZ();
			retValCheck = checkPrevReturn(returnValue, "error in get xz preview");
			if (retValCheck) return;

			returnValue = controller->processPreviewFrameData(fpreviewxz+".bin");
			retValCheck = checkPrevReturn(returnValue, "error in process xz preview");
			if (retValCheck) return;

			updatePrevFromFile(fpreviewxzImage, this->xz_img);
			

			//*******************************************
			
			// update yz preview
			returnValue = controller->getPreviewYZ();
			retValCheck = checkPrevReturn(returnValue, "error get yz preview");
			if (retValCheck) return;

			returnValue = controller->processPreviewFrameData(fpreviewyz+".bin");
			retValCheck = checkPrevReturn(returnValue, "error process yz preview");
			if (retValCheck) return;

			updatePrevFromFile(fpreviewyzImage, this->yz_img);
		
		 }

private: System::Void updateSpectrumView() {
		if (this->sp_chart)
		{
			this->sp_chart->Series["sp"]->Points->Clear();
		}

		// update spectrum
		if (controller)
		{
			//vector<short> spData = controller->getSpectrum();
			cliext::vector<int>::generic_container ^a = controller->getSpectrum();
			for (int i=0; i<a->size(); i++)
			{
				this->sp_chart->Series["sp"]->Points->AddXY(i, a[i]);
				this->spectrum_rawdata[i] = a[i];
			}
			update_chart(this->spectrum_rawdata, 20, 256, center1, a->size(), 1);
			update_chart(this->spectrum_rawdata, 20, 256, center2, a->size(), 2);
		}
	}

private: System::Void previewUpdateWorker_RunWorkerCompleted( Object^ /*sender*/, RunWorkerCompletedEventArgs^ e )
		 {
			 // do sth here
			 this->imageXZ->Invalidate();
			 this->imageYZ->Invalidate();

			 updateSpectrumView();
			 previewUpdateWorker_busy = false;
		 }
			 

private: System::Void fundusUpdateWorker_DoWork( Object^ sender, DoWorkEventArgs^ e )
		 {
			 fundusUpdateWorker_busy = true;
			 returnValue = controller->getFundusDataSet();
			 controller->processFundusBcgr();
			 controller->processFundusFrameData(fFundusDataSet+".bin");

			 updatePrevFromFile(ffudnusImage, this->fundus_img);
		 }

private: System::Void fundusUpdateWorker_RunWorkerCompleted( Object^ /*sender*/, RunWorkerCompletedEventArgs^ e )
		 {
			 // do sth here
			 this->imagefundus->Invalidate();
			 fundusUpdateWorker_busy = false;
			 m_eWorkingMode = IDEL;
			 updateBtnStatus();
		 }

private: System::Void acquireWorker_DoWork( Object^ sender, DoWorkEventArgs^ e )
		 {
			 returnValue = controller->getAcquireDataSet();
			 controller->processAcquireBcgr();
			 controller->processAcquireFrameData(fAcquireDataSet+".bin");
		 }

private: System::Void acquireWorker_RunWorkerCompleted( Object^ /*sender*/, RunWorkerCompletedEventArgs^ e )
		 {
			 // do sth here
			 //messageBoxInfo("acquire data done.");

			 fimg_dir = genAcquireDataPath(fAcquireDataSet);
			 //messageBoxInfo(fimg_dir);
			 m_eWorkingMode = GALLERY;

			 //m_eWorkingMode = IDEL;
			 updateBtnStatus();
			 this->PreviewBtn->PerformClick();
		 }

private: System::String^ genAcquireDataPath(System::String^ path)
		{
			System::String^ dir;
			System::String^ ID;
			int result = path->LastIndexOf("\\", StringComparison::CurrentCulture);
			if (result > 0)
			{
				dir = path->Substring(0, result);
				ID = path->Substring(result, path->Length - result);
			}
			dir = dir + "\\GPU_processed";
			return dir + ID;
		}
private: System::Void resetDispersion_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (m_eWorkingMode == IDEL)
			 {
				 this->EndBtn->PerformClick(); 
				 dispModeGS = true;
				 getBackground();
				 dispModeGS = false;
			 } else if (m_eWorkingMode == PREVIEWXZ)
			 {
				 this->EndBtn->PerformClick(); 
				 //messageBoxInfo("Please end Preview before setting dispersion params");
			 } else if (m_eWorkingMode == ACQUIRE)
			 {
				 messageBoxInfo("Please wait until acquiring data is done");
			 }
		 }

private: System::Void center1_m_Click(System::Object^  sender, System::EventArgs^  e) {
			 center1 -= 1;
			 center1_update();
		 }
private: System::Void center1_p_Click(System::Object^  sender, System::EventArgs^  e) {
			 center1 += 1;
			 center1_update();
		 }
private: System::Void center1_keyenter(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			 if (e->KeyCode == Keys::Enter) {
				 int center_new = Convert::ToDouble(this->center1_tb->Text);
				 center1 = center_new;
				 center1_update();
			 }
		 }

private: System::Void center2_m_Click(System::Object^  sender, System::EventArgs^  e) {
			 
			 center2 -= 1;
			 center2_update();
		 }
private: System::Void center2_p_Click(System::Object^  sender, System::EventArgs^  e) {
			 center2 += 1;
			 center2_update();
		 }
private: System::Void center2_keyenter(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			 if (e->KeyCode == Keys::Enter) {
				 int center_new = Convert::ToDouble(this->center2_tb->Text);
				 center2 = center_new;
				 center2_update();
			 }
		 }

private: System::Void initData()
		 {
			 int cols[] = {510, 545, 546, 519, 512, 506, 591, 602, 600, 613, 604, 636, 629, 635, 669, 653, 697, 676, 698, 726, 715, 742, 741, 753, 782, 765, 817, 789, 816, 831, 847, 862, 846, 887, 899, 893, 941, 900, 957, 931, 945, 983, 969, 1011, 1001, 1013, 1050, 1009, 1080, 1052, 1094, 1095, 1085, 1137, 1106, 1152, 1150, 1151, 1211, 1146, 1223, 1170, 1229, 1244, 1234, 1285, 1249, 1294, 1282, 1273, 1351, 1292, 1377, 1315, 1366, 1359, 1353, 1411, 1368, 1439, 1403, 1425, 1447, 1402, 1495, 1430, 1497, 1465, 1492, 1513, 1473, 1538, 1493, 1542, 1517, 1509, 1567, 1501, 1598, 1509, 1581, 1551, 1560, 1595, 1533, 1607, 1563, 1611, 1584, 1572, 1632, 1547, 1653, 1551, 1644, 1594, 1615, 1624, 1577, 1654, 1577, 1654, 1607, 1606, 1623, 1585, 1657, 1554, 1665, 1577, 1645, 1602, 1606, 1639, 1596, 1659, 1570, 1643, 1606, 1614, 1614, 1558, 1645, 1541, 1650, 1550, 1620, 1566, 1582, 1584, 1556, 1609, 1542, 1619, 1531, 1569, 1545, 1551, 1572, 1499, 1589, 1476, 1578, 1466, 1544, 1487, 1527, 1512, 1466, 1515, 1443, 1518, 1426, 1490, 1438, 1470, 1457, 1421, 1479, 1406, 1503, 1381, 1490, 1378, 1456, 1391, 1415, 1397, 1375, 1422, 1348, 1412, 1337, 1420, 1343, 1395, 1360, 1365, 1374, 1333, 1380, 1298, 1382, 1292, 1380, 1282, 1360, 1285, 1338, 1301, 1306, 1318, 1281, 1326, 1261, 1315, 1240, 1322, 1241, 1293, 1233, 1247, 1246, 1230, 1259, 1201, 1274, 1196, 1280, 1173, 1268, 1161, 1253, 1157, 1232, 1167, 1205, 1172, 1177, 1185, 1157, 1189, 1146, 1192, 1125, 1193, 1123, 1196, 1121, 1179, 1134, 1161, 1131, 1143, 1143, 1125, 1157, 1110, 1167, 1094, 1162, 1080, 1174, 1075, 1163, 1064, 1156, 1078, 1148, 1073, 1137, 1087, 1137, 1056, 1042, 1054, 1121, 1111, 1094, 1112, 1075, 1117, 1059, 1114, 1048, 1106, 1041, 1107, 1030, 1102, 1036, 1102, 1030, 1085, 1039, 1076, 1032, 1068, 1039, 1049, 1027, 1043, 1033, 1034, 1041, 1026, 1049, 1023, 1059, 1010, 1056, 1008, 1059, 1000, 1061, 993, 1057, 988, 1070, 995, 1072, 989, 1078, 994, 1077, 986, 1074, 984, 1074, 988, 1078, 984, 1074, 992, 1081, 990, 1081, 994, 1078, 989, 1076, 988, 1077, 990, 1068, 981, 1069, 985, 1065, 984, 1070, 984, 1062, 983, 1062, 984, 1065, 975, 1063, 976, 1064, 977, 1057, 971, 1050, 976, 1057, 969, 1055, 975, 1055, 974, 1051, 971, 1058, 967, 1056, 963, 1046, 963, 1042, 964, 1039, 966, 1048, 964, 1051, 960, 1045, 965, 1046, 967, 1047, 971, 1038, 969, 1042, 977, 1029, 977, 1028, 989, 1026, 993, 1017, 999, 1011, 1008, 999, 1011, 991, 1025, 977, 1035, 969, 1039, 968, 1045, 955, 1049, 957, 1045, 956, 1039, 964, 1038, 967, 1020, 966, 1014, 973, 991, 984, 988, 1006, 973, 1010, 962, 1012, 950, 1011, 944, 1014, 932, 1017, 933, 1007, 934, 1008, 941, 1001, 950, 994, 952, 976, 960, 972, 968, 954, 981, 937, 992, 927, 994, 923, 1002, 917, 999, 922, 984, 931, 976, 947, 951, 966, 933, 981, 916, 1001, 905, 1005, 906, 1003, 904, 996, 917, 970, 947, 951, 960, 925, 975, 897, 979, 884, 986, 895, 971, 909, 960, 925, 923, 937, 895, 959, 884, 967, 882, 952, 876, 943, 890, 928, 905, 910, 923, 892, 935, 872, 928, 866, 931, 873, 913, 874, 892, 876, 879, 907, 861, 904, 820, 913, 841, 904, 849, 880, 852, 860, 880, 827, 890, 818, 895, 811, 883, 829, 858, 842, 831, 867, 800, 886, 786, 876, 799, 849, 819, 815, 846, 785, 864, 773, 866, 783, 840, 805, 798, 835, 763, 848, 751, 836, 758, 814, 788, 781, 818, 755, 826, 748, 816, 751, 786, 780, 756, 804, 734, 805, 726, 777, 740, 760, 770, 724, 787, 709, 778, 721, 760, 739, 725, 756, 703, 754, 695, 745, 704, 722, 715, 696, 734, 681, 731, 679, 714, 693, 690, 710, 662, 708, 655, 708, 667, 689, 689, 658, 701, 647, 698, 654, 678, 664, 638, 689, 619, 679, 625, 659, 655, 626, 675, 606, 681, 616, 652, 641, 612, 666, 590, 665, 601, 621, 630, 583, 652, 574, 644, 593, 598, 628, 561, 637, 565, 612, 582, 575, 618, 541, 606, 557, 577, 583, 545, 612, 536, 598, 556, 557, 581, 525, 583, 533, 559, 546, 517, 562, 512, 564, 526, 532, 554, 510, 563, 505, 518, 514, 487, 539, 490, 532, 493, 497, 518, 485, 520, 484, 515, 509, 481, 520, 471, 511, 483, 479, 500, 440, 479, 440, 465, 484, 451, 494, 454, 479, 478, 449, 471, 432, 452, 431, 432, 464, 422, 460, 420, 406, 446, 418, 462, 416, 394, 403, 393, 448, 407, 412, 423, 387, 436, 381, 392, 398, 372, 414, 343, 378, 380, 349, 402, 372, 397, 395, 337, 322, 283, 357, 382, 352, 397, 356, 381, 385, 343, 382, 311, 307, 352, 330, 373, 330, 316, 336, 319, 369, 333, 311, 310, 293, 352, 327, 315, 353, 302, 338, 314, 248, 260, 260, 306, 322, 301, 335, 290, 316, 320, 297, 313, 262, 270, 290, 260, 294, 289, 272, 261, 221, 281, 274, 248, 279, 213, 175, 220, 250, 273, 260, 263, 283, 256, 280, 266, 257, 275, 253, 267, 273, 234, 253, 233, 248, 232, 172, 206, 226, 226, 252, 219, 211, 204, 196, 236, 234, 232, 254, 221, 242, 230, 223, 238, 222, 210, 209, 189, 230, 224, 204, 225, 191, 190, 205, 171, 205, 201, 195, 211, 177, 131, 144, 172, 204, 196, 193, 205, 192, 196, 212, 173, 199, 171, 154, 177, 180, 180, 188, 166, 187, 170, 141, 123, 141, 152, 177, 153, 174, 161, 154, 162, 153, 134, 149, 136, 148, 157, 150, 153, 163, 140, 165, 141, 144, 148, 138, 135, 148, 126, 141, 131, 128, 141, 131, 117, 144, 120, 134, 126, 114, 121, 126, 112, 125, 113, 116, 123, 110, 108, 126, 99, 123, 113, 111, 116, 116, 88, 108, 84, 90, 100, 96, 92, 111, 92, 112, 97, 103, 98, 101, 87, 104, 75, 85, 81, 76, 75, 86, 75, 90, 83, 86, 87, 89, 76, 101, 73, 79, 74, 78, 69, 67, 47, 53, 55, 65, 67, 67, 62, 86, 75, 78, 82, 75, 59, 82, 62, 69, 68, 60, 49, 62, 41, 61, 57, 64, 68, 82, 56, 71, 60, 66, 54, 59, 47, 65, 55};
			 std::vector<int> rawdata(std::begin(cols), std::end(cols));
			 this->spectrum_rawdata = gcnew cliext::vector<int>;
			 this->spectrum_rawdata->resize(rawdata.size());

			 for(int i=0; i < rawdata.size(); i++)
			 {
				 this->sp_chart->Series["sp"]->Points->AddXY(i,rawdata[i]);
				 this->spectrum_rawdata[i] = rawdata[i];
			 }

			 update_chart(this->spectrum_rawdata, 20, 256, center1, rawdata.size(), 1);
			 update_chart(this->spectrum_rawdata, 20, 256, center2, rawdata.size(), 2);
		 }
private: System::Void spchart_mousemove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {

			 int va = this->sp_chart->ChartAreas[0]->AxisX->PixelPositionToValue(e->Location.X);

			 if ( abs(va - center1) <= 5 && abs(va - center1) <= abs(va - center2))
			 {
				 Cursor = Cursors::Hand;
				 centerAreaIdx = 1;
			 }
			 else if ( abs(va - center2) <= 5 && abs(va - center1) > abs(va - center2))
			 {
				 Cursor = Cursors::Hand;
				 centerAreaIdx = 2;
			 }
			 else {
				 Cursor = Cursors::Default;
				 centerAreaIdx = 0;
			 }

			 int left = va - 256 / 2;
			 int right = va + 256 / 2;

			 if(left < 0) {
				 va = 256 / 2;
			 }
			 if (right >= 1024)
			 {
				 va = 1023 - 256 / 2;
			 }

			 if (centerMoveIdx)
			 {
				 System::String^ center_name = "center" + Convert::ToString(centerMoveIdx);
				 this->sp_chart->Series[center_name]->Points->Clear();
				 this->sp_chart->Series[center_name]->Points->AddXY(va, 0);
				 this->sp_chart->Series[center_name]->Points->AddXY(va, 3000);
			 }
		 }
private: System::Void spchart_mousedown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 centerMoveIdx = centerAreaIdx;
		 }
private: System::Void spchart_mouseup(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {

			 if (centerMoveIdx)
			 {
				 int va = this->sp_chart->ChartAreas[0]->AxisX->PixelPositionToValue(e->Location.X);
				 int left = va - 256 / 2;
				 int right = va + 256 / 2;

				 if(left < 0) {
					 va = 256 / 2;
				 }
				 if (right >= 1024)
				 {
					 va = 1023 - 256 / 2;
				 }
				 // MessageBox::Show(Convert::ToString(e->Location.X), "My Application", MessageBoxButtons::OKCancel, MessageBoxIcon::Asterisk);

				 if (centerMoveIdx == 1)
				 {
					 center1 = va;
					 try{
						 center1_update();
					 } catch ( Exception^ e ){
						 MessageBox::Show("center error", "My Application", MessageBoxButtons::OKCancel, MessageBoxIcon::Asterisk);
						 center1 = 200;
					 }
				 } else {
					 center2 = va;
					 try{
						 center2_update();
					 } catch ( Exception^ e ){
						 MessageBox::Show("center error", "My Application", MessageBoxButtons::OKCancel, MessageBoxIcon::Asterisk);
						 center2 = 200;
					 }
				 }

				 centerMoveIdx = 0;
				 centerAreaIdx = 0;

			 }

		 }
private: System::Void update_chart(cliext::vector<int>^ raw_data, int gaussian_sigma, int window_length, int center, int numPixel, int centerIdx)
		 {
			 int left = center - window_length / 2;
			 int right = center + window_length / 2;

			 if (left < 0 || right >= numPixel)
			 {
				 throw std::invalid_argument("center error");
			 }

			 std::vector<float> gaussian_value = get_normal_value(window_length, gaussian_sigma);
			 std::vector<float> gassian_data = multiply_gaussian(raw_data, gaussian_value, center);

			 int norm_left = 20;
			 int norm_right = 128;
			 int idx_max = 0;
			 std::vector<float> fft_norm_output = fft_normalized(gassian_data, window_length , norm_left, norm_right, &idx_max);

			 // update chart
			 System::String^ center_name = "center" + Convert::ToString(centerIdx);
			 System::String^ gaussian_name = "gaussian" + Convert::ToString(centerIdx);
			 System::String^ dp_name = "dp" + Convert::ToString(centerIdx);

			 // clear befor update
			 this->sp_chart->Series[center_name]->Points->Clear();
			 this->sp_chart->Series[gaussian_name]->Points->Clear();
			 this->dp_chart->Series[dp_name]->Points->Clear();

			 this->sp_chart->Series[center_name]->Points->AddXY(center, 0);
			 this->sp_chart->Series[center_name]->Points->AddXY(center, 3000);

			 for(int i=0; i < window_length; i++)
			 {
				 this->sp_chart->Series[gaussian_name]->Points->AddXY(i + center - window_length/2, gassian_data[i]);
			 }

			 for (int i = norm_left; i < norm_right; i++)
			 {
				 this->dp_chart->Series[dp_name]->Points->AddXY(i, fft_norm_output[i - norm_left]);
			 }

			 // update max idx textbox
			 if (centerIdx == 1)
			 {
				 this->dp_max1_tb->Text = Convert::ToString(idx_max);
			 } else if (centerIdx == 2)
			 {
				 this->dp_max2_tb->Text = Convert::ToString(idx_max);
			 }

			 if (this->dp_max1_tb->Text && this->dp_max2_tb->Text)
			 {
				 try{
					 int dmax_1 = Convert::ToInt32(this->dp_max1_tb->Text);
					 int dmax_2 = Convert::ToInt32(this->dp_max2_tb->Text);
					 this->delta_x_tb->Text = Convert::ToString((dmax_1 - dmax_2));
				 } catch ( Exception^ e ) {
					 this->delta_x_tb->Text = " ";
				 }

			 }
		 }

private: System::Void center1_update() {
			 this->center1_tb->Text = Convert::ToString(center1);
			 this->sp_chart->Series["center1"]->Points->Clear();
			 this->sp_chart->Series["gaussian1"]->Points->Clear();
			 this->dp_chart->Series["dp1"]->Points->Clear();
			 update_chart(this->spectrum_rawdata, 20, 256, center1, 1024, 1);
		 }

private: System::Void center2_update() {
			 this->center2_tb->Text = Convert::ToString(center2);
			 this->sp_chart->Series["center2"]->Points->Clear();
			 this->sp_chart->Series["gaussian2"]->Points->Clear();
			 this->dp_chart->Series["dp2"]->Points->Clear();
			 update_chart(this->spectrum_rawdata, 20, 256, center2, 1024, 2);
		 }


};
}
