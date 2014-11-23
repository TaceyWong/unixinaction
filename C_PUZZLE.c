#C Programming Puzzlers
[原文链接：](http://stevenkobes.com/ctest.html)These questions originally appeared as an article on programmersheaven.com, written by Ashok K. Pathak, a researcher at Bharat Electronics Limited (CRL), Ghaziabad.  They are reproduced here with minor modifications.

The questions test advanced knowledge of the C language, including some rarely-used features.  Effective C programming requires a strong understanding of concepts like undefined behavior, recursion, and pointer arithmetic, but the deliberately convoluted examples on this page are not representative of real-world code, and certainly won't win any prizes for clarity and maintainability.

Performance on these questions is not a good indicator of broader competence in software development.  As such, they are unlikely to be useful in an interview setting.

Steve Kobes, 8/25/04 (amended 6/19/11)




###1. 考虑一下下面的这个程序:
```cpp
#include <stdio.h>
#include <setjmp.h>

static jmp_buf buf;

int main(void)
{
   volatile int b = 3;

   if (setjmp(buf) != 0)
   {
      printf("%d\n", b);
      exit(0);
   }
   b = 5;
   longjmp(buf, 1);
}
```
该程序的输出是什么?

* (a)  3
* (b)  5
* (c)  0
* (d)  none of the above

> Answer: (b)  
The setjmp function stores context information for a “non-local goto”, and returns 0.  The longjmp function transfers control to the setjmp call that initialized buf, and execution continues from this point as if setjmp had returned 1.

> Note: a non-volatile automatic variable that has been modified after setjmp becomes indeterminate after longjmp.  Without the volatile qualifier, this program’s behavior would be undefined.  This rule permits better optimization of code.

###2. 考虑一下下面的这个程序:
```cpp
#include <stdio.h>

int main(void)
{
   struct node
   {
      int a;
      int b;
      int c;
   };
   struct node s = { 3, 5, 6 };
   struct node *pt = &s;

   printf("%d\n", *(int*)pt);

   return 0;
}
```
该程序的输出是什么?

* (a)  3
* (b)  5
* (c)  6
* (d)  7

>Answer: (a)  
The members of structs are arranged sequentially in memory, in the order of their declaration.  If a pointer to a struct is cast to the type of a pointer to its first member, the result points to its first member.

###3. 考虑一下下面的代码片段:
```cpp
int foo(int x, int n)
{
   int val = 1;

   if (n > 0)
   {
      if (n % 2 == 1)
         val *= x;

      val *= foo(x * x, n / 2);
   }
   return val;
}
```
What function of x and n is computed by foo?

* (a)  xn
* (b)  x × n
* (c)  nx
* (d)  none of the above


> Answer: (a)  
Consider small values of n:  
> ```cpp
foo(x, 0) = 1
foo(x, 1) = x * foo(x, 0) = x
foo(x, 2) = foo(x * x, 1) = x * x
foo(x, 3) = x * foo(x, 2) = x * (x * x)
```
An initial guess, from this pattern, of xn can be formally verified by induction:
>> Base cases of n = 0 and 1 are trivial.
Suppose foo(x, n) computes xn for all nonnegative n less than some i > 1.
If i is odd, foo(x, i) computes x * foo(x * x, (i – 1) / 2) = x * (x * x)(i – 1) / 2 = x * xi – 1 = xi.
If i is even, foo(x, i) computes foo(x * x, i / 2) = (x * x)i / 2 = xi.
By induction foo(x, n) computes xn for all nonnegative n.

###4. 考虑一下下面的程序:
```cpp
#include <stdio.h>

int main(void)
{
   int a[5] = { 1, 2, 3, 4, 5 };
   int *ptr = (int*)(&a + 1);

   printf("%d %d\n", *(a + 1), *(ptr - 1));

   return 0;
}
```
该程序输出什么?

* (a)  2 2
* (b)  2 1
* (c)  2 5
* (d)  none of the above

> ***Answer***: (c)  
Since a has type array[5] of int, &a has type pointer to array[5] of int.  Thus &a + 1 yields a pointer to the (non-existent) array[5] of int that comes after a, and ptr points to the (non-existent) int that comes after a.  Subtracting 1 from ptr yields a pointer to the last element of a.
> 
Note: in general, you can’t use the value of non-NULL pointer that does not point to a valid object.  However, an exception is made for pointers that point one past the end of an array.  This makes the initialization of ptr legal, even though it can’t be dereferenced.

###5. Consider the following program:
```cpp
#include <stdio.h>

void foo(int[][3]);

int main(void)
{
   int a[3][3] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };

   foo(a);
   printf("%d\n", a[2][1]);

   return 0;
}

void foo(int b[][3])
{
   ++b;
   b[1][1] = 9;
}
```
What is the output of this program?

(a)  8
(b)  9
(c)  7
(d)  none of the above
Hide answer

Answer: (b)

In foo, b has type pointer to array[3] of int, and after the increment, it points to the array a[1] ({4, 5, 6}).  So b[1] has type array[3] of int, and it denotes the array a[2] ({7, 8, 9}).  In value context, b[1] “decays” into a pointer to a[2][0] (7), so b[1][1] denotes a[2][1], whose value is changed from 8 to 9.

6. Consider the following program:

#include <stdio.h>

int main(void)
{
   int a, b, c, d;
   a = 3;
   b = 5;
   c = a, b;
   d = (a, b);

   printf("c=%d  ", c);
   printf("d=%d\n", d);

   return 0;
}
What is the output of this program?

(a)  c=3 d=3
(b)  c=5 d=3
(c)  c=3 d=5
(d)  c=5 d=5
Hide answer

Answer: (c)

The comma operator evaluates both of its operands and produces the value of the second.  It also has lower precedence than assignment.  Hence c = a, b is equivalent to c = a, while d = (a, b) is equivalent to d = b.

7. Consider the following program:

#include <stdio.h>

int main(void)
{
   int a[][3] = {1, 2, 3, 4, 5, 6};
   int (*ptr)[3] = a;

   printf("%d %d ", (*ptr)[1], (*ptr)[2]);

   ++ptr;
   printf("%d %d\n", (*ptr)[1], (*ptr)[2]);

   return 0;
}
What is the output of this program?

(a)  2 3 5 6
(b)  2 3 4 5
(c)  4 5 0 0
(d)  none of the above
Hide answer

Answer: (a)

Here, a has type array[2] of array[3] of int, and ptr initially points to the first array[3] ({1, 2, 3}).  After the increment, it points to the second array[3] ({4, 5, 6}).  Of course, *ptr denotes the array[3] that ptr happens to point to.

8. Consider the following code segment:

#include <stdlib.h>

int *f1(void)
{
   int x = 10;
   return &x;
}

int *f2(void)
{
   int *ptr;
   *ptr = 10;
   return ptr;
}

int *f3(void)
{
   int *ptr;
   ptr = malloc(sizeof *ptr);
   return ptr;
}
Which of these functions uses pointers incorrectly?

(a)  f3 only
(b)  f1 and f3
(c)  f1 and f2
(d)  f1, f2, and f3
Hide answer

Answer: (c)

Function f1 returns the address of a local variable.  Since the variable’s lifetime ends after the function returns, any use of the return value produces undefined behavior.

Function f2 produces undefined behavior because it dereferences and returns an uninitialized pointer.  (It has not been assigned to point to anything, and its initial value is indeterminate.)

Function f3 has no errors (although its caller should make sure the return value is not NULL before using it, and call free when the memory is no longer needed).

9. Consider the following program:

#include <stdio.h>

int main(void)
{
   int i = 3;
   int j;

   j = sizeof(++i + ++i);

   printf("i=%d j=%d\n", i, j);

   return 0;
}
What is the output of this program on an implementation where int occupies 2 bytes?

(a)  i=4 j=2
(b)  i=3 j=2
(c)  i=5 j=2
(d)  the behavior is undefined
Hide answer

Answer: (b)

Evaluating ++i + ++i would produce undefined behavior, but the operand of sizeof is not evaluated, so i remains 3 throughout the program.  The type of the expression (int) is deduced at compile time, and the size of this type (2) is assigned to j.

10. Consider the following program:

#include <stdio.h>

void f1(int*, int);
void f2(int*, int);
void (*p[2])(int*, int);

int main(void)
{
   int a = 3;
   int b = 5;

   p[0] = f1;
   p[1] = f2;

   p[0](&a, b);
   printf("%d %d ", a, b);

   p[1](&a, b);
   printf("%d %d\n", a, b);

   return 0;
}

void f1(int *p, int q)
{
   int tmp = *p;
   *p = q;
   q = tmp;
}

void f2(int *p, int q)
{
   int tmp = *p;
   *p = q;
   q = tmp;
}
What is the output of this program?

(a)  5 5 5 5
(b)  3 5 3 5
(c)  5 3 3 5
(d)  none of the above
Hide answer

Answer: (a)

The type of p is array[2] of pointers to functions taking (int*, int) and returning void; p[0] and p[1] point to f1 and f2, respectively.  Notice that f1 and f2 are identical.

Since arguments are passed by value, the assignment to q does not affect b, and p[0](&a, b) is equivalent to a = b.

11. Consider the following program:

#include <stdio.h>

void e(int);

int main(void)
{
   int a = 3;
   e(a);

   putchar('\n');
   return 0;
}

void e(int n)
{
   if (n > 0)
   {
      e(--n);
      printf("%d ", n);
      e(--n);
   }
}
What is the output of this program?

(a)  0 1 2 0
(b)  0 1 2 1
(c)  1 2 0 1
(d)  0 2 1 1
Hide answer

Answer: (a)

In pseudo-code,

e(0), e(-1) -> {}
e(1) -> {e(0), print 0, e(-1)} -> {{}, print 0, {}} -> {print 0}
e(2) -> {e(1), print 1, e(0)} -> {{print 0}, print 1, {}} -> {print 0, 1}
e(3) -> {e(2), print 2, e(1)} -> {{print 0, 1}, print 2, {print 0}} -> {print 0, 1, 2, 0}

12. Consider the following code segment:

typedef int (*test)(float*, float*);
test tmp;
What is the type of tmp?

(a)  function taking two pointer-to-float arguments and returning pointer to int
(b)  pointer to int
(c)  pointer to function taking two pointer-to-float arguments and returning int
(d)  none of the above
Hide answer

Answer: (c)

The typedef declaration creates a synonym for a type, which can then be used to declare objects of that type.  Note that the new name for the type appears in the position of a variable name, as though you were declaring a variable (which you’re not).  This can make it seem backwards in comparison to the syntax of a #define directive.

13. Consider the following program:

#include <stdio.h>

int main(void)
{
   char p;
   char buf[10] = {1, 2, 3, 4, 5, 6, 9, 8};

   p = (buf + 1)[5];
   printf("%d\n", p);

   return 0;
}
What is the output of this program?

(a)  5
(b)  6
(c)  9
(d)  none of the above
Hide answer

Answer: (c)

Remember that x[i] is equivalent to *(x + i), so (buf + 1)[5] is *(buf + 1 + 5), or buf[6].

14. Consider the following program:

#include <stdio.h>

void f(char**);

int main(void)
{
   char *argv[] = { "ab", "cd", "ef", "gh", "ij", "kl" };
   f(argv);

   return 0;
}

void f(char **p)
{
   char *t;

   t = (p += sizeof(int))[-1];

   printf("%s\n", t);
}
What is the output of this program on an implementation where int and all pointer types occupy 2 bytes?

(a)  ab
(b)  cd
(c)  ef
(d)  gh
Hide answer

Answer: (b)

In f, p initially points to the first char* ("ab") in the array of pointers argv.  Adding sizeof(int) = 2 to p makes it point to the third char* ("ef"), after which p[-1] denotes the second char* ("cd").

15. Consider the following program:

#include <stdarg.h>
#include <stdio.h>

int ripple(int n, ...)
{
   int i, j, k;
   va_list p;

   k = 0;
   j = 1;
   va_start(p, n);

   for (; j < n; ++j)
   {
      i = va_arg(p, int);
      for (; i; i &= i - 1)
         ++k;
   }
   va_end(p);
   return k;
}

int main(void)
{
   printf("%d\n", ripple(3, 5, 7));
   return 0;
}
What is the output of this program?

(a)  7
(b)  6
(c)  5
(d)  3
Hide answer

Answer: (c)

The va_arg macro produces the arguments passed as the “...” part of a variadic function.  In ripple it will be called twice, setting i first to 5, then to 7.

The expression i &= i - 1 resets the right-most 1 bit in i.  For example, if i is 6 (binary 110), i & i - 1 is 4 (binary 100).  The inner for loop executes until i is 0, so k is increased by the number of 1 bits in i.

There are two 1 bits in 5 (binary 101) and three in 7 (binary 111), so ripple returns 5.

16. Consider the following program:

#include <stdio.h>

int counter(int i)
{
   static int count = 0;
   count = count + i;
   return count;
}

int main(void)
{
   int i, j;

   for (i = 0; i <= 5; i++)
      j = counter(i);

   printf("%d\n", j);
   return 0;
}
What is the output of this program?

(a)  10
(b)  15
(c)  6
(d)  7
Hide answer

Answer: (b)

Since count is static, it maintains its value across calls to counter.  Its final value is the sum of the integers from 0 to 5.

Compute your score
