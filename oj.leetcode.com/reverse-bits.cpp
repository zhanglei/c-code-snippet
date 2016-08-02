#include <stdint.h>
#include <math.h>
#include <iostream>

using namespace std;

class Solution {
    public:
    uint32_t reverseBits(uint32_t n) {
        uint32_t reverse_n;
        uint32_t n_ = n;
        int n_cnt;
        int index;
        int n_ptr[32] = {0};
        n_cnt = 32;
        do
        {
            n_cnt --;
            n_ptr[n_cnt] = n_%2;
            n_ = (n_ - n_ptr[n_cnt])/2;
            // cout << n_cnt << ":" << n_ptr[n_cnt] << endl;
        } while(n_cnt > 0);
        reverse_n = 0;
        n_cnt = 0;
        while (n_cnt < 32)
        {
            reverse_n += pow(2, n_cnt) * n_ptr[n_cnt];
            // cout << n_ptr[n_cnt] << ":" << reverse_n << endl;
            n_cnt ++;
        }

        return reverse_n;
    }
};

int main()
{
    Solution s;
    // cout << s.reverseBits(123) << endl;
    cout << s.reverseBits(2147483648) << endl;
    // cout << s.reverseBits(43261596) << endl;
    // cout << s.reverseBits(33554432) << endl;
}
