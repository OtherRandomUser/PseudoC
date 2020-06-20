int main(int argc)
{
    int a;
    int b = 2;
    int c = b;
    a = 3 + b / c;

    // if (c > 0)
    if (c)
        a = c;
    else
        b = 2;

    return a + b - 7;
}