#include<bits/stdc++.h>
#include<fstream>
#include <sstream>
#define ll long long
using namespace std;
string dbr_mp;
string row_index;

bool customComparator(const pair<int, char>& a, const pair<int, char>& b) {
    if (a.first != b.first) {
        return a.first > b.first; 
    } else {
        return a.second < b.second; 
    }
}

void radix_sort(vector<pair<pair<int, int>, int>> & a){
    int n = a.size();
    {
        vector<int>cnt(n);
        for(auto x: a){
            cnt[x.first.second]++;
        }
        vector<pair<pair<int, int>, int>>a_new(n);
        vector<int>pos(n);
        pos[0] = 0;
        for(int i=1; i<n; i++){
            pos[i] = pos[i-1] + cnt[i-1];
        }
        for(auto x: a){
            int i=x.first.second;
            a_new[pos[i]] = x;
            pos[i]++;
        }
        a = a_new;
    }
    {
        vector<int>cnt(n);
        for(auto x: a){
            cnt[x.first.first]++;
        }
        vector<pair<pair<int, int>, int>>a_new(n);
        vector<int>pos(n);
        pos[0] = 0;
        for(int i=1; i<n; i++){
            pos[i] = pos[i-1] + cnt[i-1];
        }
        for(auto x: a){
            int i=x.first.first;
            a_new[pos[i]] = x;
            pos[i]++;
        }
        a = a_new;
    }
   
}

vector<int>apply_bwst(string &s,ll n){
    
    vector<int>p(n), c(n);
    {
        vector<pair<char, int>>a(n);
        for(int i=0; i<n; i++) a[i] = {s[i], i};
        sort(a.begin(), a.end());

        for(int i=0; i<n; i++) p[i] = a[i].second;
        c[p[0]] = 0;
        for(int i=1; i<n; i++){
            if(a[i].first == a[i-1].first){
                c[p[i]] = c[p[i-1]];
            }else{
                c[p[i]] = c[p[i-1]] +1;
            }
        }
    }

    int k = 0;
    while((1<<k) < n){
        vector<pair<pair<int, int>, int>>a(n);
        for(int i=0; i<n; i++){
            a[i] = {{c[i], c[(i+(1<<k))%n]},i};
        }
        radix_sort(a);

        for(int i=0; i<n; i++) p[i] = a[i].second;
        c[p[0]] = 0;
        for(int i=1; i<n; i++){
            if( a[i].first == a[i-1].first ){
                c[p[i]] = c[p[i-1]];
            }else {
                c[p[i]] = c[p[i-1]]+1;
            }
        }
        k++;
    } 
    for(int i=0; i<n; i++){
        if(p[i] == 0){
            row_index = to_string(i);
            break;
        }
    }
    return p;
}

vector<ll>dynamic_byte_mapping(string& s,ll n)
{
    map<char,ll> freq;
    for(auto it:s)
    {
        freq[it]++;
    }
    vector<pair<int, char>>pq;
    for(auto it:freq)
    {
        pq.push_back({it.second,it.first});
    }
    sort(pq.begin(), pq.end(), customComparator);
    int sz = pq.size();
    freq.clear();
    ll ct = 0;
    for(auto it: pq){
        freq[it.second] = ct;
        ct++;
    }
    vector<ll>sdbr(n);
    dbr_mp.resize(sz, '.');
    for(int i=0; i<n; i++){
        sdbr[i] = freq[s[i]];
        dbr_mp[freq[s[i]]] = s[i]; 
    }
    return sdbr;
}

string RLE(vector<ll> &sdbr)
{
    int length = sdbr.size();
    int starting_bit;
    if (((sdbr[0]) & (1 << 7)) == 0)
        starting_bit = 0;
    else
        starting_bit = 1;
    int cnt = 0;
    string ans;
    if (starting_bit == 1)
        ans += "0 ";
    for (int i = 7; i >= 0; i--)
    {
        for (int j = 0; j < length; j++)
        {
            int currrent_bit;
            int ele = sdbr[j];
            if (((ele) & (1 << i)) == 0)
                currrent_bit = 0;
            else
                currrent_bit = 1;
            if (currrent_bit == starting_bit)
            {
                cnt++;
            }
            else
            {
                ans += to_string(cnt);
                ans += " ";
                cnt = 1;
                starting_bit = currrent_bit;
            }
        }
    }
    ans += to_string(cnt);
    return ans;
}


string encode(string& s, ll n){
    s += "$";
    n++;
    vector<int>p = apply_bwst(s, n);
    string sbwst = "";
    for(int i=0; i<n; i++){
        sbwst += s[(p[i] + n -1)%n];
        // sbwst += s[p[i]];
        // cout<<p[i]<<" "<<s.substr(p[i], n-p[i])<<"\n";
    }
    // cout<<sbwst<<endl;
    vector<ll> sdbr = dynamic_byte_mapping(sbwst,n);
    return RLE(sdbr);
}



int main(){

// -----------------------reading the file to compress ------------------ 

// input file name
    string fileName = "test.txt";
    ifstream inFile(fileName);
     if (!inFile.is_open()) {
        cerr << "Error opening file: " << fileName << endl;
        return 1; 
    }

    stringstream buffer;
    buffer << inFile.rdbuf();
    inFile.close();
    string fileContents = buffer.str();

    int n = fileContents.size();
    string rle = encode(fileContents,n);
    
    rle += " ";
    rle += row_index;
    rle += "##";
    rle += dbr_mp;

// ------------------------writing into the compressed file
    string newFile =  "test_compressed.rle";
    ofstream outFile(newFile);
     if (!outFile.is_open()) {
        cerr << "Error opening file: " << fileName << endl;
        return 1; 
    }
    outFile.write(rle.c_str(), rle.size());
    outFile.close();
    cout << "String successfully compressed" <<endl;

}