int main(int c)
{
    int res = 12;
    int a = c > 9 ? 4000 + res : 12 - res;

    // while (res < a)
    // {
    //     if (res > 2000)
    //         continue;

    //     if (res > 1500)
    //         break;

    //     res *= c;
    // }

    return res;
}