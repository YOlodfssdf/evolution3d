#pragma once
#include "VirtualCmtyEnv.h"

namespace VirtualCommunity
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainWindow
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class MainWindow : public System::Windows::Forms::Form
	{
	public:
		MainWindow(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainWindow()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 
	private: System::Windows::Forms::ToolStripMenuItem^  �ļ�ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �򿪳���ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �༭����ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �رճ���ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �˳�ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �༭ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��ͼToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��Ӱ��ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ͼToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ͼToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ͼToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ͼToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ͼToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��ȾģʽToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �߿�ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��ɫToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �߿���ɫToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��ʼToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ֹͣToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ·��ToolStripMenuItem;
	private: System::Windows::Forms::TextBox^  UrlLabel;




	protected: 
		xVirtualCmtyEnv^ m_VirtualCommunity;
	private: System::Windows::Forms::Control^      m_Cavans;
	private: System::Windows::Forms::WebBrowser^  webBrowser;
	private: System::Windows::Forms::TabControl^  MainWindowTab;

	private: System::Windows::Forms::TabPage^  tabWeb;
	private: System::Windows::Forms::TabPage^  tabVRRender;
	private: System::Windows::Forms::TabControl^  EditorTab;
	private: System::Windows::Forms::TabPage^  tabEditor;
	private: System::Windows::Forms::TabPage^  tabScene;
	private: System::Windows::Forms::TabPage^  tabObject;
	private: System::Windows::Forms::TabPage^  tabMaterial;
	private: System::Windows::Forms::PropertyGrid^  PropertyEditor;
	private: System::Windows::Forms::PropertyGrid^  PropertScene;
	private: System::Windows::Forms::PropertyGrid^  PropertyObject;
	private: System::Windows::Forms::PropertyGrid^  PropertyMaterial;
	private: System::Windows::Forms::SplitContainer^  MainSpliter;
	private: System::Windows::Forms::TabPage^  tabObjCreator;
	private: System::Windows::Forms::Panel^  ObjCreatorPanel;
	private: System::Windows::Forms::Button^  btnInsertMesh;
	private: System::Windows::Forms::Button^  btnInsertModel;
	private: System::Windows::Forms::TabPage^  tabObjModify;
	private: System::Windows::Forms::Button^  btnInsertLight;


	protected: 


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Windows::Forms::MenuStrip^  ToolsMenu;
			this->�ļ�ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�򿪳���ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�༭����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�رճ���ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�˳�ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�༭ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��ͼToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��Ӱ��ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ͼToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ͼToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ͼToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ͼToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ͼToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��ȾģʽToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�߿�ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��ɫToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�߿���ɫToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��ʼToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ֹͣToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->·��ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->UrlLabel = (gcnew System::Windows::Forms::TextBox());
			this->webBrowser = (gcnew System::Windows::Forms::WebBrowser());
			this->MainWindowTab = (gcnew System::Windows::Forms::TabControl());
			this->tabWeb = (gcnew System::Windows::Forms::TabPage());
			this->tabVRRender = (gcnew System::Windows::Forms::TabPage());
			this->EditorTab = (gcnew System::Windows::Forms::TabControl());
			this->tabEditor = (gcnew System::Windows::Forms::TabPage());
			this->PropertyEditor = (gcnew System::Windows::Forms::PropertyGrid());
			this->tabScene = (gcnew System::Windows::Forms::TabPage());
			this->PropertScene = (gcnew System::Windows::Forms::PropertyGrid());
			this->tabObject = (gcnew System::Windows::Forms::TabPage());
			this->PropertyObject = (gcnew System::Windows::Forms::PropertyGrid());
			this->tabMaterial = (gcnew System::Windows::Forms::TabPage());
			this->PropertyMaterial = (gcnew System::Windows::Forms::PropertyGrid());
			this->MainSpliter = (gcnew System::Windows::Forms::SplitContainer());
			this->tabObjCreator = (gcnew System::Windows::Forms::TabPage());
			this->tabObjModify = (gcnew System::Windows::Forms::TabPage());
			this->ObjCreatorPanel = (gcnew System::Windows::Forms::Panel());
			this->btnInsertModel = (gcnew System::Windows::Forms::Button());
			this->btnInsertMesh = (gcnew System::Windows::Forms::Button());
			this->btnInsertLight = (gcnew System::Windows::Forms::Button());
			ToolsMenu = (gcnew System::Windows::Forms::MenuStrip());
			ToolsMenu->SuspendLayout();
			this->MainWindowTab->SuspendLayout();
			this->tabWeb->SuspendLayout();
			this->EditorTab->SuspendLayout();
			this->tabEditor->SuspendLayout();
			this->tabScene->SuspendLayout();
			this->tabObject->SuspendLayout();
			this->tabMaterial->SuspendLayout();
			this->MainSpliter->Panel1->SuspendLayout();
			this->MainSpliter->Panel2->SuspendLayout();
			this->MainSpliter->SuspendLayout();
			this->tabObjCreator->SuspendLayout();
			this->ObjCreatorPanel->SuspendLayout();
			this->SuspendLayout();
			// 
			// ToolsMenu
			// 
			ToolsMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->�ļ�ToolStripMenuItem, this->�༭ToolStripMenuItem, 
				this->��ͼToolStripMenuItem, this->����ToolStripMenuItem});
			ToolsMenu->Location = System::Drawing::Point(0, 0);
			ToolsMenu->Name = L"ToolsMenu";
			ToolsMenu->Size = System::Drawing::Size(1174, 25);
			ToolsMenu->TabIndex = 0;
			ToolsMenu->Text = L"menuStrip1";
			// 
			// �ļ�ToolStripMenuItem
			// 
			this->�ļ�ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->�򿪳���ToolStripMenuItem, 
				this->�༭����ToolStripMenuItem, this->�رճ���ToolStripMenuItem, this->�˳�ToolStripMenuItem});
			this->�ļ�ToolStripMenuItem->Name = L"�ļ�ToolStripMenuItem";
			this->�ļ�ToolStripMenuItem->Size = System::Drawing::Size(44, 21);
			this->�ļ�ToolStripMenuItem->Text = L"�ļ�";
			// 
			// �򿪳���ToolStripMenuItem
			// 
			this->�򿪳���ToolStripMenuItem->Name = L"�򿪳���ToolStripMenuItem";
			this->�򿪳���ToolStripMenuItem->Size = System::Drawing::Size(124, 22);
			this->�򿪳���ToolStripMenuItem->Text = L"�򿪳���";
			// 
			// �༭����ToolStripMenuItem
			// 
			this->�༭����ToolStripMenuItem->Name = L"�༭����ToolStripMenuItem";
			this->�༭����ToolStripMenuItem->Size = System::Drawing::Size(124, 22);
			this->�༭����ToolStripMenuItem->Text = L"���泡��";
			// 
			// �رճ���ToolStripMenuItem
			// 
			this->�رճ���ToolStripMenuItem->Name = L"�رճ���ToolStripMenuItem";
			this->�رճ���ToolStripMenuItem->Size = System::Drawing::Size(124, 22);
			this->�رճ���ToolStripMenuItem->Text = L"�رճ���";
			// 
			// �˳�ToolStripMenuItem
			// 
			this->�˳�ToolStripMenuItem->Name = L"�˳�ToolStripMenuItem";
			this->�˳�ToolStripMenuItem->Size = System::Drawing::Size(124, 22);
			this->�˳�ToolStripMenuItem->Text = L"�˳�";
			// 
			// �༭ToolStripMenuItem
			// 
			this->�༭ToolStripMenuItem->Name = L"�༭ToolStripMenuItem";
			this->�༭ToolStripMenuItem->Size = System::Drawing::Size(44, 21);
			this->�༭ToolStripMenuItem->Text = L"�༭";
			// 
			// ��ͼToolStripMenuItem
			// 
			this->��ͼToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->��Ӱ��ToolStripMenuItem, 
				this->��ȾģʽToolStripMenuItem});
			this->��ͼToolStripMenuItem->Name = L"��ͼToolStripMenuItem";
			this->��ͼToolStripMenuItem->Size = System::Drawing::Size(44, 21);
			this->��ͼToolStripMenuItem->Text = L"��ͼ";
			// 
			// ��Ӱ��ToolStripMenuItem
			// 
			this->��Ӱ��ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->����ͼToolStripMenuItem, 
				this->����ͼToolStripMenuItem, this->����ͼToolStripMenuItem, this->����ͼToolStripMenuItem, this->����ͼToolStripMenuItem});
			this->��Ӱ��ToolStripMenuItem->Name = L"��Ӱ��ToolStripMenuItem";
			this->��Ӱ��ToolStripMenuItem->Size = System::Drawing::Size(124, 22);
			this->��Ӱ��ToolStripMenuItem->Text = L"��Ӱ��";
			// 
			// ����ͼToolStripMenuItem
			// 
			this->����ͼToolStripMenuItem->Name = L"����ͼToolStripMenuItem";
			this->����ͼToolStripMenuItem->Size = System::Drawing::Size(112, 22);
			this->����ͼToolStripMenuItem->Text = L"����ͼ";
			// 
			// ����ͼToolStripMenuItem
			// 
			this->����ͼToolStripMenuItem->Name = L"����ͼToolStripMenuItem";
			this->����ͼToolStripMenuItem->Size = System::Drawing::Size(112, 22);
			this->����ͼToolStripMenuItem->Text = L"����ͼ";
			// 
			// ����ͼToolStripMenuItem
			// 
			this->����ͼToolStripMenuItem->Name = L"����ͼToolStripMenuItem";
			this->����ͼToolStripMenuItem->Size = System::Drawing::Size(112, 22);
			this->����ͼToolStripMenuItem->Text = L"����ͼ";
			// 
			// ����ͼToolStripMenuItem
			// 
			this->����ͼToolStripMenuItem->Name = L"����ͼToolStripMenuItem";
			this->����ͼToolStripMenuItem->Size = System::Drawing::Size(112, 22);
			this->����ͼToolStripMenuItem->Text = L"����ͼ";
			// 
			// ����ͼToolStripMenuItem
			// 
			this->����ͼToolStripMenuItem->Name = L"����ͼToolStripMenuItem";
			this->����ͼToolStripMenuItem->Size = System::Drawing::Size(112, 22);
			this->����ͼToolStripMenuItem->Text = L"����ͼ";
			// 
			// ��ȾģʽToolStripMenuItem
			// 
			this->��ȾģʽToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->�߿�ToolStripMenuItem, 
				this->��ɫToolStripMenuItem, this->�߿���ɫToolStripMenuItem});
			this->��ȾģʽToolStripMenuItem->Name = L"��ȾģʽToolStripMenuItem";
			this->��ȾģʽToolStripMenuItem->Size = System::Drawing::Size(124, 22);
			this->��ȾģʽToolStripMenuItem->Text = L"��Ⱦģʽ";
			// 
			// �߿�ToolStripMenuItem
			// 
			this->�߿�ToolStripMenuItem->Name = L"�߿�ToolStripMenuItem";
			this->�߿�ToolStripMenuItem->Size = System::Drawing::Size(133, 22);
			this->�߿�ToolStripMenuItem->Text = L"�߿�";
			// 
			// ��ɫToolStripMenuItem
			// 
			this->��ɫToolStripMenuItem->Name = L"��ɫToolStripMenuItem";
			this->��ɫToolStripMenuItem->Size = System::Drawing::Size(133, 22);
			this->��ɫToolStripMenuItem->Text = L"��ɫ";
			// 
			// �߿���ɫToolStripMenuItem
			// 
			this->�߿���ɫToolStripMenuItem->Name = L"�߿���ɫToolStripMenuItem";
			this->�߿���ɫToolStripMenuItem->Size = System::Drawing::Size(133, 22);
			this->�߿���ɫToolStripMenuItem->Text = L"�߿�+��ɫ";
			// 
			// ����ToolStripMenuItem
			// 
			this->����ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->��ʼToolStripMenuItem, 
				this->ֹͣToolStripMenuItem, this->·��ToolStripMenuItem});
			this->����ToolStripMenuItem->Name = L"����ToolStripMenuItem";
			this->����ToolStripMenuItem->Size = System::Drawing::Size(44, 21);
			this->����ToolStripMenuItem->Text = L"����";
			// 
			// ��ʼToolStripMenuItem
			// 
			this->��ʼToolStripMenuItem->Name = L"��ʼToolStripMenuItem";
			this->��ʼToolStripMenuItem->Size = System::Drawing::Size(100, 22);
			this->��ʼToolStripMenuItem->Text = L"��ʼ";
			// 
			// ֹͣToolStripMenuItem
			// 
			this->ֹͣToolStripMenuItem->Name = L"ֹͣToolStripMenuItem";
			this->ֹͣToolStripMenuItem->Size = System::Drawing::Size(100, 22);
			this->ֹͣToolStripMenuItem->Text = L"ֹͣ";
			// 
			// ·��ToolStripMenuItem
			// 
			this->·��ToolStripMenuItem->Name = L"·��ToolStripMenuItem";
			this->·��ToolStripMenuItem->Size = System::Drawing::Size(100, 22);
			this->·��ToolStripMenuItem->Text = L"·��";
			// 
			// UrlLabel
			// 
			this->UrlLabel->Dock = System::Windows::Forms::DockStyle::Top;
			this->UrlLabel->Location = System::Drawing::Point(0, 25);
			this->UrlLabel->Name = L"UrlLabel";
			this->UrlLabel->Size = System::Drawing::Size(1174, 21);
			this->UrlLabel->TabIndex = 1;
			this->UrlLabel->TextChanged += gcnew System::EventHandler(this, &MainWindow::UrlLabel_TextChanged);
			this->UrlLabel->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MainWindow::UrlLabel_KeyDown);
			// 
			// webBrowser
			// 
			this->webBrowser->Dock = System::Windows::Forms::DockStyle::Fill;
			this->webBrowser->Location = System::Drawing::Point(3, 3);
			this->webBrowser->MinimumSize = System::Drawing::Size(20, 20);
			this->webBrowser->Name = L"webBrowser";
			this->webBrowser->Size = System::Drawing::Size(858, 522);
			this->webBrowser->TabIndex = 3;
			this->webBrowser->Visible = false;
			// 
			// MainWindowTab
			// 
			this->MainWindowTab->Controls->Add(this->tabWeb);
			this->MainWindowTab->Controls->Add(this->tabVRRender);
			this->MainWindowTab->Dock = System::Windows::Forms::DockStyle::Fill;
			this->MainWindowTab->Location = System::Drawing::Point(0, 0);
			this->MainWindowTab->Name = L"MainWindowTab";
			this->MainWindowTab->SelectedIndex = 0;
			this->MainWindowTab->Size = System::Drawing::Size(872, 554);
			this->MainWindowTab->TabIndex = 4;
			this->MainWindowTab->SelectedIndexChanged += gcnew System::EventHandler(this, &MainWindow::MainWindowTab_SelectedIndexChanged);
			// 
			// tabWeb
			// 
			this->tabWeb->BackColor = System::Drawing::Color::Black;
			this->tabWeb->Controls->Add(this->webBrowser);
			this->tabWeb->Location = System::Drawing::Point(4, 22);
			this->tabWeb->Name = L"tabWeb";
			this->tabWeb->Padding = System::Windows::Forms::Padding(3);
			this->tabWeb->Size = System::Drawing::Size(864, 528);
			this->tabWeb->TabIndex = 0;
			this->tabWeb->Text = L"�����";
			this->tabWeb->UseVisualStyleBackColor = true;
			// 
			// tabVRRender
			// 
			this->tabVRRender->Location = System::Drawing::Point(4, 22);
			this->tabVRRender->Name = L"tabVRRender";
			this->tabVRRender->Padding = System::Windows::Forms::Padding(3);
			this->tabVRRender->Size = System::Drawing::Size(773, 528);
			this->tabVRRender->TabIndex = 1;
			this->tabVRRender->Text = L"����";
			this->tabVRRender->UseVisualStyleBackColor = true;
			this->tabVRRender->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainWindow::tabVRRender_MouseMove);
			// 
			// EditorTab
			// 
			this->EditorTab->Controls->Add(this->tabObjCreator);
			this->EditorTab->Controls->Add(this->tabObjModify);
			this->EditorTab->Controls->Add(this->tabEditor);
			this->EditorTab->Controls->Add(this->tabScene);
			this->EditorTab->Controls->Add(this->tabObject);
			this->EditorTab->Controls->Add(this->tabMaterial);
			this->EditorTab->Dock = System::Windows::Forms::DockStyle::Fill;
			this->EditorTab->Location = System::Drawing::Point(0, 0);
			this->EditorTab->Name = L"EditorTab";
			this->EditorTab->SelectedIndex = 0;
			this->EditorTab->Size = System::Drawing::Size(298, 554);
			this->EditorTab->TabIndex = 5;
			// 
			// tabEditor
			// 
			this->tabEditor->Controls->Add(this->PropertyEditor);
			this->tabEditor->Location = System::Drawing::Point(4, 22);
			this->tabEditor->Name = L"tabEditor";
			this->tabEditor->Padding = System::Windows::Forms::Padding(3);
			this->tabEditor->Size = System::Drawing::Size(381, 528);
			this->tabEditor->TabIndex = 0;
			this->tabEditor->Text = L"�༭��";
			this->tabEditor->UseVisualStyleBackColor = true;
			// 
			// PropertyEditor
			// 
			this->PropertyEditor->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PropertyEditor->Location = System::Drawing::Point(3, 3);
			this->PropertyEditor->Name = L"PropertyEditor";
			this->PropertyEditor->Size = System::Drawing::Size(375, 522);
			this->PropertyEditor->TabIndex = 0;
			// 
			// tabScene
			// 
			this->tabScene->Controls->Add(this->PropertScene);
			this->tabScene->Location = System::Drawing::Point(4, 22);
			this->tabScene->Name = L"tabScene";
			this->tabScene->Padding = System::Windows::Forms::Padding(3);
			this->tabScene->Size = System::Drawing::Size(381, 528);
			this->tabScene->TabIndex = 1;
			this->tabScene->Text = L"����";
			this->tabScene->UseVisualStyleBackColor = true;
			// 
			// PropertScene
			// 
			this->PropertScene->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PropertScene->Location = System::Drawing::Point(3, 3);
			this->PropertScene->Name = L"PropertScene";
			this->PropertScene->Size = System::Drawing::Size(375, 522);
			this->PropertScene->TabIndex = 1;
			// 
			// tabObject
			// 
			this->tabObject->Controls->Add(this->PropertyObject);
			this->tabObject->Location = System::Drawing::Point(4, 22);
			this->tabObject->Name = L"tabObject";
			this->tabObject->Size = System::Drawing::Size(381, 528);
			this->tabObject->TabIndex = 2;
			this->tabObject->Text = L"����";
			this->tabObject->UseVisualStyleBackColor = true;
			// 
			// PropertyObject
			// 
			this->PropertyObject->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PropertyObject->Location = System::Drawing::Point(0, 0);
			this->PropertyObject->Name = L"PropertyObject";
			this->PropertyObject->Size = System::Drawing::Size(381, 528);
			this->PropertyObject->TabIndex = 2;
			// 
			// tabMaterial
			// 
			this->tabMaterial->Controls->Add(this->PropertyMaterial);
			this->tabMaterial->Location = System::Drawing::Point(4, 22);
			this->tabMaterial->Name = L"tabMaterial";
			this->tabMaterial->Size = System::Drawing::Size(381, 528);
			this->tabMaterial->TabIndex = 3;
			this->tabMaterial->Text = L"����";
			this->tabMaterial->UseVisualStyleBackColor = true;
			// 
			// PropertyMaterial
			// 
			this->PropertyMaterial->Dock = System::Windows::Forms::DockStyle::Fill;
			this->PropertyMaterial->Location = System::Drawing::Point(0, 0);
			this->PropertyMaterial->Name = L"PropertyMaterial";
			this->PropertyMaterial->Size = System::Drawing::Size(381, 528);
			this->PropertyMaterial->TabIndex = 2;
			// 
			// MainSpliter
			// 
			this->MainSpliter->Dock = System::Windows::Forms::DockStyle::Fill;
			this->MainSpliter->Location = System::Drawing::Point(0, 46);
			this->MainSpliter->Name = L"MainSpliter";
			// 
			// MainSpliter.Panel1
			// 
			this->MainSpliter->Panel1->Controls->Add(this->MainWindowTab);
			// 
			// MainSpliter.Panel2
			// 
			this->MainSpliter->Panel2->Controls->Add(this->EditorTab);
			this->MainSpliter->Panel2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainWindow::splitContainer1_Panel2_Paint);
			this->MainSpliter->Size = System::Drawing::Size(1174, 554);
			this->MainSpliter->SplitterDistance = 872;
			this->MainSpliter->TabIndex = 6;
			// 
			// tabObjCreator
			// 
			this->tabObjCreator->Controls->Add(this->ObjCreatorPanel);
			this->tabObjCreator->Location = System::Drawing::Point(4, 22);
			this->tabObjCreator->Name = L"tabObjCreator";
			this->tabObjCreator->Size = System::Drawing::Size(290, 528);
			this->tabObjCreator->TabIndex = 4;
			this->tabObjCreator->Text = L"����";
			this->tabObjCreator->UseVisualStyleBackColor = true;
			// 
			// tabObjModify
			// 
			this->tabObjModify->Location = System::Drawing::Point(4, 22);
			this->tabObjModify->Name = L"tabObjModify";
			this->tabObjModify->Size = System::Drawing::Size(290, 528);
			this->tabObjModify->TabIndex = 5;
			this->tabObjModify->Text = L"�޸�";
			this->tabObjModify->UseVisualStyleBackColor = true;
			// 
			// ObjCreatorPanel
			// 
			this->ObjCreatorPanel->Controls->Add(this->btnInsertLight);
			this->ObjCreatorPanel->Controls->Add(this->btnInsertMesh);
			this->ObjCreatorPanel->Controls->Add(this->btnInsertModel);
			this->ObjCreatorPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ObjCreatorPanel->Location = System::Drawing::Point(0, 0);
			this->ObjCreatorPanel->Name = L"ObjCreatorPanel";
			this->ObjCreatorPanel->Size = System::Drawing::Size(290, 528);
			this->ObjCreatorPanel->TabIndex = 0;
			// 
			// btnInsertModel
			// 
			this->btnInsertModel->Location = System::Drawing::Point(163, 19);
			this->btnInsertModel->Name = L"btnInsertModel";
			this->btnInsertModel->Size = System::Drawing::Size(118, 36);
			this->btnInsertModel->TabIndex = 0;
			this->btnInsertModel->Text = L"���ģ��";
			this->btnInsertModel->UseVisualStyleBackColor = true;
			// 
			// btnInsertMesh
			// 
			this->btnInsertMesh->Location = System::Drawing::Point(12, 19);
			this->btnInsertMesh->Name = L"btnInsertMesh";
			this->btnInsertMesh->Size = System::Drawing::Size(118, 36);
			this->btnInsertMesh->TabIndex = 0;
			this->btnInsertMesh->Text = L"�������";
			this->btnInsertMesh->UseVisualStyleBackColor = true;
			// 
			// btnInsertLight
			// 
			this->btnInsertLight->Location = System::Drawing::Point(13, 61);
			this->btnInsertLight->Name = L"btnInsertLight";
			this->btnInsertLight->Size = System::Drawing::Size(118, 36);
			this->btnInsertLight->TabIndex = 0;
			this->btnInsertLight->Text = L"��ӹ�Դ";
			this->btnInsertLight->UseVisualStyleBackColor = true;
			// 
			// MainWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1174, 600);
			this->Controls->Add(this->MainSpliter);
			this->Controls->Add(this->UrlLabel);
			this->Controls->Add(ToolsMenu);
			this->MainMenuStrip = ToolsMenu;
			this->Name = L"MainWindow";
			this->Text = L"��ˢ�� �༭�� V1.0";
			this->Load += gcnew System::EventHandler(this, &MainWindow::MainWindow_Load);
			this->Resize += gcnew System::EventHandler(this, &MainWindow::MainWindow_Resize);
			this->ResizeEnd += gcnew System::EventHandler(this, &MainWindow::MainWindow_ResizeEnd);
			this->MaximumSizeChanged += gcnew System::EventHandler(this, &MainWindow::MainWindow_MaximumSizeChanged);
			ToolsMenu->ResumeLayout(false);
			ToolsMenu->PerformLayout();
			this->MainWindowTab->ResumeLayout(false);
			this->tabWeb->ResumeLayout(false);
			this->EditorTab->ResumeLayout(false);
			this->tabEditor->ResumeLayout(false);
			this->tabScene->ResumeLayout(false);
			this->tabObject->ResumeLayout(false);
			this->tabMaterial->ResumeLayout(false);
			this->MainSpliter->Panel1->ResumeLayout(false);
			this->MainSpliter->Panel2->ResumeLayout(false);
			this->MainSpliter->ResumeLayout(false);
			this->tabObjCreator->ResumeLayout(false);
			this->ObjCreatorPanel->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		int m_PosY ;
		//��Ϣ��Ӧ����
	private: 
		System::Void UpdateTabControl()
		{
		}
		System::Void MainWindow_Load(System::Object^  sender, System::EventArgs^  e) 
		{
			m_VirtualCommunity = gcnew xVirtualCmtyEnv();	
			m_Cavans = this->tabVRRender;
			m_VirtualCommunity->Init( (int)m_Cavans->Handle);
			m_VirtualCommunity->OnResize(m_Cavans->Width , m_Cavans->Height);
            m_PosY = MainWindowTab->Top;
            UpdateTabControl();
			Application::Idle += gcnew System::EventHandler(this , &MainWindow::OnIdle)  ;
		}

		System::Void  OnIdle(System::Object^ sender, System::EventArgs^ e)
		{
			m_VirtualCommunity->OnIdle();
		}

		System::Void UrlLabel_TextChanged(System::Object^  sender, System::EventArgs^  e) 
		{

		}

		System::Void UrlLabel_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
		{
			if(e->KeyCode == Keys::Return)
			{
				String^ url = gcnew String(UrlLabel->Text);
				url->ToLower();
				if(url->StartsWith(L"http") || url->StartsWith(L"www."))
				{
					this->webBrowser->Navigate(url);
					webBrowser->Show();
				}
				else if(url->StartsWith(L"vrm://"))
				{
				}

			}
		}

		System::Void MainWindow_ResizeEnd(System::Object^  sender, System::EventArgs^  e) 
		{
			m_VirtualCommunity->OnResize(m_Cavans->Width , m_Cavans->Height);
		}

		System::Void MainWindow_MaximumSizeChanged(System::Object^  sender, System::EventArgs^  e) 
		{
			m_VirtualCommunity->OnResize(m_Cavans->Width , m_Cavans->Height);
		}

		System::Void MainWindow_Resize(System::Object^  sender, System::EventArgs^  e) 
		{
			m_VirtualCommunity->OnResize(m_Cavans->Width , m_Cavans->Height);
		}
	
		System::Void tabVRRender_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		{

		}

		System::Void MainWindowTab_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 UpdateTabControl();
		 }

		System::Void splitContainer1_Panel2_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
		 }
};
}

