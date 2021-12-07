/*
Author: Aoun Hussain
Class: ECE 6122-A
Last Date Modified: Wed, Oct 13, 2021
Description: This is the cpp source file for the solution of problem 2, lab 2.
             This source file takes an input N as a whole number in the command
             line argument and calculates the integral of the expression in the
             question using the midpoint rule and openMP library. The resulting
             integral estimate is then output in the Lab2Prob2.txt file.

Instructions for Compiling:
            Please use the following command to compile without errors on Pace-Ice:

            "g++ -std=c++17 -fopenmp Lab2_Problem2.cpp -o test2"
*/

/*
 References:
    https://bisqwit.iki.fi/story/howto/openmp/
    https://tutorial.math.lamar.edu/classes/calcii/approximatingdefintegrals.aspx
    https://github.com/kamilmysliwiec/parallel-numerical-integration/blob/master/main.cpp
    https://kamilmysliwiec.com/parallel-numerical-integration-with-openmp
*/

#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <thread>
#include <map>
#include <vector>
#include <bitset>
#include <omp.h>
#include <iostream>
#include <iomanip>
#include <list>
#include <utility>
#include <exception>
using namespace std;

double round6(double var)
{
    /*
    function to round off the result to six decimal places
    takes the float and returns rounded to 6 decimal places
    */
    double value = (int)((var * 1000000) + .5);  //use 100 for 2 decimal places and respectively
    return (double)(value / 1000000);
}

bool is_numeric(const string &strIn, unsigned long &nInputNumber)
{
    /*
    Checks if the argument is numeric and returns true/false accordingly
    */
    bool bRC = all_of(strIn.begin(), strIn.end(), [](unsigned char c)
    {
        return ::isdigit(c);                      // http://www.cplusplus.com/reference/algorithm/all_of/
    }                                             // https://www.geeksforgeeks.org/lambda-expression-in-c/
    );                                            // http://www.cplusplus.com/reference/cctype/isdigit/

    if (bRC)
    {
        nInputNumber = stoul(strIn);              // https://www.cplusplus.com/reference/string/stoul/
        return true;
    }
    else
    {
        return false;
    }

}

double expression(const double midpoint)
{
    /*
    returns the result of the given expression to be integrated
     at the given midpoint
    */
    return (14*exp(7*midpoint));
}

double midpointRule(const unsigned long N, const double x1, const double dx)
{
    /*
    applies the midpoint rule to integrate the given expression according to the number of intervals
    and threads
    */
    double final_result = 0;   //final result of integration through midpoint rule

    //creates parallel threads and then reduces the answer to single globally shared variable
    //the function expression is called each time in the thread to calculate fx(x_n*) where
    //x_n* is the nth midpoint
    double result;
    #pragma omp parallel for private (result) reduction (+:final_result)
    for (int i = 1; i <= N; i++)
    {
        result = expression(x1 + (i * dx));  //adds the factor (fx(x_1*) + fx(x_2*) + .. + fx(x_n*))

        final_result += result;         //only accessed by one thread at a time due to reduction
    }

    final_result *= dx;     //multiplies the factor (fx(x_1*) + fx(x_2*) + .. + fx(x_n*)) with delta x (dx)
    return final_result;
}

double calIntegral(unsigned long &N)
{
    /*
    takes input a whole number which ranges from 0-1000000000
    and then uses openMP and midpoint rule to calculate the integral
    and returns it as a double
    */

    if (N == 0)          //check for all positive whole numbers as division by zero will give error
    {
        N = 1;
    }

    double x1 = 0;                                                //first limit as given
    double x2 = (log(2) / 7);                              //second limit as given
    double dx = (x2 - x1) / N;                                    //calculating number of intervals

    double estimate = midpointRule(N, x1, dx);                    //calls the multi-threaded function

    return round6(estimate);         //returns the integral estimate rounded to 6 decimal places
}

int main(int argc, char* argv[])
{
    /*
    main function to call all functions and perform multi-threading and print output to
    Lab2Prob2.txt.
    */
    if (argc == 2)
    {
        unsigned long nInputNumber{ 0 };
        string strInput(argv[1]);
        bool bIsValid = is_numeric(strInput, nInputNumber);
        if (bIsValid)
        {
            double estimate = calIntegral(nInputNumber);       //result returned by integral function

            //writes output to the file for valid inout
            ofstream myfile ("Lab2Prob2.txt");
            if (myfile.is_open())
            {
                myfile << estimate << "\n";
                myfile.close();
            }
            else cout << "Unable to open file";
        }
        else
        {
            cout << "Invalid Input" << endl;
            ofstream myfile ("Lab2Prob2.txt");
            if (myfile.is_open())
            {
                myfile << "Invalid Input" << "\n";
                myfile.close();
            }
            else cout << "Unable to open file";
        }
    }
    else
    {
        cout << "Invalid Input" <<endl;
        ofstream myfile ("Lab2Prob2.txt");
        if (myfile.is_open())
        {
            myfile << "Invalid Input" << "\n";
            myfile.close();
        }
        else cout << "Unable to open file";
    }

    return 0;
}

