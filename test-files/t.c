int main()
{
    int res = 12;

    return res;
}

int func(int c, int a, int b)
{
    int res = c + 4;

    for (int i = 0; i < a + b; i++)
        res *= 2;

    return res;
}