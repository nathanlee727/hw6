#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <ctime>
#include <cstdlib>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
HASH_INDEX_T operator()(const std::string& k) const
    {
        unsigned long long w[5] = {0}; 
        int len = k.length();
        int numBlocks = (len + 5) / 6;  
        
        for (int i = 0; i < numBlocks; ++i) {
            int blockStart = len - 6 * (i + 1);  
            blockStart = blockStart < 0 ? 0 : blockStart;
            int blockEnd = len - 6 * i; 
            unsigned long long blockValue = 0;
            unsigned long long pow36 = 1;  

            for (int j = blockEnd - 1; j >= blockStart; --j) {
                char c = std::tolower(k[j]);
                int value = letterDigitToNumber(c);
                blockValue += value * pow36;
                pow36 *= 36;
            }
            w[5 - numBlocks + i] = blockValue;
        }

        unsigned long long hashValue = 0;
        for (int i = 0; i < 5; ++i) {
            hashValue += rValues[i] * w[i];
        }
        return hashValue;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        if (isdigit(letter))
            return 26 + (letter - '0');
        else
            return letter - 'a';
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
