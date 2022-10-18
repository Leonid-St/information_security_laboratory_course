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
#include <map>
#include <type_traits>
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

	template <typename T>
	struct Info {
		Info(T left_side, T right_side, vector<T> values) {
			this->left_side = left_side;
			this->right_side = right_side;
			this->values = values;
		}
		T left_side;
		T right_side;
		vector<T> values;

	};

	template <typename T>
	struct return_val_fun_gen {
		return_val_fun_gen(T arr[], T count) {
			this->count = count
			this->arr = arr;
		}
		T arr[];
		T count;
	};
	struct seed_t
	{
		unsigned x = 1; // начальные значения могут быть любыми
		unsigned y = 123;
		unsigned z = 456;
		unsigned w = 768;
	};


	/// <summary>
	/// Сводка для MyForm
	/// </summary>
	public ref class Laba1 : public System::Windows::Forms::Form
	{
	public:
		unsigned XorShift128()
		{
			static seed_t s;
			unsigned t = s.x ^ (s.x << 11);
			s.x = s.y;
			s.y = s.z;
			s.z = s.w;
			s.w = (s.w ^ (s.w >> 19)) ^ (t ^ (t >> 8));
			return s.w;
		}
		double gen(double mn, double mx) {
			int dx = mx - mn;
			double accur = 1e3;
			int fmx = dx * accur;
			double t = rand() % fmx;
			t /= accur;
			return mn + t;
		}
		void GaussDistribution(const double m, const double s, const int samples, ofstream *f) {

			//uncomment to use a non-deterministic seed
			random_device rd;
			mt19937 gen(rd());
			//mt19937 gen(1701);

			normal_distribution <> distr(m, s);

			*f << endl;
			*f << "m() == " << fixed << setw(11) << right << setprecision(10) << distr.mean() << endl;
			*f << "s() == " << fixed << setw(11) << right << setprecision(10) << distr.stddev() << endl;
			for (int i = 0; i < 100; i++) {
				if (i % 5 == 0) {
					*f << "\n";
				}
				*f << fixed << setw(11) <<  left << distr(gen)<< " \t";
			}
			*f << endl;
		}
		unsigned PRNG()
		{
			static unsigned seed = 1; // зерно не должно быть 0
			seed = (seed * 73129 + 95121) % 100000;
			return seed;
		}

		double genM(double mn, double mx, double match) {
			random_device rd;   // non-deterministic generator
			std::mt19937 mt_rand(rd());
			uniform_real_distribution<> dist(mn, mx); 
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
		template <typename T>
		void time_avarage_dispersion(T arr[], int countArr, double time_gen, ofstream* f) {
			float sum = 0;
			double matOj = 0;
			double disp = 0;
			for (int j = 0; j < countArr; j++) {
				if (j % 5 == 0) {
					*f << " \n";
				}
				*f << arr[j] << "\t";

			}
			*f << "\n";
			*f << "Время генерации: " << "\t" << fixed << (float)time_gen << "\n";
			for (int i = 0; i < countArr; i++)
				matOj = arr[i] + matOj;
			matOj = matOj / countArr; //Математическое ожидание
			float average = matOj;
			for (int i = 0; i < countArr; i++)
				disp = pow((arr[i] - matOj), 2) + disp;
			disp = disp / countArr-1; //Дисперсия
			*f << "Среднее:" << "\t" << average << "\n";
			*f << "Дисперсия:" << "\t" << disp << "\n";
		}
		template <typename T>
		void HISTOGRAMMA_DRAW(T ptr[],
			int countArray,
			System::Windows::Forms::DataVisualization::Charting::Chart^ chart,
			ofstream *f,
			System::Drawing::Color color) {
			/*float *array = new float[countArray];
			for (int i = 0; i < countArray; i++) {
				array[i] = arr[i];
			}*/
			//double* ptr;
			double min, max;
			double temp;
			min = ptr[0];
			for (int i = 1; i < countArray; i++)
				if (min > ptr[i])
				{
					temp = ptr[i];
					ptr[i] = min;
					min = temp;
				}

			max = ptr[0];
			for (int i = 1; i < countArray; i++)
				if (max < ptr[i])
				{
					temp = ptr[i];
					ptr[i] = max;
					max = temp;
				}

			T step = (max - min) / 10;
			*f << "min element " << min << "\n";
			*f << "max element " << max << "\n";
			*f << "step " << step << "\n";
			vector<Info<T>*> infos;
			for (int i = 0; i < 10; i++) {
				vector<T> vector;
				T left_side = min + (step * i);
				T right_side = left_side + step;
				Info<T>* info = new Info<T>(left_side, right_side, vector);
				infos.push_back(info);
			}
			for (int i = 0; i < 10; i++) {
				Info<T>* info = infos[i];
				double left_side = info->left_side;
				double right_side = info->right_side;
				for (int j = 0; j < 100; j++) {
					double point = ptr[j];
					if (left_side <= point && point < right_side)
						info->values.push_back(point);

				}
			}

bool fixInt;

if (std::is_same<T, int>::value) {
	fixInt = true;
}
else {
	fixInt = false;
}
	

			for (int i = 0; i < 10; i++) {
				Info<T> info = *infos[i];
				fixInt ?
					*f << i  << " отрезок: \t" << "[" << (int)info.left_side << " : " << (int)info.right_side << "]" << " : "
					: *f << i << " отрезок: \t" << "[" << info.left_side << " : " << info.right_side << "]" << " : ";
				for (int a = 0; a < infos[i]->values.size(); a++) {
					fixInt ? *f  << (int)infos[i]->values[a] << " " : *f << infos[i]->values[a] << " ";
				}
				*f << "\n";

				chart->Series[0]->Points->AddXY(i, infos[i]->values.size());
				/*this->chart1->Series[0]->Points[i]->Color = System::Drawing::Color::Red;
				this->chart1->Legends[0]->ForeColor = System::Drawing::Color::Red;*/
				chart->Series[0]->Color = color;
			}
		}
		Laba1(void)
		{

			InitializeComponent();

			//1. Генерация равномернораспеделенных   последовательностей

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
			float ptarray[100];
			for (int count = 0; count < 100; count++) {
				ptarray[count] = genM(0, 100, 1);
			}
			int countCout = 0;
			for (int j = 0; j < 20; j++) {
				for (int k = 0; k < 5; k++) {
					f << ptarray[countCout] << "\t";
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
			for (int i = 0; i < 100; i++) {
			ptarray[i] = genM(0, 100, rand());
				sum += ptarray[i];
			}
			countCout = 0;
			const double sec12 = GetCounter();
			time_avarage_dispersion<float>(ptarray, 100, sec12, &f);
			
			ProcessStartInfo^ startInfo = gcnew ProcessStartInfo("notepad.exe");
			//startInfo->WindowStyle = ProcessWindowStyle::Minimized;
			//Process::Start(startInfo);
			startInfo->Arguments = "C://state/state.txt";
			Process::Start(startInfo);
	
			HISTOGRAMMA_DRAW<float>(ptarray, 100, this->chart1,&f, System::Drawing::Color::Red);
			f << "1.3.  Сгенрировать последовательность из 100 чисел  случайных вещественных  \
				\n чисел (с плавающей запятой) по линейному конгруэнтному агоритму.  \n";
			
			const double lower_bound = 0;
			const double upper_bound = 100;
			uniform_real_distribution<double> unif(lower_bound, upper_bound);
			random_device rand_dev;

			linear_congruential_engine<std::uint_fast32_t, 16807, 0, 2147483647> LN(rand_dev());//(rand_dev());

			for (int i = 0; i < 100; i++) {
				if ((i+1) % 5 == 0) {
				f <<  "\n";
				}
				double x = unif(LN);
				f << x << "\t";
			}
			//2.Генерация нормально распределеный последовательностей
			f << "\n";
			f << "2.1. Сформировать последовательность из 100  нормально (Гауссово) \n \
						распределених целых чисел в диапазоне от 0 до 5000. \n";
			GaussDistribution(0, 5000, 100,&f);
			f << "3. Сформировать дискретное распределение 20 целых чисел. \n";
			std::mt19937 generator;
			double mean = 0.0;
			double stddev = 1.0;
			std::normal_distribution<double> normal(mean, stddev);
			f << "Normal: " << normal(generator) << endl;
			for (int i = 0; i < 20; i++) {
				if (i % 10 == 0) {
					f << "\n";
				}
				f  << normal(generator) << endl;
			}
		
			f << "4. Сфомировать последовательность из 1000 случайных величин, использовать алгоритм PRNG.\n\
			Оценить время генерации. Рассчитать для последовательности среднее и дисперсию.\n\
			Построить гистограмму для 10 подотрезков. Отобразить в координатах XY значения сгенерированных последовательностей. \n ";
			const  int clobalCount = 1000;
			int prngV[clobalCount];
			StartCounter();
			for (int i = 0; i < clobalCount; i++) {
				prngV[i] = PRNG();
			}
			const double sec4 = GetCounter();
			time_avarage_dispersion<int>(prngV, clobalCount, sec4, &f);
			HISTOGRAMMA_DRAW<int>(prngV, 10, this->chart2, &f, System::Drawing::Color::Green);
			f << "5. Сфомировать последовательность из 1000 случайных вещественных величин,\n\
			использовать  XorShift. Оценить время генерации. Рассчитать для последовательности среднее и дисперсию.\n\
			Построить гистограмму для 10 подотрезков. Отобразить в координатах XY значения сгенерированных последовательностей.\n";
			int x_or_shift[clobalCount];
			StartCounter();
			for (int i = 0; i < clobalCount; i++) {
				x_or_shift[i] = XorShift128();
			}
			const double sec5 = GetCounter();
			time_avarage_dispersion<int>(x_or_shift, clobalCount, sec5, &f);
			HISTOGRAMMA_DRAW<int>(x_or_shift, clobalCount, this->chart3, &f, System::Drawing::Color::Purple);
			f << "6. Используя функцию rand  сгенерировать последовательность из 1000 случайных вещественных величин.\n\
				Оценить время генерации. Рассчитать для последовательности среднее и дисперсию.\n\
				Построить гистограмму для 10 подотрезков. Отобразить в координатах XY значения сгенерированных последовательностей.";
			int simpleArrayRand[clobalCount];
			StartCounter();
			for (int i = 0; i < clobalCount; i++) {
				simpleArrayRand[i] = rand();
			}
			const double sec6 = GetCounter();
			time_avarage_dispersion<int>(simpleArrayRand, clobalCount, sec6, &f);
			HISTOGRAMMA_DRAW<int>(simpleArrayRand, clobalCount, this->chart4, &f, System::Drawing::Color::Aquamarine);
			f.close();
		}
		

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~Laba1()
		{
			if (components)
			{
				delete components;
			}
		}
		private: System::Windows::Forms::DataVisualization::Charting::Chart^ chart1;
		private: System::Windows::Forms::DataVisualization::Charting::Chart^ chart3;
		private: System::Windows::Forms::DataVisualization::Charting::Chart^ chart2;
		private: System::Windows::Forms::DataVisualization::Charting::Chart^ chart4;
	protected:

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container^ components;

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
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea2 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^ series2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea3 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^ series3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea4 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^ series4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->chart3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->chart2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->chart4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart4))->BeginInit();
			this->SuspendLayout();
			// 
			// chart1
			// 
			chartArea1->Name = L"ChartArea1";
			this->chart1->ChartAreas->Add(chartArea1);
			legend1->Name = L"Legend1";
			this->chart1->Legends->Add(legend1);
			this->chart1->Location = System::Drawing::Point(12, -7);
			this->chart1->Name = L"chart1";
			series1->ChartArea = L"ChartArea1";
			series1->CustomProperties = L"DrawingStyle=Wedge";
			series1->Legend = L"Legend1";
			series1->MarkerColor = System::Drawing::Color::Red;
			series1->Name = L"bar_graph_1.2";
			this->chart1->Series->Add(series1);
			this->chart1->Size = System::Drawing::Size(730, 426);
			this->chart1->TabIndex = 0;
			this->chart1->Text = L"chart1";
			this->chart1->Click += gcnew System::EventHandler(this, &Laba1::chart1_Click);
			// 
			// chart3
			// 
			chartArea2->Name = L"ChartArea1";
			this->chart3->ChartAreas->Add(chartArea2);
			legend2->Name = L"Legend1";
			this->chart3->Legends->Add(legend2);
			this->chart3->Location = System::Drawing::Point(12, 401);
			this->chart3->Name = L"chart3";
			series2->CustomProperties = L"DrawingStyle=Wedge";
			series2->ChartArea = L"ChartArea1";
			series2->Legend = L"Legend1";
			series2->Name = L"bar_graph_5";
			this->chart3->Series->Add(series2);
			this->chart3->Size = System::Drawing::Size(730, 426);
			this->chart3->TabIndex = 2;
			this->chart3->Text = L"chart3";
			this->chart3->Click += gcnew System::EventHandler(this, &Laba1::chart3_Click);
			// 
			// chart2
			// 
			chartArea3->Name = L"ChartArea1";
			this->chart2->ChartAreas->Add(chartArea3);
			legend3->Name = L"Legend1";
			this->chart2->Legends->Add(legend3);
			this->chart2->Location = System::Drawing::Point(713, -7);
			this->chart2->Name = L"chart2";
			series3->ChartArea = L"ChartArea1";
			series3->CustomProperties = L"DrawingStyle=Wedge";
			series3->Legend = L"Legend1";
			series3->Name = L"bar_graph_4";
			this->chart2->Series->Add(series3);
			this->chart2->Size = System::Drawing::Size(754, 426);
			this->chart2->TabIndex = 3;
			this->chart2->Text = L"chart2";
			this->chart2->Click += gcnew System::EventHandler(this, &Laba1::chart2_Click);
			// 
			// chart4
			// 
			chartArea4->Name = L"ChartArea1";
			this->chart4->ChartAreas->Add(chartArea4);
			legend4->Name = L"Legend1";
			this->chart4->Legends->Add(legend4);
			this->chart4->Location = System::Drawing::Point(713, 411);
			this->chart4->Name = L"chart4";
			series4->ChartArea = L"ChartArea1";
			series4->CustomProperties = L"DrawingStyle=Wedge";
			series4->Legend = L"Legend1";
			series4->Name = L"bar_graph_6";
			this->chart4->Series->Add(series4);
			this->chart4->Size = System::Drawing::Size(740, 426);
			this->chart4->TabIndex = 4;
			this->chart4->Text = L"chart4";
			this->chart4->Click += gcnew System::EventHandler(this, &Laba1::chart4_Click);
			// 
			// Laba1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1465, 824);
			this->Controls->Add(this->chart4);
			this->Controls->Add(this->chart2);
			this->Controls->Add(this->chart3);
			this->Controls->Add(this->chart1);
			this->Name = L"Laba1";
			this->Text = L"Stepanov L.I. IS";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart4))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void chart1_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void chart3_Click(System::Object^ sender, System::EventArgs^ e) {
	}
private: System::Void chart2_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void chart4_Click(System::Object^ sender, System::EventArgs^ e) {
}
};
}
