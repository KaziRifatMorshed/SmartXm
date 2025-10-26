#include <bits/stdc++.h>
using namespace std;
#define int long long
#define maxsize (int)(2 * 1e5 + 1)
#define endl '\n'
typedef struct node
{
     int x_or, mn, mx;
} node;

vector<node> tree;

vector<int> a;
void initialize(int c_root, int l, int r)
{
     if (l == r)
     {
          tree[c_root].x_or = a[l];
          tree[c_root].mn = a[l];
          tree[c_root].mx = a[l];
          return;
     }
     int left = 2 * c_root, right = c_root * 2 + 1;
     int mid = (l + r) / 2;
     initialize(left, l, mid);
     initialize(right, mid + 1, r);
     tree[c_root].x_or = tree[left].x_or ^ tree[right].x_or;
     tree[c_root].mn = min(tree[left].mn, tree[right].mn);
     tree[c_root].mx = max(tree[left].mx, tree[right].mx);
}
void update(int c_node, int l, int r, int ind, int val)
{
     if (ind < l || ind > r)
          return;
     else if (l == r && l == ind)
     {
          tree[c_node].x_or += val;
          tree[c_node].mn += val;
          tree[c_node].mx += val;
          return;
     }
     int left = 2 * c_node, right = c_node * 2 + 1;
     int mid = (l + r) / 2;
     update(left, l, mid, ind, val);
     update(right, mid + 1, r, ind, val);
     tree[c_node].x_or = tree[left].x_or ^ tree[right].x_or;
     tree[c_node].mn = min(tree[left].mn, tree[right].mn);
     tree[c_node].mx = max(tree[left].mx, tree[right].mx);
}
pair<int, pair<int, int>> range_query(int c_node, int l, int r, int i, int j)
{
     if (l >= i && r <= j)
          return {tree[c_node].x_or, {tree[c_node].mn, tree[c_node].mx}};
     else if (r < i || l > j)
          return {0, {INT64_MAX, INT64_MIN}};
     int left = 2 * c_node, right = c_node * 2 + 1;
     int mid = (l + r) / 2;
     auto val1 = range_query(left, l, mid, i, j);
     auto val2 = range_query(right, mid + 1, r, i, j);
     return {val1.first ^ val2.first, {min(val1.second.first, val2.second.first), max(val1.second.second, val2.second.second)}};
}
int32_t main()
{
	ios::sync_with_stdio(false);
cin.tie(nullptr);
cout.tie(nullptr);
     int n, q;
     cin >> n >> q;
     a.resize(n + 1);
     tree.resize(4 * n + 1);
     int i;
     for (i = 1; i <= n; i++)
          cin >> a[i];
     initialize(1, 1, n);
     while (q--)
     {
          int l, r;
          cin >> l >> r;
          int combined_support = a[l] ^ a[r];

          auto r_q = range_query(1, 1, n, l + 1, r - 1);
          int against = r_q.first;
          if (combined_support > against)
               cout << "Yes\n";
          else
               cout << "No\n";

          int add = (r_q.second.first + r_q.second.second + 1) / 2;
          update(1, 1, n, l, add);
          update(1, 1, n, r, add);
          a[l] += add;
          a[r] += add;
         
     }
     return 0;
}