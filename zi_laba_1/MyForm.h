#pragma once
#include <iostream>
#include <random>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <cmath>
#include <stdlib.h> 
#include <thread>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath> 
#include <stdio.h>
#include <optional>
#include <algorithm>
namespace zilaba1 {

	using namespace System;
	using namespace System::Diagnostics;
	using namespace System::ComponentModel;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;


	double PCFreq = 0.0;
	__int64 CounterStart = 0;

	struct Info {
		Info(double left_side, double right_side, vector<double> values) {
			this->left_side = left_side;
			this->right_side = right_side;
			this->values = values;
		}
		double left_side;
		double right_side;
		vector<double> values;
		
	};
	// Points
	const int nPoinst = 200;
	const int halfOfNPoints = nPoinst / 2;
	float points[nPoinst] = {
	};

	/// <summary>
	/// Сводка для MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		double gen(double mn, double mx) {
			int dx = mx - mn;
			double accur = 1e3;
			int fmx = dx * accur;
			double t = rand() % fmx;
			t /= accur;
			return mn + t;
		}
		double genM(double mn, double mx,double match ){
			random_device rd;   // non-deterministic generator
			std::mt19937 mt_rand(rd());
			uniform_real_distribution<> dist(mn, mx); // distribute results between 1 and 6 inclusive.
		/*	int dx = mx - mn;
			double accur = 1e3;
			int fmx = dx * accur;
			double t = mt_rand() % fmx;
			t /= accur;*/
			return (double)dist(mt_rand);
		}
		void StartCounter()
		{

			LARGE_INTEGER li;
			if (!QueryPerformanceFrequency(&li))
				std::cout << "QueryPerformanceFrequency failed!\n";

			PCFreq = double(li.QuadPart);

			QueryPerformanceCounter(&li);
			CounterStart = li.QuadPart;
		}
		double GetCounter()
		{
			LARGE_INTEGER li;
			QueryPerformanceCounter(&li);
			return double(li.QuadPart - CounterStart) / PCFreq;
		}
		MyForm(void)
		{
			InitializeComponent();
			
			std::ofstream f;
			
			f.open("C://state/state.txt", std::ofstream::out | std::ofstream::trunc);
			if (!f)
			{
				cout << "error!";
			}
			f << "1.1. Используя<random> сгенрировать  случайную последовательность \n\
        из 100 вещественных  чисел (с плавающей запятой) на основе Вихря Мерсенна с   \n\
        заданным начальным значением" << "\n";
			std::mt19937 mt_rand(1);
			float ptrarray[100];
			for (int count = 0; count < 100; count++) {
				ptrarray[count] = genM(0,100,1);
			}
			int countCout = 0;
			for (int j = 0; j < 20; j++) {
				for (int k = 0; k < 5; k++) {
					f  << ptrarray[countCout] << "\t";
					countCout++;

				}
				f << "\n";
			}
			f << "1.2. Используя<random> сгенрировать  случайную последовательность из 100 случайных вещественных \n\
        чисел (с плавающей запятой) на основе Вихря Мерсенна с   недетерминистическим начальным значением. \n\
        Оценить время генерации. Рассчитать для последовательности среднее и дисперсию. Построить гистограмму \n\
        для 10 подотрезков. Отобразить в координатах XY значения сгенерированных последовательностей.  \n";
			float sum = 0;
			double matOj = 0;
			double disp = 0;
			StartCounter();
		//	std::mt19937 generator(gen(0, 100));
			for (int i = 0; i < 100; i++) {
				ptrarray[i] = genM(0, 100, rand());
				sum += ptrarray[i];
			}
			countCout = 0;
			const double sec = GetCounter();
			for (int j = 0; j < 20; j++) {
				for (int k = 0; k < 5; k++) {
					f  << ptrarray[countCout] << "\t";
					countCout++;
				}
				f << "\n";
			}
			f << "Время генерации: " << "\t" << fixed << (float)sec << "\n";
			for (int i = 0; i < 100; i++)
				matOj = ptrarray[i] + matOj;
			matOj = matOj / 100; //Математическое ожидание

			for (int i = 0; i < 100; i++)
				disp = pow((ptrarray[i] - matOj), 2) + disp;
			disp = disp / 99; //Дисперсия
			f << "Среднее:" << "\t" << sum / 100 << "\n";
			f << "Дисперсия:" << "\t" << disp << "\n";
			//вынести в конец
			//--
			ProcessStartInfo^ startInfo = gcnew ProcessStartInfo("notepad.exe");
			//startInfo->WindowStyle = ProcessWindowStyle::Minimized;
			//Process::Start(startInfo);
			startInfo->Arguments = "C://state/state.txt";
			Process::Start(startInfo);
			//--
			std:pair<float*,float*> minmax = std::minmax_element(std::begin(ptrarray), std::end(ptrarray));
			double minPtArray = (double)*(minmax.first);
			double maxPtArray = (double)*(minmax.second);
			double step = (maxPtArray - minPtArray) / 10;
			f << "min element " << minPtArray << "\n";
			f << "max element " << maxPtArray << "\n";
			f << "step " << step << "\n";
			vector<Info*> infos;
			for (int i = 0; i < 10; i++) {
				vector<double> vector;
				double left_side = minPtArray + (step * i);
				double right_side = left_side + step ;
				Info *info = new Info(left_side, right_side, vector);
				/*left_side = right_side;
				right_side = left_side*/
				infos.push_back(info);
			}
			for (int i = 0; i < 10; i++) {
				Info *info  = infos[i];
				double left_side = info->left_side;
				double right_side = info->right_side;
				for (int j = 0; j < 100; j++) {
					double point = ptrarray[j];
					if (left_side <= point  && point < right_side)
						info->values.push_back(point);
					
				}
			}
			for (int i = 0; i < 10; i++) {
				Info info = *infos[i];
				f << i << " отрезок: \t" << "[" << info.left_side << " : " << info.right_side << "]" << " : ";
				for (int a = 0; a < infos[i]->values.size(); a++) {
					f  << infos[i]->values[a] << " ";
				}
				f << "\n";

					this->chart1->Series[0]->Points->AddXY(i, infos[i]->values.size());
			}
			
			f.close();
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::DataVisualization::Charting::Chart^ chart1;
	protected:

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^ series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->BeginInit();
			this->SuspendLayout();
			// 
			// chart1
			// 
			chartArea1->Name = L"ChartArea1";
			this->chart1->ChartAreas->Add(chartArea1);
			legend1->Name = L"Legend1";
			this->chart1->Legends->Add(legend1);
			this->chart1->Location = System::Drawing::Point(12, 12);
			this->chart1->Name = L"chart1";
			series1->ChartArea = L"ChartArea1";
			series1->Legend = L"Legend1";
			series1->Name = L"Series1";
			this->chart1->Series->Add(series1);
			this->chart1->Size = System::Drawing::Size(1004, 475);
			this->chart1->TabIndex = 0;
			this->chart1->Text = L"chart1";
			this->chart1->Click += gcnew System::EventHandler(this, &MyForm::chart1_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1043, 499);
			this->Controls->Add(this->chart1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void chart1_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	};
}
