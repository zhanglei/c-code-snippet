#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <vector>

using namespace std;

class Solution {
    public:
    void rotate(int nums[], int n, int k)
    {
        int tmp_nums[k];
        int i;
        int nums_size = nums.size();
        for (i=0; i<n; i++)
        {
//          cout << nums_size << ":" << nums[i] << endl;
            nums_size++;
        }
//          cout << nums_size << "--" << endl;
        for (i = 0; i < k; i++)
        {
            tmp_nums[i] = nums[nums_size - k + i];
        }
        for (i = 0; i < nums_size - k; i++)
        {
             nums[nums_size - i - 1] = nums[nums_size - k - i - 1];
        }
        for (i = 0; i < k; i++)
        {
             nums[i] = tmp_nums[i];
//           cout << tmp_nums[i] << endl;
        }
    }
};


int main()
{
    Solution s;
    int i;
    int nums[7] = {1,2,3,4,5,6,7};
    s.rotate(nums, 7, 4);
    for (i=0; i<7; i++)
    {
        cout << nums[i];
    } 
    cout << endl;
    int nums1[2] = {1,2};
    s.rotate(nums1, 2, 1);
    for (i=0; i<2; i++)
    {
        cout << nums1[i];
    } 
    cout << endl;
}
