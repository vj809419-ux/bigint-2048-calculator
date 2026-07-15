#include <stdio.h>

// 2048 bits ≈ 617 decimal digits
#define INPUT_MAX 620              // max input size (including '\0')
#define BIGINT_MAX (2 * INPUT_MAX) // enough for multiplication result

typedef struct {
    int d[BIGINT_MAX];   // digits stored in reverse order
    int len;      
    int sign;     
} BigInt;

void trim(BigInt *a);

/* string length function */   
int my_strlen(char s[])  {
    int i = 0;
    while(s[i] != '\0')
        i++;
    return i;
}

/* Convert string to BigInt */
BigInt StrToBigInt(char s[])  {
    BigInt a;
    for(int i = 0; i < BIGINT_MAX; i++)
        a.d[i] = 0;

    a.len = 0;
    a.sign = 1;
    int i = 0;

    if(s[0] == '-')  {
        a.sign = -1;
        i = 1;
    }
    int n = my_strlen(s);

    for(int j = n - 1; j >= i; j--) 
        a.d[a.len++] = s[j] - '0';

    trim(&a);
    return a;
}

/* Remove leading zeros */
void trim(BigInt *a) {
    while(a->len > 1 && a->d[a->len - 1] == 0)
        a->len--;

    if(a->len == 1 && a->d[0] == 0)
        a->sign = 1;
}

/* Compare absolute values */
int absCompare(BigInt a, BigInt b)  {
    if(a.len > b.len) 
        return 1;
    if(a.len < b.len) 
        return -1;

    for(int i = a.len - 1; i >= 0; i--)  {
        if(a.d[i] > b.d[i]) 
            return 1;     
        if(a.d[i] < b.d[i]) 
            return -1;
    }
    return 0;
}

/* Compare signed values */
int compare(BigInt a, BigInt b)  {
    if(a.sign > b.sign) 
        return 1;
    if(a.sign < b.sign) 
        return -1;

    int res = absCompare(a, b);
    if(a.sign == -1)
        res= -res;

    return res;
}

/* Absolute addition */
BigInt absAdd(BigInt a, BigInt b)  {
    BigInt r;

    for(int i = 0; i < BIGINT_MAX; i++)
        r.d[i] = 0;

    if (a.len > b.len)
        r.len = a.len;
    else
        r.len = b.len;

    int carry = 0;

    for(int i = 0; i < r.len; i++)  {
        int sum = carry;

        if(i < a.len) 
            sum = sum + a.d[i];
        if(i < b.len) 
            sum = sum + b.d[i];

        r.d[i] = sum % 10;
        carry = sum / 10;
    }

    if(carry) {
        if(r.len >= BIGINT_MAX)  
        {
        printf("Addition Overflow\n");
        return StrToBigInt("0");
        }
        r.d[r.len++] = carry;
    }
    return r;
}

/* Absolute subtraction (|a| >= |b|) */
BigInt absSub(BigInt a, BigInt b) {
    BigInt r;

    for(int i = 0; i < BIGINT_MAX; i++)
        r.d[i] = 0;

    r.len = a.len;
    int borrow = 0;

    for(int i = 0; i < a.len; i++)  {
        int diff = a.d[i] - borrow;

        if(i < b.len)
            diff = diff -  b.d[i];

        if(diff < 0) {
            diff = diff + 10;
            borrow = 1;
        } 
        else 
            borrow = 0;

        r.d[i] = diff;
    }
    trim(&r);
    return r;
}

/* Signed addition */
BigInt add(BigInt a, BigInt b)  {
    BigInt r;

    if(a.sign == b.sign)  {
        r = absAdd(a, b);
        r.sign = a.sign;
    } 
    else 
    {
        if(absCompare(a, b) >= 0)  {
            r = absSub(a, b);
            r.sign = a.sign;
        } 
        else  {
            r = absSub(b, a);
            r.sign = b.sign;
        }
    }
    return r;
}

/* Signed subtraction */
BigInt sub(BigInt a, BigInt b)  {
    b.sign = -b.sign;
    return add(a, b);
}

