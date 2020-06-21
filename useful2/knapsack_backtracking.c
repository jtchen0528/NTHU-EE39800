const int N = 100, W = 100000;
int cost[N], weight[N];
int c[W + 1];
int p[N][W + 1];    // 0為不放，1為放，2為放與不放都可以。
 
void knapsack(int n, int w)
{
    memset(c, 0, sizeof(c));
    memset(p, 0, sizeof(p));
 
    for (int i = 0; i < n; ++i)
        for (int j = w; j >= weight[i]; --j)
            // 不放比較好
            if (c[j - weight[i]] + cost[i] < c[j])
            {
                p[i][j] = 0;
            }
            // 放了比較好
            else if (c[j - weight[i]] + cost[i] > c[j])
            {
                c[j] = c[j - weight[i]] + cost[i];
                p[i][j] = 1;
            }
            // 可放可不放
            else/* if (c[j - weight[i]] + cost[i] == c[j])*/
            {
//              c[j] = c[j - weight[i]] + cost[i];
                p[i][j] = 2;
            }
 
    printf( "最高的價值為" );
 
    // 往回逆推，求出所有放入的物品。
    find_path(n-1, c[w], k);
}
 
int output[N];
 
void find_path(int n, int w, int k)
{
    if (n < 0)  // 找完了
    {
        for (int i=0; i<k; ++i) printf("%d ", output[i]);
        return;
    }
 
    // 不放比較好：背包裡面沒有第n個物品
    if (p[n][w] == 0)
    {
        find_path(n-1, w, k);
    }
    // 放了比較好：背包裡面有第n個物品
    else if (p[n][w] == 1)
    {
        output[k] = n;
        find_path(n-1, w-weight[n], k+1);
    }
    // 可放可不放
    else if (p[n][w] == 2)
    {
        // 背包裡面有第n個物品
        output[k] = n;
        find_path(n-1, w-weight[n], k+1);
 
        // 背包裡面沒有第n個物品
        find_path(n-1, w, k);
    }
}
