int GCD(int a, int b)
{
    // initialize the local variables num1 and num2
    int num1 = a, num2 = b;

    while (num1 != num2)
    {
        // if num1 is greater than num2, returns the num1.
        if (num1 > num2)
        {
            num1 = num1 - num2;
        }
        // otherwise, it returns the num2.
        else
        {
            num2 = num2 - num1;
        }
    }
    // print the GCD of the number.
    return num2;
}

int main()
{
    // initialize the local variables num1 and num2
    int a = GCD(99, 30);
    return a;
}