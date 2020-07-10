int main(int c)
{
    int a = 2;
    int b = 6;

    if (c > 0 && a != 7 || b == 9)
        a = c;
    else
        b *= 2;

    int d = c > 0 && a != 7 || b == 9;

    return d;
}