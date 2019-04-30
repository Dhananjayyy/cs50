// Helper functions for music

#include <cs50.h>
#include <math.h>

#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    // Find num and den and cast as float or else num/den will be integer operation
    int num = fraction[0] - '0';
    int den = fraction[2] - '0';

    // Calculate time in terms of eighths
    int eights = (8 * num) / den;
    return eights;
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    int letter, octave;
    float expn = 0;
    float f = 440;

    letter = note[0] - 'A' + 1;

    //Check if format is XY or XYZ
    if (note[2] == '\0')
    {
        octave = (note[1]) - '0';
    }
    else
    {
        octave = (note[2]) - '0';

        //Sharp or Flat: Multiply or Divide by 2^expn
        if (note[1] == '#')
        {
            expn += 1;
        }
        else
        {
            expn += -1;
        }
    }

    // Find note frequency
    expn += 2 * (letter - 1);

    //Handle Edge Cases of E-G and B-C
    if (letter > 2)
    {
        expn += -1;

        // Prev line takes us from B_n to C_n+1
        // This line ensures that we go back an octave to fix everything
        octave --;
    }

    if (letter > 5)
    {
        expn += -1;
    }

    //Octave multiply or divided by 2
    expn += 12 * (octave - 4);
    f = f * pow(2, expn / 12);

    return (int)(round(f));
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    if (s[0] == '\0')
    {
        return true;
    }
    return false;
}