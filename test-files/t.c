int main(int c)
{
    int res = 0;

    while (res < 1500)
    {
        if (res > 2000)
            continue;

        // if (res > 1500)
        //     break;

        res *= c;
    }

    return res;
}