int main()
{
    int a;
    int b = 2;

    {
        int c = b;
    }

    a = c;

    return 0;
}