/* Multiplication */
BigInt multiply(BigInt a, BigInt b)  {
    BigInt r;

    if(a.len + b.len > BIGINT_MAX)  {
        printf("Multiplication Overflow\n");

        BigInt zero = StrToBigInt("0");
        return zero;
    }

    for(int i = 0; i < BIGINT_MAX; i++)
        r.d[i] = 0;

    r.len = a.len + b.len;

    for(int i = 0; i < a.len; i++)  {
        int carry = 0;

        for(int j = 0; j < b.len; j++)  {
            int curr = r.d[i + j] + a.d[i] * b.d[j] + carry;
            r.d[i + j] = curr % 10;
            carry = curr / 10;
        }
        r.d[i + b.len] = r.d[i + b.len] + carry;
    }

    r.sign = a.sign * b.sign;
    trim(&r);
    return r;
}

BigInt multiplyByDigit(BigInt a, int digit)  {
    BigInt r;

    for(int i=0;i<BIGINT_MAX;i++)
        r.d[i]=0;

    r.len=0;
    r.sign=1;
    int carry=0;

    for(int i=0;i<a.len;i++) {
        int cur=a.d[i]*digit + carry;

        r.d[r.len++]=cur%10;
        carry=cur/10;
    }

    while(carry)  {
        r.d[r.len++]=carry%10;
        carry/=10;
    }

    trim(&r);
    return r;
}

BigInt divide(BigInt a, BigInt b)  {
    BigInt zero = StrToBigInt("0");

    if(b.len==1 && b.d[0]==0)  {
        printf("Division by zero!\n");
        return zero;
    }

    int resultSign = a.sign * b.sign;

    a.sign = 1;
    b.sign = 1;

    if(absCompare(a,b) < 0)
        return zero;

    BigInt rem = StrToBigInt("0");

    BigInt q;

    for(int i=0;i<BIGINT_MAX;i++)
        q.d[i]=0;

    q.len = a.len;
    q.sign = resultSign;

    for(int i=a.len-1;i>=0;i--) {
        /* rem = rem * 10 + current digit */

        if(!(rem.len==1 && rem.d[0]==0))
        {
            for(int j=rem.len-1;j>=0;j--)
                rem.d[j+1]=rem.d[j];

            rem.len++;
        }

        rem.d[0]=a.d[i];

        trim(&rem);

        int digit=0;

        for(int d=9; d>=0; d--)  {
            BigInt trial = multiplyByDigit(b,d);

            if(absCompare(trial,rem)<=0) {
                digit=d;
                rem=absSub(rem,trial);
                break;
            }
        }
        q.d[i]=digit;
    }

    trim(&q);

    if(q.len==1 && q.d[0]==0)
        q.sign=1;

    return q;
}

BigInt mod(BigInt a, BigInt b)
{
    BigInt zero = StrToBigInt("0");

    if(b.len==1 && b.d[0]==0)  {
        printf("Modulo by zero!\n");
        return zero;
    }

    int dividendSign = a.sign;

    BigInt q = divide(a,b);
    BigInt prod = multiply(q,b);

    BigInt rem = sub(a,prod);

    if(!(rem.len==1 && rem.d[0]==0))
        rem.sign = dividendSign;

    trim(&rem);

    return rem;
}

BigInt power(BigInt base, int exp)
{
    BigInt result = StrToBigInt("1");
    BigInt zero   = StrToBigInt("0");

    if(exp < 0) {
        printf("Negative exponents not supported for BigInt\n");
        return zero;
    }
    /* 0^0 */
    if(exp == 0)
        return result;

    while(exp > 0) {
        if(exp % 2 == 1)
            result = multiply(result, base);

        base = multiply(base, base);
        exp /= 2;
    }
    return result;
}

BigInt IntToBigInt(int n)  {
    BigInt a;

    for(int i=0;i<BIGINT_MAX;i++)
        a.d[i]=0;

    a.sign=1;
    a.len=0;

    if(n==0)  {
        a.d[0]=0;
        a.len=1;
        return a;
    }
    while(n>0)  {
        a.d[a.len++]=n%10;
        n/=10;
    }

    return a;
}

/* Print BigInt */
void printBigInt(BigInt a)  {
    if(a.sign == -1)
        printf("-");

    for(int i = a.len - 1; i >= 0; i--)
        printf("%d", a.d[i]);
}


