#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <chrono>

using namespace std;

int Menu();
void CreateArray();
void PrintArray();
int IntLen(int);
void SortMenu();
void ClearArray();
void ShowHistory();
void Settings();

void sort_bubble();
void sort_cocktail();
void sort_selection();
void sort_insertion();
void sort_shell();

void sort_merge(int, int);
void merge(int, int, int);

void sort_quick_recursion(int, int);
struct { int L; int R; } stack[100];
void push(int, int, int&);
void pop(int&, int&, int&);
void swap(int&, int&);
void sort_quick();
void sort_quick_compressed();

int RANDOM_KEY, RANDOM_MIN, RANDOM_MAX;
int n = 0, nh = 0;
int* arr = nullptr, *arrTmp;

struct SortInfo
{
	int size;
	char type[20];
	double time;
} hist[30];

FILE* fl;

int main()
{
	cout.setf(ios_base::fixed);
	cout.precision(6);

	fopen_s(&fl, "settings.dat", "r");
	if (fl == NULL)
	{
		fopen_s(&fl, "settings.dat", "w");
		
		RANDOM_KEY = 0;
		RANDOM_MIN = -100;
		RANDOM_MAX = 100;
		fwrite(&RANDOM_KEY, sizeof(int), 1, fl);
		fwrite(&RANDOM_MIN, sizeof(int), 1, fl);
		fwrite(&RANDOM_MAX, sizeof(int), 1, fl);
		fwrite(&n, sizeof(int), 1, fl);

		fclose(fl);
	}
	else
	{
		fread(&RANDOM_KEY, sizeof(int), 1, fl);
		fread(&RANDOM_MIN, sizeof(int), 1, fl);
		fread(&RANDOM_MAX, sizeof(int), 1, fl);
		fread(&n, sizeof(int), 1, fl);

		fclose(fl);
	}

	while (true)
	{
		switch (Menu())
		{
			case 1: CreateArray(); break;
			case 2: PrintArray(); break;
			case 3: SortMenu(); break;
			case 4: ClearArray(); break;
			case 5: ShowHistory(); break;
			case 6: Settings(); break;
			default: return 0; break;
		}

		cout << endl;
		system("pause");
		system("cls");
	}
}

int Menu()
{
	cout << "+--------------------+" << endl;
	cout << "| 1 - Create array   |" << endl;
	cout << "| 2 - Print array    |" << endl;
	cout << "| 3 - Sort array     |" << endl;
	cout << "| 4 - Clear array    |" << endl;
	cout << "| 5 - Show history   |" << endl;
	cout << "| 6 - Settings       |" << endl;
	cout << "| 7 - Exit           |" << endl;
	cout << "+--------------------+" << endl;

	int k; cin >> k;
	cout << endl;

	return k;
}

void CreateArray()
{
	if (arr != nullptr)
	{
		cout << "Array already exists!" << endl;
		return;
	}
	
	if (n <= 0)
	{
		cout << "Enter array size: "; cin >> n;
	}

	if (arr == nullptr)
	{
		arr = new int[n];
		srand(RANDOM_KEY);
		for (int i = 0; i < n; i++)
			arr[i] = rand() % (RANDOM_MAX - RANDOM_MIN + 1) + RANDOM_MIN;

		cout << '[' << n << "] created." << endl;
	}
	else
	{
		n = 0;
		cout << "CREATE ERROR" << endl;
	}
}

void PrintArray()
{
	if (n > 0)
	{
		int w1 = IntLen(RANDOM_MIN);
		int w2 = IntLen(RANDOM_MAX);
		int w = 1 + ((w1 > w2) ? w1 : w2);

		cout << "Array: " << endl;
		for (int i = 0; i < n; i++)
			cout << setw(w) << arr[i] << ',';
		cout << endl;
	}
	else
	{
		cout << "PRINT ERROR" << endl;
	}
}

