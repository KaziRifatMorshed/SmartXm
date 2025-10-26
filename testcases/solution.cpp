#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;
#define int long long
#include<filesystem>
// #define endl '\n'
#define srt(v) sort(v.begin(), v.end())
#define rsrt(v) sort(v.rbegin(), v.rend())
#define rev(v) reverse(v.begin(), v.end())
#define printv(v)          \
    for (auto it : v)      \
    {                      \
        cout << it << " "; \
    }
#define printlnv(v)         \
    for (auto it : v)       \
    {                       \
        cout << it << '\n'; \
    }
#define scanv(v)       \
    for (auto &it : v) \
    {                  \
        cin >> it;     \
    }
#define maxsize 100000005
#define M 1000000007

template <class T>
struct comparator
{
    bool operator()(const T &a, const T &b) const
    {
        return a < b;
    }
};

template <class T>
using ordered_set =
    tree<T, null_type, comparator<T>, rb_tree_tag, tree_order_statistics_node_update>;
template <class T>
using ordered_setd = tree<T, null_type, greater<T>, rb_tree_tag,
                          tree_order_statistics_node_update>;

int32_t main()
{
    ios_base::sync_with_stdio(false);
        cin.tie(NULL);
        cout.tie(NULL);
    // cout<<log2(9)<<endl;
    for (int k = 1; k <= 20; k++)
    {
        string input=to_string(k)+".in";
        string out=to_string(k)+".out";
        
        ifstream cin(input.c_str());
        ofstream cout(out.c_str());

        
        int t;
        cin >> t;
        while (t--)
        {
            int n;
            cin >> n;
            int k;
            cin >> k;
            string s;
            cin >> s;
            int i;
            int flag = -1;
            int ans = 0;
            for (i = 0; i < n; i++)
            {
                if (s[i] == '1')
                {
                    if (flag == -1)
                    {
                        ans++;
                        flag = i;
                    }
                    else
                    {
                        if (i - flag > k - 1)
                            ans++;
                        flag = i;
                    }
                }
            }
            cout << ans << endl;
        }
        cin.close();
        cout.close();
    }

    return 0;
}
/*
  Md. Siam Ahmed
  CSE, Khulna University
*/