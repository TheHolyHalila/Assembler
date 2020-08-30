/* 
    Helpful global functions
*/
#include "globalHeader.h"

#define min(a,b) (a<b) ? a:b 
#define max(a,b) (a>b) ? a:b 

/*
    Description:
        Checking the existance of a char in a char array
    
    Returns:
        Boolean,
        true- If the char is in the array
        false- If the char isn't in the array
*/
bool inCharArray(char ch, const char *charArray){
    return strchr(charArray,ch) != NULL;
}

/*
    Description:
        Checking the existance of a char in a string without \0
    
    Returns:
        Boolean,
        true- If the char is in the array
        false- If the char isn't in the array
*/
bool inString(char ch, const char *charArray)
{
    int i;
    for (i = 0; charArray[i]; i++)
    {
        if (charArray[i] == ch)
        {
            return true;
        }
    }
    return false;
}
/*
    Description:
        Checking the existance of a string in a string array
    
    Returns:
        index
        -1 if not found
*/
int inStringArray(char *str, const char **stringArray,short length){
    int i;

    /* Check if the array contains the char */
    for (i = 0; i < length; i++)
    {
        if(!strcmp(str,stringArray[i])){
            return i;
        }
    }
    
    /* Returns false incase it didn't find the char */
    return NOT_FOUND;
}

/*
    Description:
        Checks if a charecter is in upper case
*/
bool isUpper(char ch){
    return (ch <= UPPERCASE_Z && ch >= UPPERCASE_A);
}

/*
    Description:
        Checks if a charecter is in lower case
*/
bool isLower(char ch)
{
    return (ch <= LOWERCASE_Z && ch >= LOWERCASE_A);
}

/*
    Description:
        Checks if a charecter is numeric
*/
bool isNumeric(char ch)
{
    return (ch <= NINE && ch >= ZERO);
}

/*
    Description:
        Checks if a charecter is alpha
*/
bool isAlpha(char ch)
{
    return (isLower(ch) || isUpper(ch));
}

/*
    Description:
        Checks if a charecter is alphanumeric
*/
bool isAlphanumeric(char ch){
    bool alpha, numeric;

    alpha = isAlpha(ch);
    numeric = isNumeric(ch);

    return (numeric || alpha);
}