void SortMenu()
{
	if (n > 0)
	{
		cout << " 1 - Bubble sort" << endl;
		cout << " 2 - Cocktail sort" << endl;
		cout << " 3 - Selection sort" << endl;
		cout << " 4 - Insertion sort" << endl;
		cout << " 5 - Shell sort" << endl;
		cout << " 6 - Merge sort" << endl;
		cout << " 7 - Quick sort recursion" << endl;
		cout << " 8 - Quick sort" << endl;
		cout << " 9 - Quick sort compressed" << endl;
		cout << "+-------------------------+" << endl;

		int k; cin >> k;

		auto t1 = std::chrono::high_resolution_clock::now();
		switch (k)
		{
			case 1: sort_bubble(); break;
			case 2: sort_cocktail(); break;
			case 3: sort_selection(); break;
			case 4: sort_insertion(); break;
			case 5: sort_shell(); break;
			case 6: arrTmp = new int[n]; sort_merge(0, n-1); delete[] arrTmp; arrTmp = nullptr; break;
			case 7: sort_quick_recursion(0, n-1); break;
			case 8: sort_quick(); break;
			case 9: sort_quick_compressed(); break;
			default: cout << "SORT NOT DEFINED" << endl; return;
		}
		auto t2 = std::chrono::high_resolution_clock::now();
		double dt = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count();

		hist[nh].size = n;
		switch (k)
		{
			case 1: strcpy_s(hist[nh].type, " Bubble: "); break;
			case 2: strcpy_s(hist[nh].type, "Cocktail: "); break;
			case 3: strcpy_s(hist[nh].type, "Selection:"); break;
			case 4: strcpy_s(hist[nh].type, "Insertion:"); break;
			case 5: strcpy_s(hist[nh].type, "  Shell:  "); break;
			case 6: strcpy_s(hist[nh].type, "  Merge:  "); break;
			case 7: strcpy_s(hist[nh].type, "Quick (R):"); break;
			case 8: strcpy_s(hist[nh].type, "  Quick:  "); break;
			case 9: strcpy_s(hist[nh].type, "Quick (C):"); break;
			default:strcpy_s(hist[nh].type, "   ???:   "); break;
		}
		hist[nh].time = dt / 1000000;
		
		cout << "Sorted." << endl;
		cout << '[' << hist[nh].size << ']' << hist[nh].type << hist[nh].time << 's' << endl;
		nh++;
	}
	else
	{
		cout << "SORT ERROR" << endl;
	}
}

void ClearArray()
{
	delete[] arr;
	arr = nullptr;
	cout << "Array cleared." << endl;
}

int IntLen(int i)
{
	int len = 0;

	if (i < 0)
	{
		len++;
		i *= -1;
	}

	while (i > 0)
	{
		i /= 10;
		len++;
	}

	return len;
}

void ShowHistory()
{
	if (nh > 0)
	{
		cout << "History:" << endl;
		for (int i = 0; i < nh; i++)
			cout << '[' << hist[i].size << ']' << hist[i].type << hist[i].time << 's' << endl;
	}
	else
	{
		cout << "EMPTY" << endl;
	}
}

void Settings()
{
	cout << " 1 - Random key: " << RANDOM_KEY << endl;
	cout << " 2 - Random min: " << RANDOM_MIN << endl;
	cout << " 3 - Random max: " << RANDOM_MAX << endl;
	cout << " 4 - Size: " << n << endl;
	cout << " 5 - Close" << endl;
	cout << "+------------------+" << endl;

	int k; cin >> k;

	switch (k)
	{
		case 1: cout << "Enter key: "; cin >> RANDOM_KEY; break;
		case 2: cout << "Enter min: "; cin >> RANDOM_MIN; break;
		case 3: cout << "Enter max: "; cin >> RANDOM_MAX; break;
		case 4: cout << "Enter size: "; cin >> n; break;
		default: return;
	}

	if (k > 0 && k < 5)
	{
		fopen_s(&fl, "settings.dat", "w");

		fwrite(&RANDOM_KEY, sizeof(int), 1, fl);
		fwrite(&RANDOM_MIN, sizeof(int), 1, fl);
		fwrite(&RANDOM_MAX, sizeof(int), 1, fl);
		fwrite(&n, sizeof(int), 1, fl);

		fclose(fl);
	}
}

//-+-+-+-+-+-+-+-+-+-+-BUBBLE SORT-+-+-+-+-+-+-+-+-+-+-
void sort_bubble()
{
	int temp;

	for (int i = 1; i < n; i++)
		for (int j = n-1; j >= i; j--)
			if (arr[j-1] > arr[j])
			{
				temp = arr[j-1];
				arr[j-1] = arr[j];
				arr[j] = temp;
			}
}

//-+-+-+-+-+-+-+-+-+-+-COCKTAIL SORT-+-+-+-+-+-+-+-+-+-+-
void sort_cocktail()
{
	int k, left = 0, right = n;
	int temp;

	do
	{
		for (int i = left+1; i < right; i++)
			if (arr[i-1] > arr[i])
			{
				temp = arr[i-1];
				arr[i-1] = arr[i];
				arr[i] = temp;
				k = i;
			}
		right = k;

		for (int j = right-1; j > left; j--)
			if (arr[j-1] > arr[j])
			{
				temp = arr[j-1];
				arr[j-1] = arr[j];
				arr[j] = temp;
				k = j;
			}
		left = k;

	} while (left < right);
}

//-+-+-+-+-+-+-+-+-+-+-SELECTION SORT-+-+-+-+-+-+-+-+-+-+-
void sort_selection()
{
	int i_temp;
	int temp;

	for (int i = 0; i < n-1; i++)
	{
		i_temp = i;
		for (int j = i+1; j < n; j++)
			if (arr[i_temp] > arr[j])
				i_temp = j;

		if (i_temp != i)
		{
			temp = arr[i_temp];
			arr[i_temp] = arr[i];
			arr[i] = temp;
		}
	}
}

