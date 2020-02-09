/*
Ripoff https://www.reddit.com/r/adventofcode/comments/a77xq6/2018_day_18_solutions/ec1109q
*/
#include <iostream>
#include <vector>
#include <map>
#include <assert.h>

using namespace std;
using ll = long long;

int main() {
  vector<vector<char>> G;
  while(true) {
    string S;
    cin >> S;
    if(S.size() == 0) { break; }
    vector<char> row;
    for(auto& c : S) {
      row.push_back(c);
    }
    G.push_back(row);
  }
  size_t R = G.size();
  size_t C = G[0].size();
  ll T = 1000000000LL;
  map<vector<vector<char>>, ll> M;
  for(ll t=0; t<T; t++) {
    vector<vector<char>> G2(R, vector<char>(C, ' '));
    for(ll r=0; r<R; r++) {
      for(ll c=0; c<C; c++) {
        ll tree = 0;
        ll lumber = 0;
        for(ll dr=-1; dr<=1; dr++) {
          for(ll dc=-1; dc<=1; dc++) {
            if(dr==0 && dc==0) { continue; }
            ll rr = r+dr;
            ll cc = c+dc;
            if(!(0<=rr && rr<R && 0<=cc&&cc<C)) { continue; }
            if(G[rr][cc] == '|') { tree++; }
            if(G[rr][cc] == '#') { lumber++; }
          }
        }
        if(G[r][c] == '.') {
          G2[r][c] = (tree>=3 ? '|' : '.');
        } else if(G[r][c] == '|') {
          G2[r][c] = (lumber>=3 ? '#' : '|');
        } else {
          G2[r][c] = (lumber>=1 && tree>=1 ? '#' : '.');
        }
      }
    }
    G = G2;
    if(M.count(G) == 1) {
      ll skip = (T-t)/(t-M[G]);
      t += skip*(t-M[G]);
      assert(t <= T);
    } else {
      M[G] = t;
    }
    /*cout << "=========" << endl;
    for(ll r=0; r<R; r++) {
      for(ll c=0; c<C; c++) {
        cout << G[r][c];
      }
      cout << endl;
    }*/
    ll tree = 0;
    ll lumber = 0;
    for(ll r=0; r<R; r++) {
      for(ll c=0; c<C; c++) {
        if(G[r][c] == '|') { tree++; }
        if(G[r][c] == '#') { lumber++; }
      }
    }
    if(t==9 || t==T-1) {
      cout << tree*lumber << endl;
    }
    //cout << "=========" << endl;
  }
}