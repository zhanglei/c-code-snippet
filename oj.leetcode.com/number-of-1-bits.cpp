#include <stdint.h>
#include <iostream>

using namespace std;

class Solution {
    public:
    int hammingWeight(uint32_t n) {
        int bit;
        uint32_t n_;
        int weight;

        n_ = n;
        weight = 0;
        do
        {
            bit = n_%2;
            n_ = (n_-bit)/2;
            if (bit == 1)
            {
                weight++;
            }
        } while(n_!=0);
        return weight;
    }
};

int main()
{
    Solution s;
    cout << s.hammingWeight(3) << endl;
    cout << s.hammingWeight(2147483648) << endl;
}



