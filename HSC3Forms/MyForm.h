#pragma once
#include "hitsounds.h"
#include <msclr\marshal_cppstd.h>

namespace HSC3Forms {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
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
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  browse1;
	protected: 
	private: System::Windows::Forms::Button^  browse2;
	private: System::Windows::Forms::TextBox^  copyfromBox;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  copytoBox;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  copyBtn;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::CheckBox^  checkBox2;
	private: System::Windows::Forms::CheckBox^  checkBox3;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;

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
			this->browse1 = (gcnew System::Windows::Forms::Button());
			this->browse2 = (gcnew System::Windows::Forms::Button());
			this->copyfromBox = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->copytoBox = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->copyBtn = (gcnew System::Windows::Forms::Button());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox2 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox3 = (gcnew System::Windows::Forms::CheckBox());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->SuspendLayout();
			// 
			// browse1
			// 
			this->browse1->Location = System::Drawing::Point(277, 23);
			this->browse1->Name = L"browse1";
			this->browse1->Size = System::Drawing::Size(75, 23);
			this->browse1->TabIndex = 0;
			this->browse1->Text = L"Browse";
			this->browse1->UseVisualStyleBackColor = true;
			this->browse1->Click += gcnew System::EventHandler(this, &MyForm::browse1_Click);
			// 
			// browse2
			// 
			this->browse2->Location = System::Drawing::Point(277, 64);
			this->browse2->Name = L"browse2";
			this->browse2->Size = System::Drawing::Size(75, 23);
			this->browse2->TabIndex = 1;
			this->browse2->Text = L"Browse";
			this->browse2->UseVisualStyleBackColor = true;
			this->browse2->Click += gcnew System::EventHandler(this, &MyForm::browse2_Click);
			// 
			// copyfromBox
			// 
			this->copyfromBox->Location = System::Drawing::Point(12, 25);
			this->copyfromBox->Name = L"copyfromBox";
			this->copyfromBox->Size = System::Drawing::Size(259, 20);
			this->copyfromBox->TabIndex = 2;
			this->copyfromBox->Text = L"D:\\games\\osu\\Songs\\54624 Hecq - Dstrukt\\Hecq - Dstrukt (grumd) [Insane].osu";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(54, 13);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Copy from";
			// 
			// copytoBox
			// 
			this->copytoBox->Location = System::Drawing::Point(12, 66);
			this->copytoBox->Name = L"copytoBox";
			this->copytoBox->Size = System::Drawing::Size(259, 20);
			this->copytoBox->TabIndex = 4;
			this->copytoBox->Text = L"D:\\games\\osu\\Songs\\54624 Hecq - Dstrukt\\Hecq - Dstrukt (grumd) [new].osu";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 50);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(43, 13);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Copy to";
			// 
			// copyBtn
			// 
			this->copyBtn->Location = System::Drawing::Point(214, 93);
			this->copyBtn->Name = L"copyBtn";
			this->copyBtn->Size = System::Drawing::Size(138, 63);
			this->copyBtn->TabIndex = 6;
			this->copyBtn->Text = L"Copy hitsounds";
			this->copyBtn->UseVisualStyleBackColor = true;
			this->copyBtn->Click += gcnew System::EventHandler(this, &MyForm::copyBtn_Click);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(12, 93);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(165, 17);
			this->checkBox1->TabIndex = 7;
			this->checkBox1->Text = L"Copy HS volume (green lines)";
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// checkBox2
			// 
			this->checkBox2->AutoSize = true;
			this->checkBox2->Location = System::Drawing::Point(12, 116);
			this->checkBox2->Name = L"checkBox2";
			this->checkBox2->Size = System::Drawing::Size(155, 17);
			this->checkBox2->TabIndex = 8;
			this->checkBox2->Text = L"Copy storyboarded samples";
			this->checkBox2->UseVisualStyleBackColor = true;
			// 
			// checkBox3
			// 
			this->checkBox3->AutoSize = true;
			this->checkBox3->Location = System::Drawing::Point(12, 139);
			this->checkBox3->Name = L"checkBox3";
			this->checkBox3->Size = System::Drawing::Size(95, 17);
			this->checkBox3->TabIndex = 9;
			this->checkBox3->Text = L"Save backups";
			this->checkBox3->UseVisualStyleBackColor = true;
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->Filter = L".osu files|*.osu";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(364, 165);
			this->Controls->Add(this->checkBox3);
			this->Controls->Add(this->checkBox2);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->copyBtn);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->copytoBox);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->copyfromBox);
			this->Controls->Add(this->browse2);
			this->Controls->Add(this->browse1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void browse1_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				   {
					   copyfromBox->Text = openFileDialog1->FileName;
				   }
			 }
private: System::Void browse2_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				   {
					   copytoBox->Text = openFileDialog1->FileName;
				   }
		 }
private: System::Void copyBtn_Click(System::Object^  sender, System::EventArgs^  e) {
			Hitsounds HS = Hitsounds();
			HS.CopyGreens = checkBox1->Checked;
			HS.CopySamples = checkBox2->Checked;
			HS.KeepBackup = checkBox3->Checked;
			string filenameFrom = msclr::interop::marshal_as< std::string >(copyfromBox->Text);
			string filenameTo = msclr::interop::marshal_as< std::string >(copytoBox->Text);
			string filenameToOld = filenameTo;
			HS.ReadFile(filenameFrom);
			//MessageBox::Show("Done!", "Done", MessageBoxButtons::OK, MessageBoxIcon::Information);
			filenameTo = filenameTo.substr(0, filenameTo.find_last_of('.'));
			filenameTo.append("_old.osu");
			rename(filenameToOld.c_str(), filenameTo.c_str());
			HS.WriteFile(filenameTo, filenameToOld);
			MessageBox::Show("Done!", "Done", MessageBoxButtons::OK, MessageBoxIcon::Information);
		 }
};
}