//-+-+-+-+-+-+-+-+-+-+-INSERTION SORT-+-+-+-+-+-+-+-+-+-+-
void sort_insertion()
{
	int x, j;

	for (int i = 1; i < n; i++)
	{
		x = arr[i];

		for (j = i-1; j >= 0 && arr[j] > x; j--)
			arr[j+1] = arr[j];
		arr[j+1] = x;
	}
}

//-+-+-+-+-+-+-+-+-+-+-SHELL SORT-+-+-+-+-+-+-+-+-+-+-
void sort_shell()
{
	int x, j;

	for (int d = 3; d > 0; d--)
	{
		for (int i = d; i < n; i++)
		{
			x = arr[i];

			for (j = i-d; j >= 0 && arr[j] > x; j-=d)
				arr[j+d] = arr[j];
			arr[j+d] = x;
		}
	}
}

//-+-+-+-+-+-+-+-+-+-+-MERGE SORT-+-+-+-+-+-+-+-+-+-+-
void sort_merge(int left, int right)
{
	if (left < right)
	{
		int middle = (left + right) / 2;
		sort_merge(left, middle);
		sort_merge(middle+1, right);
		merge(left, middle, right);
	}
}

void merge(int left, int middle, int right)
{
	int i = left, j = middle+1, k = 0;

	while ((i <= middle) && (j <= right))
	{
		if (arr[i] < arr[j])
		{
			arrTmp[k] = arr[i];
			k++;
			i++;
		}
		else
		{
			arrTmp[k] = arr[j];
			k++;
			j++;
		}
	}

	while (i <= middle)
	{
		arrTmp[k] = arr[i];
		k++;
		i++;
	}
	while (j <= right)
	{
		arrTmp[k] = arr[j];
		k++;
		j++;
	}

	k = 0;
	while (left <= right)
	{
		arr[left] = arrTmp[k];
		left++;
		k++;
	}
}

//-+-+-+-+-+-+-+-+-+-+-QUICK SORT RECURSION-+-+-+-+-+-+-+-+-+-+-
void sort_quick_recursion(int left, int right)
{
	int i = left, j = right-1, x, temp;

	x = arr[(i + j) / 2];
	do
	{
		while (arr[i] < x && i < right) i++;
		while (arr[j] > x && j > left) j--;
		
		if (i <= j)
		{
			temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
			i++;
			j--;
		}
	} while (i <= j);

	if (left < j) sort_quick_recursion(left, j);
	if (i < right) sort_quick_recursion(i, right);
}

//-+-+-+-+-+-+-+-+-+-+-QUICK SORT-+-+-+-+-+-+-+-+-+-+-
void swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

void push(int L, int R, int& k)
{
	stack[k].L = L;
	stack[k].R = R;
	k++;
}
void pop(int& L, int& R, int& k)
{
	k--;
	L = stack[k].L;
	R = stack[k].R;
}

void sort_quick()
{
	int i, j, left, right, k = 0;
	int x, temp;

	push(0, n-1, k);

	while (k != -1)
	{
		pop(left, right, k);

		while (left < right)
		{
			i = left;
			j = right;
			x = arr[(i + j) / 2];

			while (i <= j)
			{
				while (arr[i] < x) i++;
				while (arr[j] > x) j--;

				if (i <= j)
				{
					swap(arr[i], arr[j]);
					i++;
					j--;
				}
			}

			if ( (j-left) < (right-i) )
			{
				if (i < right) push(i, right, k);
				right = j;
			}
			else
			{
				if (left < j) push(left, j, k);
				left = i;
			}
		}
	}
}

//-+-+-+-+-+-+-+-+-+-+-QUICK SORT COMPRESSED-+-+-+-+-+-+-+-+-+-+-
void sort_quick_compressed()
{
	struct
	{
		int L;
		int R;
		int k;
	} stack[100];

	int i, j, left, right, k;
	int x, temp;

	k = 0;
	stack[k].L = 0;
	stack[k].R = n-1;

	while (k != -1)
	{
		left = stack[k].L;
		right = stack[k].R;
		k--;

		while (left < right)
		{
			i = left;
			j = right;
			x = arr[(i + j) / 2];

			while (i <= j)
			{
				while (arr[i] < x) i++;
				while (arr[j] > x) j--;

				if (i <= j)
				{
					temp = arr[i];
					arr[i] = arr[j];
					arr[j] = temp;
					i++;
					j--;
				}
			}

			if ( (j-left) < (right-i) )
			{
				if (i < right)
				{
					k++;
					stack[k].L = i;
					stack[k].R = right;
				}
				right = j;
			}
			else
			{
				if (left < j)
				{
					k++;
					stack[k].L = left;
					stack[k].R = j;
				}
				left = i;
			}
		}
	}

}