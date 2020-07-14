int func(int c, int a, int b)
{
    int res = c + 4;

    for (int i = 0; i < a + b; i++)
        res *= 2;

    return res;
}

int funcb()
{
    return 999;
}

void funcc()
{
}

int main()
{
    int b = 2;
    int res = func(1, b, 3);
    // int res = b + 3;

    funcb();
    funcc();

    return res;
}