int isValidNumber(char s[])  {
    int n=my_strlen(s);
    if(n==0)    
        return 0;

    int i=0;
    if(s[0]=='-') 
    {
        if(n==1)    
            return 0;
        i=1;
    }
    for(int j=i;j<n;j++)  
    {
        if(s[j]<'0' || s[j]>'9')    
            return 0;
    }
    return 1;
}

void clearBuffer()  {
    int ch;
    while((ch = getchar()) != '\n' && ch != EOF);
}

int readNumber(char s[])  {
    scanf("%619s", s);

    int ch = getchar();

    if(ch != '\n' && ch != EOF)  {
        clearBuffer();
        return 0;   /* overflow */
    }
    return 1;
}

int main()
{
    int choice;

    while(1)
    {
        printf("\n========== BIGINT CALCULATOR ==========\n");
        printf("1. Compare\n");
        printf("2. Addition\n");
        printf("3. Subtraction\n");
        printf("4. Multiplication\n");
        printf("5. Division\n");
        printf("6. Modulus\n");
        printf("7. Power\n");
        printf("8. Exit\n");
        printf("Enter choice: ");

        if(scanf("%d",&choice) != 1)  {
            printf("Invalid Choice\n");
            clearBuffer();
            continue;
        }
        clearBuffer();
        
        if(choice == 8)
        {
            printf("Exiting...\n");
            break;
        }

        if(choice >= 1 && choice <= 6)
        {
            char s1[INPUT_MAX];
            char s2[INPUT_MAX];

            printf("Enter first number: ");
            if(!readNumber(s1))    {
                printf("Input exceeds 2048-bit limit\n");
                continue;
            }

            printf("Enter second number: ");
            if(!readNumber(s2))  {
                printf("Input exceeds 2048-bit limit\n");
                continue;
            }

            if(!isValidNumber(s1) || !isValidNumber(s2))
            {
                printf("Invalid Input\n");
                continue;
            }

            BigInt a = StrToBigInt(s1);
            BigInt b = StrToBigInt(s2);

            switch(choice)
            {
                case 1:
                {
                    int cmp = compare(a,b);

                    if(cmp > 0)
                        printf("First number is greater\n");
                    else if(cmp < 0)
                        printf("Second number is greater\n");
                    else
                        printf("Both numbers are equal\n");

                    break;
                }

                case 2:
                {
                    BigInt ans = add(a,b);

                    printf("Result:\n");
                    printBigInt(ans);
                    printf("\n");

                    break;
                }

                case 3:
                {
                    BigInt ans = sub(a,b);

                    printf("Result:\n");
                    printBigInt(ans);
                    printf("\n");

                    break;
                }

                case 4:
                {
                    BigInt ans = multiply(a,b);

                    printf("Result:\n");
                    printBigInt(ans);
                    printf("\n");

                    break;
                }

                case 5:
                {
                    BigInt ans = divide(a,b);

                    printf("Quotient:\n");
                    printBigInt(ans);
                    printf("\n");

                    break;
                }

                case 6:
                {
                    BigInt ans = mod(a,b);

                    printf("Remainder:\n");
                    printBigInt(ans);
                    printf("\n");

                    break;
                }
            }
        }
        
        else if(choice == 7)
        {
            char s[INPUT_MAX];
            int exp;

            printf("Enter base: ");

            if(!readNumber(s))  {
                printf("Input exceeds 2048-bit limit\n");
                continue;
            }      

            if(!isValidNumber(s))  {
                printf("Invalid Input\n");
                continue;
            }

            printf("Enter int exponent: ");

            if(scanf("%d",&exp) != 1)
            {
                printf("Invalid exponent\n");
                clearBuffer();
                continue;
            }
            clearBuffer();

            if(exp < 0)
            {
                printf("Negative exponents not supported\n");
                continue;
            }

            BigInt base = StrToBigInt(s);

            BigInt ans = power(base,exp);

            printf("Result:\n");
            printBigInt(ans);
            printf("\n");
        }

        else
            printf("Invalid Choice\n");
    }

    return 0;
}

