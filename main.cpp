// Name: Madison Fong
// Date: 02/15/2020
// Class: CSCI 311-4

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <ctime>

using namespace std;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::cerr;
using std::ifstream;
using std::ofstream;
using std::ios;

void readFromFile(string &S, string filename);
void convertToLower(string &S);
bool lessThan(const string &S, int first, int second);
int partition(const string &S, vector<int> &indices, int low, int high, int pivotIndex);
void quickSort(const string &S, vector<int> &indices, int low, int high);
void insertion(const string &S, vector<int> &indices, int low, int high);
int selection(const string &S, vector<int> &indices, int low, int high, int k);

int main(int argc, char* argv[])
{
  string ifile = argv[1];
  string word;
  vector<int> indices;

  readFromFile(word, ifile);
  convertToLower(word);

  for (int i = 0; i < word.size(); i++)
  {
    indices.push_back(i);
  }

  if (argc < 2)
  {
    cerr << "ERROR: provide more arguments" << endl;
    return -1;
  }

  if (argc == 2)
    quickSort(word, indices, 0, word.size() - 1);

  else if (argc == 3)
  {
    int smallest = selection(word, indices, 0, word.size() - 1, atoi(argv[2]));
    cout << smallest << endl;
  }

  else if (argc == 4)
    insertion(word, indices, atoi(argv[2]), atoi(argv[3]));

  if ((argc == 2) || (argc == 4))
  {
    for (int i = 0; i < indices.size(); i++)
    {
      cout << indices[i] << " ";
    }
    cout << endl;
  }

  return 1;
}

void readFromFile(string &S, string filename)
{
  string line;
  ifstream in;

  in.open(filename);

  if (in.is_open())
  {
    while (getline(in, line))
    {
      S += line;
    }
  }

  else
  {
    cerr << "ERROR: could not open file" << endl;
  }

  in.close(); in.clear();
}

void convertToLower(string &S)
{
  for (int i = 0; i < (int)S.size(); i++)
  {
    S[i] = tolower(S[i]);
  }
}

bool lessThan(const string &S, int first, int second)
{
  while ((first < S.size()) && (second < S.size()))
  {
    if (S[first] > S[second])
      return false;

    else if (S[first] < S[second])
      return true;

    first++;
    second++;
  }

  if (first > second)
    return true;
  else
    return false;
}

int partition(const string &S, vector<int> &indices, int low, int high, int pivotIndex)
{
  int hold = indices[high];
  indices[high] = indices[pivotIndex];
  indices[pivotIndex] = hold;

  int pivot = indices[high];
  int i = low;
  int j = high - 1;

  while (i <= j)
  {
    while ((i < high) && (lessThan(S, indices[i], pivot)))
      i++;

    while ((j >= low) && (lessThan(S, pivot, indices[j])))
      j--;

    if (i < j)
    {
      int temp = indices[i];
      indices[i] = indices[j];
      indices[j] = temp;
      i++;
      j--;
    }
  }

  indices[high] = indices[i];
  indices[i] = pivot;
  return i;
}

void quickSort(const string &S, vector<int> &indices, int low, int high)
{
  if (low < high)
  {
    int pivotIndex = partition(S, indices, low, high, high);
    quickSort(S, indices, low, pivotIndex - 1);
    quickSort(S, indices, pivotIndex + 1, high);
  }
}

void insertion(const string &S, vector<int> &indices, int low, int high)
{
  for (int i = 1; i < high + 1; i++)
  {
    int key = indices[i];
    int j = i - 1;
    while ((j >= low) && (lessThan(S, key, indices[j])))
    {
      indices[j + 1] = indices[j];
      j--;
    }
    indices[j + 1] = key;
  }
}

int selection(const string &S, vector<int> &indices, int low, int high, int k)
{
  int vsize = high - low + 1;

  if (low == high)
    return indices[low];

  else if (low > high)
    return -1;

  if (vsize <= 5)
  {
    insertion(S, indices, low, high);
    return indices[k];
  }

  int msize = ceil((double) vsize/5);

  vector<int> medians(msize, 0);
  int med_ind = 0;

  for (int i = low; i <= high; i = i + 5)
  {
    vector<int> temp(5, 0);
    int j = 0;

    for (; j < 5 && (i + j) <= high; j++)
    {
      temp[j] = indices[i + j];
    }

    if (j < 5)
      temp.resize(j);

    insertion(S, temp, 0, temp.size() - 1);

    if (j == 5)
      medians[med_ind++] = temp[2];

    else
    {
      int jmid = j >> 1;

      if (j % 2 == 0)
        jmid--;

      medians[med_ind++] = temp[jmid];
    }
  }

  int mid = msize >> 1;

  if ((msize % 2) == 0)
    mid--;

  int x = selection(S, medians, 0, msize - 1, mid);

  if (x == -1)
    exit(0);

  int ind_of_x = -1;

  for (int i = low; i <= high; i++)
  {
    if (indices[i] == x)
    {
      ind_of_x = i;
      break;
    }
  }

  int pivotIndex = partition(S, indices, low, high, ind_of_x);

  if (k == pivotIndex)
    return indices[k];

  else if (pivotIndex < k)
    return selection(S, indices, pivotIndex + 1, high, k);

  else
    return selection(S, indices, low, pivotIndex - 1, k);
}
