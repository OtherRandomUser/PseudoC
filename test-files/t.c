int main()
{
    int res = 12;

    return res;
}

int func(int c, int a, int b)
{
    // int res = a > 0 && b < 0;

    int res = 0;

    if (a > 0 && b < 0)
        res = 1;
    else
        res = 2;

    return res;
}