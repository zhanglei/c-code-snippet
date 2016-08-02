#include <iostream>
#include <limits.h>

using namespace std;

class Solution {
    public:
        int atoi(const char *str) {
            int integer = 0, tmp_integer, sign;
            for (;isspace(*str); str++)
                ;
            sign = (*str == '-') ? -1 : 1;
            if (*str == '+' || *str == '-')
            {
                str++;
            }
            // ascii 0-9
            for (; isdigit(*str); str++)
            {
                tmp_integer = integer;
                for(int i=0; i<9; i++)
                {
                    tmp_integer += integer;
                    if (tmp_integer < integer)
                    {
                        if (sign > 0)
                        {
                            return INT_MAX;
                        }
                        else
                        {
                            return INT_MIN;
                        }
                    }
                }
                tmp_integer += *str - '0';
                cout << tmp_integer << integer << endl;
                if (tmp_integer < integer)
                {
                    if (sign > 0)
                    {
                        return INT_MAX;
                    }
                    else
                    {
                        return INT_MIN;
                    }
                }
                else
                {
                    integer = tmp_integer;
                }
            }

            return sign * integer;
        }
};

int main()
{
    Solution s;
    cout<<s.atoi("-2147483648")<<endl;
    cout<<s.atoi("-2147483649")<<endl;
    cout<<s.atoi("2147483647")<<endl;
    cout<<s.atoi("2147483648")<<endl;
    cout<<s.atoi("   10522545459")<<endl;
    cout<<s.atoi("")<<endl;
    return 0;
}
