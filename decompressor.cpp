#include <bits/stdc++.h>
#include <sstream>
#define ll long long
using namespace std;

class Node
{
public:
    char Data;
    Node *Next;

    Node(char data)
    {
        Data = data;
        Next = NULL;
    }
};

class InvertBWT
{
public:
    string invert(string bwtArr, int x)
    {
        int lenBwt = bwtArr.length();
        string sortedBwt = bwtArr;
        sort(sortedBwt.begin(), sortedBwt.end());
        int *lShift = new int[lenBwt];

        vector<int> *arr = new vector<int>[128];

        for (int i = 0; i < lenBwt; i++)
        {
            arr[bwtArr[i]].push_back(i);
        }

        for (int i = 0; i < lenBwt; i++)
        {
            lShift[i] = arr[sortedBwt[i]][0];
            arr[sortedBwt[i]].erase(arr[sortedBwt[i]].begin());
        }

        char *decoded = new char[lenBwt];
        for (int i = 0; i < lenBwt; i++)
        {
            x = lShift[x];
            decoded[lenBwt - 1 - i] = bwtArr[x];
        }
        string decodedStr(decoded, lenBwt);

        return string(decodedStr.rbegin(), decodedStr.rend());
    }
};

void demap(vector<ll> &arr, vector<ll> &dynamic_byte_decoder)
{
    int n;
    ll sum = 0;
    for (auto it : arr)
    {
        sum += it;
    }
    n = sum / 8;
    int bit = 0;
    int mask = 7;
    int idx = 0;
    int total = 0;
    dynamic_byte_decoder.resize(n, 0);
    for (int i = 0; i < arr.size(); i++)
    {
        if (arr[0] == 0)
        {
            bit = 1;
            continue;
        }
        for (int cnt = 0; cnt < arr[i]; cnt++)
        {
            if (idx == n)
                mask--;
            total++;
            // cout << mask << " ";
            idx = idx % n;
            //  cout<<idx<<" ";
            int ele = dynamic_byte_decoder[idx];
            ele = (ele | ((bit << mask)));
            dynamic_byte_decoder[idx] = ele;

            idx++;
        }
        bit = !bit;
    }
}
void decode(string& filecontent, string& orignal){
     size_t pos = filecontent.find("##");

    string rle, dbr_mp;
    if (pos != string::npos)
    {
        rle = filecontent.substr(0, pos);
        dbr_mp = filecontent.substr(pos + 2);
    }
    stringstream ss(rle);

    vector<ll> integerVector;

    int num;
    while (ss >> num)
    {
        integerVector.push_back(num);
    }
    int row_index = integerVector[integerVector.size()-1];
    integerVector.pop_back();

    // for(auto it: integerVector){
    //     cout<<it<<" ";
    // }

    vector<ll> sdbr;
    demap(integerVector, sdbr);
    string sbwst = "";
    for (int i = 0; i < sdbr.size(); i++)
    {
        sbwst += dbr_mp[sdbr[i]];
    }
    InvertBWT inverse;
    orignal = inverse.invert(sbwst, row_index);
    orignal.pop_back();
}

int main()
{

    // -------------------------Reading file to decompress-----------------------

    // input file name
    string fileName = "test_compressed.rle";



    ifstream inFile(fileName);
    if (!inFile.is_open())
    {
        cerr << "Error opening file: " << fileName << endl;
        return 1;
    }

    stringstream buffer;
    buffer << inFile.rdbuf();
    inFile.close();
    string filecontent = buffer.str();
    string orignal;
    decode(filecontent, orignal);

    // -------------------------Creating new decompressed file-----------------------
    string newFile = "test_decompressed.txt";
    ofstream outFile(newFile);
     if (!outFile.is_open()) {
        cerr << "Error opening file: " << fileName << endl;
        return 1; 
    }
    outFile.write(orignal.c_str(), orignal.size());
    outFile.close();
    cout<<"decompression successfully"<<endl;

}