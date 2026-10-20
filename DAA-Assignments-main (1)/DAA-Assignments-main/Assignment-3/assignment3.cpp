#include <iostream>
using namespace std;

void sortItems(int profit[], int weight[], float ratio[], int n)
{
    // Sorting according to ratio (descending order)
    for(int i = 0; i < n-1; i++)
    {
        for(int j = i+1; j < n; j++)
        {
            if(ratio[i] < ratio[j])
            {
                // Swap ratio
                float temp = ratio[i];
                ratio[i] = ratio[j];
                ratio[j] = temp;

                // Swap profit
                int p = profit[i];
                profit[i] = profit[j];
                profit[j] = p;

                // Swap weight
                int w = weight[i];
                weight[i] = weight[j];
                weight[j] = w;
            }
        }
    }
}

float fractionalKnapsack(int capacity, int profit[], int weight[], float ratio[], int n)
{
    float totalProfit = 0;

    cout << "\nSelection Process:\n";

    for(int i = 0; i < n; i++)
    {
 if(capacity >= weight[i])
        {
            // Take complete item
            capacity = capacity - weight[i];
            totalProfit = totalProfit + profit[i];

            cout << "Take full item "
                 << i+1
                 << " Profit = "
                 << profit[i]
                 << " Weight = "
                 << weight[i]
                 << endl;
        }
        else
        {
            // Take fraction of item
            float fraction = (float)capacity / weight[i];

            float addedProfit = profit[i] * fraction;

            totalProfit = totalProfit + addedProfit;

            cout << "Take "
                 << fraction * 100
                 << "% of item "
                 << i+1
                 << " Profit added = "
                 << addedProfit
                 << endl;
                 
            break;
        }
    }
return totalProfit;
}

int main()
{
    int n;
    int capacity;

    cout << "Enter number of items: ";
    cin >> n;

    int profit[n];
    int weight[n];
    float ratio[n];

    cout << "\nEnter profit and weight:\n";

    for(int i = 0; i < n; i++)
    {
        cout << "Item " << i+1 << " Profit: ";
        cin >> profit[i];

        cout << "Item " << i+1 << " Weight: ";
        cin >> weight[i];

        ratio[i] = (float)profit[i] / weight[i];
    }

    cout << "\nEnter knapsack capacity: ";
    cin >> capacity;
    cout << "\nBefore Sorting:\n";

    for(int i = 0; i < n; i++)
    {
        cout << "Item " << i+1
             << " Profit = " << profit[i]
             << " Weight = " << weight[i]
             << " Ratio = " << ratio[i]
             << endl;
    }

    sortItems(profit, weight, ratio, n);

    cout << "\nAfter Sorting (by Ratio):\n";

    for(int i = 0; i < n; i++)
    {
        cout << "Item " << i+1
             << " Profit = " << profit[i]
             << " Weight = " << weight[i]
             << " Ratio = " << ratio[i]
             << endl;
    }

    float answer = fractionalKnapsack(
        capacity,
        profit,
        weight,
        ratio,
        n
    );
 cout << "\nMaximum Profit = "
         << answer
         << endl;

    return 0;
}
