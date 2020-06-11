int conditional(int c)
{
    int a = 2;
    int b = 6;

    if (c > 0)
        a = c;
    else
        b *= 2;

    if (b > a)
        return a;
    else
        return b;
}

int for_loop(int c)
{
    int res = 0;
    
    for (int i = 6; i < 12; i++)
        res *= c;

    return res;
}

int infinite_while_loop(int c)
{
    int res = 0;

    while (1)
    {
        if (res % 2)
            continue;

        if (res > 1500)
            break;

        res *= c;
    }

    return res;
}

int ass(int c)
{
    int v;
    int a = 2;
    int b = 6;
    
    v = b * 2 + a / 4;
    return c * v;
}

int f(int a, int b, int c)
{
    int d = a + b / c;
    int e = a * b - c;
    return d + e;
}

int implicit_cast(float a, float b)
{
    return a * b;
}

int actual_cast(float a, float b)
{
    return ((int) a) * ((int) b);
}

int anoying_expression(int a)
{
    // isso eh um warning no clang heheheheh
    return a++ * --a;
}

int fcall(int a, int b)
{
    return anoying_expression(a) / conditional(b);
}

int main()
{
    return 0;
}